/**
 * @brief 正交译码器
 * @file xh_encoder.h
 * @version 1.0
 * @author Xiao Shulong
 * @date 2022-08-04 09:58:59
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-08-04 09:58:59 <td>1.0       <td>Xiao Shulong      <td>Created function
 */


#ifndef xh_encoder_h
#define xh_encoder_h
#ifdef __cplusplus
extern "C" {
#endif

#include "xh_core.h"



typedef uint32_t (*xh_encoder_get_cnt_obj)(void *val);
typedef void (*xh_encoder_set_car_obj)(void *val,uint32_t car);
typedef void (*xh_encoder_set_cnt_obj)(void *val,uint32_t cnt);
typedef void (*xh_encoder_reg_irq_obj)(void *val,void(*irq_cb)(void));

typedef struct _xh_encoder_t{
    xh_comm_t                   core;
    xh_encoder_get_cnt_obj      get_cnt;
    xh_encoder_set_car_obj      set_car;
    xh_encoder_set_cnt_obj      set_cnt;
    xh_encoder_reg_irq_obj      reg_irq;
}xh_encoder_t, *p_xh_encoder_t;

/* 驱动宏定义实现 */
#define REG_XH_ENCODER_DRV(id,var,prio,auto_init,init,deinit,get_cnt,set_car,set_cnt,reg_irq)               \
            REG_XH_DEV(xh_encoder_t,id,var,prio,auto_init,init,deinit,           \
            (xh_encoder_get_cnt_obj)get_cnt,                                     \
            (xh_encoder_set_car_obj)set_car,                                     \
            (xh_encoder_set_cnt_obj)set_cnt,                                     \
            (xh_encoder_reg_irq_obj)reg_irq,                                     \
        )

#define EXT_XH_ENCODER_DRV(id)        EXT_XH_DEV(xh_encoder_t,id)

/**
 * @brief 获取计数值
 *
 * @param [in] id            驱动ID
 *
 * @return uint32_t
 *  @retval 计数值
 * @details 特殊说明:
 * @par eg:获取计数值
 * @code
 * uint32_t cnt;
 * void get_cnt(void)
 * {
 *      cnt = xh_encoder_get_cnt(DRV_ENCODER);
 * }
 * @endcode
 */
static inline uint32_t xh_encoder_get_cnt(void *id)
{
    xh_encoder_t *p = (xh_encoder_t *)id;
    return p->get_cnt(p->core.var);
}

/**
 * @brief 设置计数最大值
 *
 * @param [in] id            驱动ID
 * @param [in] car           计数最大值
 *
 * @details 特殊说明:
 * @par eg:设置计数最大值
 * @code
 * uint32_t car = 10000;
 * void set_cnt(void)
 * {
 *      xh_encoder_set_car(DRV_ENCODER,car);
 * }
 * @endcode
 */
static inline void xh_encoder_set_car(void *id,uint32_t car)
{
    xh_encoder_t *p = (xh_encoder_t *)id;
    p->set_car(p->core.var,car);
}

/**
 * @brief 设置当前计数值
 *
 * @param [in] id            驱动ID
 * @param [in] cnt           计数值
 *
 * @details 特殊说明:
 * @par eg:设置当前计数值
 * @code
 * uint32_t cnt 5000;
 * void set_cnt(void)
 * {
 *      xh_encoder_set_cnt(DRV_ENCODER,cnt);
 * }
 * @endcode
 */
static inline void xh_encoder_set_cnt(void *id,uint32_t cnt)
{
    xh_encoder_t *p = (xh_encoder_t *)id;
    p->set_cnt(p->core.var,cnt);
}

/**
 * @brief 注册更新回调函数
 *
 * @param [in] id            驱动ID
 * @param [in] irq_cb        回调函数
 *
 * @details 内部已经完成了清除标志等处理
 * @par eg:注册的函数类型为 void (*func)(void) 没有形参和返回值
 * @code
 * void up_cb(void)
 * {
 *      ///< 函数内容
 * }
 * void init(void)
 * {
 *      xh_encoder_reg_irq(DRV_ENCODER,up_cb);
 * }
 * REG_INIT_MOD(init)
 * @endcode
 */
static inline void xh_encoder_reg_irq(void *id,void(*irq_cb)(void))
{
    xh_encoder_t *p = (xh_encoder_t *)id;
    p->reg_irq(p->core.var,irq_cb);
}


#ifdef __cplusplus
}
#endif
#endif /* xh_encoder_h */

