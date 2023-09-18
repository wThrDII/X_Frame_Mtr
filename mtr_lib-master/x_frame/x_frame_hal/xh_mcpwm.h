/**
 * @brief MCPWM驱动接口
 * @file xh_mcpwm.h
 * @version 1.0
 * @author peixiang.qiu
 * @date 2023-01-29 17:32:45
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2023-01-29 17:32:45 <td>1.0       <td>peixiang.qiu      <td>MCPWM驱动接口
 */

#ifndef __xh_mcpwm_h__
#define __xh_mcpwm_h__
#include "stdint.h"

#include "xh_core.h"

typedef void (*xh_mcpwm_disable_obj)(void *val);
typedef void (*xh_mcpwm_enable_obj)(void *val);
typedef void (*xh_mcpwm_set_duty_cycle_obj)(void *val, uint16_t ch, uint16_t duty_cycle);
typedef void (*xh_mcpwm_set_freq_obj)(void *val, uint16_t ch, uint32_t freq);
typedef void (*xh_mcpwm_set_p_deadtime_obj)(void *val, uint16_t ch, uint16_t deadtime);
typedef void (*xh_mcpwm_set_n_deadtime_obj)(void *val, uint16_t ch, uint16_t deadtime);
typedef void (*xh_mcpwm_set_ch_obj)(void *val, uint16_t ch, uint8_t cmd);
typedef void (*xh_mcpwm_reg_irq_obj)(void *val, uint32_t type, void (*irq_cb)(void));
typedef void (*xh_mcpwm_set_ch_mode_obj)(void *val, uint16_t ch, uint8_t mode);

typedef struct _xh_mcpwm_t
{
    xh_comm_t core;
    xh_mcpwm_disable_obj disable;
    xh_mcpwm_enable_obj enable;
    xh_mcpwm_set_duty_cycle_obj set_duty_cycle;
    xh_mcpwm_set_freq_obj set_freq;
    xh_mcpwm_set_p_deadtime_obj set_p_deadtime;
    xh_mcpwm_set_n_deadtime_obj set_n_deadtime;
    xh_mcpwm_set_ch_obj set_ch;
    xh_mcpwm_reg_irq_obj reg_irq;
    xh_mcpwm_set_ch_mode_obj set_ch_mode;
} xh_mcpwm_t, *p_xh_mcpwm_t;

/* 驱动宏定义实现 */
#define REG_XH_MCPWM_DRV(id, var, prio, auto_init, init, deinit, disable, enable, set_duty_cycle, set_freq, set_p_deadtime, set_n_deadtime, set_ch, reg_irq, \
                         set_ch_mode)                                                                                                                        \
    REG_XH_DEV(xh_mcpwm_t, id, var, prio, auto_init, init, deinit, (xh_mcpwm_disable_obj)disable, (xh_mcpwm_enable_obj)enable,                               \
               (xh_mcpwm_set_duty_cycle_obj)set_duty_cycle, (xh_mcpwm_set_freq_obj)set_freq, (xh_mcpwm_set_p_deadtime_obj)set_p_deadtime,                    \
               (xh_mcpwm_set_n_deadtime_obj)set_n_deadtime, (xh_mcpwm_set_ch_obj)set_ch, (xh_mcpwm_reg_irq_obj)reg_irq,                                      \
               (xh_mcpwm_set_ch_mode_obj)set_ch_mode, )

#define EXT_XH_MCPWM_DRV(id) EXT_XH_DEV(xh_mcpwm_t, id)

///< MCPWM中断类型选择
typedef enum
{
    XH_CNT0_MCPWM_T0_IRQ = 0,
    XH_CNT0_MCPWM_T1_IRQ,
    XH_CNT0_MCPWM_TH00_IRQ,
    XH_CNT0_MCPWM_TH01_IRQ,
    XH_CNT0_MCPWM_TH10_IRQ,
    XH_CNT0_MCPWM_TH11_IRQ,
    XH_CNT0_MCPWM_TH20_IRQ,
    XH_CNT0_MCPWM_TH21_IRQ,
    XH_CNT0_MCPWM_TMR0_IRQ,
    XH_CNT0_MCPWM_TMR1_IRQ,
    XH_CNT0_MCPWM_UP_IRQ, //=10

    XH_Cnt1_MCPWM_T0_IRQ,
    XH_Cnt1_MCPWM_T1_IRQ,
    XH_Cnt1_MCPWM_TH30_IRQ,
    XH_Cnt1_MCPWM_TH31_IRQ,
    XH_Cnt1_MCPWM_UP_IRQ, //=15

    XH_Cnt01_MCPWM_TMR2_IRQ,
    XH_Cnt01_MCPWM_TMR3_IRQ,
} XH_MCPWM_IRQ_TYPE;

typedef enum
{
    XH_MCPWM_CH_P_LOW_N_PWM,   ///< P拉低，N发波
    XH_MCPWM_CH_P_LOW_N_HIGHT, ///< P拉低，N拉高
    XH_MCPWM_CH_P_PWM_N_LOW,   ///< P发波，N拉低
    XH_MCPWM_CH_P_PWM_N_HIGHT, ///< P发波，N拉高
    XH_MCPWM_CH_P_HIGHT_N_PWM, ///< P拉高，N发波
    XH_MCPWM_CH_P_HIGHT_N_LOW, ///< P拉高，N拉低
    XH_MCPWM_CH_P_LOW_N_LOW,   ///< P拉低，N拉低
    XH_MCPWM_CH_P_PWM_N_PWM,   ///< P发波，N发波
} XH_MCPWM_CH_CTRL_CMD;

typedef enum
{
    XH_MCPWM_OUT_CH_012 = 7,
    XH_MCPWM_OUT_CH_3 = 8,
} XH_MCPWM_CH;

typedef enum
{
    XH_MCPWM_OUT_MODE_DEFAULT = 0, ///< MCPWM输出设定的默认值
    XH_MCPWM_OUT_MODE_PWM = 1,     ///< MCPWM输出PWM
} XH_MCPWM_CH_MODE;

/**
 * @brief 失能MCPWM,会关闭计数器
 *
 * @param [in] id           驱动ID
 *
 * @details 特殊说明:
 * @par eg:失能驱动ID为DRV_MCPWM1的MCPWM
 * @code
 *       ///< DRV_MCPWM1在hal中声明的驱动ID
 *      xh_mcpwm_disable(DRV_MCPWM1);
 * @endcode
 */
static inline void xh_mcpwm_disable(void *id)
{
    xh_mcpwm_t *p = (xh_mcpwm_t *)id;
    p->disable(p->core.var);
}

/**
 * @brief 使能定时器
 *
 * @param [in] id         驱动ID
 *
 * @details 特殊说明:
 * @par eg:开启驱动ID为DRV_MCPWM1的MCPWM
 * @code
 *      ///< DRV_MCPWM1在hal中声明的驱动ID
 *      xh_mcpwm_enable(DRV_MCPWM1);
 * @endcode
 */
static inline void xh_mcpwm_enable(void *id)
{
    xh_mcpwm_t *p = (xh_mcpwm_t *)id;
    p->enable(p->core.var);
}

/**
 * @brief 设置通道占空比
 *
 * @param [in] id            驱动ID
 * @param [in] ch            通道号
 * @param [in] duty_cycle    占空比
 *
 * @details 特殊说明: 1、占空比为0~10000，对应0%~100%
 *                    2、占空比会在下一个更新时间装载
 * @par eg:设置MCPWM1的通道0(通道名：boost_h)的占空比为40%
 * @code
 * #include "hal.h"
 * uint32_t duty = 4000;///< 比较值A 范围：0~10000
 * void set_ch_duty(void)
 * {
 *     ///< DRV_MCPWM1在hal中声明的驱动ID
 *    ///< boost_h 在hal.h中的枚举值
 *     xh_mcpwm_set_duty_cycle(DRV_MCPWM1,boost_h,duty);
 * }
 * @endcode
 */
static inline void xh_mcpwm_set_duty_cycle(void *id, uint16_t ch, uint16_t duty_cycle)
{
    xh_mcpwm_t *p = (xh_mcpwm_t *)id;
    p->set_duty_cycle(p->core.var, ch, duty_cycle);
}

/**
 * @brief 设置MCPWM频率
 *
 * @param [in] id      驱动ID
 * @param [in] ch      通道
 * @param [in] freq    频率
 *
 * @details 特殊说明: 频率会在下一个更新事件装载，通道012使用一个时基，通道3使用一个时基
 * @par eg:设置驱动ID为DRV_MCPWM1的MCPWM的通道0、1、2的频率为50KHZ
 * @code
 * #include "hal.h"
 * uint32_t freq = 50000;//频率
 * void set_freq(void)
 * {
 *     ///< DRV_MCPWM1在hal中声明的驱动ID
 *    xh_mcpwm_set_freq(DRV_MCPWM1,XH_MCPWM_OUT_CH_012,freq);
 * }
 * @endcode
 */
static inline void xh_mcpwm_set_freq(void *id, XH_MCPWM_CH ch, uint32_t freq)
{
    xh_mcpwm_t *p = (xh_mcpwm_t *)id;
    p->set_freq(p->core.var, ch, freq);
}

/**
 * @brief 设置P通道的死区时间（上升延迟时间）
 *
 * @param [in] id       驱动ID
 * @param [in] ch       通道
 * @param [in] deadtime 死区时间
 *
 * @details 特殊说明:
 * @par eg:设置MCPWM的boost_h对应的P通道死区为200
 * #include "hal.h"
 * uint32_t deadtime = 200;
 * void set_deadtime(void)
 * {
 *    ///< DRV_MCPWM1在hal中声明的驱动ID
 *    ///< boost_h 在hal.h中的枚举值
 *    xh_mcpwm_set_p_deadtime(DRV_MCPWM1,boost_h,deadtime);
 * }
 * @endcode
 */
static inline void xh_mcpwm_set_p_deadtime(void *id, uint16_t ch, uint16_t deadtime)
{
    xh_mcpwm_t *p = (xh_mcpwm_t *)id;
    p->set_p_deadtime(p->core.var, ch, deadtime);
}

/**
 * @brief 设置N通道的死区时间（上升延迟时间）
 *
 * @param [in] id       驱动ID
 * @param [in] ch       通道
 * @param [in] deadtime 死区时间
 *
 * @details 特殊说明:
 * @par eg:设置MCPWM的boost_h对应的N通道死区为200
 * #include "hal.h"
 * uint32_t deadtime = 200;
 * void set_deadtime(void)
 * {
 *    ///< DRV_MCPWM1在hal中声明的驱动ID
 *    ///< boost_h 在hal.h中的枚举值
 *    xh_mcpwm_set_n_deadtime(DRV_MCPWM1,boost_h,deadtime);
 * }
 * @endcode
 */
static inline void xh_mcpwm_set_n_deadtime(void *id, uint16_t ch, uint16_t deadtime)
{
    xh_mcpwm_t *p = (xh_mcpwm_t *)id;
    p->set_n_deadtime(p->core.var, ch, deadtime);
}

/**
 * @brief 设置通道状态，P通道和N通道一起设置
 *
 * @param [in] id            驱动ID
 * @param [in] ch            通道
 * @param [in] cmd           通道状态的命令
 *
 * @details 特殊说明:
 * @par eg:设置驱动ID为DRV_MCPWM1的MCPWM通道0(boost_h)的P通道发波，N通道拉低
 * @code
 * #include "hal.h"
 * void set_ch_sta(void)
 * {
 *     ///< DRV_MCPWM1在hal中声明的驱动ID
 *    ///< boost_h 在hal.h中的枚举值
 *    xh_mcpwm_set_ch(DRV_MCPWM1,boost_h,XH_MCPWM_CH_P_PWM_N_LOW);
 * }
 * @endcode
 */
static inline void xh_mcpwm_set_ch(void *id, uint16_t ch, XH_MCPWM_CH_CTRL_CMD cmd)
{
    xh_mcpwm_t *p = (xh_mcpwm_t *)id;
    p->set_ch(p->core.var, ch, cmd);
}

/**
 * @brief 注册中断回调
 * @param [in] id            驱动ID
 * @param [in] type          中断类型
 * @param [in] irq_cb        回调函数
 *
 * @details 特殊说明:
 * @par eg:设置驱动ID为DRV_MCPWM0的MCPWM0的t0更新中断
 * @code
 * void mcpwm1_up_cb(void)
 * {
 *      //对应的控制内容
 * }
 * void init(void)
 * {
 *      xh_mcpwm_reg_irq(DRV_MCPWM0,XH_CNT0_MCPWM_T0_IRQ,mcpwm1_up_cb);
 * }
 * REG_INIT_MOD(init)
 * @endcode
 */
static inline void xh_mcpwm_reg_irq(void *id, XH_MCPWM_IRQ_TYPE type, void (*irq_cb)(void))
{
    xh_mcpwm_t *p = (xh_mcpwm_t *)id;
    p->reg_irq(p->core.var, type, irq_cb);
}

/**
 * @brief 设置通道模式
 *
 * @param [in] id            驱动ID
 * @param [in] ch            通道号
 * @param [in] mode          通道模式
 *
 * @details none
 * @par eg:将MCPWM0通道0(boost_h)模式设置为强制拉低
 * @code
 * #include "hal"
 * void set_ch_mode(void)
 * {
 *     ///< DRV_MCPWM0在hal中声明的驱动ID
 *    ///< boost_h 在hal.h中的枚举值
 *    xh_mcpwm_set_ch_mode(DRV_MCPWM0,boost_h,XH_MCPWM_LOW);
 * }
 * @endcode
 */
static inline void xh_mcpwm_set_ch_mode(void *id, XH_MCPWM_CH ch, XH_MCPWM_CH_MODE mode)
{
    xh_mcpwm_t *p = (xh_mcpwm_t *)id;
    p->set_ch_mode(p->core.var, ch, mode);
}

#endif //__xh_mcpwm_h__
