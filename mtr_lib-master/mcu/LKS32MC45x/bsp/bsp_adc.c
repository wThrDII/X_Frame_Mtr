/**
 * @brief ADC驱动
 * @file bsp_adc.c
 * @version 1.0
 * @author peixiang.qiu
 * @date 2023-01-08 21:33:21
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2023-01-08 21:33:21 <td>1.0       <td>peixiang.qiu      <td>Created function
 */

#include "bsp_adc.h"
#include "string.h"
#include "x_frame.h"
#if PROGRAM_TYPE == PROGRAM_APL
#include "adc_cfg.h"
#endif
#if defined(__CC_ARM)
// Suppress warning message: extended constant initialiser used
#pragma diag_suppress 1296
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#endif

/**
 * @brief DMA初始化
 *
 * @param [in] adc  ADC组参数指针
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
//static void bsp_dma_config(P_BSP_ADC adc)
//{
//    if (adc->p_dma_param->dma_ch == (DMA_RegTypeDef *)DMA_NULL)
//    {
//        return;
//    }
//    DMA_Enable();
//    DMA_InitTypeDef DMAInitStruct;
//    DMAInitStruct.DMA_Channel_EN = ENABLE;                           /* DMA 通道使能*/
//    DMAInitStruct.DMA_IRQ_EN = DMA_TCIE;                             /* DMA 中断使能 */
//    DMAInitStruct.DMA_RMODE = DISABLE;                               /* 多轮传输使能 */
//    DMAInitStruct.DMA_CIRC = ENABLE;                                 /* 循环模式使能 */
//    DMAInitStruct.DMA_SINC = ENABLE;                                 /* 源地址递增使能 */
//    DMAInitStruct.DMA_DINC = ENABLE;                                 /* 目的地址递增使能 */
//    DMAInitStruct.DMA_SBTW = 2;                                      /* 源地址访问位宽， 0:byte, 1:half-word, 2:word */
//    DMAInitStruct.DMA_DBTW = 1;                                      /* 目的地址访问位宽， 0:byte, 1:half-word, 2:word */
//    DMAInitStruct.DMA_REQ_EN = adc->p_dma_param->dma_req_en;         /* 通道 x 硬件 DMA 请求使能，高有效 */
//    DMAInitStruct.DMA_TIMES = (uint32_t)(adc->p_dma_param->mem_len); /* DMA 通道 x 数据搬运次数 */
//    DMAInitStruct.DMA_SADR = (uint32_t)(adc->adc_periph);            /* DMA 通道 x 源地址 */
//    DMAInitStruct.DMA_DADR = (uint32_t)(adc->p_dma_param->mem_addr); /* DMA 通道 x 目的地址 */
//    DMA_Init(adc->p_dma_param->dma_ch, &DMAInitStruct);
//}

/**
 * @brief ADC初始化
 *
 * @param [in] adc  ADC组参数指针
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
//static void bsp_adc_config(P_BSP_ADC adc)
//{
//    SYS_SoftResetMODULE(adc->sys_module);                    // 复位
//    adc->adc_periph->DC0 = TRIM_Read(adc->TRIM_VAL_ADC_DC0); // 校准
//    adc->adc_periph->AMC0 = TRIM_Read(adc->TRIM_VAL_ADC_AMC0);
//    adc->adc_periph->DC1 = TRIM_Read(adc->TRIM_VAL_ADC_DC1);
//    adc->adc_periph->AMC1 = TRIM_Read(adc->TRIM_VAL_ADC_AMC1);

//    ADC_InitTypeDef ADC_InitStruct;
//    ADC_StructInit(&ADC_InitStruct);

//    for (uint16_t i = 0; i < adc->ch_num; i++)
//    {
//        if (adc->p_ch_param[i].gain != ADC_GAIN_LOW_DAT_3_3)
//        {
//            ADC_InitStruct.GAIN |= 1 << i; // 通道增益 高增益（1 倍）对应2.2V 和 低增益（2/3 倍）,对应3.3V，默认低增益
//        }
//    }

//    if (adc->p_dma_param->dma_ch != (DMA_RegTypeDef *)DMA_NULL) // 标志触发DMA和ADC中断无法同时存在
//    {
//        if (adc->continue_mode == ADC_CSMP_ENABLE)
//        {
//            ADC_InitStruct.RE = ADC_RE_SERR | ADC_RE_HERR; // ADC触发DMA使能 TODO:ADC_RE_SF1在连续模式下无法触发SF1和SF2中断标志
//        }
//        else
//        {
//            ADC_InitStruct.RE = ADC_RE_SF1 | ADC_RE_SF2;
//        }
//        ADC_InitStruct.IE = 0; // ADC中断使能
//    }
//    else
//    {
//        if (adc->adc_irq_type != ADC_IRQ_DISABLE)
//        {
//            ADC_InitStruct.RE = 0;
//            ADC_InitStruct.IE = adc->adc_irq_type; // ADC中断使能
//            NVIC_EnableIRQ(adc->nvic_irq);
//            NVIC_SetPriority(adc->nvic_irq, adc->priority);
//        }
//    }
//    ADC_InitStruct.Align = ADC_ALIGN_LEFT; // 采样数据对齐方式

//    ADC_InitStruct.TROVS = ADC_TROVS_ONES;     // 过采样触发模式
//    ADC_InitStruct.OVSR = adc->adc_ovsr;       // 过采率，过采样可以提高信噪比
//    ADC_InitStruct.CSMP = adc->continue_mode;  // 连续采样模式
//    ADC_InitStruct.TCNT = adc->adc_tcnt;       // 触发次数
//    ADC_InitStruct.S1 = adc->adc_s1;           // 第一段采样的通道数
//    ADC_InitStruct.S2 = adc->adc_s2;           // 第二段采样总通道数，两段采样通道数之和最大为16
//    ADC_InitStruct.NSMP = adc->adc_nsmp;       // ADC 触发模式 单段或双段
//    ADC_InitStruct.TRIG = adc->trigger_source; // MCPWM触发采样使能

//    ADC_InitStruct.GEN0 = ADC_GEN0_NONE; // 看门狗 0 通道使能
//    ADC_InitStruct.HTH0 = 0;             // 看门狗 0 上阈值
//    ADC_InitStruct.LTH0 = 0;             // 看门狗 0 下阈值
//    ADC_InitStruct.GEN1 = ADC_GEN1_NONE; // 看门狗 1 通道使能
//    ADC_InitStruct.HTH1 = 0;             // 看门狗 1 上阈值
//    ADC_InitStruct.LTH1 = 0;             // 看门狗 1 下阈值

//    ADC_Init(adc->adc_periph, &ADC_InitStruct);

//    for (uint16_t i = 0; i < adc->ch_num; i++)
//    {
//        ADC_SetPChanne(adc->adc_periph, i, adc->p_ch_param[i].p_adc_ch);
//        if (adc->p_ch_param[i].n_adc_ch != ADC_CHN_NULL)
//        {
//            ADC_SetNChanne(adc->adc_periph, i, adc->p_ch_param[i].n_adc_ch);
//        }
//    }
//}

/**
 * @brief 规则组ADC相关配置初始化
 *
 * @param [in] adc  ADC组参数指针
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_adc_init(P_BSP_ADC adc)
{
   #if PROGRAM_TYPE == PROGRAM_APL
   AdcCfg_Init();
   #endif
//    bsp_dma_config(adc);
//    bsp_adc_config(adc);
}

/**
 * @brief ADC和DMA反初始化
 *
 * @param [in] adc  ADC组参数指针
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_adc_deinit(P_BSP_ADC adc)
{
    SYS_SoftResetMODULE(adc->sys_module);
}

/**
 * @brief ADC软件触发采样
 *
 * @param [in] adc  ADC组参数指针
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_adc_set_start(P_BSP_ADC adc)
{
    // ADC_Trigger
    adc->adc_periph->SWT = 0x5aa5;
}

/**
 * @brief ADC读取单个通道值,DMA模式使用
 *
 * @param [in] adc         ADC组参数指针
 * @param [in] index       ADC通道的索引
 *
 * @return uint32_t
 *  @retval ADC通道采样值
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
int32_t bsp_adc_read_value(P_BSP_ADC adc, uint32_t index)
{
    // ADC_GetData
    return ((int32_t *)adc->adc_periph)[index];
    //  adc->p_dma_param->mem_addr[index];
}

/**
 * @brief ADC读所有通道数据
 *
 * @param [in] adc         ADC组参数指针
 * @param [in] p_adc_value ADC数据缓存指针
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
uint32_t bsp_adc_read_all_value(P_BSP_ADC adc, int16_t *p_adc_value, uint32_t buf_len)
{
    uint32_t len;
    len = X_FRAME_MIN(buf_len, adc->ch_num);
    for (uint32_t i = 0; i < len; i++)
    {
        *(p_adc_value + i) = ((int32_t *)adc->adc_periph)[i];
    }
    return 0;
}

/**
 * @brief ADC使能
 *
 * @param [in] adc    ADC组参数指针
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_adc_enable(P_BSP_ADC adc)
{
    if (adc->p_dma_param->dma_ch != (DMA_RegTypeDef *)DMA_NULL)
    {
        adc->p_dma_param->dma_ch->CTMS = adc->p_dma_param->mem_len;
        adc->p_dma_param->dma_ch->SADR = (uint32_t)(adc->adc_periph);
        adc->p_dma_param->dma_ch->DADR = (uint32_t)adc->p_dma_param->mem_addr;
        adc->p_dma_param->dma_ch->CCR = adc->p_dma_param->dma_ch->CCR | BIT0;
    }
    SYS_ModuleClockCmd(adc->sys_module, ENABLE);
    //	SYS_ModuleClockCmd(SYS_MODULE_ADC2, DISABLE);
    //    AFE_ModuleClockCmd(AFE_MODULE_ADC, ENABLE);
}

/**
 * @brief adc禁能
 *
 * @param [in] adc      adc组参数指针
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_adc_disable(P_BSP_ADC adc)
{
    SYS_ModuleClockCmd(adc->sys_module, DISABLE); // TODO:无法关闭ADC
                                                  //    AFE_ModuleClockCmd(AFE_MODULE_ADC, DISABLE);
    if (adc->p_dma_param->dma_ch != (DMA_RegTypeDef *)DMA_NULL)
    {
        adc->p_dma_param->dma_ch->CCR = adc->p_dma_param->dma_ch->CCR & ~BIT0; // 关通道TODO:无法关闭
    }
}

/**
 * @brief 修改dma接收的地址
 *
 * @param [in] adc              adc组参数指针
 * @param [in] merroy_address   内存地址
 * @param [in] num              传输个数，需为通道个数的整数倍
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_adc_change_memory_address(P_BSP_ADC adc, int16_t *memory_address, uint32_t num)
{
    // 传输个数必须为通道个数的整数倍
    if (num % adc->ch_num != 0 || adc->p_dma_param->dma_ch == (DMA_RegTypeDef *)DMA_NULL)
        return;
    SYS_ModuleClockCmd(adc->sys_module, DISABLE);
    adc->p_dma_param->dma_ch->CCR = adc->p_dma_param->dma_ch->CCR & ~BIT0; // 关通道

    adc->p_dma_param->mem_len = num;
    adc->p_dma_param->mem_addr = (int16_t *)memory_address;
    adc->p_dma_param->dma_ch->CTMS = adc->p_dma_param->mem_len;
    adc->p_dma_param->dma_ch->SADR = (uint32_t)(adc->adc_periph);
    adc->p_dma_param->dma_ch->DADR = (uint32_t)adc->p_dma_param->mem_addr;

    adc->p_dma_param->dma_ch->CCR = adc->p_dma_param->dma_ch->CCR | BIT0;
    SYS_ModuleClockCmd(adc->sys_module, ENABLE);
}

/**
 * @brief 注册ADC中断回调函数
 *
 * @param [in] adc      ADC参数组指针
 * @param [in] irq_cb   ADC中断回调函数指针
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_adc_reg_irq(P_BSP_ADC adc, void (*irq_cb)(void))
{
    adc->adc_cb = irq_cb;
}

/**
 * @brief ADC中断回调实体函数
 *
 * @param [in] adc  ADC参数组指针
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_adc_irp_handle(P_BSP_ADC adc)
{
    if (ADC_GetIRQFlag(adc->adc_periph, adc->adc_irq_type)) // adc->adc_irq_type为中断类型，对应寄存器IE和IF一致
    {
        if (adc->adc_cb != NULL)
        {
            adc->adc_cb();
        }
        ADC_ClearIRQFlag(adc->adc_periph, adc->adc_irq_type);
    }
}
