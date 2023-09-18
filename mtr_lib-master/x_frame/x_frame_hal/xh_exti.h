/**
 * @brief EXTI驱动接口
 * @file xh_exti.h
 * @version 1.0
 * @author Xiao Shulong
 * @date 2022-07-08 18:33:31
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-08 18:33:31 <td>1.0       <td>Xiao Shulong      <td>Created function
 */

#ifndef xh_exti_h
#define xh_exti_h
#ifdef __cplusplus
extern "C" {
#endif

#include "xh_core.h"


typedef void (*xh_exti_reg_irq_obj)(void *var,void(*irq_cb)(void));

typedef struct _xh_exti_t{
    xh_comm_t core;
    xh_exti_reg_irq_obj     reg_irq;
}xh_exti_t, *p_xh_exti_t;

/**
 * @brief 注册ADC驱动
 *
 * @param [in] id            驱动ID
 * @param [in] var           驱动参数变量
 * @param [in] prio          初始化优先级
 * @param [in] auto_init     自动初始化
 * @param [in] init          初始化函数
 * @param [in] deinit        反初始化函数
 * @param [in] reg_irq       注册回调函数
 *
 * @details
 */
#define REG_XH_EXTI_DRV(id,var,prio,auto_init,init,deinit,reg_irq)                \
        REG_XH_DEV(xh_exti_t,id,var,prio,auto_init,init,deinit,                   \
            (xh_exti_reg_irq_obj)reg_irq                                          \
        )

#define EXT_XH_EXTI_DRV(id)        EXT_XH_DEV(xh_exti_t,id)

/**
 * @brief 
 * 
 * @param [in] id            驱动ID
 * @param [in] irq_cb        回调函数
 * 
 * @details 特殊说明: 
 * @par eg:注册事件中断服务
 * @code
 *      void func_cb(void)
 *      {    
 *          //......
 *      }
 *      void test_task()
 *      {
 *          xh_exti_reg_irq(DRV_EXTI1,func_cb);
 *      }
 *      REG_INIT_MOD(test_task)
 * @endcode
 */
static inline void xh_exti_reg_irq(void *const id,void(*irq_cb)(void))
{
    xh_exti_t *p = (xh_exti_t *)id;
    p->reg_irq(p->core.var,irq_cb);
}
#ifdef __cplusplus
}
#endif
#endif /* xh_exti_h */

