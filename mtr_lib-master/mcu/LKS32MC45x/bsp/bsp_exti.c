/**
 * @brief
 * @file bsp_exti.c
 * @version 1.0
 * @author yaqi.wang
 * @date 2023-01-16 14:27:20
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 *
 * @details 文件详细描述，包含使用场景，依赖项等
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2023-01-16 14:27:20      <td>yaqi.wang      <td>创建文件
 */
#include "bsp_exti.h"

/**
 * @brief 获取中断源
 * 
 * @param [in] p_param_exti  
 * 
 * @details 特殊说明: 
 * @par eg:
 * @code
 *    
 * @endcode
 */
static void _get_exti_pin_source(P_BSP_EXTI p_param_exti)
{
    uint8_t pin_source = 0;
    uint32_t pin = p_param_exti->exti_source->pin;
    while( pin > 1)
    {
        pin >>= 1;
        pin_source ++;
    }
    
    p_param_exti->exti_source->pin_source = pin_source;
}

/**
 * @brief 事件中断配置，初始化
 * 
 * 
 * @details 特殊说明: exti_init_cnt记录初始化函数运行此时
 * 仅在第一次运行时调用软件复位函数
 * @par eg:
 * @code
 *    
 * @endcode
 */
static uint8_t exti_init_cnt = 0;
void bsp_exti_gpio_init(P_BSP_EXTI p_param_exti)
{
    exti_init_cnt ++ ;
    GPIO_InitTypeDef GPIO_InitStruct;
    if(exti_init_cnt == 1)
    {
        SYS_SoftResetMODULE(SYS_MODULE_GPIO);
    }
    _get_exti_pin_source(p_param_exti);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; // GPIO 输入模式
    GPIO_InitStruct.GPIO_Pin = p_param_exti->exti_source->pin;
    if (p_param_exti->trig_mode == GPIO_EXTI_MODE_EDGE)
        GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    else if (p_param_exti->trig_mode == GPIO_EXTI_MODE_NEGEDGE)
        GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    else if (p_param_exti->trig_mode == GPIO_EXTI_MODE_POSEDGE)
        GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(p_param_exti->exti_source->port, &GPIO_InitStruct);

    if(p_param_exti->exti_source->pin_source < 8)//中断模式使能
    {
        p_param_exti->exti_prot->CR0 &= ~(p_param_exti->trig_mode << (p_param_exti->exti_source->pin_source) * 2);
        p_param_exti->exti_prot->CR0 |= (p_param_exti->trig_mode << (p_param_exti->exti_source->pin_source) * 2);
    }
    else
    {
        p_param_exti->exti_prot->CR1 &= ~(p_param_exti->trig_mode << ((p_param_exti->exti_source->pin_source - 8) * 2));
        p_param_exti->exti_prot->CR1 |= (p_param_exti->trig_mode << ((p_param_exti->exti_source->pin_source - 8) * 2));
    }

    GPIO_ClearIRQFlag(p_param_exti->exti_source->port, p_param_exti->exti_source->pin);
    NVIC_EnableIRQ(EXTI_IRQn); //使能 EXTI 中断
    NVIC_SetPriority(EXTI_IRQn, 2);
}

/**
 * @brief 中断服务函数
 * 
 * @param [in] p_param_exti  
 * 
 * @details 特殊说明: 
 * @par eg:
 * @code
 *    
 * @endcode
 */
void exti_irq_action(P_BSP_EXTI p_param_exti)
{
    if(GPIO_GetIRQFlag(p_param_exti->exti_source->port,p_param_exti->exti_source->pin_source) == 1)
    {
        if(p_param_exti->exti_cb!=NULL){
            p_param_exti->exti_cb();
        }   
        GPIO_ClearIRQFlag(p_param_exti->exti_source->port,p_param_exti->exti_source->pin_source);
    }
}

/**
 * @brief 中断回调 函数指针
 *
 * @param [in] p_param_exti
 * @param [in] pDest
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_exti_reg_irq(P_BSP_EXTI p_param_exti, uint8_t *pDest)
{
    p_param_exti->exti_cb = (irq_cb)pDest;
}
