/********************** File Info ***************************
** File name:               can_s.h
** Latest modified date:    2021-11-21
** Latest version:          V1.0.0
** Description:             can总线 - 字符流
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2021-11-21
** Version:                 V1.0.0
** Descriptions:            can总线 - 字符流
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#ifndef can_src_h
#define can_src_h

#include "x_frame.h"
#include "xh_can_s.h"
#include "xh_can.h"
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "string.h"

/* 以下类型，应该放入xh_can.h文件中 */
#define CAN_ID_TYPE_STANDARD                0   //标准帧类型
#define CAN_ID_TYPE_EXTEND                  1   //扩展帧类型

#define CAN_SRC_PACK_DAT_LEN            8   //CAN帧数据最大长度。只能为8

/**
 * CAN ID构成：
 * | Bit[28]   | Bit[27~22]    | Bit[21~16]    | Bit[15~8] | Bit[7~0]      |
 * |-----------|---------------|---------------|-----------|---------------|
 * | Ver(1Bit) | cnt(6Bit)     | total(6Bit)   | src[8Bit] | dest[8Bit]    |
 * 
 */
typedef struct _CAN_SRC_ID_OBJ {
    uint32_t dest:          8;          /* 目的地址 */
    uint32_t src:           8;          /* 源地址 */
    uint32_t total_pack:    6;          /* 总包数 */
    uint32_t curr_no:       6;          /* 当前包序号 */
    uint32_t ver:           1;          /* 恒为0 */
    
    /* 内部使用 */
    uint32_t whole:         1;          /* 接收完整包标记 */
}CAN_SRC_ID_OBJ ,*P_CAN_SRC_ID_OBJ;

typedef struct _CAN_SRC_CACHE_OBJ CAN_SRC_CACHE_OBJ ,*P_CAN_SRC_CACHE_OBJ;

typedef struct _CAN_SRC_PACK_OBJ {
    CAN_SRC_ID_OBJ id;
    uint8_t len;
    uint8_t data[CAN_SRC_PACK_DAT_LEN];
}CAN_SRC_PACK_OBJ ,*P_CAN_SRC_PACK_OBJ;

struct _CAN_SRC_CACHE_OBJ {
    CAN_SRC_PACK_OBJ pack;
    uint32_t rece_time;
    P_CAN_SRC_CACHE_OBJ p_head_list;
    P_CAN_SRC_CACHE_OBJ p_son_list;
};

typedef struct _CAN_SRC_OBJ {
    void *const can_drv_id;
    uint8_t dev_addr;                   /* 设备地址 */
    P_CAN_SRC_CACHE_OBJ p_r_cache;
    uint16_t r_cache_num;
    uint32_t r_timeout;
    struct{
        uint32_t free_cache;            /* 空闲帧数目 */
        
        CAN_SRC_CACHE_OBJ norm_list;
        CAN_SRC_CACHE_OBJ empt_list;
        
        /* 临时解决缓存丢失问题 */
        //uint8_t reset_mark;
    }exec;
}CAN_SRC_OBJ ,*P_CAN_SRC_OBJ;

#define REG_CAN_COMP_DRV(id,prio,can_drv_id_,dev_s_addr,r_timeout,r_cache_num)                              \
        CAN_SRC_CACHE_OBJ g_can_src_r_cache_##id[r_cache_num];                                              \
        CAN_SRC_OBJ g_can_src_##id = {                                                                      \
            PTR_TYPE_XH_DEV(can_drv_id_),                                                                   \
            dev_s_addr,                                                                                     \
            g_can_src_r_cache_##id,                                                                         \
            r_cache_num,                                                                                    \
            r_timeout,                                                                                      \
        };                                                                                                  \
        void can_src_rece_pack_##id(uint32_t pack_type, uint32_t pack_id, uint8_t *p_pack, uint32_t pack_len)       \
        {                                                                                                           \
            can_src_rece_pack(&g_can_src_##id,pack_type,pack_id,p_pack,pack_len);                                   \
        }                                                                                                           \
        void can_src_init_##id(P_CAN_SRC_OBJ p_obj)                                                                 \
        {                                                                                                           \
            can_src_init(p_obj);                                                                                    \
            xh_can_reg_rx_irq_cb(p_obj->can_drv_id,can_src_rece_pack_##id);                                         \
        }                                                                                                           \
        void can_src_task_##id(void){can_src_task(&g_can_src_##id);} REG_TASK(VH_TASK, 10, can_src_task_##id);      \
        REG_XH_DEV(xh_stream_t,id,&g_can_src_##id,prio,DEV_AUTO_INIT,can_src_init_##id,NULL,                        \
            (xh_stream_read_obj)can_src_read_data,                                                                  \
            (xh_stream_write_obj)can_src_send_data,                                                                 \
            (xh_stream_set_baudrate_obj)can_src_set_baudrate,                                                       \
        )
        
extern void can_src_init(P_CAN_SRC_OBJ p_obj);
extern void can_src_task(P_CAN_SRC_OBJ p_obj);
        
/* 驱动层数据包发送和接收can帧接口 */
extern void can_src_rece_pack(P_CAN_SRC_OBJ p_obj, uint32_t pack_type, uint32_t pack_id, uint8_t *p_pack, uint32_t pack_len);

/* 应用层发送和读取字符流数据接口 */
extern uint32_t can_src_send_data(P_CAN_SRC_OBJ p_obj, uint8_t *p_data,uint32_t len);
extern uint32_t can_src_read_data(P_CAN_SRC_OBJ p_obj, uint8_t *p_data,uint32_t len);
extern uint32_t can_src_set_baudrate(P_CAN_SRC_OBJ p_obj, uint32_t baudrate);
#endif /* can_src_h */



