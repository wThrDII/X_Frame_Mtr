/**
 * @brief PWM驱动接口
 * @file xh_pwm.h
 * @version 1.1
 * @author Xiao Shulong
 * @date 2022-07-08 19:20:16
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-08 19:20:16 <td>1.0       <td>Xiao Shulong      <td>Created function
 * <tr><td>2022-07-18 17:40:31 <td>1.1       <td>Xiao Shulong      <td>1.增加设置PWM通道状态 2.修改设置死区函数参数
 */


#ifndef __xh_pwm_H__
#define __xh_pwm_H__
#include "stdint.h"


#include "xh_core.h"



typedef void (*xh_pwm_disable_obj)(void *val);
typedef void (*xh_pwm_enable_obj)(void *val);
typedef void (*xh_pwm_set_duty_cycle_obj)(void *val,uint16_t ch,uint16_t duty_cycle);
typedef void (*xh_pwm_set_freq_obj)(void *val,uint32_t freq);
typedef void (*xh_pwm_set_deadtime_obj)(void *val,uint8_t deadtime);
typedef void (*xh_pwm_set_ch_obj)(void *val,uint16_t ch,uint8_t cmd);
typedef void (*xh_pwm_reg_irq_obj)(void *val,uint32_t type,void(*irq_cb)(void));
typedef void (*xh_pwm_set_ch_mode_obj)(void *val,uint16_t ch,uint8_t mode);

typedef struct _xh_pwm_t{
    xh_comm_t                   core;
    xh_pwm_disable_obj          disable;
    xh_pwm_enable_obj           enable;
    xh_pwm_set_duty_cycle_obj   set_duty_cycle;
    xh_pwm_set_freq_obj         set_freq;
    xh_pwm_set_deadtime_obj     set_deadtime;
    xh_pwm_set_ch_obj           set_ch;
    xh_pwm_reg_irq_obj          reg_irq;
    xh_pwm_set_ch_mode_obj      set_ch_mode;
}xh_pwm_t, *p_xh_pwm_t;

/* 驱动宏定义实现 */
#define REG_XH_PWM_DRV(id,var,prio,auto_init,init,deinit,disable,enable,set_duty_cycle,set_freq,set_deadtime,set_ch,reg_irq,set_ch_mode)               \
            REG_XH_DEV(xh_pwm_t,id,var,prio,auto_init,init,deinit,                     \
            (xh_pwm_disable_obj)disable,                                       \
            (xh_pwm_enable_obj)enable,                                         \
            (xh_pwm_set_duty_cycle_obj)set_duty_cycle,                         \
            (xh_pwm_set_freq_obj)set_freq,                                     \
            (xh_pwm_set_deadtime_obj)set_deadtime,                             \
            (xh_pwm_set_ch_obj)set_ch,                                         \
            (xh_pwm_reg_irq_obj)reg_irq,                                       \
            (xh_pwm_set_ch_mode_obj)set_ch_mode,                               \
        )

#define EXT_XH_PWM_DRV(id)        EXT_XH_DEV(xh_pwm_t,id)

/**
* @brief 中断类型
*/
typedef enum {
    XH_PWM_UPDATA,
    XH_PWM_BRK,
    XH_PWM_TRG,
    XH_PWM_CMT,
    XH_PWM_CH_0,
    XH_PWM_CH_1,
    XH_PWM_CH_2,
    XH_PWM_CH_3,
}XH_PWM_IRQ_TYPE;

typedef enum {
    XH_PWM_CHO_ON, //主通道开启
    XH_PWM_CHO_OFF, //主通道关闭
    XH_PWM_CHON_ON, //互补开启
    XH_PWM_CHON_OFF,  //互补关闭
}XH_PWM_CTRL_CMD;

typedef enum {
    XH_PWM_TIMING,      //时基模式，比较值不起作用
    XH_PWM_ACTIVE,      //匹配时为高
    XH_PWM_INACTIVE,    //匹配时为低
    XH_PWM_TOGGLE,      //匹配时翻转
    XH_PWM_LOW,         //强制为低
    XH_PWM_HIGH,        //强制为高
    XH_PWM_PWM0,        //向上计数时，小于比较值为高
    XH_PWM_PWM1,        //向上计数时，小于比较值为低
}XH_PWM_CH_MODE_LIST;
/**
 * @brief 失能定时器
 * @param [in] id            驱动ID
 * @details 如果是高级定时器，同时会关闭通道输出
 * @par eg:关闭PWM1的定时器
 * @code
 * #include "hal.h"
 * void epwm_disable(void)
 * {
 *      ///< DRV_PWM1在hal中声明的驱动ID
 *      xh_pwm_disable(DRV_PWM1);
 * }
 * @endcode
 */
static inline void xh_pwm_disable(void *id)
{
    xh_pwm_t *p = (xh_pwm_t *)id;
    p->disable(p->core.var);
}

/**
 * @brief 使能定时器
 * @param [in] id            驱动ID
 * @details 如果是高级定时器，同时会开启通道输出
 * @par eg:开启PWM1的定时器
 * @code
 * #include "hal.h"
 * void epwm_enable(void)
 * {
 *      ///< DRV_PWM1在hal中声明的驱动ID
 *      xh_pwm_enable(DRV_PWM1);
 * }
 * @endcode
 */
static inline void xh_pwm_enable(void *id)
{
    xh_pwm_t *p = (xh_pwm_t *)id;
    p->enable(p->core.var);
}

/**
 * @brief 设置通道占空比
 *
 * @param [in] id            驱动ID
 * @param [in] ch            通道号
 * @param [in] duty_cycle    占空比
 *
 * @details 1、占空比为0~10000，对应0%~100%
 *          2、占空比会在下一个更新时间装载
 * @par eg:设置PWM1的通道0(通道名：boost_h)的占空比为40%
 * @code
 * #include "hal.h"
 * uint32_t duty = 4000;///< 比较值A 范围：0~10000
 * void set_ch_duty(void)
 * {
 *     ///< DRV_PWM1在hal中声明的驱动ID
 *    ///< boost_h 在hal.h中的枚举值
 *     xh_pwm_set_duty_cycle(DRV_PWM1,boost_h,duty);
 * }
 * @endcode
 */
static inline void xh_pwm_set_duty_cycle(void *id,uint16_t ch,uint16_t duty_cycle)
{
    xh_pwm_t *p = (xh_pwm_t *)id;
    p->set_duty_cycle(p->core.var,ch,duty_cycle);
}

/**
 * @brief 设置PWM频率
 * @param [in] id            驱动ID
 * @param [in] freq          频率
 * @details 频率会在下一个更新事件装载
 * @par eg:设置PWM1的频率为50KHZ
 * @code
 * #include "hal.h"
 * uint32_t freq = 50000;//频率
 * void set_freq(void)
 * {
 *     ///< DRV_PWM1在hal中声明的驱动ID
 *    xh_pwm_set_freq(DRV_PWM1,freq);
 * }
 * @endcode
 */
static inline void xh_pwm_set_freq(void *id,uint32_t freq)
{
    xh_pwm_t *p = (xh_pwm_t *)id;
    p->set_freq(p->core.var,freq);
}

/**
 * @brief 设置死区时间
 * @param [in] id            驱动ID
 * @param [in] deadtime      死区时间
 * @details 设置的死区时间为寄存器值，没有单位
 * @par eg:设置PWM1的死区为55
 * @code
 * #include "hal.h"
 * uint32_t deadtime = 55;//死区 取值范围0~255
 * void set_deadtime(void)
 * {
 *     ///< DRV_PWM1在hal中声明的驱动ID
 *    xh_pwm_set_deadtime(DRV_PWM1,deadtime);
 * }
 * @endcode
 */
static inline void xh_pwm_set_deadtime(void *id,uint8_t deadtime)
{
    xh_pwm_t *p = (xh_pwm_t *)id;
    p->set_deadtime(p->core.var,deadtime);
}

/**
 * @brief 设置通道状态
 *
 * @param [in] id            驱动ID
 * @param [in] ch            通道号
 * @param [in] cmd           通道状态的命令
 *
 * @details ch 在hal中的枚举中，cmd为枚举表XH_PWM_CTRL_CMD
 * @par eg:开启PWM0的通道0(boost_h)的主通道，关闭通道0的互补通道
 * @code
 * #include "hal.h"
 * void set_ch_sta(void)
 * {
 *     ///< DRV_PWM1在hal中声明的驱动ID
 *    ///< boost_h 在hal.h中的枚举值
 *    ///< XH_PWM_CHO_ON : 枚举表XH_PWM_CTRL_CMD 中的值
 *   ///< XH_PWM_CHON_OFF : 枚举表XH_PWM_CTRL_CMD 中的值
 *    xh_pwm_set_ch(DRV_PWM0,boost_h,XH_PWM_CHO_ON);
 *    xh_pwm_set_ch(DRV_PWM0,boost_h,XH_PWM_CHON_OFF);
 * }
 * @endcode
 */
static inline void xh_pwm_set_ch(void *id,uint16_t ch,uint8_t cmd)
{
    xh_pwm_t *p = (xh_pwm_t *)id;
    p->set_ch(p->core.var,ch,cmd);
}

/**
 * @brief 注册中断回调
 * @param [in] id            驱动ID
 * @param [in] type          中断类型
 * @param [in] irq_cb        回调函数
 * @details none
 * @par eg:注册PWM0的更新中断
 * @code
 * void pwm1_up_cb(void)
 * {
 *      //对应的控制内容
 * }
 * void init(void)
 * {
 *      xh_pwm_reg_irq(DRV_PWM0,XH_PWM_UPDATA,pwm1_up_cb);
 * }
 * REG_INIT_MOD(init)
 * @endcode
 */
static inline void xh_pwm_reg_irq(void *id,uint32_t type,void(*irq_cb)(void))
{
    xh_pwm_t *p = (xh_pwm_t *)id;
    p->reg_irq(p->core.var,type,irq_cb);
}

/**
 * @brief 设置通道模式
 *
 * @param [in] id            驱动ID
 * @param [in] ch            通道号
 * @param [in] mode          通道模式
 *
 * @details none
 * @par eg:将PWM0通道0(boost_h)模式设置为强制拉低
 * @code
 * #include "hal"
 * void set_ch_mode(void)
 * {
 *     ///< DRV_PWM1在hal中声明的驱动ID
 *    ///< boost_h 在hal.h中的枚举值
 *      ///< XH_PWM_LOW 为枚举表 XH_PWM_CH_MODE_LIST 中的值
 *    xh_pwm_set_ch_mode(DRV_PWM0,boost_h,XH_PWM_LOW);
 * }
 * @endcode
 */
static inline void xh_pwm_set_ch_mode(void *id,uint16_t ch,uint8_t mode)
{
    xh_pwm_t *p = (xh_pwm_t *)id;
    p->set_ch_mode(p->core.var,ch,mode);
}

#endif //__xh_pwm_H__


