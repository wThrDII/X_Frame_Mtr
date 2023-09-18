#define ADC_CFG_MODULE

#include "adc_cfg.h"
#include "mtr_drv_cfg.h"
#include "x_frame.h"

void AdcCfg_GpioInit(void)
{
    GPIO_Config(GPIO4,0,GPIO_Mode_ANA,GPIO_AF_ANA);  //OPA0
	GPIO_Config(GPIO3,15,GPIO_Mode_ANA,GPIO_AF_ANA);
    
	GPIO_Config(GPIO3,9,GPIO_Mode_ANA,GPIO_AF_ANA);  //OPA2
	GPIO_Config(GPIO3,10,GPIO_Mode_ANA,GPIO_AF_ANA);
    
    //emf1
    GPIO_Config(GPIO5,0,GPIO_Mode_ANA,GPIO_AF_ANA);
    GPIO_Config(GPIO5,1,GPIO_Mode_ANA,GPIO_AF_ANA);
    GPIO_Config(GPIO4,12,GPIO_Mode_ANA,GPIO_AF_ANA);
    //emf2
    GPIO_Config(GPIO3,11,GPIO_Mode_ANA,GPIO_AF_ANA);
    GPIO_Config(GPIO3,12,GPIO_Mode_ANA,GPIO_AF_ANA);
    GPIO_Config(GPIO3,13,GPIO_Mode_ANA,GPIO_AF_ANA);
}

void AdcCfg_BaseInit(void)
{
    // ADC0配置说明
    // 通过软件触发ADC进行采样，超采样获得16位ADC精度
    // ADC采集芯片内温度传感器数据
    ADC_InitTypeDef ADC_InitStruct;
    ADC_StructInit(&ADC_InitStruct);
    
    ADC_InitStruct.IE           = ADC_IE_SF1|ADC_IE_SF2;                  // ADC中断使能 
//      ADC_InitStruct.IE           = 0;                  // ADC中断使能 
    ADC_InitStruct.RE           = 0;                  // ADC触发DMA使能 
    ADC_InitStruct.Align        = ADC_ALIGN_LEFT;     // 采样数据对齐方式 
    ADC_InitStruct.GAIN         = 0; // 通道增益 高增益（1 倍）和低增益（2/3 倍），默认低增益量程为正负3.3V
    ADC_InitStruct.TROVS        = ADC_TROVS_ONES;     // 过采样触发模式
    ADC_InitStruct.OVSR         = ADC_OVSR_1;         // 过采率，过采样可以提高信噪比
    ADC_InitStruct.CSMP         = ADC_CSMP_DISABLE;   // 连续采样模式
    ADC_InitStruct.TCNT         = ADC_TCNT_1;         // 触发次数
    ADC_InitStruct.S1           = ADC_S1_1;           // 第一段采样的通道数
    ADC_InitStruct.S2           = ADC_S2_1;			  // 第二段采样总通道数，两段采样通道数之和最大为16
    ADC_InitStruct.NSMP         = ADC_NSMP_2;         // ADC 触发模式 单段或双段
    ADC_InitStruct.TRIG         = ADC_TRIG_MCPWM0_T0_EN|ADC_TRIG_MCPWM0_T1_EN;	// MCPWM触发采样使能
    ADC_InitStruct.GEN0         = ADC_GEN0_NONE;      // 看门狗 0 通道使能
    ADC_InitStruct.HTH0         = 0;                  // 看门狗 0 上阈值
    ADC_InitStruct.LTH0         = 0;                  // 看门狗 0 下阈值
    ADC_InitStruct.GEN1         = ADC_GEN1_NONE;      // 看门狗 1 通道使能
    ADC_InitStruct.HTH1         = 0;                  // 看门狗 1 上阈值
    ADC_InitStruct.LTH1         = 0;                  // 看门狗 1 下阈值
    ADC_SetPChanne(ADC0,ADC_DAT_0,ADC_CHN_0);//电流采样
    ADC_SetPChanne(ADC0,ADC_DAT_1,ADC_CHN_0);			//电流采样
//        ADC_SetPChanne(ADC0,ADC_DAT_2,ADC_CHN_6);			//EU
//        ADC_SetPChanne(ADC0,ADC_DAT_3,ADC_CHN_7); 			//EV
//        ADC_SetPChanne(ADC0,ADC_DAT_4,ADC_CHN_12);		//EW

    ADC_Init(ADC0,&ADC_InitStruct);

    ADC_ClearIRQFlag(ADC0,0xff);

    
        // ADC2配置说明
    // 通过软件触发ADC进行采样，超采样获得16位ADC精度
    // ADC采集芯片内温度传感器数据
    ADC_StructInit(&ADC_InitStruct);
    
    ADC_InitStruct.IE           = ADC_IE_SF1|ADC_IE_SF2;                  // ADC中断使能 
//      ADC_InitStruct.IE           = 0;                  // ADC中断使能 
    ADC_InitStruct.RE           = 0;                  // ADC触发DMA使能 
    ADC_InitStruct.Align        = ADC_ALIGN_LEFT;     // 采样数据对齐方式 
    ADC_InitStruct.GAIN         = 0; // 通道增益 高增益（1 倍）和低增益（2/3 倍），默认低增益量程为正负3.3V
    ADC_InitStruct.TROVS        = ADC_TROVS_ONES;     // 过采样触发模式
    ADC_InitStruct.OVSR         = ADC_OVSR_1;         // 过采率，过采样可以提高信噪比
    ADC_InitStruct.CSMP         = ADC_CSMP_DISABLE;   // 连续采样模式
    ADC_InitStruct.TCNT         = ADC_TCNT_1;         // 触发次数
    ADC_InitStruct.S1           = ADC_S1_1;           // 第一段采样的通道数
    ADC_InitStruct.S2           = ADC_S2_1;			  // 第二段采样总通道数，两段采样通道数之和最大为16
    ADC_InitStruct.NSMP         = ADC_NSMP_2;         // ADC 触发模式 单段或双段
    ADC_InitStruct.TRIG         = ADC_TRIG_MCPWM1_T0_EN|ADC_TRIG_MCPWM1_T1_EN;	// MCPWM触发采样使能
    ADC_InitStruct.GEN0         = ADC_GEN0_NONE;      // 看门狗 0 通道使能
    ADC_InitStruct.HTH0         = 0;                  // 看门狗 0 上阈值
    ADC_InitStruct.LTH0         = 0;                  // 看门狗 0 下阈值
    ADC_InitStruct.GEN1         = ADC_GEN1_NONE;      // 看门狗 1 通道使能
    ADC_InitStruct.HTH1         = 0;                  // 看门狗 1 上阈值
    ADC_InitStruct.LTH1         = 0;                  // 看门狗 1 下阈值
    ADC_SetPChanne(ADC2,ADC_DAT_0,ADC_CHN_2);//电流采样
    ADC_SetPChanne(ADC2,ADC_DAT_1,ADC_CHN_2);		//电流采样	
//        ADC_SetPChanne(ADC2,ADC_DAT_2,ADC_CHN_7);		//EU
//        ADC_SetPChanne(ADC2,ADC_DAT_3,ADC_CHN_6); 			//EV
//        ADC_SetPChanne(ADC2,ADC_DAT_4,ADC_CHN_5);	//EW

    ADC_Init(ADC2,&ADC_InitStruct);
    ADC_ClearIRQFlag(ADC2,0xff);
    
    
        // ADC1配置说明
    ADC_StructInit(&ADC_InitStruct);
//      ADC_InitStruct.IE           = ADC_IE_SF1;                  // ADC中断使能 
    ADC_InitStruct.IE           = 0;                  // ADC中断使能 
    ADC_InitStruct.RE           = 0;                  // ADC触发DMA使能 
    ADC_InitStruct.Align        = ADC_ALIGN_LEFT;     // 采样数据对齐方式 
    ADC_InitStruct.GAIN         = 0; // 通道增益 高增益（1 倍）和低增益（2/3 倍），默认低增益
    ADC_InitStruct.TROVS        = ADC_TROVS_ONES;     // 过采样触发模式
    ADC_InitStruct.OVSR         = ADC_OVSR_1;         // 过采率，过采样可以提高信噪比
    ADC_InitStruct.CSMP         = ADC_CSMP_DISABLE;   // 连续采样模式
    ADC_InitStruct.TCNT         = ADC_TCNT_1;         // 触发次数
    ADC_InitStruct.S1           = ADC_S1_1;           // 第一段采样的通道数
    ADC_InitStruct.S2           = 0;				// 第二段采样总通道数，两段采样通道数之和最大为16
    ADC_InitStruct.NSMP         = ADC_NSMP_1;         // ADC 触发模式 单段或双段
    ADC_InitStruct.TRIG         = ADC_TRIG_MCPWM0_T0_EN;	// MCPWM触发采样使能
    ADC_InitStruct.GEN0         = ADC_GEN0_NONE;      // 看门狗 0 通道使能
    ADC_InitStruct.HTH0         = 0;                  // 看门狗 0 上阈值
    ADC_InitStruct.LTH0         = 0;                  // 看门狗 0 下阈值
    ADC_InitStruct.GEN1         = ADC_GEN1_NONE;      // 看门狗 1 通道使能
    ADC_InitStruct.HTH1         = 0;                  // 看门狗 1 上阈值
    ADC_InitStruct.LTH1         = 0;                  // 看门狗 1 下阈值
    
    ADC_SetPChanne(ADC1,ADC_DAT_0,ADC_CHN_7);		//VBUS
    ADC_Init(ADC1,&ADC_InitStruct);

    ADC_ClearIRQFlag(ADC1,0xff);
}

void AdcCfg_Init(void)
{
    AdcCfg_GpioInit();
    AdcCfg_BaseInit();
}

void MTR0_ADC_GET(hal_adc_data_t *val)
{
    
}

void MTR1_ADC_GET(hal_adc_data_t *val)
{
    
}
