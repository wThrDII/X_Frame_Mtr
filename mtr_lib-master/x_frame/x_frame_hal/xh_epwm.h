#ifndef xh_epwm_h
#define xh_epwm_h
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

#include "xh_core.h"
    typedef enum
    {
        XH_EPWM_OUTPUT_A = 0,
        XH_EPWM_OUTPUT_B = 2,
    } XH_EPWM_OUTPUT;

    typedef enum
    {
        XH_CH_EPWM = 0, ///< 发波模式
        XH_CH_LOW = 1,  ///< 拉低
        XH_CH_HIGH = 2, ///< 拉高
    } XH_EPWM_CH_STA;

    typedef enum
    {
        XH_EPWM_ZERO = 0,       ///< 下溢
        XH_EPWM_PERIOD = 2,     ///< 上溢
        XH_EPWM_UP_CMPA = 4,    ///< 向上计数时 CMPA比较事件
        XH_EPWM_DOWN_CMPA = 6,  ///< 向下计数时 CMPA比较事件
        XH_EPWM_UP_CMPB = 8,    ///< 向上计数时 CMPB比较事件
        XH_EPWM_DOWN_CMPB = 10, ///< 向下计数时 CMPB比较事件
        ///< T1 T2
    } XH_EPWM_EVENT;

    typedef enum
    {
        XH_EPWM_NO_CHANGE = 0, //!< 没有动作
        XH_EPWM_LOW = 1,       //!< 拉低
        XH_EPWM_HIGH = 2,      //!< 拉高
        XH_EPWM_TOGGLE = 3     //!< 翻转
    } XH_EPWM_ACTION;

    typedef enum
    {
        XH_CTR_ZERO = 1,       // 下溢中断
        XH_CTR_PERIOD,         // 上溢中断
        XH_CTR_ZERO_OR_PERIOD, // 上溢下溢中断
        XH_CTR_U_CMPA,         // 向上计数时 CMPA
        XH_CTR_D_CMPA,         // 向下计数时 CMPA
        XH_CTR_U_CMPB,         // 向上计数时 CMPB
        XH_CTR_D_CMPB,         // 向下计数时 CMPB
        XH_CTR_TZ_EVENT = 16,  // TZ事件中断
    } XH_EPWM_IRQ_TYPE;

    typedef enum
    {
        XH_EPWM_TZ_FLAG_CBC    = 0x2U,  ///< TZ事件CBC标志置位，CMPSS和IO封波都会返回此值
        XH_EPWM_TZ_FLAG_OST    = 0x4U,  ///< TZ事件OST标志置位，CMPSS和IO封波都会返回此值
    } XH_TZ_FLAG;

    typedef enum
    {
        XH_EPWM_TZ_ACTION_EVENT_TZA = 0,     //!< TZ1 - TZ6, EPWM通道A
        XH_EPWM_TZ_ACTION_EVENT_TZB = 2     //!< TZ1 - TZ6, EPWM通道B
    } XH_EPWM_TZ_EVENT;

    typedef enum
    {
        XH_EPWM_TZ_ACTION_HIGH_Z  = 0, //!< High impedance output
        XH_EPWM_TZ_ACTION_HIGH    = 1, //!< High voltage state
        XH_EPWM_TZ_ACTION_LOW     = 2, //!< Low voltage state
        XH_EPWM_TZ_ACTION_DISABLE = 3  //!< Disable action
    } XH_EPWM_TZ_ACTION;

    typedef void (*xh_epwm_enable_obj)(void *val);
    typedef void (*xh_epwm_disable_obj)(void *val);
    typedef void (*xh_epwm_set_ch_sta_obj)(void *val, uint32_t ch, uint32_t sta);
    typedef void (*xh_epwm_set_freq_obj)(void *val, uint32_t freq);
    typedef void (*xh_epwm_set_prd_obj)(void *val, uint32_t prd);
    typedef void (*xh_epwm_set_clk_div_obj)(void *val, uint32_t clkdiv, uint32_t hsp_clkdiv);
    typedef void (*xh_epwm_set_compa_obj)(void *val, uint32_t duty_cycle);
    typedef void (*xh_epwm_set_compa_directly_obj)(void *val, uint32_t compa);
    typedef void (*xh_epwm_set_compb_obj)(void *val, uint32_t duty_cycle);
    typedef void (*xh_epwm_set_compb_directly_obj)(void *val, uint32_t compb);
    typedef void (*xh_epwm_set_aq_obj)(void *val, uint32_t ch, XH_EPWM_EVENT event, XH_EPWM_ACTION act);
    typedef void (*xh_epwm_set_red_obj)(void *val, uint32_t deadtime);
    typedef void (*xh_epwm_set_fed_obj)(void *val, uint32_t deadtime);
    typedef void (*xh_epwm_reg_irq_obj)(void *val, uint32_t irq_type, uint32_t cnt, void (*irq_cb)(void));
    typedef uint32_t (*xh_epwm_get_tz_flag_obj)(void *val);
    typedef uint32_t (*xh_epwm_get_counter_value_obj)(void *val);
    typedef uint32_t (*xh_epwm_get_prd_obj)(void *val);
    typedef void (*xh_epwm_clear_tz_flag_obj)(void *val);
    typedef void (*xh_epwm_set_tz_action_obj)(void *val, XH_EPWM_TZ_EVENT tz_event, XH_EPWM_TZ_ACTION tz_action);

    typedef struct _xh_epwm_t
    {
        xh_comm_t core;
        xh_epwm_enable_obj enable;
        xh_epwm_disable_obj disable;
        xh_epwm_set_ch_sta_obj set_ch_sta;
        xh_epwm_set_freq_obj set_freq;
        xh_epwm_set_prd_obj set_prd;
        xh_epwm_set_clk_div_obj set_clk_div;
        xh_epwm_set_compa_obj set_compa;
        xh_epwm_set_compa_directly_obj set_compa_directly;
        xh_epwm_set_compb_obj set_compb;
        xh_epwm_set_compb_directly_obj set_compb_directly;
        xh_epwm_set_aq_obj set_aq;
        xh_epwm_set_red_obj set_red;
        xh_epwm_set_fed_obj set_fed;
        xh_epwm_reg_irq_obj reg_irq;
        xh_epwm_get_tz_flag_obj get_tz_flag;
        xh_epwm_get_counter_value_obj get_counter_value;
        xh_epwm_get_prd_obj get_prd;
        xh_epwm_clear_tz_flag_obj clear_tz_flag;
        xh_epwm_set_tz_action_obj set_tz_action;
    } xh_epwm_t, *p_xh_epwm_t;

/* 驱动宏定义实现 */
#define REG_XH_EPWM_DRV(id, var, prio, auto_init, init, deinit, enable, disable, set_ch_sta, set_freq, set_prd,     \
                        set_clk_div, set_compa, set_compa_directly, set_compb, set_compb_directly, set_aq, set_red, \
                        set_fed, reg_irq,get_tz_flag, get_counter_value, get_prd, clear_tz_flag, set_tz_action)     \
    REG_XH_DEV(xh_epwm_t, id, var, prio, auto_init, init, deinit,                                                   \
               (xh_epwm_enable_obj)enable,                                                                          \
               (xh_epwm_disable_obj)disable,                                                                        \
               (xh_epwm_set_ch_sta_obj)set_ch_sta,                                                                  \
               (xh_epwm_set_freq_obj)set_freq,                                                                      \
               (xh_epwm_set_prd_obj)set_prd,                                                                        \
               (xh_epwm_set_clk_div_obj)set_clk_div,                                                                \
               (xh_epwm_set_compa_obj)set_compa,                                                                    \
               (xh_epwm_set_compa_directly_obj)set_compa_directly,                                                  \
               (xh_epwm_set_compb_obj)set_compb,                                                                    \
               (xh_epwm_set_compb_directly_obj)set_compb_directly,                                                  \
               (xh_epwm_set_aq_obj)set_aq,                                                                          \
               (xh_epwm_set_red_obj)set_red,                                                                        \
               (xh_epwm_set_fed_obj)set_fed,                                                                        \
               (xh_epwm_reg_irq_obj)reg_irq,                                                                        \
               (xh_epwm_get_tz_flag_obj)get_tz_flag,                                                                \
               (xh_epwm_get_counter_value_obj)get_counter_value,                                                    \
               (xh_epwm_get_prd_obj) get_prd,                                                                       \
               (xh_epwm_clear_tz_flag_obj)clear_tz_flag,                                                             \
               (xh_epwm_set_tz_action_obj)set_tz_action) 

#define EXT_XH_EPWM_DRV(id) EXT_XH_DEV(xh_epwm_t, id)

    /**
     * @brief 使能ePWM
     *
     * @param [in] id            驱动ID
     *
     * @details 使能ePWM时钟 和 使能时基，使用同步模式时，只需要使能作为同步源的ePWM
     * @par eg:使能ePWM1
     * @code
     * #include "hal.h"
     * void epwm_enable(void)
     * {
     *          ///< DRV_EPWM1在hal中声明的驱动ID
     *      xh_epwm_enable(DRV_EPWM1);
     * }
     * @endcode
     */
    static inline void xh_epwm_enable(void *id)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->enable(p->core.var);
    }

    /**
     * @brief 失能ePWM
     *
     * @param [in] id            驱动ID
     *
     * @details 失能ePWM时钟
     * @par eg:失能ePWM1
     * @code
     * #include "hal.h"
     * void epwm_disable(void)
     * {
     *          ///< DRV_EPWM1在hal中声明的驱动ID
     *      xh_epwm_disable(DRV_EPWM1);///< DRV_EPWM1在hal中声明的驱动ID
     * }
     * @endcode
     */
    static inline void xh_epwm_disable(void *id)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->disable(p->core.var);
    }

    /**
     * @brief 设置通道状态，优化等级为2耗时 0.6us —— 1us
     *
     * @param [in] id            驱动ID
     * @param [in] ch            通道号
     * @param [in] sta           状态
     *
     * @details ch：hal中的枚举
     *          sta：通道状态 XH_EPWM_CH_STA
     * @par eg:设置EPWM1的通道A(boost_l)为发波状态
     * @code
     * #include "hal.h"
     * void enable_ch(void)
     * {
     *          ///< DRV_EPWM1在hal中声明的驱动ID
     *      //< boost_l 是在hal.h中的通道号枚举
     *      //< XH_CH_EPWM 是在xh_epwm.h 中的枚举表 XH_EPWM_CH_STA 里
     *    xh_epwm_set_ch_sta(DRV_EPWM1,boost_l,XH_CH_EPWM);
     * }
     * @endcode
     */
    static inline void xh_epwm_set_ch_sta(void *id, uint32_t ch, uint32_t sta)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->set_ch_sta(p->core.var, ch, sta);
    }

    /**
     * @brief 设置时基频率,优化等级为2耗时 1.96us
     *
     * @param [in] id            驱动ID
     * @param [in] freq          频率
     *
     * @details none
     * @par eg:设置EPWM1的频率为40KHZ
     * @code
     * #include "hal.h"
     * uint32_t freq = 40000;///< 频率
     * void enable_ch(void)
     * {
     *     ///< DRV_EPWM1在hal中声明的驱动ID
     *    xh_epwm_set_freq(DRV_EPWM1,freq);
     * }
     *
     * @endcode
     */
    static inline void xh_epwm_set_freq(void *id, uint32_t freq)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->set_freq(p->core.var, freq);
    }

    /**
     * @brief 直接设置ePWM的PRD值，优化等级为2耗时 0.46us
     *
     * @param [in] id         驱动ID
     * @param [in] prd        设置的prd值
     *
     * @details 特殊说明: prd最大值为0xFFFF
     * @par eg:设置驱动ID为DRV_EPWM1的ePWM的PRD值为500
     * @code
     *    xh_epwm_set_prd(DRV_EPWM1,500);
     * @endcode
     */
    static inline void xh_epwm_set_prd(void *id, uint32_t prd)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->set_prd(p->core.var, prd);
    }

    /**
     * @brief 设置ePWM的分频值
     *
     * @param [in] id           驱动ID
     * @param [in] clkdiv       分频值
     * @param [in] hsp_clkdiv   HSP分频值
     *
     * @details 特殊说明:
     * TBCLK = EPWMCLK/(HSPCLKDIV * CLKDIV),HSPCLKDIV为hsp_clkdiv分频值,CLKDIV为clkdiv分频值
     *
     * clkdiv设置值:  0    1   2   3   4   5   6   7
     * clkdiv分频值: /1   /2  /4  /8  /16 /32 /64 /128
     *
     * hsp_clkdiv设置值:  0    1   2   3   4   5   6   7
     * hsp_clkdiv分频值: /1   /2  /4  /8  /16 /32 /64 /128
     *
     * @par eg:设置ePWM时钟0分频，驱动ID为DRV_EPWM1
     * @code
     *  xh_epwm_set_clk_div(DRV_EPWM1, 0, 0);
     * @endcode
     */
    static inline void xh_epwm_set_clk_div(void *id, uint32_t clkdiv, uint32_t hsp_clkdiv)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->set_clk_div(p->core.var, clkdiv, hsp_clkdiv);
    }

    /**
     * @brief 设置比较值A，输入值为占空比，内部会根据占空比算COMPA值，优化等级为2耗时 0.76us
     *
     * @param [in] id            驱动ID
     * @param [in] duty_cycle    占空比
     *
     * @details duty_cycle：占空比 0~10000对应 0%~100%
     * @par eg:设置EPWM1的比较值A为66.66%
     * @code
     * #include "hal.h"
     * uint32_t dutya = 6666;///< 比较值A 范围：0~10000
     * void set_ch_duty(void)
     * {
     *     ///< DRV_EPWM1在hal中声明的驱动ID
     *    xh_epwm_set_compa(DRV_EPWM1,dutya);
     * }
     *
     * @endcode
     */
    static inline void xh_epwm_set_compa(void *id, uint32_t duty_cycle)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->set_compa(p->core.var, duty_cycle);
    }

    /**
     * @brief 直接设置比较值A，输入值为COMPA值，优化等级为2耗时 0.4us
     *
     * @param [in] id            驱动ID
     * @param [in] compa         COMPA值
     *
     * @details 特殊说明:compa最大为0xFFFF
     * @par eg:设置COMPA值为200,驱动ID为DRV_EPWM1
     * @code
     *  xh_epwm_set_compa_directly(DRV_EPWM1, 200);
     * @endcode
     */
    static inline void xh_epwm_set_compa_directly(void *id, uint32_t compa)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->set_compa_directly(p->core.var, compa);
    }

    /**
     * @brief 设置比较值B，输入值为占空比，内部会根据占空比算COMPB值，优化等级为2耗时 0.76us
     *
     * @param [in] id            驱动ID
     * @param [in] duty_cycle    占空比
     *
     * @details duty_cycle：占空比 0~10000对应 0%~100%
     * @par eg:设置EPWM1的比较值B为77.77%
     * @code
     * #include "hal.h"
     * uint32_t dutyb = 6666;///< 比较值B 范围：0~10000
     * void set_ch_duty(void)
     * {
     *     ///< DRV_EPWM1在hal中声明的驱动ID
     *    xh_epwm_set_compb(DRV_EPWM1,dutyb);
     * }
     * @endcode
     */
    static inline void xh_epwm_set_compb(void *id, uint32_t duty_cycle)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->set_compb(p->core.var, duty_cycle);
    }

    /**
     * @brief 直接设置比较值b，输入值为COMPB值，优化等级为2耗时 0.46us
     *
     * @param [in] id            驱动ID
     * @param [in] compb         COMPB值
     *
     * @details 特殊说明: compb最大为0xFFFF
     * @par eg:设置COMPB值为200,驱动ID为DRV_EPWM1
     * @code
     *  xh_epwm_set_compb_directly(DRV_EPWM1, 200);
     * @endcode
     */
    static inline void xh_epwm_set_compb_directly(void *id, uint32_t compb)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->set_compb_directly(p->core.var, compb);
    }

    /**
     * @brief 设置通道动作
     *
     * @param [in] id            驱动ID
     * @param [in] epwmoutput    ePWM通道
     * @param [in] event         事件
     * @param [in] act           动作
     *
     * @details 设置通道在各个事件时的动作
     *          epwmoutput：通道 XH_EPWM_OUTPUT中的枚举 (注意不是通道号)
     *          event：事件XH_EPWM_EVENT中的枚举
     *          act：动作 XH_EPWM_ACTION中的枚举
     * @par eg:设置EPWM1的通道A在向上计数比较值A时拉高
     * @code
     * #include "hal.h"
     * void set_ch_aq(void)
     * {
     *     ///< DRV_EPWM1在hal中声明的驱动ID
     *     ///< XH_EPWM_OUTPUT_A：输出通道；枚举表XH_EPWM_OUTPUT中
     *     ///< XH_EPWM_UP_CMPA：事件；枚举表XH_EPWM_EVENT中
     *     ///< XH_EPWM_HIGH: 动作； 枚举表XH_EPWM_ACTION中
     *    xh_epwm_set_aq(DRV_EPWM1,XH_EPWM_OUTPUT_A,XH_EPWM_UP_CMPA,XH_EPWM_HIGH);
     * }
     * @endcode
     */
    static inline void xh_epwm_set_aq(void *id, XH_EPWM_OUTPUT epwmoutput, XH_EPWM_EVENT event, XH_EPWM_ACTION act)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->set_aq(p->core.var, epwmoutput, event, act);
    }

    /**
     * @brief 设置上升沿延时时间
     *
     * @param [in] id            驱动ID
     * @param [in] deadtime      延时时间
     *
     * @details deadtime：延时时间，延时多少个计数值
     * @par eg:设置EPWM1上升沿延时100个计数值
     * @code
     * #include "hal.h"
     * uint32_t red = 100;///< 上升沿延时时间
     * void set_red(void)
     * {
     *     ///< DRV_EPWM1在hal中声明的驱动ID
     *      xh_epwm_set_red(DRV_EPWM1,red);
     * }
     * @endcode
     */
    static inline void xh_epwm_set_red(void *id, uint32_t deadtime)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->set_red(p->core.var, deadtime);
    }

    /**
     * @brief 设置下降沿延时时间
     *
     * @param [in] id            驱动ID
     * @param [in] deadtime      延时时间
     *
     * @details deadtime：延时时间，延时多少个计数值
     * @par eg:设置EPWM1下降沿延时100个计数值
     * @code
     * #include "hal.h"
     * uint32_t fed = 100;///< 下降沿延时时间
     * void set_fed(void)
     * {
     *     ///< DRV_EPWM1在hal中声明的驱动ID
     *      xh_epwm_set_fed(DRV_EPWM1,fed);
     * }
     * @endcode
     */
    static inline void xh_epwm_set_fed(void *id, uint32_t deadtime)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->set_fed(p->core.var, deadtime);
    }

    /**
     * @brief 注册中断回调函数
     *
     * @param [in] id            驱动ID
     * @param [in] irq_type      中断类型
     * @param [in] cnt           第几次事件产生中断
     * @param [in] irq_cb        回调函数
     *
     * @details irq_type：XH_EPWM_IRQ_TYPE中的枚举
     *          cnt：第几次事件产生中断，
     *          irq_cb：类型为 void (*func)(void) 没有形参和返回值
     * @par eg:在EPWM1的每次上溢中断触发回调函数
     * @code
     * void reg_epwm_cb(void)
     * {
     *      ///< 函数内容
     * }
     * void init(void)
     * {
     *      ///< XH_EPWM_PERIOD :中断类型；在枚举表XH_EPWM_IRQ_TYPE
     *      xh_epwm_reg_irq(DRV_EPWM1,XH_EPWM_PERIOD,1,reg_epwm_cb)
     * }
     * REG_INIT_MOD(init)
     * @endcode
     */
    static inline void xh_epwm_reg_irq(void *id, uint32_t irq_type, uint32_t cnt, void (*irq_cb)(void))
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->reg_irq(p->core.var, irq_type, cnt, irq_cb);
    }

    /**
     * @brief 
     * 
     * @param [in] id           驱动ID
     * @param [in] tz_event     TZ类型，选择通道A或者通道B
     * @param [in] tz_action    TZ动作
     * 
     * @details 特殊说明:
     * @par eg:设置EPWM的通道A和通道B发生TZ事件时都拉低
     * @code
     *    xh_epwm_set_tz_action(DRV_EPWM1, XH_EPWM_TZ_ACTION_EVENT_TZA, XH_EPWM_TZ_ACTION_LOW);
     *    xh_epwm_set_tz_action(DRV_EPWM1, XH_EPWM_TZ_ACTION_EVENT_TZB, XH_EPWM_TZ_ACTION_LOW);
     * @endcode
     */
    static inline void xh_epwm_set_tz_action(void *id, XH_EPWM_TZ_EVENT tz_event, XH_EPWM_TZ_ACTION tz_action)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->set_tz_action(p->core.var, tz_event, tz_action);
    }

    /**
     * @brief 获取TZ标志位
     * 
     * @param [in] id     驱动ID
     * 
     * @return uint32_t 
     *  @retval 返回获取的标志
     * @details 特殊说明: 返回的数值意义在枚举 XH_TZ_FLAG 中定义，读取后不会清除标志位，
     *                    需调用xh_epwm_clear_tz_flag清除标志位
     * @par eg:CBC模式下触发了封波
     * @code
     *  uint16_t flag = 0;
     *  flag = xh_epwm_get_tz_flag(DRV_EPWM1); //此时flag应等于XH_EPWM_TZ_FLAG_CBC
     * @endcode
     */
    static inline uint32_t xh_epwm_get_tz_flag(void *id)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        return p->get_tz_flag(p->core.var);
    }

    /**
     * @brief 获取EPWM的计数值（TBCTR值）
     *
     * @param [in] id   驱动ID
     *
     * @return uint32_t
     *  @retval 返回EPWM的计数值
     * @details 特殊说明:
     * @par eg:获取驱动ID为DRV_EPWM1的EPWM的计数值
     * @code
     * uint16_t counter_val = 0;
     * counter_val = xh_epwm_get_counter_value(DRV_EPWM1);
     * @endcode
     */
    static inline uint32_t xh_epwm_get_counter_value(void *id)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        return p->get_counter_value(p->core.var);
    }

    /**
     * @brief 获取EPWM的PRD值（Period值）
     * 
     * @param [in] id            
     * 
     * @return uint32_t 
     *  @retval 返回PRD值
     * @details 特殊说明: 
     * @par eg:获取驱动ID为DRV_EPWM1的EPWM的PRD值
     * @code
     * uint16_t prd = 0;
     * prd = xh_epwm_get_prd(DRV_EPWM1);
     * @endcode
     */
    static inline uint32_t xh_epwm_get_prd(void *id)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        return p->get_prd(p->core.var);
    }

    /**
     * @brief 单次tz封波清除标志位，清除标志后重新发波
     *
     * @param [in] id       驱动ID
     *
     * @details 特殊说明:
     * @par eg:清除驱动ID为DRV_EPWM1的TZ标志，清除后若不触发封波则发波，一般用于单次封波模式
     * @code
     *    xh_epwm_clear_tz_flag(DRV_EPWM1);
     * @endcode
     */
    static inline void xh_epwm_clear_tz_flag(void *id)
    {
        xh_epwm_t *p = (xh_epwm_t *)id;
        p->clear_tz_flag(p->core.var);
    }

#ifdef __cplusplus
}
#endif
#endif /* xh_epwm_h */
