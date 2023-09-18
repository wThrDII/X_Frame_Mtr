/**
 * @brief xf_bbox.c header file
 * @file xf_bbox.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-15 10:37:20
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-15 10:37:20
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __XF_BBOX_H__
#define __XF_BBOX_H__

#define BBOX_MAX_READ_DATA_CNT  5       ///< 常规数据最多读取5个
#define BBOX_PAGE_SIZE_MAX      1000    ///< 数据读取的页大小最大值

enum{
    BBOX_CTRL_NORMAL = 0,
    BBOX_CTRL_RECORD,
    BBOX_CTRL_MAX
};

typedef enum{
    BBOX_NORMAL_STATE_120_SEC,          ///< 120秒保存一次
    BBOX_NORMAL_STATE_5_SEC,            ///< 5秒保存一次
    BBOX_NORMAL_STATE_1_SEC,            ///< 1秒保存一次
    BBOX_ERROR_STATE,                   ///< 进入永久故障处理，处理完成后自动切换到normal
    BBOX_FAULT_STATE,                   ///< 进入一般故障处理，处理完成后自动切换到normal
    BBOX_TRIGGER_STATE_TO_NORMAL,       ///< 读取后切换到normal
    BBOX_TRIGGER_STATE_TO_CLOSE,        ///< 读取后切换到close
    BBOX_CLOSE,                         ///< 关闭
    BBOX_MAX,
}XF_BBOX_STATE_E;
    
typedef struct{
    uint16_t    year;
    uint8_t     month;
    uint8_t     day;
    uint8_t     hour;
    uint8_t     min;
    uint8_t     sec;
	int8_t      zone;
}BBOX_TIME_T;

#pragma pack(1)

typedef struct{
    uint8_t     type;
    uint32_t    index;
    BBOX_TIME_T time;
    uint16_t	event_name;
    uint16_t	event_type;
    uint32_t    bbox_ver;
}BBOX_HIDE_DATA_T;

#pragma pack()

typedef struct{
    uint32_t normal_part_id;
    uint32_t fault_part_id;
    uint32_t record_part_id;
}BBOX_FLASH_PART_CFG_T;

typedef struct{
    void (*pfn_data_get)(BBOX_HIDE_DATA_T *p_param, uint8_t *p_buff, uint32_t len);
    void (*pfn_data_set)(uint8_t *p_buff, uint32_t len);
    bool (*pfn_data_upload)(bool rec_cfg, uint8_t link_id, uint8_t dest_addr);
    uint8_t *p_flash_buf;
    uint8_t *p_buf;
    uint32_t buf_size;  ///< buf块大小，当前实际的数据大小
    uint32_t dat_size;  ///< 存到flash的数据块大小
    uint8_t head_cnt;   ///< record类型下触发保存时之前需要临时记录数据的数量或者普通类型的总读取数量
    uint8_t tail_cnt;   ///< record类型下触发保存时之后需要临时记录数据的数量
    uint8_t type;       ///< 普通bbox类型或者record类型
}BBOX_FLASH_CFG_T;

#define REG_BBOX_LINK(link_id)                                              REG_LINK_ACTION(link_id,FUNC_CMDSET_REC,CMD_ID_BBOX_UPLOAD_SET,bbox_upload_param_set);\
                                                                            REG_LINK_ACTION(link_id,FUNC_CMDSET_REC,CMD_ID_BBOX_STOP_AND_ERASE,bbox_record_cmd_act);\
                                                                            REG_LINK_ACTION(link_id,FUNC_CMDSET_REC,CMD_ID_BBOX_REPLENISH,bbox_record_replenish_data_act);\
                                                                            REG_LINK_ACTION(link_id,FUNC_CMDSET_REC,CMD_ID_BBOX_CONFIG_UPLOAD,bbox_config_upload_set);\
                                                                            REG_LINK_ACTION(link_id,FUNC_CMDSET_REC,CMD_ID_BBOX_FINISH_UPLOAD,bbox_finish_upload_set);
#define REG_BBOX_FLASH(normal_part_id,fault_part_id,record_part_id)         static const BBOX_FLASH_PART_CFG_T __box_flash_cfg = {normal_part_id,fault_part_id,record_part_id};\
                                                                            AUTO_REG_ITEM(__box_flash_cfg,AUTO_REG_BBOX_FLASH,(void*)&__box_flash_cfg)

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

extern int32_t bbox_upload_param_set(hal_frame_info_t* p_frame_info);
extern int32_t bbox_record_replenish_data_act(hal_frame_info_t* p_frame_info);
extern int32_t bbox_record_cmd_act(hal_frame_info_t* p_frame_info);
extern int32_t bbox_config_upload_set(hal_frame_info_t* p_frame_info);
extern int32_t bbox_finish_upload_set(hal_frame_info_t* p_frame_info);

/**
 * @brief bbox状态切换     （保存normal和errord状态数据下使用，record不在切换状态内）
 * 
 * @param [in] name          事件名        dataview会根据设置的值显示对应的事件名称  
 * @param [in] type          事件类型      dataview会根据设置的值显示对应的事件类型     //事件名和事件类型有对应的维护表单，具体咨询平台组
 * @param [in] state         新的状态（  BBOX_NORMAL_STATE_120_SEC,          ///< 120秒保存一次
                                        BBOX_NORMAL_STATE_5_SEC,            ///< 5秒保存一次
                                        BBOX_NORMAL_STATE_1_SEC,            ///< 1秒保存一次
                                        BBOX_ERROR_STATE,                   ///< 进入错误处理，处理完成后自动切换到normal
                                        BBOX_TRIGGER_STATE_TO_NORMAL,       ///< 读取后切换到normal
                                        BBOX_TRIGGER_STATE_TO_CLOSE,        ///< 读取后切换到close
                                        BBOX_CLOSE,                         ///< 关闭               ）                            
 * @details 特殊说明: 
 * @par eg:     将bbox状态设置为：BBOX_NORMAL_STATE_120_SEC。
 * @code
 *      bbox_state_set(0, 0, BBOX_NORMAL_STATE_120_SEC);        //对应的事件名为：定时事件、事件类型为：静置
 * @endcode
 */
extern void bbox_state_set(uint16_t name, uint16_t type, XF_BBOX_STATE_E state);


/**
 * @brief bbox数据录制，每调用一次则读取一次数据        //与bbox_recorded_data_save接口连用实现数据录制功能
 * 
 * @details 特殊说明:      1. 在bbox_recorded_data_save接口调用前，每次读取的数据会存到bbox内部定义的头环形队列缓存区，
 *                           在bbox_recorded_data_save接口调用后，每次读取的数据会存到bbox内部定义的尾环形队列缓存区，
 *                           (头尾缓存区大小根据所配置的录制触发前后保存的数据量设定)；
 * @par eg:   1ms读取一次数据（由于没有触发保存，所读取的数据会在头环形队列缓存区不断更新，数据并没有存到flash中）。
 * @code
        void test_task(void)
        {
        bbox_data_record();  
        }
        REG_TASK(1,1,test_task)
 * @endcode
 */
extern void bbox_data_record(void);


/**
 * @brief 保存bbox录制的数据            //与bbox_data_record接口连用实现数据录制功能
 * 
* @param [in] name          事件名        dataview会根据设置的值显示对应的事件名称  
* @param [in] type          事件类型      dataview会根据设置的值显示对应的事件类型     //事件名和事件类型有对应的维护表单，具体咨询平台组
 * 
 * @details 特殊说明:   调用接口后不会立即保存数据到flash，会继续读取所配置的触发保存后录制的数据量(需继续调用bbox_data_record接口)，直至完成后才会推到flash。
 * @par eg:    录制触发保存的前后各5个数据。
 * @code
        uint32_t i = 0;
        void test_task(void)
        {
            if(i % 20 == 0)
            {
                bbox_recorded_data_save(0, 0);         //对应的事件名为：定时事件、事件类型为：静置
                i = 0;
            }
            bbox_data_record();
            i++;  
        }
        REG_TASK(1,1,test_task)

        //假设所录制的变量为i,则调用一次bbox_recorded_data_save(0, 0)最终i变量存到flash中的数值为：15，16，17，18，19，20，21，22，23，24.  
        //bbox_recorded_data_save(0, 0)在i=19和i=20之间触发。触发录制后，头环形队列缓存区保存着触发录制前bbox_data_record()读取的最新的5个数据（15，16，17，18，19），触发录制后将继续调用bbox_data_record（）读取5个数据（20，21，22，23，24）存到尾环形队列缓存区，
        //在bbox_data_record()读取完尾环形队列缓存区所需数据量后，则将头尾尾环形队列缓存区一起存到flash,完成一次数据录制的保存。
 * @endcode
 */
extern void bbox_recorded_data_save(uint16_t name, uint16_t type);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __XF_BBOX_H__
