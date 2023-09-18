/**
 * @brief tasks.c.xtemp header file
 * @file tasks.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 15:48:34
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 15:48:34
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __TASKS_H__
#define __TASKS_H__
#include "list.h"

#define VH_TASK 0
#define HI_TASK 1
#define LW_TASK 2
#define ID_TASK 3

#define MAX_TASKS_GROUP 4
#define MAX_TASKS_NUM 50

typedef void (*pf_exe_func)(void);

typedef struct
{
    uint32_t task_num;
    list_t task_head;
} task_ctrl_t;

typedef struct
{
    const char     *func_name;
    uint32_t        group;
    uint32_t        perio;
    pf_exe_func     exe_func;    
}task_config_t;

typedef struct
{
    const task_config_t *config;
    uint32_t min_run_time;      ///< 单次任务最小运行时间，单位ms
    uint32_t max_run_time;      ///< 单次任务最大运行时间，单位ms
    uint32_t last_run_time;     ///< 上次任务运行的时间，单位ms
    uint32_t tatol_run_times;   ///< 任务运行的累计次数    
    uint32_t last_exe_time;
    uint32_t task_used_time;
    uint32_t flag_run_firs:1;
    uint32_t flag_delete:1;
    list_t list;
} task_item_t;

/**
 * @brief                   任务注册
 * @param [in] group        注册任务的组别
 * @param [in] perio        任务间隔时间 单位:ms
 * @param [in] task_name    任务函数指针
 *
 * @details:                1.这是内部使用的宏，外部使用的宏重新封装
 *                          2.注册的任务本质是轮询执行，不存在优先级的概念
 * @par e.g:
 * @code
 *      void func(void)
 *      {
 *          ;
 *      }
 *      REG_TASK(VH_TASK,1,func);  //VH_TASK 是设定的组别
 *
 *
 * @endcode
 */
#define REG_TASK(group, perio, task_name)                                 \
    static const task_config_t task_cfg_##task_name = {#task_name,group,perio,task_name};\
    static task_item_t task_##task_name = {&task_cfg_##task_name,0xffffffff};\
    AUTO_REG_ITEM(TASK_##task_name, AUTO_REG_TYPE_TASK, (void *)&task_##task_name);

/**
 * @brief 任务运行过程的单次执行最小开销时间、最大开销时间、上次开销时间、总运行次数数据的获取
 *
 * @param [in] task_name        任务函数指针
 * @param [out] p_min_time      最小开销时间变量指针，uint32_t *
 * @param [out] p_max_time      最大开销时间变量指针，uint32_t *
 * @param [out] p_last_time     上次运行时间变量指针，uint32_t *
 * @param [out] p_tatol_times   总运行次数变量指针，uint32_t *
 *
 * @details:
 * @par e.g:
 * @code
 *      void task_demo(void);
 *      {
 *          uint32_t min_time,max_time,last_time,tatol_times;
 *          static uint8_t time = 1;
 *
 *          task_time_info_read(task_demo, &min_time, &max_time, &last_time, &tatol_times);
 *          DBG_I("min time:%d, max time:%d, last time:%d, tatol times:%d", min_time, max_time, last_time, tatol_times);
 *          block_ms(time++);   ///< 阻塞一定时间，每次阻塞的时间都变化
 *      }
 *      REG_TASK(VH_TASK,1000,task_demo);
 *
 * @endcode
 */
#define task_time_info_read(task_name,p_min_time,p_max_time,p_last_time,p_tatol_times)          task_time_info_get(#task_name,p_min_time,p_max_time,p_last_time,p_tatol_times);

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif // __cplusplus
    /**
     * @brief               调用任务
     *
     * @param [in] group    需要调用的任务的组别
     *
     * @details:            任务本质是轮询执行，不存在优先级的概念
     * @par e.g:
     * @code
     *      main(){
     *          while(1) {
     *              task_sch(VH_TASK);  //VH_TASK 是注册任务是设置的组别
     *          }
     *      }
     *
     * @endcode
     */
    void task_sch(uint8_t group);
    /**
     * @brief               删除当前正在运行的任务
     *
     * @details:
     * @par e.g:
     * @code
     *      current_task_delete();
     * @endcode
     */

    void current_task_delete(void);


    void task_time_info_get(const char *name, uint32_t *min_time, uint32_t *max_time, uint32_t *last_time, uint32_t *tatol_times);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif // __cplusplus

#endif // __TASKS_H__
