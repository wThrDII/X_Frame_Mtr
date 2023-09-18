#define MCPWM_CFG_MODULE

#include "mcpwm_cfg.h"
#include "mtr_drv_cfg.h"
#include "x_frame.h"

void Mcpwm0Cfg_GpioInit(void)
{
    GPIO_Config(GPIO2, 9,  GPIO_Mode_OUT, GPIO_AF_MCPWM);
    GPIO_Config(GPIO2, 10, GPIO_Mode_OUT, GPIO_AF_MCPWM);
    GPIO_Config(GPIO2, 11, GPIO_Mode_OUT, GPIO_AF_MCPWM);
    GPIO_Config(GPIO2, 12, GPIO_Mode_OUT, GPIO_AF_MCPWM);
    GPIO_Config(GPIO2, 13, GPIO_Mode_OUT, GPIO_AF_MCPWM);
    GPIO_Config(GPIO2, 14, GPIO_Mode_OUT, GPIO_AF_MCPWM);
}

void Mcpwm1Cfg_GpioInit(void)
{
    GPIO_Config(GPIO2, 3, GPIO_Mode_OUT, GPIO_AF_MCPWM);
    GPIO_Config(GPIO2, 4, GPIO_Mode_OUT, GPIO_AF_MCPWM);
    GPIO_Config(GPIO2, 5, GPIO_Mode_OUT, GPIO_AF_MCPWM);
    GPIO_Config(GPIO2, 6, GPIO_Mode_OUT, GPIO_AF_MCPWM);
    GPIO_Config(GPIO2, 7, GPIO_Mode_OUT, GPIO_AF_MCPWM);
    GPIO_Config(GPIO2, 8, GPIO_Mode_OUT, GPIO_AF_MCPWM);
}

void Mctime_0_PwmEnable(void)
{
	MCPWM0->PRT = 0x0000DEAD;
	MCPWM0->FAIL012 |= MCPWM_MOE_ENABLE_MASK;
	MCPWM0->PRT = 0x0000CAFE;
}

void Mctime_1_PwmEnable(void)
{
	MCPWM1->PRT = 0x0000DEAD;
	MCPWM1->FAIL012 |= MCPWM_MOE_ENABLE_MASK;
	MCPWM1->PRT = 0x0000CAFE;
}

void Mcpwm0Cfg_BaseInit(void)
{
    MCPWM_InitTypeDef MCPWM_InitStructure;

    MCPWM_StructInit(&MCPWM_InitStructure);

    MCPWM_InitStructure.TimeBase0_PERIOD        = PWM_PERIOD ;    /* 时期0周期设置 ((192M/FREQ)-1)/2*/
    MCPWM_InitStructure.TimeBase1_PERIOD        = PWM_PERIOD ;   /* 时期1周期设置*/
    MCPWM_InitStructure.CLK_DIV                 = 0     ;   /* MCPWM 分频系数 */
    MCPWM_InitStructure.MCLK_EN                 = ENABLE;   /* MCPWM 时钟使能开关 */
    MCPWM_InitStructure.TMR2_TimeBase_Sel       = 0     ;   /* TMR2 比较门限寄存器 时基选择 0:时基0 | 1:时基1 */
    MCPWM_InitStructure.TMR3_TimeBase_Sel       = 0     ;   /* TMR3 比较门限寄存器 时基选择 0:时基0 | 1:时基1 */
    MCPWM_InitStructure.MCPWM_Cnt0_EN           = DISABLE;   /* MCPWM 时基0主计数器使能开关 */
    MCPWM_InitStructure.TimeBase0_Trig_Enable   = 1     ;   /* 时基0 外部触发使能 */
    MCPWM_InitStructure.TimeBase0Init_CNT       = 0     ;   /* 时基0 计数器初始值 */
    MCPWM_InitStructure.TimeBase_TrigEvt0       = (1<<8);   /* 时基0 外部触发事件选择 time0_cmp0 */
    MCPWM_InitStructure.MCPWM_Cnt1_EN           = DISABLE;   /* MCPWM 时基1主计数器使能开关 */
    MCPWM_InitStructure.TimeBase1_Trig_Enable   = 0     ;   /* 时基1 外部触发使能*/
    MCPWM_InitStructure.TimeBase1Init_CNT       = 0     ;   /* 时基1 计数器初始值 */
    MCPWM_InitStructure.TimeBase_TrigEvt1       = 0     ;   /* 时基1 外部触发事件选择 */
    MCPWM_InitStructure.MCPWM_WorkModeCH0       = MCPWM_CENTRAL_PWM_MODE     ;   /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
    MCPWM_InitStructure.MCPWM_WorkModeCH1       = MCPWM_CENTRAL_PWM_MODE     ;   /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
    MCPWM_InitStructure.MCPWM_WorkModeCH2       = MCPWM_CENTRAL_PWM_MODE     ;   /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
    MCPWM_InitStructure.MCPWM_WorkModeCH3       = MCPWM_CENTRAL_PWM_MODE     ;   /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
    MCPWM_InitStructure.TriggerPoint0           = 0     ;   /* PWM触发ADC事件0，时间点设置 */
    MCPWM_InitStructure.TriggerPoint1           = 0     ;   /* PWM触发ADC事件1，时间点设置 */
    MCPWM_InitStructure.TriggerPoint2           = -PWM_PERIOD;     ;   /* PWM触发ADC事件2，时间点设置 */
    MCPWM_InitStructure.TriggerPoint3           = -PWM_PERIOD+200      ;   /* PWM触发ADC事件3，时间点设置 */
    MCPWM_InitStructure.DeadTimeCH0N            = 100   ;   /* CH0N死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH0P            = 100   ;   /* CH0P死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH1N            = 100   ;   /* CH1N死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH1P            = 100   ;   /* CH1P死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH2N            = 100   ;   /* CH2N死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH2P            = 100   ;   /* CH2P死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH3N            = 100   ;   /* CH3N死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH3P            = 100   ;   /* CH3P死区时间设置　*/
    MCPWM_InitStructure.CH0N_Polarity_INV       = DISABLE     ;   /* CH0N输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH0P_Polarity_INV       = DISABLE     ;   /* CH0P输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH1N_Polarity_INV       = DISABLE     ;   /* CH1N输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH1P_Polarity_INV       = DISABLE     ;   /* CH1P输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH2N_Polarity_INV       = DISABLE     ;   /* CH2N输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH2P_Polarity_INV       = DISABLE     ;   /* CH2P输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH3N_Polarity_INV       = DISABLE     ;   /* CH3N输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH3P_Polarity_INV       = DISABLE     ;   /* CH3P输出极性取反，0:正常输出；1:取反输出 */
    //暂时保留NP对调的配置
    MCPWM_InitStructure.Switch_CH0N_CH0P        = ENABLE   ;   /* 交换CH0N, CH0P信号输出使能开关 */
    MCPWM_InitStructure.Switch_CH1N_CH1P        = ENABLE   ;   /* 交换CH1N, CH1P信号输出使能开关 */
    MCPWM_InitStructure.Switch_CH2N_CH2P        = ENABLE   ;   /* 交换CH2N, CH2P信号输出使能开关 */
    MCPWM_InitStructure.Switch_CH3N_CH3P        = ENABLE   ;   /* 交换CH3N, CH3P信号输出使能开关 */
    
    MCPWM_InitStructure.MCPWM_UpdateInterval    = 0         ;   /* MCPWM T0/T1事件更新间隔 */
    MCPWM_InitStructure.MCPWM_Base0T0_UpdateEN  = ENABLE    ;   /* MCPWM 时基0 T0事件更新使能 */
    MCPWM_InitStructure.MCPWM_Base0T1_UpdateEN  = DISABLE   ;   /* MCPWM 时基0 T1事件更新使能 */
    MCPWM_InitStructure.MCPWM_Base1T0_UpdateEN  = ENABLE    ;   /* MCPWM 时基1 T0事件更新使能 */
    MCPWM_InitStructure.MCPWM_Base1T1_UpdateEN  = DISABLE   ;   /* MCPWM 时基1 T1事件更新使能 */
    MCPWM_InitStructure.MCPWM_Auto_ERR_EN       = DISABLE   ;   /* MCPWM 更新事件是否自动打开MOE, 使能开关 */
    MCPWM_InitStructure.DebugMode_PWM_out       = DISABLE   ;   /* Debug时，MCU进入Halt, MCPWM信号是否正常输出 */
    MCPWM_InitStructure.GPIO_BKIN_Filter        = 1     ;   /* GPIO输入滤波时钟设置1-16 */
    MCPWM_InitStructure.CMP_BKIN_Filter         = 1     ;   /* 比较器CMP输入滤波时钟设置1-16 */
    
    MCPWM_InitStructure.FAIL0_INPUT_EN          = DISABLE   ;   /* FAIL0 输入功能使能 */
    MCPWM_InitStructure.FAIL0_INT_EN            = DISABLE   ;   /* FAIL0事件中断使能 */
    MCPWM_InitStructure.FAIL0_Signal_Sel        = 1     ;   /* FAIL0 信号选择，比较器0或GPIO */
    MCPWM_InitStructure.FAIL0_Polarity          = 0     ;   /* FAIL0 信号极性设置，高有效或低有效 */
    
    MCPWM_InitStructure.FAIL1_INPUT_EN          = DISABLE     ;   /* FAIL1 输入功能使能 */
    MCPWM_InitStructure.FAIL1_INT_EN            = DISABLE     ;   /* FAIL1事件中断使能 */
    MCPWM_InitStructure.FAIL1_Signal_Sel        = 0     ;   /* FAIL1 信号选择，比较器0或GPIO */
    MCPWM_InitStructure.FAIL1_Polarity          = 0     ;   /* FAIL1 信号极性设置，高有效或低有效 */
    
    MCPWM_InitStructure.FAIL2_INPUT_EN          = DISABLE     ;   /* FAIL2 输入功能使能 */
    MCPWM_InitStructure.FAIL2_INT_EN            = DISABLE     ;   /* FAIL2事件中断使能 */
    MCPWM_InitStructure.FAIL2_Signal_Sel        = 0     ;   /* FAIL2 信号选择，比较器0或GPIO */
    MCPWM_InitStructure.FAIL2_Polarity          = 0     ;   /* FAIL2 信号极性设置，高有效或低有效 */
    
    MCPWM_InitStructure.FAIL3_INPUT_EN          = DISABLE     ;   /* FAIL3 输入功能使能 */
    MCPWM_InitStructure.FAIL3_INT_EN            = DISABLE     ;   /* FAIL3事件中断使能 */
    MCPWM_InitStructure.FAIL3_Signal_Sel        = 0     ;   /* FAIL3 信号选择，比较器0或GPIO */
    MCPWM_InitStructure.FAIL3_Polarity          = 0     ;   /* FAIL3 信号极性设置，高有效或低有效 */
    
    MCPWM_InitStructure.CH0P_default_output     = 0     ;   /* CH0P MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH0N_default_output     = 0     ;   /* CH0N MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH1P_default_output     = 0     ;   /* CH1P MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH1N_default_output     = 0     ;   /* CH1N MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH2P_default_output     = 0     ;   /* CH2P MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH2N_default_output     = 0     ;   /* CH2N MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH3P_default_output     = 0     ;   /* CH3P MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH3N_default_output     = 0     ;   /* CH3N MOE为0时或发生FAIL事件时，默认电平输出 */
    
    MCPWM_InitStructure.T0_Update_INT_EN        = 1     ;   /* T0更新事件中斷使能 */
    MCPWM_InitStructure.T1_Update_INT_EN        = 0     ;   /* T1更新事件中斷使能 */
    MCPWM_InitStructure.TMR0_Match_INT_EN       = 0     ;   /* TMR0计数事件匹配事件中断使能 */
    MCPWM_InitStructure.TMR1_Match_INT_EN       = 0     ;   /* TMR1计数事件匹配事件中断使能 */
    MCPWM_InitStructure.AUEN                    =   MCPWM_AUEN_TH00 |
                                                    MCPWM_AUEN_TH01 | 
                                                    MCPWM_AUEN_TH10 | 
                                                    MCPWM_AUEN_TH11 |
                                                    MCPWM_AUEN_TH20 | 
                                                    MCPWM_AUEN_TH21 |   /* 自动更新使能寄存器 */
                                                    MCPWM_AUEN_TMR0 | 
                                                    MCPWM_AUEN_TMR1 |
                                                    MCPWM_AUEN_TMR2 |
                                                    MCPWM_AUEN_TMR3 ;   /* 自动更新使能寄存器 */
    MCPWM_Init(MCPWM0,&MCPWM_InitStructure);
    NVIC_SetPriority(MCPWM00_IRQn,1);
    NVIC_EnableIRQ (MCPWM00_IRQn);

    // Mctime_0_PwmUpdate(0.5,0.5,0.5);
    
    // MCPWM0_PRT = 0xDEAD;
    // MCPWM0->IE0 = 1<<13;	//TMR3中断
    // MCPWM0_PRT = 0x00;

     //   CORDIC_Enable();//开启三角运算时钟使能
       // Time0_Init(); //初始化timer0.
    Mctime_0_PwmEnable();//使能MOE主输出
}

void Mcpwm1Cfg_BaseInit(void)
{
    MCPWM_InitTypeDef MCPWM_InitStructure;

    MCPWM_StructInit(&MCPWM_InitStructure);

    MCPWM_InitStructure.TimeBase0_PERIOD        = PWM_PERIOD ;    /* 时期0周期设置 ((192M/FREQ)-1)/2*/
    MCPWM_InitStructure.TimeBase1_PERIOD        = PWM_PERIOD ;   /* 时期1周期设置*/
    MCPWM_InitStructure.CLK_DIV                 = 0     ;   /* MCPWM 分频系数 */
    MCPWM_InitStructure.MCLK_EN                 = ENABLE;   /* MCPWM 时钟使能开关 */
    MCPWM_InitStructure.TMR2_TimeBase_Sel       = 0     ;   /* TMR2 比较门限寄存器 时基选择 0:时基0 | 1:时基1 */
    MCPWM_InitStructure.TMR3_TimeBase_Sel       = 0     ;   /* TMR3 比较门限寄存器 时基选择 0:时基0 | 1:时基1 */
    MCPWM_InitStructure.MCPWM_Cnt0_EN           = DISABLE;   /* MCPWM 时基0主计数器使能开关 */
    MCPWM_InitStructure.TimeBase0_Trig_Enable   = 1     ;   /* 时基0 外部触发使能 */
    MCPWM_InitStructure.TimeBase0Init_CNT       = -PWM_PERIOD;   /* 时基0 计数器初始值 */
    MCPWM_InitStructure.TimeBase_TrigEvt0       = (1<<8);   /* 时基0 外部触发事件选择 time0_cmp0 */
    MCPWM_InitStructure.MCPWM_Cnt1_EN           = DISABLE;   /* MCPWM 时基1主计数器使能开关 */
    MCPWM_InitStructure.TimeBase1_Trig_Enable   = 0     ;   /* 时基1 外部触发使能*/
    MCPWM_InitStructure.TimeBase1Init_CNT       = 0     ;   /* 时基1 计数器初始值 */
    MCPWM_InitStructure.TimeBase_TrigEvt1       = 0     ;   /* 时基1 外部触发事件选择 */
    MCPWM_InitStructure.MCPWM_WorkModeCH0       = MCPWM_CENTRAL_PWM_MODE     ;   /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
    MCPWM_InitStructure.MCPWM_WorkModeCH1       = MCPWM_CENTRAL_PWM_MODE     ;   /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
    MCPWM_InitStructure.MCPWM_WorkModeCH2       = MCPWM_CENTRAL_PWM_MODE     ;   /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
    MCPWM_InitStructure.MCPWM_WorkModeCH3       = MCPWM_CENTRAL_PWM_MODE     ;   /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
    MCPWM_InitStructure.TriggerPoint0           = 0     ;   /* PWM触发ADC事件0，时间点设置 */
    MCPWM_InitStructure.TriggerPoint1           = 0     ;   /* PWM触发ADC事件1，时间点设置 */
    MCPWM_InitStructure.TriggerPoint2           = -PWM_PERIOD;     ;   /* PWM触发ADC事件2，时间点设置 */
    MCPWM_InitStructure.TriggerPoint3           = -PWM_PERIOD+200      ;   /* PWM触发ADC事件3，时间点设置 */
    MCPWM_InitStructure.DeadTimeCH0N            = 100   ;   /* CH0N死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH0P            = 100   ;   /* CH0P死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH1N            = 100   ;   /* CH1N死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH1P            = 100   ;   /* CH1P死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH2N            = 100   ;   /* CH2N死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH2P            = 100   ;   /* CH2P死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH3N            = 100   ;   /* CH3N死区时间设置　*/
    MCPWM_InitStructure.DeadTimeCH3P            = 100   ;   /* CH3P死区时间设置　*/
    MCPWM_InitStructure.CH0N_Polarity_INV       = DISABLE     ;   /* CH0N输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH0P_Polarity_INV       = DISABLE     ;   /* CH0P输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH1N_Polarity_INV       = DISABLE     ;   /* CH1N输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH1P_Polarity_INV       = DISABLE     ;   /* CH1P输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH2N_Polarity_INV       = DISABLE     ;   /* CH2N输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH2P_Polarity_INV       = DISABLE     ;   /* CH2P输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH3N_Polarity_INV       = DISABLE     ;   /* CH3N输出极性取反，0:正常输出；1:取反输出 */
    MCPWM_InitStructure.CH3P_Polarity_INV       = DISABLE     ;   /* CH3P输出极性取反，0:正常输出；1:取反输出 */
    //暂时保留NP对调的配置
    MCPWM_InitStructure.Switch_CH0N_CH0P        = ENABLE   ;   /* 交换CH0N, CH0P信号输出使能开关 */
    MCPWM_InitStructure.Switch_CH1N_CH1P        = ENABLE   ;   /* 交换CH1N, CH1P信号输出使能开关 */
    MCPWM_InitStructure.Switch_CH2N_CH2P        = ENABLE   ;   /* 交换CH2N, CH2P信号输出使能开关 */
    MCPWM_InitStructure.Switch_CH3N_CH3P        = ENABLE   ;   /* 交换CH3N, CH3P信号输出使能开关 */
    
    MCPWM_InitStructure.MCPWM_UpdateInterval    = 0         ;   /* MCPWM T0/T1事件更新间隔 */
    MCPWM_InitStructure.MCPWM_Base0T0_UpdateEN  = ENABLE    ;   /* MCPWM 时基0 T0事件更新使能 */
    MCPWM_InitStructure.MCPWM_Base0T1_UpdateEN  = DISABLE   ;   /* MCPWM 时基0 T1事件更新使能 */
    MCPWM_InitStructure.MCPWM_Base1T0_UpdateEN  = ENABLE    ;   /* MCPWM 时基1 T0事件更新使能 */
    MCPWM_InitStructure.MCPWM_Base1T1_UpdateEN  = DISABLE   ;   /* MCPWM 时基1 T1事件更新使能 */
    MCPWM_InitStructure.MCPWM_Auto_ERR_EN       = DISABLE   ;   /* MCPWM 更新事件是否自动打开MOE, 使能开关 */
    MCPWM_InitStructure.DebugMode_PWM_out       = DISABLE   ;   /* Debug时，MCU进入Halt, MCPWM信号是否正常输出 */
    MCPWM_InitStructure.GPIO_BKIN_Filter        = 1     ;   /* GPIO输入滤波时钟设置1-16 */
    MCPWM_InitStructure.CMP_BKIN_Filter         = 1     ;   /* 比较器CMP输入滤波时钟设置1-16 */
    
    MCPWM_InitStructure.FAIL0_INPUT_EN          = DISABLE   ;   /* FAIL0 输入功能使能 */
    MCPWM_InitStructure.FAIL0_INT_EN            = DISABLE   ;   /* FAIL0事件中断使能 */
    MCPWM_InitStructure.FAIL0_Signal_Sel        = 1     ;   /* FAIL0 信号选择，比较器0或GPIO */
    MCPWM_InitStructure.FAIL0_Polarity          = 0     ;   /* FAIL0 信号极性设置，高有效或低有效 */
    
    MCPWM_InitStructure.FAIL1_INPUT_EN          = DISABLE     ;   /* FAIL1 输入功能使能 */
    MCPWM_InitStructure.FAIL1_INT_EN            = DISABLE     ;   /* FAIL1事件中断使能 */
    MCPWM_InitStructure.FAIL1_Signal_Sel        = 0     ;   /* FAIL1 信号选择，比较器0或GPIO */
    MCPWM_InitStructure.FAIL1_Polarity          = 0     ;   /* FAIL1 信号极性设置，高有效 */
    
    MCPWM_InitStructure.FAIL2_INPUT_EN          = DISABLE     ;   /* FAIL2 输入功能使能 */
    MCPWM_InitStructure.FAIL2_INT_EN            = DISABLE     ;   /* FAIL2事件中断使能 */
    MCPWM_InitStructure.FAIL2_Signal_Sel        = 0     ;   /* FAIL2 信号选择，比较器0或GPIO */
    MCPWM_InitStructure.FAIL2_Polarity          = 0     ;   /* FAIL2 信号极性设置，高有效或低有效 */
    
    MCPWM_InitStructure.FAIL3_INPUT_EN          = DISABLE     ;   /* FAIL3 输入功能使能 */
    MCPWM_InitStructure.FAIL3_INT_EN            = DISABLE     ;   /* FAIL3事件中断使能 */
    MCPWM_InitStructure.FAIL3_Signal_Sel        = 0     ;   /* FAIL3 信号选择，比较器0或GPIO */
    MCPWM_InitStructure.FAIL3_Polarity          = 0     ;   /* FAIL3 信号极性设置，高有效或低有效 */
    
    MCPWM_InitStructure.CH0P_default_output     = 0     ;   /* CH0P MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH0N_default_output     = 0     ;   /* CH0N MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH1P_default_output     = 0     ;   /* CH1P MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH1N_default_output     = 0     ;   /* CH1N MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH2P_default_output     = 0     ;   /* CH2P MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH2N_default_output     = 0     ;   /* CH2N MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH3P_default_output     = 0     ;   /* CH3P MOE为0时或发生FAIL事件时，默认电平输出 */
    MCPWM_InitStructure.CH3N_default_output     = 0     ;   /* CH3N MOE为0时或发生FAIL事件时，默认电平输出 */
    
    MCPWM_InitStructure.T0_Update_INT_EN        = 1     ;   /* T0更新事件中斷使能 */
    MCPWM_InitStructure.T1_Update_INT_EN        = 0     ;   /* T1更新事件中斷使能 */
    MCPWM_InitStructure.TMR0_Match_INT_EN       = 0     ;   /* TMR0计数事件匹配事件中断使能 */
    MCPWM_InitStructure.TMR1_Match_INT_EN       = 0     ;   /* TMR1计数事件匹配事件中断使能 */
    MCPWM_InitStructure.AUEN                    =   MCPWM_AUEN_TH00 |
                                                    MCPWM_AUEN_TH01 | 
                                                    MCPWM_AUEN_TH10 | 
                                                    MCPWM_AUEN_TH11 |
                                                    MCPWM_AUEN_TH20 | 
                                                    MCPWM_AUEN_TH21 |   /* 自动更新使能寄存器 */
                                                    MCPWM_AUEN_TMR0 | 
                                                    MCPWM_AUEN_TMR1 |
                                                    MCPWM_AUEN_TMR2 |
                                                    MCPWM_AUEN_TMR3 ;   /* 自动更新使能寄存器 */
    MCPWM_Init(MCPWM1,&MCPWM_InitStructure);
    NVIC_SetPriority(MCPWM10_IRQn,1);
    NVIC_EnableIRQ (MCPWM10_IRQn);

    // Mctime_1_PwmUpdate(0.5,0.5,0.5);
    
    // MCPWM1_PRT = 0xDEAD;
    // MCPWM1->IE0 = 1<<13;	//TMR3中断
    // MCPWM1_PRT = 0x00;

   Mctime_1_PwmEnable();//使能MOE主输出
    // Time0_Init();
}

void Time0_Init(void)
{
    UTIMER_InitTypeDef UTIMER_InitStruct;
    UTIMER_StructInit(&UTIMER_InitStruct);

    UTIMER_InitStruct.EN            = ENABLE                ;   // Timer 模块整体使能，高有效
    UTIMER_InitStruct.ETON          = 0                     ;   // Timer 计数器计数使能配置 0:自动运行 1:等待外部事件触发计数
    UTIMER_InitStruct.CLK_DIV       = UTIMER_Clk_Div8		;   // Timer 计数器分频设置 24Mhz
    UTIMER_InitStruct.CLK_SRC       = UTIMER_CLK_SRC_MCLK   ;   // Timer 时钟源
    UTIMER_InitStruct.TH            = 24000    				;   // Timer 计数器计数门限。1ms
    UTIMER_InitStruct.IE            = UTIMER_IRQEna_Zero	;   // 中断配置
    UTIMER_InitStruct.RE            = 0                     ;   // DMA请求
    UTIMER_InitStruct.CH0_MODE      = 0                     ;   //比较模式
    UTIMER_InitStruct.CMP0          = 10                    ;   //设置比较值，该值用于产生触发信号同步两个PWM

    UTIMER_Init(UTIMER0,&UTIMER_InitStruct);
    NVIC_SetPriority(TIMER0_IRQn,2);
    NVIC_EnableIRQ (TIMER0_IRQn);
}

void McpwmCfg_Init(void)
{
    Mcpwm0Cfg_GpioInit();
    Mcpwm1Cfg_GpioInit();
    Mcpwm0Cfg_BaseInit();
    Mcpwm1Cfg_BaseInit();
    Time0_Init();
}

void MTR0_MCPWM_OUT(math_vec3_t *out)
{
    MTR0_PWMA_OUT(out->Value[0]);
    MTR0_PWMB_OUT(out->Value[1]);
    MTR0_PWMC_OUT(out->Value[2]);
}

void MTR1_MCPWM_OUT(math_vec3_t *out)
{
    MTR1_PWMA_OUT(out->Value[0]);
    MTR1_PWMB_OUT(out->Value[1]);
    MTR1_PWMC_OUT(out->Value[2]);
}
