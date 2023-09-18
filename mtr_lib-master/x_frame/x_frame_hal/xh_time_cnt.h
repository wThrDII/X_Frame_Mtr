/**
 * @brief 定时器计数
 * @file xh_time_cnt.h
 * @version 1.0
 * @author Xiao Shulong
 * @date 2022-07-08 19:31:56
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-08 19:31:56 <td>1.0       <td>Xiao Shulong      <td>Created function
 */

#ifndef xh_time_cnt_h
#define xh_time_cnt_h
#ifdef __cplusplus
extern "C" {
#endif

#include "xh_core.h"

typedef void (*xh_timer_enable_obj)(void *var);
typedef void (*xh_timer_disable_obj)(void *var);
typedef void (*xh_timer_set_freq_obj)(void *var,uint32_t freq);
typedef void (*xh_timer_reg_irq_obj)(void *var,void(*irq_cb)(void));

typedef struct _xh_timer_cnt_t{
    xh_comm_t core;
    xh_timer_enable_obj         timer_enable;
    xh_timer_disable_obj        timer_disable;
    xh_timer_set_freq_obj       timer_set_freq;
    xh_timer_reg_irq_obj        timer_reg_irq;
}xh_timer_cnt_t, *p_xh_timer_cnt_t;

/* 驱动宏定义实现 */
#define REG_XH_TIMER_CNT_DRV(id,var,prio,auto_init,init,deinit,timer_enable,timer_disable,timer_set_freq,timer_reg_irq)                \
            REG_XH_DEV(xh_timer_cnt_t,id,var,prio,auto_init,init,deinit,                \
            (xh_timer_enable_obj)timer_enable,                                          \
            (xh_timer_disable_obj)timer_disable,                                        \
            (xh_timer_set_freq_obj)timer_set_freq,                                      \
            (xh_timer_reg_irq_obj)timer_reg_irq,                                        \
        )

#define EXT_XH_TIMER_CNT_DRV(id)        EXT_XH_DEV(xh_timer_cnt_t,id)

/**
 * @brief 定时器使能
 *
 * @param [in] id            驱动ID
 *
 * @details 定时器默认开启(注意不要和PWM接口搞混了)
 * @par eg:开启定时器4
 * @code
 *    xh_timer_enable(DRV_TIMER4);
 * @endcode
 */
static inline void xh_timer_enable(void *const id)
{
    xh_timer_cnt_t *p = (xh_timer_cnt_t *)id;
    p->timer_enable(p->core.var);
}

/**
 * @brief 定时器失能
 *
 * @param [in] id            驱动ID
 *
 * @details 定时器默认开启(注意不要和PWM接口搞混了)
 * @par eg:关闭定时器4
 * @code
 *    xh_timer_disable(DRV_TIMER4);
 * @endcode
 */
static inline void xh_timer_disable(void *const id)
{
    xh_timer_cnt_t *p = (xh_timer_cnt_t *)id;
    p->timer_disable(p->core.var);
}

/**
 * @brief 设置定时器频率
 *
 * @param [in] id            驱动ID
 * @param [in] freq          频率
 *
 * @details none
 * @par eg:设置定时器4频率为30KHZ
 * @code
 * uint32_t freq = 30000;
 * void set_freq(void)
 * {
 *    xh_timer_set_freq(DRV_TIMER4,freq);
 * }
 * @endcode
 */
static inline void xh_timer_set_freq(void *const id,uint32_t freq)
{
    xh_timer_cnt_t *p = (xh_timer_cnt_t *)id;
    p->timer_set_freq(p->core.var,freq);
}

/**
 * @brief 注册中断回调函数
 *
 * @param [in] id            驱动ID
 * @param [in] irq_cb        回调函数
 *
 * @details 内部已经完成了清除标志未等处理，一般用于定时的任务
 * @par eg:注册的函数类型为 void (*func)(void) 没有形参和返回值
 * @code
 * void reg_timer_cb(void)
 * {
 *      ///< 函数内容
 * }
 * void init(void)
 * {
 *      xh_timer_reg_irq(DRV_TIMER4,reg_timer_cb);
 * }
 * REG_INIT_MOD(init)
 * @endcode
 */
static inline void xh_timer_reg_irq(void *const id,void(*irq_cb)(void))
{
    xh_timer_cnt_t *p = (xh_timer_cnt_t *)id;
    p->timer_reg_irq(p->core.var,irq_cb);
}

#ifdef __cplusplus
}
#endif
#endif /* xh_time_cnt_h */
