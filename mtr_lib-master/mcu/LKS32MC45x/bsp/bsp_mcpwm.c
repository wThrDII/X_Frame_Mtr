/**
 * @brief LKS32MC45X的mcpwm驱动
 * @file bsp_mcpwm.c
 * @version 1.0
 * @author peixiang.qiu
 * @date 2023-01-12 16:50:17
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2023-01-12 16:50:17 <td>1.0       <td>peixiang.qiu      <td>Created function
 */

#include "bsp_mcpwm.h"
#include "x_frame.h"
#if PROGRAM_TYPE == PROGRAM_APL
#include "mcpwm_cfg.h"
#endif

#define BSP_PWM_MAX_DUTY_CYCLE 10000
BSP_MCPWM_FAIL_OBJ bsp_mcpwm_fail_NULL; ///< 不使用fail
/**
 * @brief mcpwm的定时器初始化
 *
 * @param [in] p_mcpwm       MCPWM参数指针
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
//static void bsp_mcpwm_gpio_config(P_BSP_MCPWM_OBJ_T p_mcpwm)
//{
//    for (uint16_t cir = 0; cir < p_mcpwm->ch_num; cir++)
//    {
//        GPIO_Config((GPIO_TypeDef *)p_mcpwm->p_mcpwm_ch[cir].p_gpio_port, p_mcpwm->p_mcpwm_ch[cir].p_gpio_pin, GPIO_Mode_OUT, GPIO_AF_MCPWM);
//        if (p_mcpwm->p_mcpwm_ch[cir].n_gpio_port != NULL)
//        {
//            GPIO_Config((GPIO_TypeDef *)p_mcpwm->p_mcpwm_ch[cir].n_gpio_port, p_mcpwm->p_mcpwm_ch[cir].n_gpio_pin, GPIO_Mode_OUT, GPIO_AF_MCPWM);
//        }
//    }
//}

#define MCPWM_CLK_DIV_NUM 4
static const uint8_t mcpwm_clk_div[][MCPWM_CLK_DIV_NUM] = {{0, 1}, {1, 2}, {2, 4}, {3, 8}}; // 1分频对应值寄存器值0

static void bsp_mcpwm_get_clk(P_BSP_MCPWM_CLOCK p_clk)
{
    uint32_t i;
    for (i = 0; i < MCPWM_CLK_DIV_NUM; i++)
    {
        p_clk->max_cnt_012 = SystemCoreClock / mcpwm_clk_div[i][1] / p_clk->freq_012;
        p_clk->max_cnt_3 = SystemCoreClock / mcpwm_clk_div[i][1] / p_clk->freq_3;

        if (p_clk->max_cnt_012 <= 0xFFFFU && p_clk->max_cnt_3 <= 0xFFFFU)
        {
            break;
        }
    }
    p_clk->clkdiv = mcpwm_clk_div[i][0];
}

/**
 * @brief 定时器配置
 *
 * @param [in] p_mcpwm       MCPWM参数指针
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
//static void bsp_mcpwm_timer_config(P_BSP_MCPWM_OBJ_T p_mcpwm)
//{
//    if (p_mcpwm->aligned_mode_012 == MCPWM_CENTRAL_PWM_MODE)
//    {
//        p_mcpwm->clk.freq_012 *= 2;
//    }
//    if (p_mcpwm->aligned_mode_3 == MCPWM_CENTRAL_PWM_MODE)
//    {
//        p_mcpwm->clk.freq_3 *= 2;
//    }
//    bsp_mcpwm_get_clk(&(p_mcpwm->clk));
//    SYS_SoftResetMODULE(p_mcpwm->sys_module);

//    MCPWM_InitTypeDef MCPWM_InitStructure;
//    MCPWM_StructInit(&MCPWM_InitStructure);

//    MCPWM_InitStructure.TimeBase0_PERIOD = p_mcpwm->clk.max_cnt_012; /* 时期0周期设置 MCPWMx->TH0*/
//    MCPWM_InitStructure.TimeBase1_PERIOD = p_mcpwm->clk.max_cnt_3;   /* 时期1周期设置 MCPWMx->TH1*/

//    // MCPWMx->TCLK
//    MCPWM_InitStructure.CLK_DIV = p_mcpwm->clk.clkdiv;                  /* MCPWM 分频系数 */
//    MCPWM_InitStructure.MCLK_EN = ENABLE;                               /* MCPWM 时钟使能开关 */
//    MCPWM_InitStructure.TMR2_TimeBase_Sel = p_mcpwm->tmr2_timebase_sel; /* TMR2 比较门限寄存器 时基选择 0:时基0 | 1:时基1 */
//    MCPWM_InitStructure.TMR3_TimeBase_Sel = p_mcpwm->tmr3_timebase_sel; /* TMR3 比较门限寄存器 时基选择 0:时基0 | 1:时基1 */

//    MCPWM_InitStructure.MCPWM_Cnt0_EN = ENABLE;    /* MCPWM 时基0主计数器使能开关 */
//    MCPWM_InitStructure.TimeBase0_Trig_Enable = 0; /* 时基0 外部触发使能 */
//    MCPWM_InitStructure.TimeBase0Init_CNT = 0;     /* 时基0 计数器初始值 */
//    MCPWM_InitStructure.TimeBase_TrigEvt0 = 0;     /* 时基0 外部触发事件选择 */

//    MCPWM_InitStructure.MCPWM_Cnt1_EN = ENABLE;    /* MCPWM 时基1主计数器使能开关 */
//    MCPWM_InitStructure.TimeBase1_Trig_Enable = 0; /* 时基1 外部触发使能*/
//    MCPWM_InitStructure.TimeBase1Init_CNT = 0;     /* 时基1 计数器初始值 */
//    MCPWM_InitStructure.TimeBase_TrigEvt1 = 0;     /* 时基1 外部触发事件选择 */

//    //    uint8_t flag = 0;
//    for (uint16_t i = 0; i < p_mcpwm->ch_num; i++)
//    {
//        if (p_mcpwm->p_mcpwm_ch[i].mcpwm_ch == BSP_MCPWM_CH_0)
//        {
//            // MCPWMx->IO01
//            /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
//            MCPWM_InitStructure.MCPWM_WorkModeCH0 = p_mcpwm->aligned_mode_012;
//            /* CH0N输出极性取反，0:正常输出；1:取反输出 */
//            MCPWM_InitStructure.CH0N_Polarity_INV = p_mcpwm->p_mcpwm_ch[i].n_polarity_inv;
//            /* CH0P输出极性取反，0:正常输出；1:取反输出 */
//            MCPWM_InitStructure.CH0P_Polarity_INV = p_mcpwm->p_mcpwm_ch[i].p_polarity_inv;
//            /* 交换CH0N, CH0P信号输出使能开关 */
//            MCPWM_InitStructure.Switch_CH0N_CH0P = p_mcpwm->p_mcpwm_ch[i].ch_pn_change_sw;
//            /* CH0N死区时间设置　//MCPWMx->DTH00*/
//            MCPWM_InitStructure.DeadTimeCH0N = p_mcpwm->p_mcpwm_ch[i].n_deadtime;
//            /* CH0P死区时间设置　MCPWMx->DTH01*/
//            MCPWM_InitStructure.DeadTimeCH0P = p_mcpwm->p_mcpwm_ch[i].p_deadtime;

//            // MCPWMx->FAIL012
//            MCPWM_InitStructure.CH0P_default_output = p_mcpwm->p_mcpwm_ch[i].p_default_output; /* CH0P MOE为0时或发生FAIL事件时，默认电平输出 */
//            MCPWM_InitStructure.CH0N_default_output = p_mcpwm->p_mcpwm_ch[i].n_default_output; /* CH0N MOE为0时或发生FAIL事件时，默认电平输出 */

//            p_mcpwm->mcpwm_periph->TH00 = -(p_mcpwm->clk.max_cnt_012 * p_mcpwm->p_mcpwm_ch[i].def_duty_cycle / BSP_PWM_MAX_DUTY_CYCLE);
//            p_mcpwm->mcpwm_periph->TH01 = p_mcpwm->clk.max_cnt_012 * p_mcpwm->p_mcpwm_ch[i].def_duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
//        }
//        else if (p_mcpwm->p_mcpwm_ch[i].mcpwm_ch == BSP_MCPWM_CH_1)
//        {
//            // MCPWMx->IO01
//            /* MCPWM CH1工作模式：边沿对齐/中心对齐 */
//            MCPWM_InitStructure.MCPWM_WorkModeCH1 = p_mcpwm->aligned_mode_012;
//            /* CH1N输出极性取反，0:正常输出；1:取反输出 */
//            MCPWM_InitStructure.CH1N_Polarity_INV = p_mcpwm->p_mcpwm_ch[i].n_polarity_inv;
//            /* CH1P输出极性取反，0:正常输出；1:取反输出 */
//            MCPWM_InitStructure.CH1P_Polarity_INV = p_mcpwm->p_mcpwm_ch[i].p_polarity_inv;
//            /* 交换CH1N, CH1P信号输出使能开关 */
//            MCPWM_InitStructure.Switch_CH1N_CH1P = p_mcpwm->p_mcpwm_ch[i].ch_pn_change_sw;
//            /* CH1N死区时间设置　MCPWMx->DTH10*/
//            MCPWM_InitStructure.DeadTimeCH1N = p_mcpwm->p_mcpwm_ch[i].n_deadtime;
//            /* CH1P死区时间设置　MCPWMx->DTH11*/
//            MCPWM_InitStructure.DeadTimeCH1P = p_mcpwm->p_mcpwm_ch[i].p_deadtime;

//            MCPWM_InitStructure.CH1P_default_output = p_mcpwm->p_mcpwm_ch[i].p_default_output; /* CH1P MOE为0时或发生FAIL事件时，默认电平输出 */
//            MCPWM_InitStructure.CH1N_default_output = p_mcpwm->p_mcpwm_ch[i].n_default_output; /* CH1N MOE为0时或发生FAIL事件时，默认电平输出 */

//            p_mcpwm->mcpwm_periph->TH10 = -p_mcpwm->clk.max_cnt_012 * p_mcpwm->p_mcpwm_ch[i].def_duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
//            p_mcpwm->mcpwm_periph->TH11 = p_mcpwm->clk.max_cnt_012 * p_mcpwm->p_mcpwm_ch[i].def_duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
//        }
//        else if (p_mcpwm->p_mcpwm_ch[i].mcpwm_ch == BSP_MCPWM_CH_2)
//        {
//            // MCPWMx->IO23
//            /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
//            MCPWM_InitStructure.MCPWM_WorkModeCH2 = p_mcpwm->aligned_mode_012;
//            /* CH2N输出极性取反，0:正常输出；1:取反输出 */
//            MCPWM_InitStructure.CH2N_Polarity_INV = p_mcpwm->p_mcpwm_ch[i].n_polarity_inv;
//            /* CH2P输出极性取反，0:正常输出；1:取反输出 */
//            MCPWM_InitStructure.CH2P_Polarity_INV = p_mcpwm->p_mcpwm_ch[i].p_polarity_inv;
//            /* 交换CH2N, CH2P信号输出使能开关 */
//            MCPWM_InitStructure.Switch_CH2N_CH2P = p_mcpwm->p_mcpwm_ch[i].ch_pn_change_sw;
//            /* CH2N死区时间设置　MCPWMx->DTH20*/
//            MCPWM_InitStructure.DeadTimeCH2N = p_mcpwm->p_mcpwm_ch[i].n_deadtime;
//            /* CH2P死区时间设置　MCPWMx->DTH21*/
//            MCPWM_InitStructure.DeadTimeCH2P = p_mcpwm->p_mcpwm_ch[i].p_deadtime;

//            MCPWM_InitStructure.CH2P_default_output = p_mcpwm->p_mcpwm_ch[i].p_default_output; /* CH2P MOE为0时或发生FAIL事件时，默认电平输出 */
//            MCPWM_InitStructure.CH2N_default_output = p_mcpwm->p_mcpwm_ch[i].n_default_output; /* CH2N MOE为0时或发生FAIL事件时，默认电平输出 */

//            p_mcpwm->mcpwm_periph->TH20 = -p_mcpwm->clk.max_cnt_012 * p_mcpwm->p_mcpwm_ch[i].def_duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
//            p_mcpwm->mcpwm_periph->TH21 = p_mcpwm->clk.max_cnt_012 * p_mcpwm->p_mcpwm_ch[i].def_duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
//        }
//        else if (p_mcpwm->p_mcpwm_ch[i].mcpwm_ch == BSP_MCPWM_CH_3)
//        {
//            // MCPWMx->IO23
//            /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
//            MCPWM_InitStructure.MCPWM_WorkModeCH3 = p_mcpwm->aligned_mode_3;
//            /* CH3N输出极性取反，0:正常输出；1:取反输出 */
//            MCPWM_InitStructure.CH3N_Polarity_INV = p_mcpwm->p_mcpwm_ch[i].n_polarity_inv;
//            /* CH3P输出极性取反，0:正常输出；1:取反输出 */
//            MCPWM_InitStructure.CH3P_Polarity_INV = p_mcpwm->p_mcpwm_ch[i].p_polarity_inv;
//            /* 交换CH3N, CH3P信号输出使能开关 */
//            MCPWM_InitStructure.Switch_CH3N_CH3P = p_mcpwm->p_mcpwm_ch[i].ch_pn_change_sw;
//            /* CH3N死区时间设置　MCPWMx->DTH30*/
//            MCPWM_InitStructure.DeadTimeCH3N = p_mcpwm->p_mcpwm_ch[i].n_deadtime;
//            /* CH3P死区时间设置　MCPWMx->DTH31*/
//            MCPWM_InitStructure.DeadTimeCH3P = p_mcpwm->p_mcpwm_ch[i].p_deadtime;
//            // p_mcpwm->PP //MCPWM推挽模式TODO:

//            // MCPWMx->FAIL3
//            MCPWM_InitStructure.CH3P_default_output = p_mcpwm->p_mcpwm_ch[i].p_default_output; /* CH3P MOE为0时或发生FAIL事件时，默认电平输出 */
//            MCPWM_InitStructure.CH3N_default_output = p_mcpwm->p_mcpwm_ch[i].n_default_output; /* CH3N MOE为0时或发生FAIL事件时，默认电平输出 */

//            p_mcpwm->mcpwm_periph->TH30 = -p_mcpwm->clk.max_cnt_3 * p_mcpwm->p_mcpwm_ch[i].def_duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
//            p_mcpwm->mcpwm_periph->TH31 = p_mcpwm->clk.max_cnt_3 * p_mcpwm->p_mcpwm_ch[i].def_duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
//        }
//    }

//    MCPWM_InitStructure.TriggerPoint0 = p_mcpwm->clk.max_cnt_012 * p_mcpwm->tmr0 / BSP_PWM_MAX_DUTY_CYCLE; /* PWM触发ADC事件0，时间点设置 MCPWMx->TMR0*/
//    MCPWM_InitStructure.TriggerPoint1 = p_mcpwm->clk.max_cnt_012 * p_mcpwm->tmr1 / BSP_PWM_MAX_DUTY_CYCLE; /* PWM触发ADC事件1，时间点设置 MCPWMx->TMR1*/
//    if (p_mcpwm->tmr2_timebase_sel == TMR_TimeBase_Sel_0)
//    {
//        MCPWM_InitStructure.TriggerPoint2 = p_mcpwm->clk.max_cnt_012 * p_mcpwm->tmr2 / BSP_PWM_MAX_DUTY_CYCLE; /* PWM触发ADC事件2，时间点设置 MCPWMx->TMR2*/
//    }
//    else
//    {
//        MCPWM_InitStructure.TriggerPoint2 = p_mcpwm->clk.max_cnt_3 * p_mcpwm->tmr2 / BSP_PWM_MAX_DUTY_CYCLE; /* PWM触发ADC事件2，时间点设置 MCPWMx->TMR2*/
//    }

//    if (p_mcpwm->tmr3_timebase_sel == TMR_TimeBase_Sel_0)
//    {
//        MCPWM_InitStructure.TriggerPoint3 = p_mcpwm->clk.max_cnt_012 * p_mcpwm->tmr3 / BSP_PWM_MAX_DUTY_CYCLE; /* PWM触发ADC事件3，时间点设置 MCPWMx->TMR3*/
//    }
//    else
//    {
//        MCPWM_InitStructure.TriggerPoint3 = p_mcpwm->clk.max_cnt_3 * p_mcpwm->tmr3 / BSP_PWM_MAX_DUTY_CYCLE; /* PWM触发ADC事件3，时间点设置 MCPWMx->TMR3*/
//    }

//    // MCPWMx->SDCFG
//    MCPWM_InitStructure.MCPWM_UpdateInterval = 1;         /* MCPWM T0/T1事件更新间隔 */
//    MCPWM_InitStructure.MCPWM_Base0T0_UpdateEN = ENABLE;  /* MCPWM 时基0 T0事件更新使能 */
//    MCPWM_InitStructure.MCPWM_Base0T1_UpdateEN = DISABLE; /* MCPWM 时基0 T1事件更新使能 */
//    MCPWM_InitStructure.MCPWM_Base1T0_UpdateEN = ENABLE;  /* MCPWM 时基1 T0事件更新使能 */
//    MCPWM_InitStructure.MCPWM_Base1T1_UpdateEN = DISABLE; /* MCPWM 时基1 T1事件更新使能 */

//    MCPWM_InitStructure.DebugMode_PWM_out = DISABLE; /* TODO:Debug时，MCU进入Halt, MCPWM信号是否正常输出 MCPWMx->FAIL3  MCPWMx->FAIL012*/
//    MCPWM_InitStructure.GPIO_BKIN_Filter = 1;        /* GPIO输入滤波时钟设置1-16 MCPWMx->FLT*/
//    MCPWM_InitStructure.CMP_BKIN_Filter = 1;         /* 比较器CMP输入滤波时钟设置1-16 MCPWMx->FLT*/

//    for (uint16_t i = 0; i < p_mcpwm->mcpwm_fail->fail_gpio_num; i++)
//    {
//        GPIO_Config(p_mcpwm->mcpwm_fail->p_mcpwm_fail_gpio->bkin_gpio_port, p_mcpwm->mcpwm_fail->p_mcpwm_fail_gpio->bkin_gpio_pin, GPIO_Mode_IN, GPIO_AF_MCPWM);
//    }
//    MCPWM_InitStructure.MCPWM_Auto_ERR_EN = p_mcpwm->mcpwm_fail->mcpwm_auto_err_en; /* MCPWM 更新事件是否自动打开MOE, 使能开关 */
//    // MCPWMx->FAIL012
//    MCPWM_InitStructure.FAIL0_INPUT_EN = p_mcpwm->mcpwm_fail->fail0_input_en; /* FAIL0 输入功能使能 */
//    MCPWM_InitStructure.FAIL0_Signal_Sel = MCPWM_FAIL_SEL_IO;                 /* FAIL0 信号选择，比较器0或GPIO */
//    MCPWM_InitStructure.FAIL0_Polarity = p_mcpwm->mcpwm_fail->fail0_polarity; /* FAIL0 信号极性设置，高有效或低有效 */
//    MCPWM_InitStructure.FAIL1_INPUT_EN = p_mcpwm->mcpwm_fail->fail1_input_en; /* FAIL1 输入功能使能 */
//    MCPWM_InitStructure.FAIL1_Signal_Sel = MCPWM_FAIL_SEL_IO;                 /* FAIL1 信号选择，比较器0或GPIO */
//    MCPWM_InitStructure.FAIL1_Polarity = p_mcpwm->mcpwm_fail->fail1_polarity;
//    ;                                                                         /* FAIL1 信号极性设置，高有效或低有效 */
//    MCPWM_InitStructure.FAIL2_INPUT_EN = p_mcpwm->mcpwm_fail->fail2_input_en; /* FAIL2 输入功能使能 */
//    MCPWM_InitStructure.FAIL2_Signal_Sel = MCPWM_FAIL_SEL_IO;                 /* FAIL2 信号选择，比较器0或GPIO */
//    MCPWM_InitStructure.FAIL2_Polarity = p_mcpwm->mcpwm_fail->fail2_polarity;
//    ; /* FAIL2 信号极性设置，高有效或低有效 */

//    // MCPWMx->FAIL3
//    MCPWM_InitStructure.FAIL3_INPUT_EN = p_mcpwm->mcpwm_fail->fail3_input_en; /* FAIL3 输入功能使能 */
//    MCPWM_InitStructure.FAIL3_Signal_Sel = MCPWM_FAIL_SEL_IO;                 /* FAIL3 信号选择，比较器0或GPIO */
//    MCPWM_InitStructure.FAIL3_Polarity = p_mcpwm->mcpwm_fail->fail3_polarity;
//    ; /* FAIL3 信号极性设置，高有效或低有效 */

//    // MCPWMx->EIE
//    MCPWM_InitStructure.FAIL0_INT_EN = DISABLE; /* FAIL0事件中断使能 */
//    MCPWM_InitStructure.FAIL1_INT_EN = DISABLE; /* FAIL1事件中断使能 */
//    MCPWM_InitStructure.FAIL2_INT_EN = DISABLE; /* FAIL2事件中断使能 */
//    MCPWM_InitStructure.FAIL3_INT_EN = DISABLE; /* FAIL3事件中断使能 */

//    // MCPWMx->IE0  没有使用该库，后面直接操作IE寄存器
//    MCPWM_InitStructure.T0_Update_INT_EN = 0;  /* T0更新事件中斷使能 */
//    MCPWM_InitStructure.T1_Update_INT_EN = 0;  /* T1更新事件中斷使能 */
//    MCPWM_InitStructure.TMR0_Match_INT_EN = 0; /* TMR0计数事件匹配事件中断使能 */
//    MCPWM_InitStructure.TMR1_Match_INT_EN = 0; /* TMR1计数事件匹配事件中断使能 */

//    // MCPWMx->AUEN
//    MCPWM_InitStructure.AUEN = MCPWM_AUEN_TH00 | MCPWM_AUEN_TH01 | MCPWM_AUEN_TH10 | MCPWM_AUEN_TH11 | MCPWM_AUEN_TH20 | MCPWM_AUEN_TH21 | MCPWM_AUEN_TH30 |
//                               MCPWM_AUEN_TH31 | MCPWM_AUEN_TH0 | MCPWM_AUEN_TH1 | MCPWM_AUEN_TMR0 | MCPWM_AUEN_TMR1 | MCPWM_AUEN_TMR2 |
//                               MCPWM_AUEN_TMR3 ; /* 自动更新使能寄存器 */

//    MCPWM_Init(p_mcpwm->mcpwm_periph, &MCPWM_InitStructure);

//    // 补充库没有写TR1_AEC寄存器值操作
//    p_mcpwm->mcpwm_periph->PRT = 0x0000DEAD; /*enter password to unlock write protection */
//    p_mcpwm->mcpwm_periph->SDCFG |= (u32)((MCPWM_InitStructure.MCPWM_Auto_ERR_EN << 14) | (MCPWM_InitStructure.MCPWM_UpdateInterval << 8));
//    p_mcpwm->mcpwm_periph->PRT = 0x0000CAFE; // write any value other than 0xDEAD to enable write protection

//    bsp_mcpwm_disable(p_mcpwm);
//    // MCPWM_OutputMode(p_mcpwm->mcpwm_periph, MCPWM_OUT_CHN_012, MCPWM_OUT_MODE_PWM);
//    // MCPWM_OutputMode(p_mcpwm->mcpwm_periph, MCPWM_OUT_CHN_3, MCPWM_OUT_MODE_PWM);
//}

/**
 * @brief PWM初???化
 * @param [in] p_mcpwm         PWM参数指针
 * @details
 */
void bsp_mcpwm_init(P_BSP_MCPWM_OBJ_T p_mcpwm)
{
    #if PROGRAM_TYPE == PROGRAM_APL
    McpwmCfg_Init();
    #endif
//    ///< GPIO 配置
//    bsp_mcpwm_gpio_config(p_mcpwm);
//    ///< 定时器配置
//    bsp_mcpwm_timer_config(p_mcpwm);
}

/**
 * @brief PWM反初始化
 * @param [in] p_mcpwm         PWM参数指针
 * @details
 */
void bsp_mcpwm_deinit(P_BSP_MCPWM_OBJ_T p_mcpwm)
{
    SYS_SoftResetMODULE(p_mcpwm->sys_module);
}

/**
 * @brief 定时器失能
 * @param [in] p_mcpwm         PWM参数指针
 * @details
 */
void bsp_mcpwm_disable(P_BSP_MCPWM_OBJ_T p_mcpwm) // TODO:关时钟不会关闭计数器
{
    // MCPWM_Disable(p_mcpwm->mcpwm_periph);
    SYS_ModuleClockCmd(p_mcpwm->sys_module, DISABLE);
    p_mcpwm->mcpwm_periph->PRT = 0x0000DEAD;
    p_mcpwm->mcpwm_periph->TCLK &= ~MCPWM_CNT_CTRL_REG; //关闭计数器
    p_mcpwm->mcpwm_periph->PRT = 0x0000CAFE;
}

/**
 * @brief 定时器使能
 * @param [in] p_mcpwm         PWM参数指针
 * @details 使能时打开通道输出使能
 */
void bsp_mcpwm_enable(P_BSP_MCPWM_OBJ_T p_mcpwm) //TODO:关闭后重新开启有概率出现一个半波
{
    // MCPWM_Enable(p_mcpwm->mcpwm_periph );
    p_mcpwm->mcpwm_periph->CNT0 = (u32)0;
    p_mcpwm->mcpwm_periph->CNT1 = (u32)0;
		p_mcpwm->mcpwm_periph->UPDATE = 0xffff; 
    SYS_ModuleClockCmd(p_mcpwm->sys_module, ENABLE);
    p_mcpwm->mcpwm_periph->PRT = 0x0000DEAD;
    p_mcpwm->mcpwm_periph->TCLK |= MCPWM_CNT_CTRL_REG;
    p_mcpwm->mcpwm_periph->PRT = 0x0000CAFE;
}

/**
 * @brief 设置通道占空比
 * @param [in] p_mcpwm         PWM参数指针
 * @param [in] ch            通道
 * @param [in] duty_cycle    占空比
 * @details
 */
void bsp_mcpwm_set_duty_cycle(P_BSP_MCPWM_OBJ_T p_mcpwm, uint16_t ch, uint16_t duty_cycle)
{
    switch (ch)
    {
        case BSP_MCPWM_CH_0:
        {
            // uint16_t ch_value = p_mcpwm->clk.max_cnt_012 * duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
            p_mcpwm->mcpwm_periph->TH00 = -p_mcpwm->clk.max_cnt_012 * duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
            p_mcpwm->mcpwm_periph->TH01 = p_mcpwm->clk.max_cnt_012 * duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
            break;
        }
        case BSP_MCPWM_CH_1:
        {
            p_mcpwm->mcpwm_periph->TH10 = -p_mcpwm->clk.max_cnt_012 * duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
            p_mcpwm->mcpwm_periph->TH11 = p_mcpwm->clk.max_cnt_012 * duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
            break;
        }
        case BSP_MCPWM_CH_2:
        {
            p_mcpwm->mcpwm_periph->TH20 = -p_mcpwm->clk.max_cnt_012 * duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
            p_mcpwm->mcpwm_periph->TH21 = p_mcpwm->clk.max_cnt_012 * duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
            break;
        }
        case BSP_MCPWM_CH_3:
        {
            p_mcpwm->mcpwm_periph->TH30 = -p_mcpwm->clk.max_cnt_3 * duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
            p_mcpwm->mcpwm_periph->TH31 = p_mcpwm->clk.max_cnt_3 * duty_cycle / BSP_PWM_MAX_DUTY_CYCLE;
            break;
        }
    }
}

void bsp_mcpwm_set_freq(P_BSP_MCPWM_OBJ_T p_mcpwm, uint16_t ch, uint32_t freq)
{
    if (ch == MCPWM_OUT_CHN_012)
    {
        p_mcpwm->clk.freq_012 = freq;
        if (p_mcpwm->aligned_mode_012 == MCPWM_CENTRAL_PWM_MODE)
        {
            p_mcpwm->clk.freq_012 *= 2;
        }
        bsp_mcpwm_get_clk(&(p_mcpwm->clk));
        p_mcpwm->mcpwm_periph->PRT = 0x0000DEAD; /*enter password to unlock write protection */
        p_mcpwm->mcpwm_periph->TCLK |= (u32)p_mcpwm->clk.clkdiv;
        p_mcpwm->mcpwm_periph->TH0 = p_mcpwm->clk.max_cnt_012; /* 时期0周期设置 MCPWMx->TH0*/
        p_mcpwm->mcpwm_periph->PRT = 0x0000CAFE;               // write any value other than 0xDEAD to enable write protection
    }
    else
    {
        p_mcpwm->clk.freq_3 = freq;
        if (p_mcpwm->aligned_mode_3 == MCPWM_CENTRAL_PWM_MODE)
        {
            p_mcpwm->clk.freq_3 *= 2;
        }
        bsp_mcpwm_get_clk(&(p_mcpwm->clk));
        p_mcpwm->mcpwm_periph->PRT = 0x0000DEAD; /*enter password to unlock write protection */
        p_mcpwm->mcpwm_periph->TCLK |= (u32)p_mcpwm->clk.clkdiv;
        p_mcpwm->mcpwm_periph->TH1 = p_mcpwm->clk.max_cnt_3; /* 时期1周期设置 MCPWMx->TH1*/
        p_mcpwm->mcpwm_periph->PRT = 0x0000CAFE;             // write any value other than 0xDEAD to enable write protection
    }
}

/**
 * @brief 设置死区时间
 * @param [in] p_mcpwm         PWM参数指针
 * @param [in] pdeadtime     死区时间
 * @details
 */
void bsp_mcpwm_set_p_deadtime(P_BSP_MCPWM_OBJ_T p_mcpwm, uint16_t ch, uint16_t deadtime)
{
    p_mcpwm->mcpwm_periph->PRT = 0x0000DEAD; /*enter password to unlock write protection */
    switch (ch)
    {
        case BSP_MCPWM_CH_0:
        {
            p_mcpwm->mcpwm_periph->DTH00 = deadtime; // 写保护
            break;
        }
        case BSP_MCPWM_CH_1:
        {
            p_mcpwm->mcpwm_periph->DTH10 = deadtime;
            break;
        }
        case BSP_MCPWM_CH_2:
        {
            p_mcpwm->mcpwm_periph->DTH20 = deadtime;
            break;
        }
        case BSP_MCPWM_CH_3:
        {
            p_mcpwm->mcpwm_periph->DTH30 = deadtime;
            break;
        }
    }
    p_mcpwm->mcpwm_periph->PRT = 0x0000CAFE; // write any value other than 0xDEAD to enable write protection
}

void bsp_mcpwm_set_n_deadtime(P_BSP_MCPWM_OBJ_T p_mcpwm, uint16_t ch, uint16_t deadtime)
{
    p_mcpwm->mcpwm_periph->PRT = 0x0000DEAD; /*enter password to unlock write protection */
    switch (ch)
    {
        case BSP_MCPWM_CH_0:
        {
            p_mcpwm->mcpwm_periph->DTH01 = deadtime; // 写保护
            break;
        }
        case BSP_MCPWM_CH_1:
        {
            p_mcpwm->mcpwm_periph->DTH11 = deadtime;
            break;
        }
        case BSP_MCPWM_CH_2:
        {
            p_mcpwm->mcpwm_periph->DTH21 = deadtime;
            break;
        }
        case BSP_MCPWM_CH_3:
        {
            p_mcpwm->mcpwm_periph->DTH31 = deadtime;
            break;
        }
    }
    p_mcpwm->mcpwm_periph->PRT = 0x0000CAFE; // write any value other than 0xDEAD to enable write protection
}

static const uint32_t mcpwm_ch_ctrl_tap[BSP_MCPWM_CH_CTRL_MAX] = {0x00000048, 0x0000005C, 0x00000044, 0x00000017,
                                                                  0x0000002B, 0x0000006C, 0x0000004C, 0x00000040};
static const uint32_t mcpwm_ch_ctrl_reg_offset_tap[4] = {0xA0, 0xA0, 0xA4, 0xA4}; ///< 0xA0为IO01偏移地址，0xA4为IO23偏移地址
/**
 * @brief 设置通道状态
 * @param [in] p_mcpwm         PWM参数指针
 * @param [in] ch            通道
 * @param [in] cmd           状态命令
 * @details
 */
// uint32_t mcpwm_reg[2] = {0};
void bsp_mcpwm_set_ch(P_BSP_MCPWM_OBJ_T p_mcpwm, uint16_t ch, uint8_t cmd)
{
    p_mcpwm->mcpwm_periph->PRT = 0x0000DEAD;
    uint32_t reg_cmd = 0;
    reg_cmd = (REG32((uint32_t)p_mcpwm->mcpwm_periph + mcpwm_ch_ctrl_reg_offset_tap[ch]) & (~(CH_IO_CTRL_REG << (8 * (ch % 2))))) | (uint32_t)mcpwm_ch_ctrl_tap[cmd] << (8 * (ch % 2));
    REG32((uint32_t)p_mcpwm->mcpwm_periph + mcpwm_ch_ctrl_reg_offset_tap[ch]) = reg_cmd;
    p_mcpwm->mcpwm_periph->PRT = 0x0000CAFE;
}

/**
 * @brief 设置通道的PWM模式
 * @param [in] p_mcpwm         PWM参数指针
 * @param [in] ch            通道
 * @param [in] mode          通道模式
 * @details
 */
void bsp_mcpwm_set_ch_mode(P_BSP_MCPWM_OBJ_T p_mcpwm, uint16_t ch, uint8_t mode)
{
    MCPWM_OutputMode(p_mcpwm->mcpwm_periph, ch, mode);
}

static const IRQn_Type mcpwm0_irq_select_tap
    [BSP_IE_MCPWM_IRQ_MAX] = {MCPWM00_IRQn, MCPWM00_IRQn, MCPWM00_IRQn, MCPWM00_IRQn, MCPWM00_IRQn, MCPWM00_IRQn, MCPWM00_IRQn,
                              MCPWM00_IRQn, MCPWM00_IRQn, MCPWM00_IRQn, MCPWM00_IRQn, MCPWM01_IRQn, MCPWM01_IRQn, MCPWM01_IRQn,
                              MCPWM01_IRQn, MCPWM01_IRQn, MCPWM00_IRQn, MCPWM00_IRQn}; // MCPWM0中断号IRQn选择表，默认TMR选择时基0，TMR_TimeBase_Sel_0
// static const uint8_t mcpwm_irq_offset_tap[2] = {0, 2}; // MCPWM1中断号相对MCPWM0中断号偏移为2,不用该表，使用((((uint32_t)p_mcpwm->mcpwm_periph) % 0x40013400)
// / 0x200)计算得出

static void bsp_mcpwm_get_nvic_irq(P_BSP_MCPWM_OBJ_T p_mcpwm, uint32_t type)
{
    p_mcpwm->nvic_irq = (IRQn_Type)(mcpwm0_irq_select_tap[type] + ((((uint32_t)p_mcpwm->mcpwm_periph) % 0x40013400) / 0x200));
    if (((type == BSP_IE01_MCPWM_TMR2_IRQ) && (p_mcpwm->tmr2_timebase_sel == TMR_TimeBase_Sel_1)) ||
        ((type == BSP_IE01_MCPWM_TMR3_IRQ) && (p_mcpwm->tmr3_timebase_sel == TMR_TimeBase_Sel_1)))
    {
        if (p_mcpwm->mcpwm_periph == MCPWM0)
        {
            p_mcpwm->nvic_irq = MCPWM01_IRQn;
        }
        else
        {
            p_mcpwm->nvic_irq = MCPWM11_IRQn;
        }
    }
}

///< 中断使能寄存器表
static const uint32_t mcpwm_irq_reg_enable_tap[BSP_IE_MCPWM_IRQ_MAX] = {BIT0,  BIT1,  BIT2, BIT3, BIT4, BIT5, BIT6,  BIT7,  BIT10,
                                                                        BIT11, BIT14, BIT0, BIT1, BIT8, BIT9, BIT14, BIT12, BIT13};

void bsp_mcpwm_reg_irq(P_BSP_MCPWM_OBJ_T p_mcpwm, uint32_t type, void (*irq_cb)(void))
{
    if ((type <= BSP_IE0_MCPWM_UP_IRQ) || (type == BSP_IE01_MCPWM_TMR2_IRQ && p_mcpwm->tmr2_timebase_sel == TMR_TimeBase_Sel_0) ||
        (type == BSP_IE01_MCPWM_TMR3_IRQ &&
         p_mcpwm->tmr3_timebase_sel ==
             TMR_TimeBase_Sel_0)) // IE0（时基0）和IE1（时基1）寄存器位是一样的，TMR2和TMR3受使用IE0还是IE1受TRM2和TMR3时基选择影响，TH30和TH31只在IE1中有效
    {
        p_mcpwm->mcpwm_periph->PRT = 0x0000DEAD;
        p_mcpwm->mcpwm_periph->IE0 |= mcpwm_irq_reg_enable_tap[type];
        p_mcpwm->mcpwm_periph->PRT = 0x0000CAFE;
        p_mcpwm->mcpwm_irq_x0_cb = irq_cb;
        p_mcpwm->mcpwm_x0_irq_flag = mcpwm_irq_reg_enable_tap[type];
    }
    else
    {
        p_mcpwm->mcpwm_periph->PRT = 0x0000DEAD;
        p_mcpwm->mcpwm_periph->IE1 |= mcpwm_irq_reg_enable_tap[type];
        p_mcpwm->mcpwm_periph->PRT = 0x0000CAFE;
        p_mcpwm->mcpwm_irq_x1_cb = irq_cb;
        p_mcpwm->mcpwm_x1_irq_flag = mcpwm_irq_reg_enable_tap[type];
    }
    bsp_mcpwm_get_nvic_irq(p_mcpwm, type);
    NVIC_EnableIRQ(p_mcpwm->nvic_irq);
    NVIC_SetPriority(p_mcpwm->nvic_irq, p_mcpwm->priority);
}

void bsp_mcpwm_x0_cb(P_BSP_MCPWM_OBJ_T p_mcpwm)
{
    if (MCPWM_GetCnt0IRQFlag(p_mcpwm->mcpwm_periph, p_mcpwm->mcpwm_x0_irq_flag) == 1)
    {
        if (p_mcpwm->mcpwm_irq_x0_cb != NULL)
        {
            p_mcpwm->mcpwm_irq_x0_cb();
        }
        MCPWM_ClearCnt0IRQFlag(p_mcpwm->mcpwm_periph, p_mcpwm->mcpwm_x0_irq_flag);
    }
}

void bsp_mcpwm_x1_cb(P_BSP_MCPWM_OBJ_T p_mcpwm)
{
    if (MCPWM_GetCnt1IRQFlag(p_mcpwm->mcpwm_periph, p_mcpwm->mcpwm_x1_irq_flag) == 1)
    {
        if (p_mcpwm->mcpwm_irq_x1_cb != NULL)
        {
            p_mcpwm->mcpwm_irq_x1_cb();
        }
        MCPWM_ClearCnt1IRQFlag(p_mcpwm->mcpwm_periph, p_mcpwm->mcpwm_x1_irq_flag);
    }
}
