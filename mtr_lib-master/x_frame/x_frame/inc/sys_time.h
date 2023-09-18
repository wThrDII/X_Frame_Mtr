/**
 * @brief sys_time.c.xtemp header file
 * @file sys_time.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 15:36:9
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 15:36:9
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __SYS_TIME_H__
#define __SYS_TIME_H__


/**
 * @brief 增加系统时钟计数和工作时间计数,单位 ms
 *
 * @param [in] cnt 增加的时间计数,单位 ms
 * 
 * @details:
 * @par e.g:
 * @code
 *      RUN_XF_SYS_TIME(50); //系统时钟计数和工作时间计数增加50ms
 * @endcode
 */
#define RUN_XF_SYS_TIME(cnt)                   {sys_tick_ms += cnt; sys_work_time += cnt;}

/**
 * @brief 定义系统时钟驱动ID
 *
 * @param [in] id 需要定义的系统时钟驱动id
 *
 * @details:
 * @par e.g:
 * @code
 *      #define SYS_CLOCK 1    //系统时钟驱动id
 *
 *      DEF_XF_XH_SYS_ID(SYS_CLOCK);
 * @endcode
 */
#define DEF_XF_XH_SYS_ID(id)                   void * const gx_xh_sys_id = PTR_TYPE_XH_DEV(id);

/**
 * @brief 获取系统时间计数,单位 ms
 *
 * @return [out] uint32_t
 *  @retval 当前的系统时间计数,单位 ms
 *
 * @details:
 * @par e.g:
 * @code
 *      uint32_t cur_time;
 *
 *      cur_time = SYS_MS_FUNC;
 * @endcode
 */
#define SYS_MS_FUNC                             get_sys_ms()

/**
 * @brief 获取两个时间的间隔,单位 ms
 *
 * @param [in] cur_ms 当前时间,单位 ms
 * @param [in] last 上一个时间,单位 ms
 *
 * @return [out] uint32_t
 *  @retval 两个时间的间隔,单位 ms
 *
 * @details:
 * @par e.g:
 * @code
 *      uint32_t interval_time;
 * 
 *      interval_time = TIME_INTERVAL_FUNC(get_sys_ms(), 50); //50ms 到当前时间的时间间隔
 * @endcode
 */
#define TIME_INTERVAL_FUNC(cur_ms,last_ms)      ((cur_ms) - (last_ms))

/**
 * @brief 限制当前func与上一个受限制func的运行时间间隔
 *
 * @param [in] ms 限制时间,单位 ms
 * @param [in] func 需要限制的func
 *
 * @details: 1. 主要用于限制一些需要时间响应的func,例如读取某些数据,在发送读取命令以后，需要等待一段时间才能获取数据
 *           2. 这个宏会先运行输入的func再进行时间限制
 * @par e.g:
 * @code
 *     void user_func();//声明
 *
 *      RUN_BY_LIMIT_FUNC(10, user_func);
 * @endcode
 */
#define RUN_BY_LIMIT_FUNC(ms, func)     	\
{                                           \
    static uint32_t last_exec_ms = 0;  		\
    if((sys_tick_ms - last_exec_ms) > ms) 	\
    {                                       \
        last_exec_ms = sys_tick_ms;         \
        func();                             \
    }                                       \
}

/**
 * @brief 限制当前func与上一个受限制func的运行时间间隔
 *
 * @param [in] ms 限制时间,单位 ms
 * @param [in] func 需要限制的func
 *
 * @details: 1. 主要用于限制一些需要时间响应的func,例如读取某些数据,在发送读取命令以后，需要等待一段时间才能获取数据
 *           2. 这个宏会先进行时间限制再运行输入的func   
 * @par e.g:
 * @code
 *     void user_func();//声明
 *
 *      RUN_BY_LIMIT_AFTER_FUNC(10, user_func);
 * @endcode
 */
#define RUN_BY_LIMIT_AFTER_FUNC(ms, func)             \
    {                                                 \
        static uint32_t last_exec_ms = 0;             \
        uint32_t cur_tick_ms = sys_tick_ms;           \
        if (last_exec_ms == 0) {                      \
            last_exec_ms = cur_tick_ms;               \
        }                                             \
        else if ((cur_tick_ms - last_exec_ms) > ms) { \
            last_exec_ms = cur_tick_ms;               \
            func();                                   \
        }                                             \
    }

/**
 * @brief 限制当前block与上一个受限制block的运行时间间隔
 *
 * @param [in] ms 限制时间,单位 ms
 * @param [in] block 需要限制的block,例如读取某些数据,在发送读取命令以后，需要等待一段时间才能获取数据
 *
 * @details: 1. 要用于限制一些需要时间响应的block
 *           2. 这个宏会先运行输入的block再进行时间限制
 * @par e.g:
 * @code
 *      RUN_BY_LIMIT_FUNC(10,do{
 *                         ...
 *                        }while();
 *                        );
 * @endcode
 */
#define RUN_BY_LIMIT_BLOCK(ms, block)            \
    {                                            \
        static uint32_t last_exec_ms = 0;        \
        if ((sys_tick_ms - last_exec_ms) > ms) { \
            last_exec_ms = sys_tick_ms;          \
            block;                               \
        }                                        \
    }

/**
 * @brief 限制当前block与上一个受限制block的运行时间间隔
 *
 * @param [in] ms 限制时间,单位 ms
 * @param [in] block 需要限制的block,例如读取某些数据,在发送读取命令以后，需要等待一段时间才能获取数据
 *
 * @details: 1. 要用于限制一些需要时间响应的block
 *           2. 这个宏会先进行时间限制再运行输入的block
 * @par e.g:
 * @code
 *      RUN_BY_LIMIT_AFTER_BLOCK(10,do{
 *                         ...
 *                        }while();
 *                        );
 * @endcode
 */
#define RUN_BY_LIMIT_AFTER_BLOCK(ms, block)           \
    {                                                 \
        static uint32_t last_exec_ms = 0;             \
        uint32_t cur_tick_ms = sys_tick_ms;           \
        if (last_exec_ms == 0) {                      \
            last_exec_ms = cur_tick_ms;               \
        }                                             \
        else if ((cur_tick_ms - last_exec_ms) > ms) { \
            last_exec_ms = cur_tick_ms;               \
            block;                                    \
        }                                             \
    }

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

extern volatile uint32_t sys_tick_ms;
extern volatile uint64_t sys_work_time;
extern void * const gx_xh_sys_id;

/**
 * @brief 获取系统时间，单位毫秒
 *
 * @return uint32_t 
 *  @retval 返回系统时间，单位 ms
 * 
 * @details:
 * @par e.g:
 * @code
 *      uint32_t user_val;
 *
 *      user_val = get_sys_ms();
 * @endcode
 */
extern uint32_t get_sys_ms(void);			/* 获取系统毫秒计数 */

/**
 * @brief 获取系统时间，单位秒
 *
 * @return uint32_t 
 *  @retval 返回系统时间，单位 s
 * 
 * @details:
 * @par e.g:
 * @code
 *      uint32_t user_val;
 *
 *      user_val = get_sys_s();
 * @endcode
 */
extern uint32_t get_sys_s(void);			/* 获取系统秒毫秒 */

/**
 * @brief 获取系统时间，单位分钟
 *
 * @return uint32_t 
 *  @retval 返回系统时间，单位 min
 * 
 * @details:
 * @par e.g:
 * @code
 *      uint32_t user_val;
 *
 *      user_val = get_sys_min();
 * @endcode
 */
extern uint32_t get_sys_min(void);			/* 获取系统分钟计数 */

/**
 * @brief 获取系统工作时长,单位毫秒
 *
 * @return uint64_t 
 *  @retval 返回系统工作时长，单位 ms
 * 
 * @details:
 * @par e.g:
 * @code
 *      uint64_t user_val;
 *
 *      user_val = get_sys_work_time();
 * @endcode
 */
extern uint64_t get_sys_work_time(void);	/* 获取系统工作时长，单位ms */

/**
 * @brief 获取经过的时间长度，单位毫秒
 *
 * @param [in] start_time 起始时间，单位ms
 *
 * @return uint32_t 
 *  @retval 返回经过的时间长度，单位 ms
 * 
 * @details:
 * @par e.g:
 * @code
 *      uint32_t user_val;
 *
 *      user_val = get_elapse_time(200);  //200ms 到当前时间的时间间隔
 *
 * @endcode
 */
extern uint32_t get_elapse_time(uint32_t start_time);	/* 获取经过的时间计数，单位ms */

/**
 * @brief 阻塞延时，单位毫秒
 *
 * @param [in] ms 需要阻塞的时间长度，单位 ms
 *
 * @details: 慎用,会导致程序停止运行相应时间
 * @par e.g:
 * @code
 *      block_ms(10); //延时10ms
 * @endcode
 */
extern void block_ms(uint32_t ms);			/* 阻塞延时，慎用,会导致程序停止运行相应时间 */

/**
 * @brief 检查是否超时
 *
 * @param [in] start_time 开始时间，单位 ms
 * @param [in] time_out 超过时间，单位 ms
 *
 * @return bool
 *  @retval true  开始时间到当前时间大于输入的超过时间
 *          false 开始时间到当前时间小于输入的超过时间
 * @details:
 * @par e.g:
 * @code
 *      bool user_val;
 *
 *      user_val = check_time_out(50, 5); // 50ms 到当前时间是否经过了5ms
 * @endcode
 */
extern bool check_time_out(uint32_t start_time, uint32_t time_out);	/* 判断超时 */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __SYS_TIME_H__
