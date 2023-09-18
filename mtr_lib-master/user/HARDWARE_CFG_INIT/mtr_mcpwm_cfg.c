/*
*********************************************************************************************************
*                                                  MTR_MCPWM_CFG
*                                              MTR_MCPWM_CFG MOUDULE
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                  MTR_MCPWM_CFG
* Filename     :mtr_mcpwm_cfg.c
*
*********************************************************************************************************
*/
#define DEF_DBG_I
#define DEF_DBG_W
#define DEF_DBG_E

#define MRT_MCPWM_CFG_MODULE

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "mtr_mcpwm_cfg.h"
#include "x_frame.h"
#include "hardware_cfg.h"

#include "lks32mc45x_lib.h"
#include <assert.h>
#include "IQmath.h"

#define MCPWM0_PIN_0    GPIO1,1
#define MCPWM0_PIN_1    GPIO1,2
#define MCPWM0_PIN_2    GPIO1,3
#define MCPWM0_PIN_3    GPIO1,4
#define MCPWM0_PIN_4    GPIO1,5
#define MCPWM0_PIN_5    GPIO1,6

void Mctime0_Init(void);
void Mctime_PwmUpdate(float u,float v,float w);
void Mctime_PwmEnable(void);

/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
static void Delay(volatile u32 cnt)
{
    volatile u32 x_cnt=0;
    volatile u32 y_cnt=0;
    for(y_cnt = 0; y_cnt < cnt; y_cnt++)
    {
		for(x_cnt = 0;x_cnt<19200;x_cnt++)
		{
			__NOP();
		}
    }
}
void mtr_mcpwm_config(void)
{	
	//VBUS PWM模拟过压输出
	UTIMER2_CMP1=BUCK_UTIME_TH*(130/162.4);//3.3*49.214 = 162.407
	CORDIC_Enable();
	SYS_SoftResetMODULE(SYS_MODULE_CORDIC);
	
	Mctime0_Init();
	Delay(10);
	MCPWM0_EIF = 0XFF;
	
	MCPWM0_PRT = 0xDEAD;
	MCPWM0_TCLK |= 3<<6;
	MCPWM0_PRT = 0x00;    

	Mctime_PwmEnable();
	
	GPIO_Config(GPIO0,GPIO_Pin_14,GPIO_Mode_IN,GPIO_AF_GPIO);	//按钮
	GPIO0->PIE |=1<<14;
}







void Mctime0_Init(void)
{
    GPIO_Config(MCPWM0_PIN_0,GPIO_Mode_OUT,GPIO_AF_MCPWM);
    GPIO_Config(MCPWM0_PIN_1,GPIO_Mode_OUT,GPIO_AF_MCPWM);
    GPIO_Config(MCPWM0_PIN_2,GPIO_Mode_OUT,GPIO_AF_MCPWM);
    GPIO_Config(MCPWM0_PIN_3,GPIO_Mode_OUT,GPIO_AF_MCPWM);
    GPIO_Config(MCPWM0_PIN_4,GPIO_Mode_OUT,GPIO_AF_MCPWM);
    GPIO_Config(MCPWM0_PIN_5,GPIO_Mode_OUT,GPIO_AF_MCPWM);
	
    GPIO_Config(GPIO1,7,GPIO_Mode_OUT,GPIO_AF_MCPWM);	//制动电阻
    GPIO_Config(GPIO1,9,GPIO_Mode_IN,GPIO_AF_MCPWM);	//BKN0 过流
    GPIO_Config(GPIO1,10,GPIO_Mode_IN,GPIO_AF_MCPWM);	//BKN1 过压
	GPIO1->F3210                         &=  0x000F;
	GPIO1->F7654                         &=  0xF000;
    {

        MCPWM_InitTypeDef MCPWM_InitStructure;

        MCPWM_StructInit(&MCPWM_InitStructure);

        MCPWM_InitStructure.TimeBase0_PERIOD        = MTR_PWM_PERIOD ;	 /* 时期0周期设置 ((192M/FREQ)-1)/2*/
        MCPWM_InitStructure.TimeBase1_PERIOD        = MTR_PWM_PERIOD ;   /* 时期1周期设置*/
        MCPWM_InitStructure.CLK_DIV                 = 0     ;   /* MCPWM 分频系数 */
        MCPWM_InitStructure.MCLK_EN                 = ENABLE;   /* MCPWM 时钟使能开关 */
        MCPWM_InitStructure.TMR2_TimeBase_Sel       = 0     ;   /* TMR2 比较门限寄存器 时基选择 0:时基0 | 1:时基1 */
        MCPWM_InitStructure.TMR3_TimeBase_Sel       = 0     ;   /* TMR3 比较门限寄存器 时基选择 0:时基0 | 1:时基1 */
        MCPWM_InitStructure.MCPWM_Cnt0_EN           = DISABLE;   /* MCPWM 时基0主计数器使能开关 */
        MCPWM_InitStructure.TimeBase0_Trig_Enable   = 0     ;   /* 时基0 外部触发使能 */
        MCPWM_InitStructure.TimeBase0Init_CNT       = 0     ;   /* 时基0 计数器初始值 */
        MCPWM_InitStructure.TimeBase_TrigEvt0       = 0     ;   /* 时基0 外部触发事件选择 */
        MCPWM_InitStructure.MCPWM_Cnt1_EN           = DISABLE;   /* MCPWM 时基1主计数器使能开关 */
        MCPWM_InitStructure.TimeBase1_Trig_Enable   = 0     ;   /* 时基1 外部触发使能*/
        MCPWM_InitStructure.TimeBase1Init_CNT       = 0     ;   /* 时基1 计数器初始值 */
        MCPWM_InitStructure.TimeBase_TrigEvt1       = 0     ;   /* 时基1 外部触发事件选择 */
        MCPWM_InitStructure.MCPWM_WorkModeCH0       = MCPWM_CENTRAL_PWM_MODE     ;   /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
        MCPWM_InitStructure.MCPWM_WorkModeCH1       = MCPWM_CENTRAL_PWM_MODE     ;   /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
        MCPWM_InitStructure.MCPWM_WorkModeCH2       = MCPWM_CENTRAL_PWM_MODE     ;   /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
        MCPWM_InitStructure.MCPWM_WorkModeCH3       = MCPWM_CENTRAL_PWM_MODE     ;   /* MCPWM CH0工作模式：边沿对齐/中心对齐 */
        MCPWM_InitStructure.TriggerPoint0           = MTR_PWM_PERIOD	     ;   /* PWM触发ADC事件0，时间点设置 */
        MCPWM_InitStructure.TriggerPoint1           = 0      ;   /* PWM触发ADC事件1，时间点设置 */
        MCPWM_InitStructure.TriggerPoint2           = -MTR_PWM_PERIOD;   	  /* PWM触发ADC事件2，时间点设置 */
        MCPWM_InitStructure.TriggerPoint3           = -MTR_PWM_PERIOD+200 ;   /* 用于触发PWM中断，新的一个周期延后一段时间触发 防止ADC采样未完成 */
        MCPWM_InitStructure.DeadTimeCH0N            = MTR_DT   ;   /* CH0N死区时间设置　*/
        MCPWM_InitStructure.DeadTimeCH0P            = MTR_DT   ;   /* CH0P死区时间设置　*/
        MCPWM_InitStructure.DeadTimeCH1N            = MTR_DT   ;   /* CH1N死区时间设置　*/
        MCPWM_InitStructure.DeadTimeCH1P            = MTR_DT   ;   /* CH1P死区时间设置　*/
        MCPWM_InitStructure.DeadTimeCH2N            = MTR_DT   ;   /* CH2N死区时间设置　*/
        MCPWM_InitStructure.DeadTimeCH2P            = MTR_DT   ;   /* CH2P死区时间设置　*/
        MCPWM_InitStructure.DeadTimeCH3N            = MTR_DT   ;   /* CH3N死区时间设置　*/
        MCPWM_InitStructure.DeadTimeCH3P            = MTR_DT   ;   /* CH3P死区时间设置　*/
        MCPWM_InitStructure.CH0N_Polarity_INV       = DISABLE     ;   /* CH0N输出极性取反，0:正常输出；1:取反输出 */
        MCPWM_InitStructure.CH0P_Polarity_INV       = DISABLE     ;   /* CH0P输出极性取反，0:正常输出；1:取反输出 */
        MCPWM_InitStructure.CH1N_Polarity_INV       = DISABLE     ;   /* CH1N输出极性取反，0:正常输出；1:取反输出 */
        MCPWM_InitStructure.CH1P_Polarity_INV       = DISABLE     ;   /* CH1P输出极性取反，0:正常输出；1:取反输出 */
        MCPWM_InitStructure.CH2N_Polarity_INV       = DISABLE     ;   /* CH2N输出极性取反，0:正常输出；1:取反输出 */
        MCPWM_InitStructure.CH2P_Polarity_INV       = DISABLE     ;   /* CH2P输出极性取反，0:正常输出；1:取反输出 */
        MCPWM_InitStructure.CH3N_Polarity_INV       = DISABLE     ;   /* CH3N输出极性取反，0:正常输出；1:取反输出 */
        MCPWM_InitStructure.CH3P_Polarity_INV       = DISABLE     ;   /* CH3P输出极性取反，0:正常输出；1:取反输出 */
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
        MCPWM_InitStructure.DebugMode_PWM_out       = ENABLE   ;   /* Debug时，MCU进入Halt, MCPWM信号是否正常输出 */
        MCPWM_InitStructure.GPIO_BKIN_Filter        = 1     ;   /* GPIO输入滤波时钟设置1-16 */
        MCPWM_InitStructure.CMP_BKIN_Filter         = 1     ;   /* 比较器CMP输入滤波时钟设置1-16 */
        
        MCPWM_InitStructure.FAIL0_INPUT_EN          = DISABLE   ;   /* FAIL0 输入功能使能 */
        MCPWM_InitStructure.FAIL0_INT_EN            = DISABLE   ;   /* FAIL0事件中断使能 */
        MCPWM_InitStructure.FAIL0_Signal_Sel        = 0     ;   /* FAIL0 信号选择，比较器0或GPIO */
        MCPWM_InitStructure.FAIL0_Polarity          = 0     ;   /* FAIL0 信号极性设置，高有效或低有效 */
        
        MCPWM_InitStructure.FAIL1_INPUT_EN          = ENABLE     ;   /* FAIL1 输入功能使能 */
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
        MCPWM_InitStructure.CH0N_default_output     = 1     ;   /* CH0N MOE为0时或发生FAIL事件时，默认电平输出 */
        MCPWM_InitStructure.CH1P_default_output     = 0     ;   /* CH1P MOE为0时或发生FAIL事件时，默认电平输出 */
        MCPWM_InitStructure.CH1N_default_output     = 1     ;   /* CH1N MOE为0时或发生FAIL事件时，默认电平输出 */
        MCPWM_InitStructure.CH2P_default_output     = 0     ;   /* CH2P MOE为0时或发生FAIL事件时，默认电平输出 */
        MCPWM_InitStructure.CH2N_default_output     = 1      ;   /* CH2N MOE为0时或发生FAIL事件时，默认电平输出 */
        MCPWM_InitStructure.CH3P_default_output     = 0     ;   /* CH3P MOE为0时或发生FAIL事件时，默认电平输出 */
        MCPWM_InitStructure.CH3N_default_output     = 0     ;   /* CH3N MOE为0时或发生FAIL事件时，默认电平输出 */
        
        MCPWM_InitStructure.T0_Update_INT_EN        = 0     ;   /* T0更新事件中斷使能 */
        MCPWM_InitStructure.T1_Update_INT_EN        = 0     ;   /* T1更新事件中斷使能 */
        MCPWM_InitStructure.TMR0_Match_INT_EN       = 0     ;   /* TMR0计数事件匹配事件中断使能 */
        MCPWM_InitStructure.TMR1_Match_INT_EN       = 0     ;   /* TMR1计数事件匹配事件中断使能 */
        MCPWM_InitStructure.AUEN                    =   MCPWM_AUEN_TH00 |
                                                        MCPWM_AUEN_TH01 | 
                                                        MCPWM_AUEN_TH10 | 
                                                        MCPWM_AUEN_TH11 |
                                                        MCPWM_AUEN_TH20 | 
                                                        MCPWM_AUEN_TH21 |
														MCPWM_AUEN_TMR0 | 
														MCPWM_AUEN_TMR1 |
														MCPWM_AUEN_TMR2 |
														MCPWM_AUEN_TMR3 ;   /* 自动更新使能寄存器 */
        MCPWM_Init(MCPWM0,&MCPWM_InitStructure);
//			NVIC_SetPriority(MCPWM00_IRQn,1);
//			NVIC_EnableIRQ (MCPWM00_IRQn);
		MCPWM0->TH00 = 0;
		MCPWM0->TH01 = 0;
		MCPWM0->TH10 = 0;
		MCPWM0->TH11 = 0;
		MCPWM0->TH20 = 0;
		MCPWM0->TH21 = 0;
		


//		MCPWM0_PRT = 0xDEAD;
//		MCPWM0->IE0 = 1<<13;	//TMR3中断
//		MCPWM0_PRT = 0x00;	
		
	}
}

void Mctime_PwmEnable(void)
{
	MCPWM0->PRT = 0x0000DEAD;
	MCPWM0->FAIL012 |= MCPWM_MOE_ENABLE_MASK;
	MCPWM0->PRT = 0x0000CAFE;

}
void Mctime_PwmDisable(void)
{
	MCPWM0->PRT = 0x0000DEAD;
	MCPWM0->FAIL012 &= MCPWM_MOE_DISABLE_MASK;
	MCPWM0->PRT = 0x0000CAFE;
}


void Mctime_PwmUpdate(float u,float v,float w)
{
    s16 a,b,c,th;
    th = MCPWM0->TH0;
    a = u * th;
    b = v * th;
    c = w * th;
    MCPWM0->TH00 = -a;
    MCPWM0->TH01 = a;
    MCPWM0->TH10 = -b;
    MCPWM0->TH11 = b;
    MCPWM0->TH20 = -c;
    MCPWM0->TH21 = c;
}
