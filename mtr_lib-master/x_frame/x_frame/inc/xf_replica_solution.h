/**
 * @brief xf_replica_solution.c 的头文件
 * @file xf_replica_solution.h
 * @version 1.0
 * @author hzf
 * @date 2023-1-6 17:15:43
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2023. All rights reserved.
 *
 * @history
 * 1.Date        : 2023-1-6 17:15:43
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __XF_REPLICA_SOLUTION_H__
#define __XF_REPLICA_SOLUTION_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

typedef int32_t (*replica_send_cb)(hal_frame_info_t *);
typedef void (*replica_task_cb)(uint8_t link, uint8_t dest);

typedef struct{
    uint8_t link_id;
    uint8_t dest;
    uint8_t dync;   ///< 该静态地址是否存在动态增减，考虑如果存在动态插拔需要做定时发送
    
}xf_replica_item_s;


typedef struct{
    const xf_replica_item_s* p_item;
    uint8_t *p_sort;
    replica_send_cb pfn_send;
    replica_task_cb pfn_task;
    uint16_t main_id;                       ///< xframe内部使用范围0x0100 ~ 0xffff，应用上使用范围0 ~ 0x00ff
    uint8_t item_cnt;
    uint8_t main_link;
    uint8_t main_dest;
    uint8_t active_flag:1;          ///< 触发发送标志位
    uint8_t dync_flag:1;            ///< 定时发送标志位
    uint32_t active_time_ms;
    list_t node;
}xf_solution_item_s;


/**
 * @brief 注册从机功能信息
 *
 * @param [in] id               // 主机功能id
 * @param [in] info_link        // 获取信息的目标链路
 * @param [in] info_link        // 获取信息的目标地址
 * @param [in] task_func        // 循环处理的主机回调
 * @param [in] send_func        // 主机发送数据回调
 * @param [in] item...          // 从机信息
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 * @par eg:        
 * @code
 * @endcode
 */ 
#define REG_REPLICA_INFO(id,info_link,info_dest,task_func,send_func,item...)                    static const xf_replica_item_s __rep_##id##_list[] = {item};\
                                                                                                static uint8_t __rep_sort_##id[ARRY_ITEMS_NUM(__rep_##id##_list)];\
                                                                                                static xf_solution_item_s __solu_##id = {__rep_##id##_list,__rep_sort_##id,send_func,task_func,id,ARRY_ITEMS_NUM(__rep_##id##_list),info_link,info_dest};\
                                                                                                AUTO_REG_ITEM(__solu_##id,AUTO_REG_REP_SOLUTION,(void*)&__solu_##id)
                                                                
#define DEF_REPLICA_ITEM(link_id,dest,is_dynamic)                                               {link_id,dest,is_dynamic}

extern int32_t replica_info_send_trigger(uint16_t main_id);
extern int32_t replica_info_set_ack(uint16_t main_id, hal_frame_info_t *p_frame);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __XF_REPLICA_SOLUTION_H__
