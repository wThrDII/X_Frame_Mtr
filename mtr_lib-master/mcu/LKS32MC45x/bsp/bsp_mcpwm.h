/**
 * @brief LKS32MC45X的mcpwm驱动
 * @file bsp_mcpwm.h
 * @version 1.0
 * @author peixiang.qiu
 * @date 2023-01-12 11:14:17
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2023-01-12 11:14:17 <td>1.0       <td>peixiang.qiu      <td>Created function
 */

#ifndef BSP_MCPWM_H
#define BSP_MCPWM_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "lks32mc45x.h"
#include "lks32mc45x_lib.h"
// #include "bsp_time_base.h"
#include "system_lks32mc45x.h"
#include "xh_mcpwm.h"

///< 内部BSP层使用
/*************************************************************/
#define CH_IO_CTRL_REG (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6) ///< 通道输出控制寄存器位

#define CH0_IO_CTRL_REG (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6)      ///< 通道0输出控制寄存器位
#define CH1_IO_CTRL_REG (BIT8 | BIT9 | BIT10 | BIT11 | BIT12 | BIT13 | BIT14) ///< 通道1输出控制寄存器位
#define CH2_IO_CTRL_REG (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6)      ///< 通道2输出控制寄存器位
#define CH3_IO_CTRL_REG (BIT8 | BIT9 | BIT10 | BIT11 | BIT12 | BIT13 | BIT14) ///< 通道3输出控制寄存器位

#define MCPWM_CNT_CTRL_REG (BIT6 | BIT7) ///< 计数器控制位
    /*************************************************************/

#define TRIGGER_NONE 12345

#define BSP_MCPWM_CH_0 0 ///< 通道选择
#define BSP_MCPWM_CH_1 1
#define BSP_MCPWM_CH_2 2
#define BSP_MCPWM_CH_3 3
#define CH_MAX 4

#define TMR_TimeBase_Sel_0 0 ///< TMR的时基选择
#define TMR_TimeBase_Sel_1 1

#define BSP_DEFAULT_OUTPUT_LOW 0 ///< MCPWM的默认输出
#define BSP_DEFAULT_OUTPUT_HIGHT 1

#define BSP_CH_PN_CHANGE_ENABLE 0  ///< 交换P和N的输出
#define BSP_CH_PN_CHANGE_DISABLE 1 ///< 不交换P和N的输出

#define FAIL_Polarity_ACTIVE_LOW 0   ///< FAIL极性设置低有效
#define FAIL_POLARITY_ACTIVE_HIGHT 1 ///< FAIL极性设置高有效

    typedef enum
    {
        BSP_MCPWM_CH_P_LOW_N_PWM,   ///< P拉低，N发波
        BSP_MCPWM_CH_P_LOW_N_HIGHT, ///< P拉低，N拉高
        BSP_MCPWM_CH_P_PWM_N_LOW,   ///< P发波，N拉低
        BSP_MCPWM_CH_P_PWM_N_HIGHT, ///< P发波，N拉高
        BSP_MCPWM_CH_P_HIGHT_N_PWM, ///< P拉高，N发波
        BSP_MCPWM_CH_P_HIGHT_N_LOW, ///< P拉高，N拉低
        BSP_MCPWM_CH_P_LOW_N_LOW,   ///< P拉低，N拉低
        BSP_MCPWM_CH_P_PWM_N_PWM,   ///< P发波，N发波
        BSP_MCPWM_CH_CTRL_MAX,      ///< P发波，N发波
    } BSP_MCPWM_CTRL_CMD;

    ///< MCPWM中断类型选择,和IE寄存器位对应
    typedef enum
    {
        BSP_IE0_MCPWM_T0_IRQ = 0, ///< IE0控制
        BSP_IE0_MCPWM_T1_IRQ,
        BSP_IE0_MCPWM_TH00_IRQ,
        BSP_IE0_MCPWM_TH01_IRQ,
        BSP_IE0_MCPWM_TH10_IRQ,
        BSP_IE0_MCPWM_TH11_IRQ,
        BSP_IE0_MCPWM_TH20_IRQ,
        BSP_IE0_MCPWM_TH21_IRQ,
        BSP_IE0_MCPWM_TMR0_IRQ,
        BSP_IE0_MCPWM_TMR1_IRQ,
        BSP_IE0_MCPWM_UP_IRQ, //=10

        BSP_IE1_MCPWM_T0_IRQ, ///< IE1控制
        BSP_IE1_MCPWM_T1_IRQ,
        BSP_IE1_MCPWM_TH30_IRQ,
        BSP_IE1_MCPWM_TH31_IRQ,
        BSP_IE1_MCPWM_UP_IRQ, //=15

        BSP_IE01_MCPWM_TMR2_IRQ, ///< 根据时基选择IE0或IE1控制
        BSP_IE01_MCPWM_TMR3_IRQ,
        BSP_IE_MCPWM_IRQ_MAX,

        // BSP_IE0_MCPWM_FAIL3_IRQ, ///< FAIL控制 TODO:FAIL中断事件未确定
        // BSP_IE0_MCPWM_FAIL2_IRQ,
        // BSP_IE0_MCPWM_FAIL1_IRQ,
        // BSP_IE0_MCPWM_FAIL0_IRQ,
    } BSP_MCPWM_INT_LIST;

    typedef struct _BSP_MCPWM_FAIL_GPIO_OBJ
    {
        GPIO_TypeDef *bkin_gpio_port;
        uint32_t bkin_gpio_pin;
    } BSP_MCPWM_FAIL_GPIO_OBJ, *P_BSP_MCPWM_FAIL_GPIO_OBJ;

    typedef struct _BSP_MCPWM_FAIL_OBJ
    {
        FuncState mcpwm_auto_err_en; ///< 封波后可自动恢复开关
        FuncState fail0_input_en;
        FuncState fail1_input_en;
        FuncState fail2_input_en;
        FuncState fail3_input_en;
        uint8_t fail0_polarity;
        uint8_t fail1_polarity;
        uint8_t fail2_polarity;
        uint8_t fail3_polarity;
        P_BSP_MCPWM_FAIL_GPIO_OBJ p_mcpwm_fail_gpio;
        uint8_t fail_gpio_num;
    } BSP_MCPWM_FAIL_OBJ, *P_BSP_MCPWM_FAIL_OBJ;
    extern BSP_MCPWM_FAIL_OBJ bsp_mcpwm_fail_NULL; ///< 不使用FAIL

//< 封波FAIL参数
#define DEF_MCPWM_FAIL(mcpwm_fail, mcpwm_auto_err_en, fail0_input_en, fail1_input_en, fail2_input_en, fail3_input_en, fail0_polarity, fail1_polarity, \
                       fail2_polarity, fail3_polarity, fail_gpio...)                                                                                  \
    BSP_MCPWM_FAIL_GPIO_OBJ bsp_mcpwm_fail_gpio_##mcpwm_fail[] = {fail_gpio};                                                                         \
    BSP_MCPWM_FAIL_OBJ bsp_mcpwm_fail_##mcpwm_fail = {                                                                                                \
        mcpwm_auto_err_en,                                                                                                                            \
        fail0_input_en,                                                                                                                               \
        fail1_input_en,                                                                                                                               \
        fail2_input_en,                                                                                                                               \
        fail3_input_en,                                                                                                                               \
        fail0_polarity,                                                                                                                               \
        fail1_polarity,                                                                                                                               \
        fail2_polarity,                                                                                                                               \
        fail3_polarity,                                                                                                                               \
        bsp_mcpwm_fail_gpio_##mcpwm_fail,                                                                                                             \
        sizeof(bsp_mcpwm_fail_gpio_##mcpwm_fail) / sizeof(bsp_mcpwm_fail_gpio_##mcpwm_fail[0]),                                                       \
    };

    typedef struct _BSP_MCPWM_CLOCK
    {
        uint32_t freq_012;
        uint32_t freq_3;
        uint32_t clkdiv;
        uint16_t max_cnt_012;
        uint16_t max_cnt_3;
    } BSP_MCPWM_CLOCK, *P_BSP_MCPWM_CLOCK;
    typedef struct _BSP_MCPWM_CH_OBJ
    {
        uint8_t mcpwm_ch;
        uint8_t ch_pn_change_sw; /* 交换CH0N, CH0P信号输出使能开关 */
        float def_duty_cycle;    /* 0 ~ 10000 */
        // uint16_t aligned_mode;
        GPIO_TypeDef *p_gpio_port;
        uint32_t p_gpio_pin;
        GPIO_TypeDef *n_gpio_port;
        uint32_t n_gpio_pin;
        uint8_t p_polarity_inv;
        uint8_t n_polarity_inv;
        uint8_t p_default_output;
        uint8_t n_default_output;
        uint16_t p_deadtime;
        uint16_t n_deadtime;
    } BSP_MCPWM_CH_OBJ, *P_BSP_MCPWM_CH_OBJ;

    typedef struct _BSP_MCPWM_OBJ_T
    {
        char *mcpwm;
        MCPWM_TypeDef *mcpwm_periph;
        uint16_t aligned_mode_012;
        uint16_t aligned_mode_3;
        uint32_t sys_module;
        BSP_MCPWM_CLOCK clk;
        uint8_t tmr2_timebase_sel;
        uint8_t tmr3_timebase_sel;
        int16_t tmr0;
        int16_t tmr1;
        int16_t tmr2;
        int16_t tmr3;

        P_BSP_MCPWM_CH_OBJ p_mcpwm_ch;
        uint8_t ch_num;
        P_BSP_MCPWM_FAIL_OBJ mcpwm_fail;
        IRQn_Type nvic_irq;
        uint16_t mcpwm_x0_irq_flag;    // MCPWM标志位
        uint16_t mcpwm_x1_irq_flag;    // MCPWM标志位
        uint8_t priority;              // 中断优先级
        void (*mcpwm_irq_x0_cb)(void); // IE0中断回调函数
        void (*mcpwm_irq_x1_cb)(void); // IE1中断回调函数
    } BSP_MCPWM_OBJ_T, *P_BSP_MCPWM_OBJ_T;

/**
 * @brief 注册PWM驱动
 *
 * @param [in] dev_id            驱动ID
 * @param [in] prio              驱动注册优先级
 * @param [in] timer_periph      定时器
 * @param [in] freq              频率
 * @param [in] alignedmode       对齐方式
 * @param [in] deadtime          死区时间
 * @param [in] re_counter        重复计数器值
 * @param [in] nvic_irq          中断号
 * @param [in] priority          优先级
 * @param [in] sub_priority      从优先级
 * @param [in] time_updata_cb    回调函数
 * @param [in] trigger           使能前触发源
 * @param [in] input_trigger     输入源
 * @param [in] output_trigger    使能后触发源
 * @param [in] pwm_ch          通道参数
 * @details 无触发源则填 TRIGGER_NONE
 */
//#define REG_XH_MCPWM(dev_id, prio, mcpwm_periph, aligned_mode_012, aligned_mode_3, freq_012, freq_3, tmr2_timebase_sel, tmr3_timebase_sel, tmr0, tmr1, tmr2,   \
//                     tmr3, mcpwm_fail, pwm_ch...)                                                                                                              \
//    BSP_MCPWM_CH_OBJ g_bsp_pwm_ch_##dev_id[] = {pwm_ch};                                                                                                       \
//    BSP_MCPWM_OBJ_T _##dev_id = {                                                                                                                              \
//        #mcpwm_periph,                                                                                                                                         \
//        mcpwm_periph,                                                                                                                                          \
//        aligned_mode_012,                                                                                                                                      \
//        aligned_mode_3,                                                                                                                                        \
//        SYS_MODULE_##mcpwm_periph,                                                                                                                             \
//        {freq_012, freq_3, 0},                                                                                                                                 \
//        tmr2_timebase_sel,                                                                                                                                     \
//        tmr3_timebase_sel,                                                                                                                                     \
//        tmr0,                                                                                                                                                  \
//        tmr1,                                                                                                                                                  \
//        tmr2,                                                                                                                                                  \
//        tmr3,                                                                                                                                                  \
//        g_bsp_pwm_ch_##dev_id,                                                                                                                                 \
//        sizeof(g_bsp_pwm_ch_##dev_id) / sizeof(g_bsp_pwm_ch_##dev_id[0]),                                                                                      \
//        &bsp_mcpwm_fail_##mcpwm_fail,                                                                                                                          \
//    };                                                                                                                                                         \
//    void mcpwm_periph##0_IRQHandler(void)                                                                                                                      \
//    {                                                                                                                                                          \
//        bsp_mcpwm_x0_cb(&_##dev_id);                                                                                                                           \
//    }                                                                                                                                                          \
//    void mcpwm_periph##1_IRQHandler(void)                                                                                                                      \
//    {                                                                                                                                                          \
//        bsp_mcpwm_x1_cb(&_##dev_id);                                                                                                                           \
//    }                                                                                                                                                          \
//    REG_XH_MCPWM_DRV(dev_id, &_##dev_id, prio, DEV_AUTO_INIT, bsp_mcpwm_init, bsp_mcpwm_deinit, bsp_mcpwm_disable, bsp_mcpwm_enable, bsp_mcpwm_set_duty_cycle, \
//                     bsp_mcpwm_set_freq, bsp_mcpwm_set_p_deadtime, bsp_mcpwm_set_n_deadtime, bsp_mcpwm_set_ch, bsp_mcpwm_reg_irq, bsp_mcpwm_set_ch_mode);

#define REG_XH_MCPWM(dev_id, prio, mcpwm_periph)                                                                                                               \
    BSP_MCPWM_OBJ_T _##dev_id = {                                                                                                                              \
            #mcpwm_periph,                                                                                                                                     \
            mcpwm_periph,                                                                                                                                      \
    };                                                                                                                                                         \
    REG_XH_MCPWM_DRV(dev_id, &_##dev_id, prio, DEV_AUTO_INIT, bsp_mcpwm_init, bsp_mcpwm_deinit, bsp_mcpwm_disable, bsp_mcpwm_enable, bsp_mcpwm_set_duty_cycle, \
                     bsp_mcpwm_set_freq, bsp_mcpwm_set_p_deadtime, bsp_mcpwm_set_n_deadtime, bsp_mcpwm_set_ch, bsp_mcpwm_reg_irq, bsp_mcpwm_set_ch_mode);

    
    extern void bsp_mcpwm_init(P_BSP_MCPWM_OBJ_T p_mcpwm);
    extern void bsp_mcpwm_deinit(P_BSP_MCPWM_OBJ_T p_mcpwm);
    extern void bsp_mcpwm_disable(P_BSP_MCPWM_OBJ_T p_mcpwm);
    extern void bsp_mcpwm_enable(P_BSP_MCPWM_OBJ_T p_mcpwm);
    extern void bsp_mcpwm_set_duty_cycle(P_BSP_MCPWM_OBJ_T p_mcpwm, uint16_t ch, uint16_t duty_cycle);
    extern void bsp_mcpwm_set_freq(P_BSP_MCPWM_OBJ_T p_mcpwm, uint16_t ch, uint32_t freq);

    extern void bsp_mcpwm_set_p_deadtime(P_BSP_MCPWM_OBJ_T p_mcpwm, uint16_t ch, uint16_t deadtime);
    extern void bsp_mcpwm_set_n_deadtime(P_BSP_MCPWM_OBJ_T p_mcpwm, uint16_t ch, uint16_t deadtime);
    extern void bsp_mcpwm_set_ch(P_BSP_MCPWM_OBJ_T p_mcpwm, uint16_t ch, uint8_t cmd);
    extern void bsp_mcpwm_reg_irq(P_BSP_MCPWM_OBJ_T p_mcpwm, uint32_t type, void (*irq_cb)(void));
    extern void bsp_mcpwm_set_ch_mode(P_BSP_MCPWM_OBJ_T p_mcpwm, uint16_t ch, uint8_t mode);

    extern void bsp_mcpwm_x0_cb(P_BSP_MCPWM_OBJ_T p_mcpwm);
    extern void bsp_mcpwm_x1_cb(P_BSP_MCPWM_OBJ_T p_mcpwm);
#ifdef __cplusplus
}
#endif
#endif /* BSP_MCPWM_H */
