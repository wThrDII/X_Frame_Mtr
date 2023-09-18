/*
*********************************************************************************************************
*                                                  MTR_ADC_CFG
*                                              MTR_ADC_CFG MOUDULE
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                  MTR_ADC_CFG
* Filename     :mtr_adc_cfg.c
*
*********************************************************************************************************
*/
#define DEF_DBG_I
#define DEF_DBG_W
#define DEF_DBG_E

#define MTR_ADC_CFG_MODULE

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "mtr_adc_cfg.h"
#include "x_frame.h"
#include "hardware_cfg.h"
/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
void Adc_Init(void);

void mtr_adc_config(void)
{
	Adc_Init();
}

#include "lks32mc45x_lib.h"
void Opa_Init(void)
{
    SYS_WR_PROTECT = 0x7a83;      /* 解除系统寄存器写保护 */
//	GPIO_Config(GPIO3,  GPIO_PinSource_11, GPIO_Mode_ANA, GPIO_AF_GPIO);
//	SYS_AFE_REG2 |=3;		//OPA2OUT
//	AFE_OpaOutGpio3Pin11(AFE_OUT_P3_11_OPA2);                 //opa0输出至IO
	SYS_AFE_REG0 &=~(7<<8);
	SYS_AFE_REG0 |=(3<<8);		//	OPA2	 16
	SYS_AFE_REG0 &=~(7<<12);
	SYS_AFE_REG0 |=(3<<12);		//	OPA3	 16
	SYS_AFE_REG1 &=~(7<<4);
	SYS_AFE_REG1 |=(3<<4);		//	OPA5	 16
	SYS_AFE_REG9 |=1<<2;
	SYS_AFE_REG9 |=1<<3;
	SYS_AFE_REG9 |=1<<5;		//使能
	SYS_WR_PROTECT = 0x0;         /*关闭系统寄存器写操作*/

}

void CMP_init(void)
{
		
		AFE_ModuleClockCmd(AFE_MODULE_DAC0,ENABLE);             // 打开DAC0
		AFE_DacSetRange(AFE_MODULE_DAC0,AFE_DAC_RANGE_3_0V);    // 设置DAC0的量程为3V
		AFE_DacSetVoltage(AFE_MODULE_DAC0,2.5);					//  DAC
//		AFE_Dac0OutGpio3Pin4(ENABLE);                           // 将DAC0输出到P3.4	

		GPIO_Config(GPIO3,3,GPIO_Mode_OUT,GPIO_AF_CMP);	//CMP2 	U
		GPIO_Config(GPIO2,15,GPIO_Mode_OUT,GPIO_AF_CMP);//CMP3	V
		GPIO_Config(GPIO0,5,GPIO_Mode_OUT,GPIO_AF_CMP);	//CMP5	W
	

        CMP_InitTypeDef cmp_config;

        // 比较器IO配置
        cmp_config.FT = DISABLE;                  // 比较器快速比较使能
        cmp_config.HYS = CMP_HYS_20mV;            // 比较器回差选择
        cmp_config.IT_CMP = ENABLE;               // 比较速度选择 0:比较速度 150ns    1:比较速度 600ns
        cmp_config.CLK01_DIV = 2048;                 // 比较器01滤波系数，1-2048
        cmp_config.CLK23_DIV = 2048;                 // 比较器01滤波系数，1-2048
        cmp_config.CLK45_DIV = 2048;                 // 比较器01滤波系数，1-2048
		//-------------cmp0(buck负电流保护)-------------------
		cmp_config.cmp0.SELP = AFE_CMP0_SELP_OPA1_OUT_HF;
		cmp_config.cmp0.SELN = AFE_CMP0_SELN_IN;
		cmp_config.cmp0.EN   = ENABLE;
		cmp_config.cmp0.IE   = DISABLE;//DISABLE;
		cmp_config.cmp0.RE   = DISABLE;
		cmp_config.cmp0.POL  = DISABLE;
		cmp_config.cmp0.IRQ_TRIG = DISABLE;//DISABLE;
		cmp_config.cmp0.IN_EN = ENABLE;  //信号输入使能
		cmp_config.cmp0.W_PWM_POL = DISABLE;
		cmp_config.cmp0.CHN3P_PWM0 = DISABLE;
		cmp_config.cmp0.CHN2P_PWM0 = DISABLE;
		cmp_config.cmp0.CHN1P_PWM0 = DISABLE;
		cmp_config.cmp0.CHN0P_PWM0 = DISABLE;
		cmp_config.cmp0.CHN3P_PWM1 = DISABLE;
		cmp_config.cmp0.CHN2P_PWM1 = DISABLE;
		cmp_config.cmp0.CHN1P_PWM1 = DISABLE;
		cmp_config.cmp0.CHN0P_PWM1 = DISABLE;
		//-------------cmp1(buck大电流保护)-------------------
		cmp_config.cmp1.SELP = AFE_CMP1_SELP_OPA1_OUT_HF;
		cmp_config.cmp1.SELN = AFE_CMP1_SELN_IN;
		cmp_config.cmp1.EN   = ENABLE;
		cmp_config.cmp1.IE   = DISABLE;//DISABLE;
		cmp_config.cmp1.RE   = DISABLE;
		cmp_config.cmp1.POL  = DISABLE;
		cmp_config.cmp1.IRQ_TRIG = DISABLE;//DISABLE;
		cmp_config.cmp1.IN_EN = ENABLE;  //信号输入使能
		cmp_config.cmp1.W_PWM_POL = DISABLE;
		cmp_config.cmp1.CHN3P_PWM0 = DISABLE;
		cmp_config.cmp1.CHN2P_PWM0 = DISABLE;
		cmp_config.cmp1.CHN1P_PWM0 = DISABLE;
		cmp_config.cmp1.CHN0P_PWM0 = DISABLE;
		cmp_config.cmp1.CHN3P_PWM1 = DISABLE;
		cmp_config.cmp1.CHN2P_PWM1 = DISABLE;
		cmp_config.cmp1.CHN1P_PWM1 = DISABLE;
		cmp_config.cmp1.CHN0P_PWM1 = DISABLE;
		//-------------cmp235(相电流保护)-------------------
        cmp_config.cmp2.SELP = AFE_CMP2_SELP_OPA2_OUT_HF; // 比较器 0 信号正端选择
        cmp_config.cmp2.SELN = AFE_CMP2_SELN_DAC0; // 比较器 0 信号负端选择
        cmp_config.cmp2.EN = ENABLE;              // 比较器 0 使能
        cmp_config.cmp2.IE = ENABLE;              // 比较器 0 中断使能
        cmp_config.cmp2.RE = DISABLE;             // 比较器 0 DMA 请求使能
        cmp_config.cmp2.POL = DISABLE;            // 比较器 0 极性选择
        cmp_config.cmp2.IRQ_TRIG = DISABLE;        // 比较器 0 边沿触发使能
        cmp_config.cmp2.IN_EN = ENABLE;           // 比较器 0 信号输入使能
        cmp_config.cmp2.W_PWM_POL = DISABLE;      // 比较器 0 开窗 PWM 信号极性选择
        cmp_config.cmp2.CHN3P_PWM0 = DISABLE;     // MCPWM0 模块 CHN3_P 通道使能比较器 0 开窗
        cmp_config.cmp2.CHN2P_PWM0 = DISABLE;     // MCPWM0 模块 CHN2_P 通道使能比较器 0 开窗
        cmp_config.cmp2.CHN1P_PWM0 = DISABLE;     // MCPWM0 模块 CHN1_P 通道使能比较器 0 开窗
        cmp_config.cmp2.CHN0P_PWM0 = DISABLE;     // MCPWM0 模块 CHN0_P 通道使能比较器 0 开窗
        cmp_config.cmp2.CHN3P_PWM1 = DISABLE;     // MCPWM1 模块 CHN3_P 通道使能比较器 0 开窗
        cmp_config.cmp2.CHN2P_PWM1 = DISABLE;     // MCPWM1 模块 CHN2_P 通道使能比较器 0 开窗
        cmp_config.cmp2.CHN1P_PWM1 = DISABLE;     // MCPWM1 模块 CHN1_P 通道使能比较器 0 开窗
        cmp_config.cmp2.CHN0P_PWM1 = DISABLE;     // MCPWM1 模块 CHN0_P 通道使能比较器 0 开窗; // CMP0配置

        cmp_config.cmp3.SELP = AFE_CMP3_SELP_OPA3_OUT_HF; // 比较器 0 信号正端选择
        cmp_config.cmp3.SELN = AFE_CMP3_SELN_DAC0; // 比较器 0 信号负端选择
        cmp_config.cmp3.EN = ENABLE;              // 比较器 0 使能
        cmp_config.cmp3.IE = ENABLE;              // 比较器 0 中断使能
        cmp_config.cmp3.RE = DISABLE;             // 比较器 0 DMA 请求使能
        cmp_config.cmp3.POL = DISABLE;            // 比较器 0 极性选择
        cmp_config.cmp3.IRQ_TRIG = DISABLE;        // 比较器 0 边沿触发使能
        cmp_config.cmp3.IN_EN = ENABLE;           // 比较器 0 信号输入使能
        cmp_config.cmp3.W_PWM_POL = DISABLE;      // 比较器 0 开窗 PWM 信号极性选择
        cmp_config.cmp3.CHN3P_PWM0 = DISABLE;     // MCPWM0 模块 CHN3_P 通道使能比较器 0 开窗
        cmp_config.cmp3.CHN2P_PWM0 = DISABLE;     // MCPWM0 模块 CHN2_P 通道使能比较器 0 开窗
        cmp_config.cmp3.CHN1P_PWM0 = DISABLE;     // MCPWM0 模块 CHN1_P 通道使能比较器 0 开窗
        cmp_config.cmp3.CHN0P_PWM0 = DISABLE;     // MCPWM0 模块 CHN0_P 通道使能比较器 0 开窗
        cmp_config.cmp3.CHN3P_PWM1 = DISABLE;     // MCPWM1 模块 CHN3_P 通道使能比较器 0 开窗
        cmp_config.cmp3.CHN2P_PWM1 = DISABLE;     // MCPWM1 模块 CHN2_P 通道使能比较器 0 开窗
        cmp_config.cmp3.CHN1P_PWM1 = DISABLE;     // MCPWM1 模块 CHN1_P 通道使能比较器 0 开窗
        cmp_config.cmp3.CHN0P_PWM1 = DISABLE;     // MCPWM1 模块 CHN0_P 通道使能比较器 0 开窗; // CMP0配置
		
        cmp_config.cmp5.SELP = AFE_CMP5_SELP_OPA5_OUT_HF; // 比较器 0 信号正端选择
        cmp_config.cmp5.SELN = AFE_CMP5_SELN_DAC0; // 比较器 0 信号负端选择
        cmp_config.cmp5.EN = ENABLE;              // 比较器 0 使能
        cmp_config.cmp5.IE = ENABLE;              // 比较器 0 中断使能
        cmp_config.cmp5.RE = DISABLE;             // 比较器 0 DMA 请求使能
        cmp_config.cmp5.POL = DISABLE;            // 比较器 0 极性选择
        cmp_config.cmp5.IRQ_TRIG = DISABLE;        // 比较器 0 边沿触发使能
        cmp_config.cmp5.IN_EN = ENABLE;           // 比较器 0 信号输入使能
        cmp_config.cmp5.W_PWM_POL = DISABLE;      // 比较器 0 开窗 PWM 信号极性选择
        cmp_config.cmp5.CHN3P_PWM0 = DISABLE;     // MCPWM0 模块 CHN3_P 通道使能比较器 0 开窗
        cmp_config.cmp5.CHN2P_PWM0 = DISABLE;     // MCPWM0 模块 CHN2_P 通道使能比较器 0 开窗
        cmp_config.cmp5.CHN1P_PWM0 = DISABLE;     // MCPWM0 模块 CHN1_P 通道使能比较器 0 开窗
        cmp_config.cmp5.CHN0P_PWM0 = DISABLE;     // MCPWM0 模块 CHN0_P 通道使能比较器 0 开窗
        cmp_config.cmp5.CHN3P_PWM1 = DISABLE;     // MCPWM1 模块 CHN3_P 通道使能比较器 0 开窗
        cmp_config.cmp5.CHN2P_PWM1 = DISABLE;     // MCPWM1 模块 CHN2_P 通道使能比较器 0 开窗
        cmp_config.cmp5.CHN1P_PWM1 = DISABLE;     // MCPWM1 模块 CHN1_P 通道使能比较器 0 开窗
        cmp_config.cmp5.CHN0P_PWM1 = DISABLE;     // MCPWM1 模块 CHN0_P 通道使能比较器 0 开窗; // CMP0配置

        CMP_Init(&cmp_config);
//		NVIC_SetPriority(CMP0_IRQn,0);
//		NVIC_EnableIRQ (CMP0_IRQn);
		
	    SYS_WR_PROTECT = 0x7a83;           /* 使能系统寄存器写操作*/
		SYS_AFE_REG4 |= 0<<14;           //设置CMP0的负端选择为CMP0_IN
		
		SYS_AFE_REG5 |= 2<<2;			//比较器2负端DAC0
		SYS_AFE_REG5 |= 2<<0;			//比较器3负端DAC0
		SYS_AFE_REG5 |= 2<<4;			//比较器5负端DAC0
		SYS_WR_PROTECT = 0;           /* 使能系统寄存器写操作*/
		
		CMP_AnalogEnable( CMP_CHN_0);
        CMP_AnalogEnable( CMP_CHN_1);
        CMP_AnalogEnable( CMP_CHN_2);
        CMP_AnalogEnable( CMP_CHN_3);
        CMP_AnalogEnable( CMP_CHN_5);



}
void Adc_Init(void)
{
//	SYS_WR_PROTECT = 0x7a83;           /* 使能系统寄存器写操作*/
//	SYS_AFE_REG2 |= 1<<12;
//	SYS_WR_PROTECT = 0;					/* 使能系统寄存器写操作*/
	
	Opa_Init();
	CMP_init();

	GPIO_Config(GPIO5, 1,GPIO_Mode_ANA,GPIO_AF_ANA);//EMFU
	GPIO_Config(GPIO4,15,GPIO_Mode_ANA,GPIO_AF_ANA);//EMFV
	GPIO_Config(GPIO4,14,GPIO_Mode_ANA,GPIO_AF_ANA);//EMFW
	GPIO_Config(GPIO4,5,GPIO_Mode_ANA,GPIO_AF_ANA);//VBUS

	
        // ADC配置说明
        // 通过软件触发ADC进行采样，超采样获得16位ADC精度
        // ADC采集芯片内温度传感器数据
        ADC_InitTypeDef ADC_InitStruct;
        ADC_StructInit(&ADC_InitStruct);
        
        ADC_InitStruct.IE           = ADC_IE_SF1;                  // ADC中断使能 
//      ADC_InitStruct.IE           = 0;                  // ADC中断使能 
        ADC_InitStruct.RE           = 0;                  // ADC触发DMA使能 
        ADC_InitStruct.Align        = ADC_ALIGN_LEFT;     // 采样数据对齐方式 
        ADC_InitStruct.GAIN         = 0; // 通道增益 高增益（1 倍）和低增益（2/3 倍），默认低增益
        ADC_InitStruct.TROVS        = ADC_TROVS_ONES;     // 过采样触发模式
        ADC_InitStruct.OVSR         = ADC_OVSR_1;         // 过采率，过采样可以提高信噪比
        ADC_InitStruct.CSMP         = ADC_CSMP_DISABLE;   // 连续采样模式
        ADC_InitStruct.TCNT         = ADC_TCNT_1;         // 触发次数
        ADC_InitStruct.S1           = ADC_S1_7;           // 第一段采样的通道数
        ADC_InitStruct.S2           = 0;			  // 第二段采样总通道数，两段采样通道数之和最大为16
        ADC_InitStruct.NSMP         = ADC_NSMP_1;         // ADC 触发模式 单段或双段
        ADC_InitStruct.TRIG         = ADC_TRIG_MCPWM0_T0_EN;	// MCPWM触发采样使能
        ADC_InitStruct.GEN0         = ADC_GEN0_NONE;      // 看门狗 0 通道使能
        ADC_InitStruct.HTH0         = 0;                  // 看门狗 0 上阈值
        ADC_InitStruct.LTH0         = 0;                  // 看门狗 0 下阈值
        ADC_InitStruct.GEN1         = ADC_GEN1_NONE;      // 看门狗 1 通道使能
        ADC_InitStruct.HTH1         = 0;                  // 看门狗 1 上阈值
        ADC_InitStruct.LTH1         = 0;                  // 看门狗 1 下阈值
        ADC_SetPChanne(ADC0,ADC_DAT_0,ADC_CHN_2);		//OPA2
        ADC_SetPChanne(ADC0,ADC_DAT_1,ADC_CHN_3);		//OPA3
        ADC_SetPChanne(ADC0,ADC_DAT_2,ADC_CHN_5);		//OPA5
		ADC_SetPChanne(ADC0,ADC_DAT_3,ADC_CHN_11);			//vbus
        ADC_SetPChanne(ADC0,ADC_DAT_4,ADC_CHN_6); 			//EU
        ADC_SetPChanne(ADC0,ADC_DAT_5,ADC_CHN_8);			//EV
        ADC_SetPChanne(ADC0,ADC_DAT_6,ADC_CHN_9);			//EW


        ADC_Init(ADC0,&ADC_InitStruct);

		ADC_ClearIRQFlag(ADC0,0xff);
		NVIC_SetPriority(ADC0_IRQn,2);
		NVIC_EnableIRQ (ADC0_IRQn);
		
		
		
		
}

