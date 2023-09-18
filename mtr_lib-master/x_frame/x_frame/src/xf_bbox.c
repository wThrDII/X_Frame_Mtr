/**
 * @brief 黑匣子功能
 * @file xf_bbox.c
 * @version 1.0
 * @author hzf
 * @date 2022-7-14 16:56:3
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-14 16:56:3
 *   Author      : hzf
 *   Modification: Created file
 */


#include "x_frame.h"

#define FLASH_ERASE_VAL (0xffffffff)
#define BBOX_FSM_ID     (0xff)

#define CRC_LEN         (4)
#define TYPE_LEN        (1)

#define FLASH_WRT_TEST   0
#define UPLOAD_RETRY_COUNT  5

#define BBOX_DATA_SEND_MIN_INTERVAL_TIME_MS  	100
#define BBOX_DATA_SEND_MAX_INTERVAL_TIME_MS  	5000
#define BBOX_UPLOAD_TIMEOUT_MS                  (60 * 1000)

#define BBOX_MAIN_VER   1
#define BBOX_SUB_VER    0
#define BBOX_DBG_VER    0
#define BBOX_TEST_VER   1

enum{
    BBOX_FLASH_NORMAL = 0,          ///< 常规数据区，对应DV上COMMON分区
    BBOX_FLASH_FAULT,               ///< 故障数据区，对应DV上SERIOUS分区
    BBOX_FLASH_RECORD,              ///< 录播数据区，对应DV上RECORD分区
    BBOX_FLASH_MAX
};

enum{
    BBOX_FSM_CLOSE_STATE = 0,       ///< 数据记录功能关闭
//    BBOX_FSM_TRIGGER_STATE,
    BBOX_FSM_ERROR_STATE,           ///< 永久故障
    BBOX_FSM_FAULT_STATE,           ///< 一般故障
    BBOX_FSM_NORMAL_STATE,          ///< 常规状态
    BBOX_FSM_MAX,
};

enum{
	CMD_REC_ERASE = 0X01,           ///< 数据擦除指令
	CMD_REC_STOP = 0X05,            ///< 上报停止指令
};

#pragma pack(1)
///< (FE,30)指令的设置数据结构
typedef struct{
	uint8_t		data_type;          ///< 数据类型，0x05:常规数据,0x0a:故障数据,0x0f:录播数据
	uint16_t 	page_size;          ///< 页大小
	uint16_t 	page_num;           ///< 页码
	uint16_t 	data_interval_time; ///< 数据上报间隔
}BBOX_UPLOAD_PARAM_T;

///< (FE,30)指令的响应数据结构
typedef struct{
	uint8_t		data_type;          ///< 数据类型，0x05:常规数据,0x0a:故障数据,0x0f:录播数据
	uint32_t	index_start;        ///< 数据上报的起始索引值
	uint32_t 	index_stop;         ///< 数据上报的结束索引值
    uint8_t     cmd_func;           ///< 数据上报的指令cmd_set
    uint8_t     cmd_id;             ///< 数据上报的指令id
}BBOX_ACK_TYPE_T;

///< rec配置帧获取及响应指令(FE,35)数据结构
typedef struct{
	uint8_t		data_type;          ///< 数据类型，0x05:常规数据,0x0a:故障数据,0x0f:录播数据
	uint8_t		state;              ///< 1:获取,0:停止
}BBOX_REC_CONFIG_T,BBOX_REC_CONFIG_ACK_T;

///> 补采指令(FE,31)数据结构
typedef struct{
	uint8_t		data_type;          ///< 数据类型，0x05:常规数据,0x0a:故障数据,0x0f:录播数据
	uint32_t	data_index;         ///< 补采位图的起始数据索引值
    uint8_t     bits_map[0];        ///< 补采位图，共125字节大小，1000条数据位
}BBOX_RE_READ_TYPE_T;

///> 数据上报结束指令(FE,36)数据结构
typedef struct{
	uint8_t		data_type;
    uint8_t     cmd_type;   ///< 0:通知,1:查询,2:停止
    uint8_t     state;      ///< 0:未结束,1:结束,仅在通知指令下有效
}BBOX_UPLOAD_FINISH_T,BBOX_UPLOAD_FINISH_ACK_T;

#pragma pack()

typedef struct{
    BBOX_FLASH_CFG_T *p_cfg;    ///< rec 配置
    uint32_t part_id;
    uint32_t index_size;        ///< 索引区总大小
    uint32_t index;             ///< 下一个可写的索引值
    uint32_t index_cnt;         ///< 索引区能存放的索引值总数量
    uint32_t data_size;         ///< 数据区总大小
    uint32_t data_len;          ///< 数据块大小
    uint32_t data_cnt;          ///< 数据区能存放的数据块总数量
    uint32_t data_act_cnt;      ///< 数据区当前实际的存储数量
    uint32_t refuse_time;       ///< 拒绝保存的时间戳
    uint8_t is_read_all;        ///< 是否读取所有数据
}BBOX_FLASH_DATA_T;

typedef struct{
    uint16_t event_name;
    uint16_t event_type;
    uint8_t on_off;             ///< 开关读取 0：停止读取，1：进行读取
    uint8_t head_read_index;    ///< 下一个读取数据的buf索引值
    uint8_t head_read_cnt;      ///< 读取的数量
    uint8_t tail_read_index;    ///< 下一个读取数据的buf索引值
    uint8_t tail_read_cnt;      ///< 读取的数量
    uint8_t read_tail;          ///< 只读取到尾部buf
    uint8_t read_finish;        ///< 读取完成标志，一般是尾部数据buf读满则判断完成
}BBOX_PARAM_T;          ///< head:用来频繁读取暂存黑匣子，tail：NORMAL时用来读取暂存trigger单次数据，RECORD时用来触发后读取暂存之后的录波数据

typedef struct{
    uint8_t state;
    void (*state_in)(void);
    void (*state_exec)(void);
    void (*state_out)(void);
}BBOX_FSM_PARAM_T;

typedef struct{
    const BBOX_FSM_PARAM_T *cur_param;
    uint8_t cur_state;
    uint8_t new_state;
    uint8_t change_flag;
}BBOX_FSM_DATA_T;

typedef struct{
    uint32_t start_index;           ///< 上报数据的起始索引值
    uint32_t stop_index;            ///< 上报数据的结束索引值
    uint32_t remain_len;            ///< 补采数据的剩余数据条数
    uint32_t upload_len;            ///< 补采数据的总条数
    uint32_t upload_base_time;      ///< 上报数据参数设置限制解除的超时基础时间
    uint8_t upload_bits[(BBOX_PAGE_SIZE_MAX % 8) ? (BBOX_PAGE_SIZE_MAX / 8 + 1) : (BBOX_PAGE_SIZE_MAX / 8)];    ///< 上报数据位图
    uint16_t bits_index;            ///< 当前位图的索引值
    uint8_t dest_addr;              ///< 数据上报的目的地址
    uint8_t link_id;                ///< 数据上报的link id
    uint8_t data_type;              ///< 数据类型，0x05:常规数据,0x0a:故障数据,0x0f:录播数据
    uint8_t cfg_retry_cnt;          ///< 配置帧上报次数
    uint8_t finish_retry_cnt;       ///< 数据上报结束指令上报次数
    uint8_t state;                  ///< 0:停止上报,1:开始上报
}BBOX_UPLOAD_DATA_T;

typedef struct{
    BBOX_PARAM_T param[BBOX_CTRL_MAX];              ///< 当前数据读取的参数
    BBOX_TIME_T time;                               ///< 时间戳参数
    BBOX_FSM_DATA_T fsm;                            ///< 状态机参数
    BBOX_FLASH_DATA_T flash[BBOX_FLASH_MAX];        ///< flash参数
    BBOX_UPLOAD_DATA_T upload;                      ///< 上报参数
    uint32_t upload_period_ms;                      ///< 上报时间，所有黑匣子数据共用，使用最后一次配置
    uint32_t save_period_ms;                        ///< 数据保存间隔
    uint16_t normal_event_name;                     ///< 常规状态的事件名
    uint16_t normal_event_type;                     ///< 常规状态的事件类型
    uint8_t error_remained;                         ///< 错误数据剩余保存数量
    uint8_t upload_cfg_remained;                    ///< rec配置上报剩余循环次数
}BBOX_CTRL_DATA_T;

static void __bbox_fsm_in_func(void);
static void __bbox_fsm_out_func(void);
static void __bbox_fsm_normal_exec_func(void);
static void __bbox_fsm_error_in_func(void);
static void __bbox_fsm_fault_in_func(void);
static void __bbox_fsm_error_exec_func(void);
static void __bbox_fsm_fault_exec_func(void);

static BBOX_CTRL_DATA_T bbox_ctrl;

///> 默认列表第一个成员为默认参数
static const BBOX_FSM_PARAM_T bbox_fsm_list[] = {
    [BBOX_FSM_CLOSE_STATE] = {BBOX_FSM_CLOSE_STATE, NULL, NULL, NULL},
    [BBOX_FSM_NORMAL_STATE] = {BBOX_FSM_NORMAL_STATE, NULL, __bbox_fsm_normal_exec_func, NULL},
    [BBOX_FSM_ERROR_STATE] = {BBOX_FSM_ERROR_STATE, __bbox_fsm_error_in_func, __bbox_fsm_error_exec_func, NULL},
    [BBOX_FSM_FAULT_STATE] = {BBOX_FSM_FAULT_STATE, __bbox_fsm_fault_in_func, __bbox_fsm_fault_exec_func, NULL}
};

static int32_t __bbox_flash_cur_index_erase(BBOX_FLASH_DATA_T *p_flash)
{
    uint32_t block_size = flash_part_get_sector_size(p_flash->part_id);
    uint32_t wr_offset;

    ///> 避免重复擦除
    if(p_flash->index == 0){
        return 0;
    }

    wr_offset = ((p_flash->index - 1) % p_flash->index_cnt) * sizeof(p_flash->index);
    flash_part_erase_len(p_flash->part_id, (wr_offset / block_size) * block_size, block_size);

    return 0;
}

///> 读取管理区的数据区数据
static int32_t __bbox_flash_data_read(BBOX_FLASH_DATA_T *p_flash, uint32_t index, uint8_t *p_buf, uint32_t len)
{
    uint32_t crc = 0;
    uint32_t read_crc = 0;

    memset(p_flash->p_cfg->p_flash_buf, 0, p_flash->p_cfg->dat_size);
    flash_part_read(p_flash->part_id, p_flash->index_size + (index % p_flash->data_cnt) * p_flash->data_len, p_flash->p_cfg->p_flash_buf, p_flash->p_cfg->dat_size);
    memcpy(&read_crc, p_flash->p_cfg->p_flash_buf, sizeof(read_crc));

    crc = calc_chk_val(u_chk_crc16, p_flash->p_cfg->p_flash_buf + sizeof(crc), p_flash->p_cfg->dat_size - sizeof(crc));

    if(crc != read_crc){
        DBG_E("bbox flash data crc err,index:%d, crc:%d, read crc:%d", index, crc, read_crc);
        return -1;
    }

    if(p_buf != NULL)
        memcpy(p_buf, p_flash->p_cfg->p_flash_buf + sizeof(read_crc), len);
    return 0;
}

static int32_t __bbox_flash_data_read_safe(BBOX_FLASH_DATA_T *p_flash, uint32_t index, uint8_t *p_buf, uint32_t len)
{
    ///> 判断读取的index是否在有效范围内
    if(index >= p_flash->index || index < (p_flash->index - p_flash->data_act_cnt)){
        return -1;
    }
    return __bbox_flash_data_read(p_flash, index, p_buf, len);
}

///> 写入管理区的数据区数据
static int32_t __bbox_flash_data_write(BBOX_FLASH_DATA_T *p_flash, uint8_t *p_buf, uint32_t len)
{
    uint32_t block_size = flash_part_get_sector_size(p_flash->part_id);
    uint32_t wr_offset = (p_flash->index % p_flash->data_cnt) * p_flash->data_len;
    uint32_t crc = 0;

    memset(p_flash->p_cfg->p_flash_buf, 0, p_flash->p_cfg->dat_size);
//    DBG_I("wr offset : %d", wr_offset);
    if((wr_offset / block_size) != ((wr_offset + p_flash->data_len) / block_size)){
        uint32_t er_offset = (((wr_offset / block_size) + 1) % (p_flash->data_size / block_size)) * block_size;
        flash_part_erase_len(p_flash->part_id, er_offset + p_flash->index_size, block_size);
//        DBG_I("erase next block addr:%p", er_offset);
    }

    if((wr_offset % block_size) == 0){
        flash_part_erase_len(p_flash->part_id, wr_offset + p_flash->index_size, block_size);
//        DBG_I("erase cur block addr:%p", wr_offset);
    }

    memcpy(p_flash->p_cfg->p_flash_buf + sizeof(crc), p_buf, X_FRAME_MIN(len, p_flash->p_cfg->dat_size - sizeof(crc)));
    
    crc = calc_chk_val(u_chk_crc16, p_flash->p_cfg->p_flash_buf + sizeof(crc), p_flash->p_cfg->dat_size - sizeof(crc));
//    DBG_I("crc:%d", crc);
    memcpy(p_flash->p_cfg->p_flash_buf, &crc, sizeof(crc));
    
    flash_part_write(p_flash->part_id, wr_offset + p_flash->index_size, p_flash->p_cfg->p_flash_buf, p_flash->p_cfg->dat_size);

#if FLASH_WRT_TEST
    uint32_t read_crc;
    memset(p_flash->p_cfg->p_flash_buf, 0, p_flash->p_cfg->dat_size);
    flash_part_read(p_flash->part_id, wr_offset + p_flash->index_size, p_flash->p_cfg->p_flash_buf, p_flash->p_cfg->dat_size);
    memcpy(&read_crc, p_flash->p_cfg->p_flash_buf, sizeof(read_crc));
//    DBG_I("read crc:%d", crc);
    if(crc != read_crc){
        DBG_E("write crc err, cal crc:%d, read crc:%d", crc, read_crc);
        return -1;
    }
#endif

    if(p_flash->data_act_cnt < p_flash->data_cnt){
        p_flash->data_act_cnt++;
    }else if(p_flash->index < p_flash->data_cnt){
        p_flash->data_act_cnt = p_flash->index;
    }
    
    return 0;
}

static int32_t __bbox_flash_index_write(BBOX_FLASH_DATA_T *p_flash, uint8_t *p_buf, uint32_t len)
{
    uint32_t block_size = flash_part_get_sector_size(p_flash->part_id);
    uint32_t wr_offset = (p_flash->index % p_flash->index_cnt) * sizeof(p_flash->index);
    uint32_t index = p_flash->index;
    
    if(wr_offset % block_size == 0){
        flash_part_erase_len(p_flash->part_id, wr_offset, block_size);
        flash_part_write(p_flash->part_id, wr_offset, (uint8_t *)&index, sizeof(index));
#if FLASH_WRT_TEST
        uint32_t read_index;
        flash_part_read(p_flash->part_id, wr_offset, (uint8_t *)&read_index, sizeof(read_index));
//      DBG_I("read index:%d", read_index);
        if(read_index != index){
            DBG_E("write index err, wrt index:%d, read crc:%d", index, read_index);
            return -1;
        }
#endif
        wr_offset = (wr_offset > 0) ? (wr_offset - block_size) : (p_flash->index_size - block_size);
        flash_part_erase_len(p_flash->part_id, wr_offset, block_size); ///< 清除前一块数据
    }else{
        flash_part_write(p_flash->part_id, wr_offset, (uint8_t *)&index, sizeof(index));
#if FLASH_WRT_TEST
        uint32_t read_index;
        flash_part_read(p_flash->part_id, wr_offset, (uint8_t *)&read_index, sizeof(read_index));
//      DBG_I("read index:%d", read_index);
        if(read_index != index){
            DBG_E("write index err, wrt index:%d, read crc:%d", index, read_index);
            return -1;
        }
#endif
    }

    return 0;
}

static bool __bbox_index_valid_judge(BBOX_FLASH_DATA_T *p_flash, uint32_t index, uint32_t addr, uint32_t off_set)
{
    if((index % p_flash->index_cnt) == (addr + off_set)){
        return true;
    }else{
        return false;
    }
}

static uint32_t __bbox_max_index_calc(BBOX_FLASH_DATA_T *p_flash, uint32_t block_size)
{
    uint32_t index_buf[64] = {0};
    uint32_t i, j;
    uint32_t block_cnt = p_flash->index_size / block_size;
    uint32_t block_index = 0;   ///< 块下标值
    uint32_t sum = 0;           ///< 存在有效数据的block数量
    uint32_t index = 0;         ///< 临时索引值
//    uint32_t overflow_val = FLASH_ERASE_VAL;
    uint32_t has_list[5] = {0}; ///< 记录存有index的block下标，方便做快速擦除，最多记录5个
    
    //!> 遍历寻找当前最大索引值的block位置
    index = 0;
    for(i = 0; i < block_cnt; i++){
        flash_part_read(p_flash->part_id, i * block_size, (uint8_t *)index_buf, sizeof(index_buf[0]) * 2);
        if(FLASH_ERASE_VAL == index_buf[0] && FLASH_ERASE_VAL == index_buf[1]){
            ///< 被擦除过
            continue;
        }else{
//            DBG_I_BLOCK(10,"index buf[0]:%d",index_buf[0]);
            if(__bbox_index_valid_judge(p_flash, index_buf[0], i * block_size / sizeof(index_buf[0]), 0) && index <= index_buf[0]){
                index = index_buf[0];
                block_index = i;
            }
            
            if(sum < ARRY_ITEMS_NUM(has_list)){
                has_list[sum] = i;
            }
            sum++;
        }
    }
    //!<
    
    //!> 擦除多余的index block
    if(sum > 1){
        if(sum < ARRY_ITEMS_NUM(has_list)){
            for(i = 0; i < sum; i++){
                if(block_index != has_list[i]){
                    flash_part_erase_len(p_flash->part_id, has_list[i] * block_size, block_size);
                }
            }
        }else{
            for(i = 0; i < block_cnt; i++){
                flash_part_read(p_flash->part_id, i * block_size, (uint8_t *)index_buf, sizeof(index_buf[0]) * 2);
                if(FLASH_ERASE_VAL == index_buf[0] && FLASH_ERASE_VAL == index_buf[1]){
                    continue;
                }else if(block_index == i){
                    continue;
                }else{
                    flash_part_erase_len(p_flash->part_id, i * block_size, block_size);
                    if(--sum == 1){
                        break;
                    }
                }
            }
        }
    }
    //!<

    //!> 读取最大的index有效值
    for(i = 0; i < block_size; i += sizeof(index_buf)){
        flash_part_read(p_flash->part_id, block_index * block_size + i, (uint8_t *)index_buf, sizeof(index_buf));
        for(j = 0; j < ARRY_ITEMS_NUM(index_buf); j++){
            if(FLASH_ERASE_VAL == index_buf[j]){
                return index;
            }
            if(__bbox_index_valid_judge(p_flash, index_buf[j], block_index * block_size / sizeof(index_buf[0]), i / sizeof(index_buf[0]) + j)){
                index = index_buf[j];
            }else{
                return index;
            }            
       }
    }
    //!<

    return index;
}

static uint32_t __bbox_data_search_by_index(BBOX_FLASH_DATA_T *p_flash, uint32_t max_index)
{
    uint32_t min_index = (max_index > 30) ? (max_index - 30) : (0);
    uint32_t index;
    int32_t ret;
    
    for(index = max_index; index > min_index ; index--){
        ret = __bbox_flash_data_read(p_flash, index, NULL, 0);
        if(ret == 0){
            return (index + 1); ///< 返回下一个可写入的索引值
        }
    }
    
    return index;
    
}

///> 初始化管理区的flash数据
static int32_t __bbox_data_init(BBOX_FLASH_DATA_T *p_flash)
{
    uint32_t part_size = flash_part_get_part_size(p_flash->part_id);
    uint32_t block_size = flash_part_get_sector_size(p_flash->part_id);
    uint32_t total_cnt = part_size / (p_flash->data_len + sizeof(p_flash->index));
    uint32_t data_index;

//    DBG_I_BLOCK(100,"part size:%d, block:%d,total:%d,data len:%d", part_size, block_size,total_cnt,p_flash->data_len);
    p_flash->data_size = (uint32_t)((total_cnt * p_flash->data_len) / block_size + 0.9) * block_size;
    p_flash->index_size = part_size - p_flash->data_size;

    if(0 == p_flash->index_size){
        p_flash->data_size = p_flash->data_size - block_size;
        p_flash->index_size = block_size;
    }

//    DBG_I("bbox part id:%d, data size:%d, index size:%d", p_flash->part_id, p_flash->data_size , p_flash->index_size);
    
    p_flash->index_cnt = p_flash->index_size / sizeof(p_flash->index);
    p_flash->data_cnt = p_flash->data_size / p_flash->data_len;
    
    data_index = __bbox_max_index_calc(p_flash, block_size);
    p_flash->index = __bbox_data_search_by_index(p_flash, data_index);

    if(p_flash->index >= p_flash->data_cnt){
        p_flash->data_act_cnt = p_flash->data_cnt;
    }else{
        p_flash->data_act_cnt = p_flash->index % p_flash->data_cnt;
    }
//    DBG_I_BLOCK(10, "index:%d,act_cnt:%d",p_flash->index,p_flash->data_act_cnt);
    return 0;
}

static void __bbox_init(void)
{
    BBOX_FLASH_CFG_T *p_cfg = NULL;
    BBOX_FLASH_PART_CFG_T *p_flash = NULL;
    uint32_t i = 0;
    uint8_t flash_flag = 0;

    memset(&bbox_ctrl, 0, sizeof(bbox_ctrl));
    
    while(1){
        p_flash = (BBOX_FLASH_PART_CFG_T *)auto_reg_get_func_cont_by_index(AUTO_REG_BBOX_FLASH, i);		
        if(p_flash == NULL){
           break;
        }

        bbox_ctrl.flash[BBOX_FLASH_NORMAL].part_id = p_flash->normal_part_id;
        bbox_ctrl.flash[BBOX_FLASH_FAULT].part_id = p_flash->fault_part_id;
        bbox_ctrl.flash[BBOX_FLASH_RECORD].part_id = p_flash->record_part_id;
        flash_flag = 1;
        i++;
    }

    if(0 == flash_flag){
        return;
    }
    
    i = 0;
    
    while(1){
        p_cfg = (BBOX_FLASH_CFG_T *)auto_reg_get_func_cont_by_index(AUTO_REG_BBOX, i);		
        if(p_cfg == NULL){
           break;
        }
        
		if(p_cfg->type == BBOX_CTRL_NORMAL && bbox_ctrl.flash[BBOX_FLASH_NORMAL].p_cfg == NULL ){
            bbox_ctrl.flash[BBOX_FLASH_NORMAL].p_cfg = p_cfg;
            bbox_ctrl.flash[BBOX_FLASH_FAULT].p_cfg = p_cfg;
            bbox_ctrl.flash[BBOX_FLASH_NORMAL].data_len = p_cfg->dat_size;
            bbox_ctrl.flash[BBOX_FLASH_FAULT].data_len = p_cfg->dat_size;

            __bbox_data_init(&bbox_ctrl.flash[BBOX_FLASH_NORMAL]);
            __bbox_data_init(&bbox_ctrl.flash[BBOX_FLASH_FAULT]);
        }else if(p_cfg->type == BBOX_CTRL_RECORD && bbox_ctrl.flash[BBOX_FLASH_RECORD].p_cfg == NULL){
            bbox_ctrl.flash[BBOX_FLASH_RECORD].p_cfg = p_cfg;
            bbox_ctrl.flash[BBOX_FLASH_RECORD].data_len = p_cfg->dat_size;
            __bbox_data_init(&bbox_ctrl.flash[BBOX_FLASH_RECORD]);
        }else{
            DBG_E("bbox type :%d repeat!", p_cfg->type);
        }

        i++;
    }
    
    bbox_ctrl.fsm.cur_state = bbox_fsm_list[0].state;
    bbox_ctrl.fsm.cur_param = &bbox_fsm_list[0];
    bbox_ctrl.save_period_ms = 12 * 1000;
    bbox_ctrl.upload_period_ms = 1000;
}

REG_INIT_MOD_BASIC(__bbox_init);

static uint8_t __flash_data_type_find(BBOX_FLASH_DATA_T *p_flash)
{
    uint8_t i;
    for(i = BBOX_FLASH_NORMAL; i < BBOX_FLASH_MAX; i++){
        if(p_flash == &bbox_ctrl.flash[i]){
            return i * 5 + 0x05;
        }
    }

    return BBOX_FLASH_NORMAL;
}

static void __hide_data_pack(BBOX_FLASH_DATA_T *p_flash, BBOX_PARAM_T *p_param, uint8_t *p_buf)
{
    uint8_t flash_type = __flash_data_type_find(p_flash);

    memcpy(p_buf, &flash_type, sizeof(flash_type));
    p_buf += sizeof(flash_type);
    memcpy(p_buf, &p_flash->index, sizeof(p_flash->index));
    p_buf += (sizeof(p_flash->index) + sizeof(BBOX_TIME_T));
    memcpy(p_buf, &p_param->event_name, sizeof(p_param->event_name) + sizeof(p_param->event_type));
}

static void __bbox_time_update(void)
{
    X_CALENDAR_DATE date;
    get_calendar_date(&date);

    bbox_ctrl.time.year = date.year;
    bbox_ctrl.time.month = date.month;
    bbox_ctrl.time.day = date.day;
    bbox_ctrl.time.hour = date.hour;
    bbox_ctrl.time.min = date.min;
    bbox_ctrl.time.sec = date.sec;
    bbox_ctrl.time.zone = get_time_zone();
}

static void __bbox_record_head_data_get(BBOX_FLASH_CFG_T *p_config, BBOX_PARAM_T *p_param)
{
    if(p_config->head_cnt == 0){
        return;
    }
    
    BBOX_HIDE_DATA_T data = {0};
    uint8_t index = p_param->head_read_index;
    
    memcpy(&data.time, &bbox_ctrl.time,sizeof(bbox_ctrl.time));
    data.bbox_ver = (((BBOX_MAIN_VER) << 24) | ((BBOX_SUB_VER) << 16) | (BBOX_DBG_VER << 8) | (BBOX_TEST_VER));

    p_config->pfn_data_get(&data, p_config->p_buf + index * p_config->buf_size, p_config->buf_size);
    p_param->head_read_index = (p_param->head_read_index + 1) % p_config->head_cnt;
    if(p_param->head_read_cnt < p_config->head_cnt){
        p_param->head_read_cnt++;
    }
}

static void __bbox_record_tail_data_get(BBOX_FLASH_CFG_T *p_config, BBOX_PARAM_T *p_param)
{
    if(p_config->tail_cnt == 0){
        return;
    }
    
    BBOX_HIDE_DATA_T data = {0};
    uint8_t index = p_config->head_cnt + p_param->tail_read_index;
    
    memcpy(&data.time, &bbox_ctrl.time,sizeof(bbox_ctrl.time));
    data.bbox_ver = (((BBOX_MAIN_VER) << 24) | ((BBOX_SUB_VER) << 16) | (BBOX_DBG_VER << 8) | (BBOX_TEST_VER));

    p_config->pfn_data_get(&data, p_config->p_buf + index * p_config->buf_size, p_config->buf_size);
    p_param->tail_read_index = (p_param->tail_read_index + 1) % p_config->tail_cnt;
    if(p_param->tail_read_cnt < p_config->tail_cnt){
        p_param->tail_read_cnt++;
    }
}

static void __bbox_head_data_save_recent(BBOX_PARAM_T *p_param, BBOX_FLASH_DATA_T *p_flash)
{
    if(p_flash->is_read_all){
        return;
    }
    BBOX_FLASH_CFG_T *p_config = p_flash->p_cfg;
    uint32_t start_save_index = (p_param->head_read_index + p_config->head_cnt - 1) % p_config->head_cnt;

    if(p_param->head_read_cnt != 0){
//        DBG_I("save index:%d", p_flash->index);
        __bbox_flash_index_write(p_flash, (uint8_t *)&p_flash->index, sizeof(p_flash->index));        

        __hide_data_pack(p_flash, p_param, &p_flash->p_cfg->p_buf[p_flash->p_cfg->buf_size * start_save_index]);
        
        __bbox_flash_data_write(p_flash, &p_flash->p_cfg->p_buf[p_flash->p_cfg->buf_size * start_save_index], p_flash->p_cfg->buf_size);
        p_flash->index++;
        p_param->head_read_cnt--;
    }
}

static void __bbox_tail_data_save_recent(uint16_t name, uint16_t type, BBOX_PARAM_T *p_param, BBOX_FLASH_DATA_T *p_flash)
{
    if(p_flash->is_read_all){
        return;
    }
    uint32_t start_save_index = (p_param->tail_read_index + p_flash->p_cfg->tail_cnt - 1) % p_flash->p_cfg->tail_cnt;
    BBOX_PARAM_T param = {0};
    
    if(p_param->tail_read_cnt != 0){
//        DBG_I("save index:%d", p_flash->index);
        __bbox_flash_index_write(p_flash, (uint8_t *)&p_flash->index, sizeof(p_flash->index));
        
        memcpy(&param, p_param, sizeof(BBOX_PARAM_T));
        param.event_name = name;
        param.event_type = type;
        __hide_data_pack(p_flash, &param, &p_flash->p_cfg->p_buf[p_flash->p_cfg->buf_size * (start_save_index + p_flash->p_cfg->head_cnt)]);

        __bbox_flash_data_write(p_flash, &p_flash->p_cfg->p_buf[p_flash->p_cfg->buf_size * (start_save_index + p_flash->p_cfg->head_cnt)], p_flash->p_cfg->buf_size);
        p_flash->index++;
        p_param->tail_read_cnt--;
        if(0 == p_param->tail_read_cnt){
            p_param->read_tail = 0;
        }
    }    
}

static void __bbox_data_save_all(BBOX_PARAM_T *p_param, BBOX_FLASH_DATA_T *p_flash)
{
    if(p_flash->is_read_all){
        return;
    }
    
    BBOX_FLASH_CFG_T *p_config = p_flash->p_cfg;
    uint32_t start_save_index = 0;
    uint32_t i;
    
    if(p_param->head_read_cnt != 0){
        start_save_index = (p_param->head_read_index + p_config->head_cnt - p_param->head_read_cnt) % p_config->head_cnt;
        for(i = 0; i < p_param->head_read_cnt; i++){     
//            DBG_I_BLOCK(10, "save index:%d", p_flash->index);
            __bbox_flash_index_write(p_flash, (uint8_t *)&p_flash->index, sizeof(p_flash->index)); 
            __hide_data_pack(p_flash, p_param, &p_flash->p_cfg->p_buf[p_flash->p_cfg->buf_size * start_save_index]);
            __bbox_flash_data_write(p_flash, &p_config->p_buf[p_config->buf_size * start_save_index], p_config->buf_size);
            start_save_index = (start_save_index + 1) % p_config->head_cnt;
            p_flash->index++;
        }
        
        p_param->head_read_cnt = 0;
    }
    
    if(p_param->read_tail && p_param->tail_read_cnt != 0){
        start_save_index = (p_param->tail_read_index + p_config->tail_cnt - p_param->tail_read_cnt) % p_config->tail_cnt;
        for(i = 0; i < p_param->tail_read_cnt; i++){
//            DBG_I_BLOCK(10,"save index:%d", p_flash->index);
            __bbox_flash_index_write(p_flash, (uint8_t *)&p_flash->index, sizeof(p_flash->index));
            __hide_data_pack(p_flash, p_param, &p_flash->p_cfg->p_buf[p_flash->p_cfg->buf_size * (start_save_index + p_config->head_cnt)]);

            __bbox_flash_data_write(p_flash, &p_config->p_buf[p_config->buf_size * (start_save_index + p_config->head_cnt)], p_config->buf_size);
            start_save_index = (start_save_index + 1) % p_config->tail_cnt;
            p_flash->index++;
        }

        p_param->tail_read_cnt = 0;
    }

    p_param->read_tail = 0;
    
}

static bool __bbox_data_upload(BBOX_UPLOAD_DATA_T *p_upload, BBOX_FLASH_DATA_T *p_flash, uint32_t index)
{
    ///> 如果数据破坏，无法正常读取或者校验通过，则强制将所有数据置0，用来表示该数据不可参考
    if(0 != __bbox_flash_data_read_safe(p_flash, index, NULL, 0)){
        memset(p_flash->p_cfg->p_flash_buf, 0, p_flash->p_cfg->dat_size);
        *(p_flash->p_cfg->p_flash_buf + CRC_LEN) = __flash_data_type_find(p_flash);
        memcpy(p_flash->p_cfg->p_flash_buf + CRC_LEN + TYPE_LEN, &index, sizeof(index));
    }
    
    if(p_flash->p_cfg->pfn_data_set != NULL) {\
        p_flash->p_cfg->pfn_data_set(p_flash->p_cfg->p_flash_buf + CRC_LEN, p_flash->p_cfg->dat_size - CRC_LEN);
    }

    if(p_flash->p_cfg->pfn_data_upload != NULL) {
        return p_flash->p_cfg->pfn_data_upload(false, p_upload->link_id, p_upload->dest_addr);
    }    
    return true;
}

static void __bbox_upload_handle(void)
{
    if(bbox_ctrl.upload.cfg_retry_cnt > 0){
        RUN_BY_LIMIT_BLOCK(300, 
            if(true == bbox_ctrl.flash[bbox_ctrl.upload.data_type].p_cfg->pfn_data_upload(true, bbox_ctrl.upload.link_id, bbox_ctrl.upload.dest_addr)){
                bbox_ctrl.upload.cfg_retry_cnt--;
            }
//                DBG_I_BLOCK(10,"upload cfg");
        );
        bbox_ctrl.upload.upload_base_time = get_sys_ms();
        return;
    }

    if(bbox_ctrl.upload.finish_retry_cnt > 0){
        BBOX_UPLOAD_FINISH_T finish = {(bbox_ctrl.upload.data_type + 1) * 0x05, 0, (bbox_ctrl.upload.state && (bbox_ctrl.upload.remain_len > 0)) ? (0) : (1)};
        hal_frame_info_t frame_info = {bbox_ctrl.upload.link_id, 0, bbox_ctrl.upload.dest_addr};
        RUN_BY_LIMIT_BLOCK(300,
                frame_info.cmd_func = FUNC_CMDSET_REC;
                frame_info.cmd_id = CMD_ID_BBOX_FINISH_UPLOAD;
                frame_info.data_len = sizeof(BBOX_UPLOAD_FINISH_T);
                frame_info.pdata = (uint8_t *)&finish;
                if(0 == protocol_send_by_id(&frame_info)){bbox_ctrl.upload.finish_retry_cnt--;};
        );
        bbox_ctrl.upload.upload_base_time = get_sys_ms();
        return;
    }
    
    if(bbox_ctrl.upload.state){
        bbox_ctrl.upload.upload_base_time = get_sys_ms();
        if(bbox_ctrl.upload.remain_len > 0){
            for(; bbox_ctrl.upload.bits_index < bbox_ctrl.upload.upload_len; bbox_ctrl.upload.bits_index++){
                if(( bbox_ctrl.upload.upload_bits[bbox_ctrl.upload.bits_index >> 3] & (1 << (bbox_ctrl.upload.bits_index & 0x07)) ) == 0){                    
                    if(true == __bbox_data_upload(&bbox_ctrl.upload, &bbox_ctrl.flash[bbox_ctrl.upload.data_type], bbox_ctrl.upload.start_index + bbox_ctrl.upload.bits_index)){
                        bbox_ctrl.upload.upload_bits[bbox_ctrl.upload.bits_index >> 3] |= (1 << (bbox_ctrl.upload.bits_index & 0x07));
                        bbox_ctrl.upload.remain_len--;
                        bbox_ctrl.upload.bits_index++;
                        bbox_ctrl.flash[bbox_ctrl.upload.data_type].refuse_time = get_sys_ms();
                    }
                    return;
                }
            }           
            bbox_ctrl.upload.remain_len = 0;
            bbox_ctrl.upload.state = 0;
        }else{
            bbox_ctrl.upload.state = 0;
        }
        bbox_ctrl.upload.finish_retry_cnt = UPLOAD_RETRY_COUNT;
        return;
    }else{
        if(bbox_ctrl.upload.remain_len > 0 && check_time_out(bbox_ctrl.upload.upload_base_time, BBOX_UPLOAD_TIMEOUT_MS)){
            bbox_ctrl.upload.remain_len = 0;    ///< 置零后才能响应新的读取指令
        }
    }

}

static int32_t __bbox_fsm_state_change(uint8_t new_state, uint16_t name, uint16_t type)
{
    if(new_state < BBOX_FSM_MAX){
        if(bbox_ctrl.fsm.new_state == BBOX_FSM_ERROR_STATE && new_state > BBOX_FSM_ERROR_STATE && bbox_ctrl.error_remained > 0){
            return -1;
        }
        if(bbox_ctrl.fsm.new_state == BBOX_FSM_FAULT_STATE && new_state > BBOX_FSM_FAULT_STATE && bbox_ctrl.error_remained > 0){
            return -1;
        }
        bbox_ctrl.fsm.new_state = new_state;
        bbox_ctrl.fsm.change_flag = 1;
        bbox_ctrl.param[BBOX_CTRL_NORMAL].event_name = name;
        bbox_ctrl.param[BBOX_CTRL_NORMAL].event_type = type;
        return 0;
//        DBG_I("change new state:%d", new_state);
    }
    return -1;
}

static void __bbox_fsm_in_func(void)
{
    RUN_BY_LIMIT_BLOCK(1000, 
        __bbox_time_update();
    
        if(bbox_ctrl.param[BBOX_CTRL_NORMAL].on_off){
            __bbox_record_head_data_get(bbox_ctrl.flash[BBOX_FLASH_NORMAL].p_cfg, &bbox_ctrl.param[BBOX_CTRL_NORMAL]);
        }

        if(bbox_ctrl.param[BBOX_CTRL_RECORD].read_finish){
            __bbox_data_save_all(&bbox_ctrl.param[BBOX_CTRL_RECORD], &bbox_ctrl.flash[BBOX_FLASH_RECORD]);
            bbox_ctrl.param[BBOX_CTRL_RECORD].read_finish = 0;
        }
        
    );
}
static void __bbox_fsm_out_func(void)
{
    RUN_BY_LIMIT_BLOCK(bbox_ctrl.upload_period_ms, __bbox_upload_handle(););

    uint8_t i;
    RUN_BY_LIMIT_BLOCK(1000, 
        for(i = 0; i < BBOX_FLASH_MAX; i++){
            if(bbox_ctrl.flash[i].is_read_all && check_time_out(bbox_ctrl.flash[i].refuse_time, 30 * 1000)){
                bbox_ctrl.flash[i].is_read_all = 0;
            }
        }
    );
}

static void __bbox_fsm_normal_exec_func(void)
{
    RUN_BY_LIMIT_BLOCK(bbox_ctrl.save_period_ms, __bbox_head_data_save_recent(&bbox_ctrl.param[BBOX_CTRL_NORMAL], \
                        &bbox_ctrl.flash[BBOX_FLASH_NORMAL]));
}

static void __bbox_fsm_error_in_func(void)
{
    __bbox_data_save_all(&bbox_ctrl.param[BBOX_CTRL_NORMAL], &bbox_ctrl.flash[BBOX_FLASH_FAULT]);
    bbox_ctrl.error_remained = bbox_ctrl.flash[BBOX_FLASH_FAULT].p_cfg->head_cnt;
}

static void __bbox_fsm_fault_in_func(void)
{
    __bbox_data_save_all(&bbox_ctrl.param[BBOX_CTRL_NORMAL], &bbox_ctrl.flash[BBOX_FLASH_NORMAL]);
    bbox_ctrl.error_remained = bbox_ctrl.flash[BBOX_FLASH_FAULT].p_cfg->head_cnt;
}

static void __bbox_fsm_error_exec_func(void)
{
    RUN_BY_LIMIT_BLOCK(1000, 
        if(bbox_ctrl.error_remained > 0 && bbox_ctrl.param[BBOX_CTRL_NORMAL].head_read_cnt > 0){
            __bbox_head_data_save_recent(&bbox_ctrl.param[BBOX_CTRL_NORMAL], &bbox_ctrl.flash[BBOX_FLASH_FAULT]);
            bbox_ctrl.error_remained--;
        }
        if(0 == bbox_ctrl.error_remained){
            __bbox_fsm_state_change(BBOX_FSM_NORMAL_STATE, bbox_ctrl.normal_event_name, bbox_ctrl.normal_event_type);
        }
   );
}

static void __bbox_fsm_fault_exec_func(void)
{
    RUN_BY_LIMIT_BLOCK(1000, 
        if(bbox_ctrl.error_remained > 0 && bbox_ctrl.param[BBOX_CTRL_NORMAL].head_read_cnt > 0){
            __bbox_head_data_save_recent(&bbox_ctrl.param[BBOX_CTRL_NORMAL], &bbox_ctrl.flash[BBOX_FLASH_NORMAL]);
            bbox_ctrl.error_remained--;
        }
        if(0 == bbox_ctrl.error_remained){
            __bbox_fsm_state_change(BBOX_FSM_NORMAL_STATE, bbox_ctrl.normal_event_name, bbox_ctrl.normal_event_type);
        }
   );
}

static void __bbox_fsm_task(void)
{
    __bbox_fsm_in_func();

    if(bbox_ctrl.fsm.change_flag != 0){
        bbox_ctrl.fsm.cur_state = bbox_ctrl.fsm.new_state;
        bbox_ctrl.fsm.change_flag = 0;
        
        if(bbox_ctrl.fsm.cur_param->state_out != NULL){
            bbox_ctrl.fsm.cur_param->state_out();  
        }

        bbox_ctrl.fsm.cur_param = &bbox_fsm_list[bbox_ctrl.fsm.cur_state];
        if(bbox_ctrl.fsm.cur_param->state_in != NULL){
            bbox_ctrl.fsm.cur_param->state_in();
        }
        
    }else{
        if(bbox_ctrl.fsm.cur_param != NULL && bbox_ctrl.fsm.cur_param->state_exec != NULL){
            bbox_ctrl.fsm.cur_param->state_exec();
        }
    }
    
    __bbox_fsm_out_func();
}

REG_TASK(ID_TASK, 50, __bbox_fsm_task);

static int __bbox_data_type_exchange(uint8_t set_type, uint8_t *p_type_result)
{
    if(set_type == 0x05){
		*p_type_result = BBOX_FLASH_NORMAL;
	}else if(set_type == 0x0A){
		*p_type_result = BBOX_FLASH_FAULT;
	}else if(set_type == 0x0F){
        *p_type_result = BBOX_FLASH_RECORD;
    }else{//不是以上两种类型，直接返回
		return -1;
    }

    return 0;
}

//读取数据
int32_t bbox_upload_param_set(hal_frame_info_t* p_frame_info)
{
	BBOX_UPLOAD_PARAM_T read_cmd = {0};
    BBOX_ACK_TYPE_T read_ack={0};   
	uint8_t data_type ;
    uint32_t index_tmp = 0;

	if(p_frame_info->data_len == sizeof(BBOX_UPLOAD_PARAM_T)){
		memcpy((uint8_t *)&read_cmd, (uint8_t *)p_frame_info->pdata, p_frame_info->data_len);

		if(__bbox_data_type_exchange(read_cmd.data_type, &data_type) < 0){
            return -1;
        }

        ///> 未使能该类型的黑匣子功能
        if(bbox_ctrl.flash[data_type].p_cfg == NULL){
            read_ack.data_type = read_cmd.data_type;
            protocol_send_ack(p_frame_info, (uint8_t*)&read_ack, sizeof(read_ack));
            return -1;
        }

        if(0 == bbox_ctrl.upload.remain_len){
            if(read_cmd.page_size >= bbox_ctrl.flash[data_type].data_act_cnt || read_cmd.page_num == 0 || read_cmd.page_size == 0){
                bbox_ctrl.upload.upload_len = index_tmp = bbox_ctrl.flash[data_type].data_act_cnt;
            }else{
                index_tmp = read_cmd.page_num * read_cmd.page_size;
                index_tmp = (index_tmp > bbox_ctrl.flash[data_type].data_act_cnt) ? (bbox_ctrl.flash[data_type].data_act_cnt) : (index_tmp);
                bbox_ctrl.upload.upload_len = X_FRAME_MIN(read_cmd.page_size, index_tmp);
            }

            bbox_ctrl.upload.data_type = data_type;
            bbox_ctrl.upload.start_index = bbox_ctrl.flash[data_type].index - index_tmp;
            bbox_ctrl.upload.stop_index = bbox_ctrl.upload.start_index + bbox_ctrl.upload.upload_len;

            if(bbox_ctrl.upload.upload_len == bbox_ctrl.flash[data_type].data_act_cnt){   ///< 读取全部黑匣子分区数据
                bbox_ctrl.flash[data_type].is_read_all = 1;
                bbox_ctrl.flash[data_type].refuse_time = get_sys_ms();
            }
            
            if(bbox_ctrl.upload.upload_len > BBOX_PAGE_SIZE_MAX){
                bbox_ctrl.upload.upload_len = BBOX_PAGE_SIZE_MAX;
            }
            bbox_ctrl.upload.remain_len = bbox_ctrl.upload.upload_len;
    		bbox_ctrl.upload.dest_addr = p_frame_info->src; 
    		bbox_ctrl.upload.link_id = p_frame_info->link_id;
            bbox_ctrl.upload.cfg_retry_cnt = 0;     ///< 立即中断当前的配置帧上报
            bbox_ctrl.upload.finish_retry_cnt = 0;  ///< 立即中断当前的"数据上报结束"上报
            bbox_ctrl.upload.upload_base_time = get_sys_ms();
        }
        
        read_ack.data_type   = (bbox_ctrl.upload.data_type + 1) * 0x05;
        read_ack.index_stop  = bbox_ctrl.upload.stop_index;
        read_ack.index_start = bbox_ctrl.upload.start_index;
        read_ack.cmd_func = FUNC_CMDSET_REC;
        read_ack.cmd_id = (bbox_ctrl.upload.data_type == BBOX_FLASH_RECORD) ? (CMD_ID_BBOX_RECORD_UPLOAD) : (CMD_ID_BBOX_NORMAL_UPLOAD);
        protocol_send_ack(p_frame_info, (uint8_t*)&read_ack, sizeof(read_ack));
//        DBG_I("len:%d,start:%d,stop:%d", bbox_ctrl.upload[data_type].upload_len, read_ack.index_start, read_ack.index_stop);
		if(bbox_ctrl.upload.upload_len != 0){
//			bbox_ctrl.upload[data_type].cfg_retry_cnt = 2;
			if(read_cmd.data_interval_time > BBOX_DATA_SEND_MAX_INTERVAL_TIME_MS)
				bbox_ctrl.upload_period_ms = BBOX_DATA_SEND_MAX_INTERVAL_TIME_MS;
			else if(read_cmd.data_interval_time < BBOX_DATA_SEND_MIN_INTERVAL_TIME_MS)
				bbox_ctrl.upload_period_ms = BBOX_DATA_SEND_MIN_INTERVAL_TIME_MS;
			else
				bbox_ctrl.upload_period_ms = read_cmd.data_interval_time;
		}
	}
 	return 0;
}

//补采数据
int32_t bbox_record_replenish_data_act(hal_frame_info_t* p_frame_info)
{
	BBOX_RE_READ_TYPE_T re_read_cmd = {0};
	uint8_t data_type ;
    uint16_t len = sizeof(BBOX_RE_READ_TYPE_T) + ((BBOX_PAGE_SIZE_MAX % 8) ? (BBOX_PAGE_SIZE_MAX / 8 + 1) : (BBOX_PAGE_SIZE_MAX / 8));
    
	if(p_frame_info->data_len == len)
    {
		memcpy((uint8_t *)&re_read_cmd, (uint8_t *)p_frame_info->pdata, sizeof(re_read_cmd));
		if(__bbox_data_type_exchange(re_read_cmd.data_type, &data_type) < 0){
            return -1;
        } 

        if(data_type == bbox_ctrl.upload.data_type && p_frame_info->src == bbox_ctrl.upload.dest_addr \
                && p_frame_info->link_id == bbox_ctrl.upload.link_id && bbox_ctrl.upload.upload_len > 0 && 0 == bbox_ctrl.upload.state){
            memcpy(bbox_ctrl.upload.upload_bits, p_frame_info->pdata + sizeof(BBOX_RE_READ_TYPE_T), sizeof(bbox_ctrl.upload.upload_bits));
            bbox_ctrl.upload.state = 1;
            bbox_ctrl.upload.remain_len = bbox_ctrl.upload.upload_len;  ///< 保证能继续触发上报，并且能阻止用户在当前过程去读取其它类型的数据
            bbox_ctrl.upload.bits_index = 0;
            bbox_ctrl.upload.start_index = re_read_cmd.data_index;
            bbox_ctrl.flash[data_type].refuse_time = get_sys_ms();
            
            if(re_read_cmd.data_index < (bbox_ctrl.flash[data_type].index - bbox_ctrl.flash[data_type].data_act_cnt)){
                re_read_cmd.data_index = bbox_ctrl.flash[data_type].index - bbox_ctrl.flash[data_type].data_act_cnt;
                for(; bbox_ctrl.upload.bits_index < (bbox_ctrl.upload.bits_index + (re_read_cmd.data_index - bbox_ctrl.upload.start_index)); bbox_ctrl.upload.bits_index++){
                        bbox_ctrl.upload.upload_bits[bbox_ctrl.upload.bits_index >> 3] |= (1 << (bbox_ctrl.upload.bits_index & 0x07));
                        bbox_ctrl.upload.remain_len--;
                        bbox_ctrl.upload.bits_index++;
                }
            }
            protocol_send_ack(p_frame_info, (uint8_t*)&re_read_cmd, sizeof(re_read_cmd));
        }
	}   
 	return 0;
}

//命令，停止/清除数据
int32_t bbox_record_cmd_act(hal_frame_info_t* p_frame_info)
{
	uint8_t record_cmd = 0;
	static uint32_t erase_time = 0;

	if(p_frame_info->data_len == sizeof(uint8_t))
	{
		memcpy((uint8_t *)&record_cmd, (uint8_t *)p_frame_info->pdata, p_frame_info->data_len);
		switch(record_cmd){
			case CMD_REC_STOP:
                memset(&bbox_ctrl.upload, 0, sizeof(bbox_ctrl.upload));
                protocol_send_ack(p_frame_info, &record_cmd, sizeof(record_cmd));
				break;
			case CMD_REC_ERASE:
				if(check_time_out(erase_time, 10000))
				{
					memset(&bbox_ctrl.upload, 0, sizeof(bbox_ctrl.upload));
                    if(bbox_ctrl.flash[BBOX_FLASH_NORMAL].p_cfg != NULL) { 
                        __bbox_flash_cur_index_erase(&bbox_ctrl.flash[BBOX_FLASH_NORMAL]);
                        bbox_ctrl.flash[BBOX_FLASH_NORMAL].index = 0;
                        bbox_ctrl.flash[BBOX_FLASH_NORMAL].data_act_cnt = 0;
                    }
                    if(bbox_ctrl.flash[BBOX_FLASH_RECORD].p_cfg != NULL) {
                        __bbox_flash_cur_index_erase(&bbox_ctrl.flash[BBOX_FLASH_RECORD]);
                        bbox_ctrl.flash[BBOX_FLASH_RECORD].index = 0;
                        bbox_ctrl.flash[BBOX_FLASH_RECORD].data_act_cnt = 0;
                    }
					erase_time = get_sys_ms();
                    protocol_send_ack(p_frame_info, &record_cmd, sizeof(record_cmd));
				}
				break;
			default:
				break;
		}
	}
 	return 0;
}

int32_t bbox_config_upload_set(hal_frame_info_t* p_frame_info)
{
    BBOX_REC_CONFIG_T config;
    uint8_t data_type ;
    
    if(p_frame_info->data_len == sizeof(BBOX_REC_CONFIG_T)){
        memcpy((uint8_t *)&config, (uint8_t *)p_frame_info->pdata, p_frame_info->data_len);
        if(__bbox_data_type_exchange(config.data_type, &data_type) < 0){
            return -1;
        } 
        
        if(data_type == bbox_ctrl.upload.data_type){
            bbox_ctrl.upload.cfg_retry_cnt = (config.state) ? (UPLOAD_RETRY_COUNT) : (0);
            protocol_send_ack(p_frame_info, (uint8_t*)&config, sizeof(config));
        }
    }
    return 0;
}

int32_t bbox_finish_upload_set(hal_frame_info_t* p_frame_info)
{
    BBOX_UPLOAD_FINISH_T finish;
    uint8_t data_type ;
    
    if(p_frame_info->data_len == sizeof(BBOX_UPLOAD_FINISH_T)){
        memcpy((uint8_t *)&finish, (uint8_t *)p_frame_info->pdata, p_frame_info->data_len);
        if(__bbox_data_type_exchange(finish.data_type, &data_type) < 0){
            return -1;
        }
        
        if(data_type == bbox_ctrl.upload.data_type){
            if(finish.cmd_type == 1){ ///< 查询
                bbox_ctrl.upload.finish_retry_cnt = UPLOAD_RETRY_COUNT;
                protocol_send_ack(p_frame_info, (uint8_t*)&finish, sizeof(finish));
            }else if(finish.cmd_type == 2){ ///< 停止
                bbox_ctrl.upload.finish_retry_cnt = 0;
            }
        }
    }
    return 0;
}

void bbox_state_set(uint16_t name, uint16_t type, XF_BBOX_STATE_E state)
{
    uint8_t last = bbox_ctrl.param[BBOX_CTRL_NORMAL].on_off;

    bbox_ctrl.param[BBOX_CTRL_NORMAL].on_off = 1;

    if(state < BBOX_MAX){
        __bbox_record_tail_data_get(bbox_ctrl.flash[BBOX_FLASH_FAULT].p_cfg, &bbox_ctrl.param[BBOX_CTRL_NORMAL]);

        if(state == BBOX_FAULT_STATE || state == BBOX_ERROR_STATE){
            uint16_t event[2] = {bbox_ctrl.param[BBOX_CTRL_NORMAL].event_name, bbox_ctrl.param[BBOX_CTRL_NORMAL].event_type};
            bbox_ctrl.param[BBOX_CTRL_NORMAL].event_name = name;
            bbox_ctrl.param[BBOX_CTRL_NORMAL].event_type = type;
            BBOX_FLASH_DATA_T *p_flash = (state == BBOX_ERROR_STATE) ? (&bbox_ctrl.flash[BBOX_FLASH_FAULT]) : (&bbox_ctrl.flash[BBOX_FLASH_NORMAL]);
            __bbox_data_save_all(&bbox_ctrl.param[BBOX_CTRL_NORMAL], p_flash);
            bbox_ctrl.param[BBOX_CTRL_NORMAL].event_name = event[0];
            bbox_ctrl.param[BBOX_CTRL_NORMAL].event_type = event[1];
        }else{
            __bbox_tail_data_save_recent(name, type, &bbox_ctrl.param[BBOX_CTRL_NORMAL], &bbox_ctrl.flash[BBOX_FLASH_NORMAL]);
        }

    }
    
    switch(state){
        case BBOX_NORMAL_STATE_120_SEC:
            bbox_ctrl.normal_event_name = name;
            bbox_ctrl.normal_event_type = type;
            bbox_ctrl.save_period_ms = 120 * 1000;
            __bbox_fsm_state_change(BBOX_FSM_NORMAL_STATE, name, type);
        break;
        
        case BBOX_NORMAL_STATE_5_SEC:
            bbox_ctrl.normal_event_name = name;
            bbox_ctrl.normal_event_type = type;
            bbox_ctrl.save_period_ms = 5 * 1000;
            __bbox_fsm_state_change(BBOX_FSM_NORMAL_STATE, name, type);
        break;

        case BBOX_NORMAL_STATE_1_SEC:
            bbox_ctrl.normal_event_name = name;
            bbox_ctrl.normal_event_type = type;
            bbox_ctrl.save_period_ms = 1 * 1000;
            __bbox_fsm_state_change(BBOX_FSM_NORMAL_STATE, name, type);
        break;
        
        case BBOX_ERROR_STATE:
            __bbox_fsm_state_change(BBOX_FSM_ERROR_STATE, name, type);
        break;

        case BBOX_FAULT_STATE:
            __bbox_fsm_state_change(BBOX_FSM_FAULT_STATE, name, type);
        break;
        
        case BBOX_TRIGGER_STATE_TO_NORMAL:
            __bbox_fsm_state_change(BBOX_FSM_NORMAL_STATE, bbox_ctrl.normal_event_name, bbox_ctrl.normal_event_type);
        break;

        case BBOX_TRIGGER_STATE_TO_CLOSE:
        case BBOX_CLOSE:
            bbox_ctrl.param[BBOX_CTRL_NORMAL].on_off = 0;
            __bbox_fsm_state_change(BBOX_FSM_CLOSE_STATE, name, type);
        break;

        default:
            bbox_ctrl.param[BBOX_CTRL_NORMAL].on_off = last;
            return;
    }
}

void bbox_data_record(void)
{
    if(bbox_ctrl.param[BBOX_CTRL_RECORD].read_finish == 1){
        return;
    }
    
    if(bbox_ctrl.param[BBOX_CTRL_RECORD].read_tail == 0){
        __bbox_record_head_data_get(bbox_ctrl.flash[BBOX_FLASH_RECORD].p_cfg, &bbox_ctrl.param[BBOX_CTRL_RECORD]);
    }else{
        __bbox_record_tail_data_get(bbox_ctrl.flash[BBOX_FLASH_RECORD].p_cfg, &bbox_ctrl.param[BBOX_CTRL_RECORD]);
        if(bbox_ctrl.param[BBOX_CTRL_RECORD].tail_read_cnt == bbox_ctrl.flash[BBOX_FLASH_RECORD].p_cfg->tail_cnt){
            bbox_ctrl.param[BBOX_CTRL_RECORD].read_finish = 1;
        }
    }
}

void bbox_recorded_data_save(uint16_t name, uint16_t type)
{
    if(bbox_ctrl.param[BBOX_CTRL_RECORD].read_finish == 1 || bbox_ctrl.param[BBOX_CTRL_RECORD].read_tail == 1){
        return;
    }

    bbox_ctrl.param[BBOX_CTRL_RECORD].event_name = name;
    bbox_ctrl.param[BBOX_CTRL_RECORD].event_type = type;

    if(0 == bbox_ctrl.flash[BBOX_FLASH_RECORD].p_cfg->tail_cnt){
        bbox_ctrl.param[BBOX_CTRL_RECORD].read_finish = 1;
    }else{
        bbox_ctrl.param[BBOX_CTRL_RECORD].read_tail = 1;
    }
}

