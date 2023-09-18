/**
 * @brief xf_unix_time_update.c 的头文件
 * @file xf_unix_time_update.h
 * @version 1.0
 * @author hzf
 * @date 2023-1-6 21:10:24
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2023. All rights reserved.
 *
 * @history
 * 1.Date        : 2023-1-6 21:10:24
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __XF_UNIX_TIME_UPDATE_H__
#define __XF_UNIX_TIME_UPDATE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

#define _RTC_UPDATE_MAIN_ID      (0x0100)

typedef struct{
    uint32_t time_ms;
    uint32_t timeout_ms;
}xf_unix_time_get_s;

#define _DEF_UNIX_TIME_TASK_FUNC(info_link,func_name)    \
        static xf_unix_time_get_s __rtc_update_main = {0};\
        static void func_name(uint8_t dest_link, uint8_t dest)\
        {\
            if(check_time_out(__rtc_update_main.time_ms, __rtc_update_main.timeout_ms)){\
                uint32_t time_list[] = {1000, 2000, 5000, 10 * 1000, 30 * 1000, 30 * 1000};\
                uint8_t i;\
                uint8_t list_len = ARRY_ITEMS_NUM(time_list) - 1;\
            \
                hal_frame_info_t frame = {0};\
        \
                frame.link_id = dest_link;\
                frame.dest = dest;\
                frame.cmd_func = COMM_CMD_SET;\
                frame.cmd_id = CMD_ID_GET_UNIX_TIME;\
                frame.need_ack = 1;\
        \
                protocol_send_by_id(&frame);\
                \
                for(i = 0; i < list_len; i++){\
                    if(__rtc_update_main.timeout_ms == time_list[i]){\
                        break;\
                    }\
                }\
                \
                __rtc_update_main.time_ms = get_sys_ms();\
                __rtc_update_main.timeout_ms = time_list[(i + 1) % ARRY_ITEMS_NUM(time_list)];   /* 按照列表超时时间顺序进行获取unix time，最大超时为30s并持续保持 */ \
            }\
            \
        }\
        \
        static int32_t __unix_time_get_ack(hal_frame_info_t *p_frame)\
        {\
            uint16_t read_len = X_FRAME_MIN(sizeof(rtc_time_t), p_frame->data_len);\
            rtc_time_t time;\
            memcpy(&time, p_frame->pdata, read_len);\
            set_calendar_sec(time.unix_time);\
            set_time_zone(time.time_zone);\
        \
            __rtc_update_main.timeout_ms = 60 * 60 * 1000;      /* 下一次同步为1小时后 */\
            return replica_info_send_trigger(_RTC_UPDATE_MAIN_ID);\
        }\
        \
        REG_LINK_ACTION_ACK(info_link, COMM_CMD_SET, CMD_ID_GET_UNIX_TIME, __unix_time_get_ack);

#define _DEF_UNIX_TIME_SYNC_FUNC(func_name)    \
        static int32_t func_name(hal_frame_info_t *p_frame)\
        {\
            rtc_time_t rtc;\
            rtc.unix_time = get_calendar_sec();\
            rtc.time_zone = get_time_zone();\
            p_frame->cmd_func = COMM_CMD_SET;\
            p_frame->cmd_id = CMD_ID_SET_UNIX_TIME;\
            p_frame->pdata = (uint8_t *)&rtc;\
            p_frame->data_len = sizeof(rtc);\
            p_frame->need_ack = 1;\
        \
            return protocol_send_by_id(p_frame);\
        }

/**
 * @brief 注册unix time同步功能链路
 *
 * @param [in] link...                 // 不定参数长度的链路id
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *    REG_UNIX_TIME_FUNC_REPLICA_LINK(LINK_UART1,  LINK_UART2)  // 注册两个链路
 * @endcode
 */
#define REG_UNIX_TIME_FUNC_REPLICA_LINK(link...)                                        static int32_t __unix_time_set_ack(hal_frame_info_t *p_frame)\
                                                                                        {\
                                                                                            return replica_info_set_ack(_RTC_UPDATE_MAIN_ID, p_frame);\
                                                                                        }\
                                                                                        REG_MULTI_LINK_ACTION_ACK(COMM_CMD_SET, CMD_ID_SET_UNIX_TIME, __unix_time_set_ack, link)
/**
 * @brief 注册unix time同步功能从机列表信息
 *
 * @param [in] info_link        // 获取unix time信息的目标链路
 * @param [in] info_dest        // 获取unix time信息的目标地址
 * @param [in] item...          // 从机信息
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 * @par eg:        
 * @code
 *      // 假设从IOT模块地址获取unix time，IOT模块的链路为LINK_IOT_A，地址为 0x35
 *      // 3个是固定从机，都在同一个链路上LINK_A_B，地址分别为 0x10 0x11 0x12
 *      // 1个是动态从机，即改从机可能存在也可能不存在，在链路LINK_A_C，地址为 0x13
 *      // 使用:
 *      REG_UNIX_TIME_FUNC_REPLICA_INFO(LINK_IOT_A，0x35，
 *          UNIX_TIME_REPLICA_ITEM(LINK_A_B, 0x10, 0)
 *          UNIX_TIME_REPLICA_ITEM(LINK_A_B, 0x11, 0)
 *          UNIX_TIME_REPLICA_ITEM(LINK_A_B, 0x12, 0)
 *          UNIX_TIME_REPLICA_ITEM(LINK_A_C, 0x13, 1)
 *          )
 * @endcode
 */                                                                                        
#define REG_UNIX_TIME_FUNC_REPLICA_INFO(info_link,info_dest,item...)                    _DEF_UNIX_TIME_SYNC_FUNC(__unix_time_sync)\
                                                                                        _DEF_UNIX_TIME_TASK_FUNC(info_link,__unix_time_get_task)\
                                                                                        REG_REPLICA_INFO(_RTC_UPDATE_MAIN_ID,info_link,info_dest,__unix_time_get_task,__unix_time_sync,item)

/**
 * @brief unix time同步功能从机信息设置
 *
 * @param [in] link_id          // 从机所在的链路
 * @param [in] dest             // 从机地址
 * @param [in] is_dynamic       // 动态从机标志，1：动态从机，0：静态从机
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 * @par eg:        
 * @code
 *      // 假设从IOT模块地址获取unix time，IOT模块的链路为LINK_IOT_A，地址为 0x35
 *      // 3个是固定从机，都在同一个链路上LINK_A_B，地址分别为 0x10 0x11 0x12
 *      // 1个是动态从机，即改从机可能存在也可能不存在，在链路LINK_A_C，地址为 0x13
 *      // 使用:
 *      REG_UNIX_TIME_FUNC_REPLICA_INFO(LINK_IOT_A，0x35，
 *          UNIX_TIME_REPLICA_ITEM(LINK_A_B, 0x10, 0)
 *          UNIX_TIME_REPLICA_ITEM(LINK_A_B, 0x11, 0)
 *          UNIX_TIME_REPLICA_ITEM(LINK_A_B, 0x12, 0)
 *          UNIX_TIME_REPLICA_ITEM(LINK_A_C, 0x13, 1)
 *          )
 * @endcode
 */ 
#define UNIX_TIME_REPLICA_ITEM(link_id,dest,is_dynamic)                                 DEF_REPLICA_ITEM(link_id,dest,is_dynamic)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __XF_UNIX_TIME_UPDATE_H__
