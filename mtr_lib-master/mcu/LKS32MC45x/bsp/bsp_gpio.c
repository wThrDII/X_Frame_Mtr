/**
 * @brief
 * @file bsp_gpio.c
 * @version 1.0
 * @author yaqi.wang
 * @date 2023-01-12 15:55:19
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 *
 * @details 文件详细描述，包含使用场景，依赖项等
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2023-01-12 15:55:19      <td>yaqi.wang      <td>创建文件
 */

#include "bsp_gpio.h"

/**
 * @brief gpio初始化
 *
 * @param [in] p_gpio_group
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_gpio_init(P_BSP_GPIO_GROUP p_gpio_group)
{
    uint32_t i = 0;
    for (i = 0; i < p_gpio_group->gpio_num; i++)
    {
        GPIO_Config(p_gpio_group->p_gpio_param[i].port, p_gpio_group->p_gpio_param[i].pin,
                    p_gpio_group->p_gpio_param[i].mode, GPIO_AF_GPIO);
        if(p_gpio_group->p_gpio_param[i].mode == GPIO_Mode_IN)
        {
            p_gpio_group->p_gpio_param[i].default_level = GPIO_PuPd_DOWN;
        }
    }
}
/**
 * @brief 输出高低电平
 *
 * @param [in] p_gpio_group
 * @param [in] ch
 * @param [in] bit_value
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_gpio_write(P_BSP_GPIO_GROUP p_gpio_group, uint16_t ch, uint32_t bit_value)
{
    BSP_GPIO_PARAM *p_gpio = &p_gpio_group->p_gpio_param[ch];
    if(RESET != bit_value)
        GPIO_SetPinBits(p_gpio->port,p_gpio->pin);
    else
        GPIO_ResetPinBits(p_gpio->port,p_gpio->pin);
}

/**
 * @brief 获取电平信号
 *
 * @param [in] p_gpio_group
 * @param [in] ch
 *
 * @return uint32_t
 *  @retval
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
uint32_t bsp_gpio_read(P_BSP_GPIO_GROUP p_gpio_group, uint16_t ch)
{
    BSP_GPIO_PARAM *p_gpio = &p_gpio_group->p_gpio_param[ch];
    if((uint32_t)RESET != GPIO_ReadPinBit(p_gpio->port,p_gpio->pin))
        return SET;
    else
        return RESET;
}

/**
 * @brief 修改GPIO模式
 *
 * @param [in] p_gpio_group
 * @param [in] ch
 * @param [in] mode
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
void bsp_gpio_change_mode(P_BSP_GPIO_GROUP p_gpio_group, uint16_t ch, uint32_t mode)
{
    BSP_GPIO_PARAM *p_gpio = &p_gpio_group->p_gpio_param[ch];
    switch (mode)
    {
        case XH_GPIO_MODE_AIN: 
            p_gpio->mode = GPIO_Mode_ANA; 
            p_gpio->default_level = GPIO_PuPd_NOPULL;
        break;

        case XH_GPIO_MODE_IN_FLOATING: 
            p_gpio->mode = GPIO_Mode_IN; 
            p_gpio->default_level = GPIO_PuPd_NOPULL;
        break;

        case XH_GPIO_MODE_IPD: 
            p_gpio->mode = GPIO_Mode_IN;
            p_gpio->default_level = GPIO_PuPd_DOWN;
        break;

        case XH_GPIO_MODE_IPU: 
            p_gpio->mode = GPIO_Mode_IN;
            p_gpio->default_level = GPIO_PuPd_UP;
        break;

        case XH_GPIO_MODE_OUT_OD: 
            p_gpio->mode = GPIO_Mode_INOUT;
            p_gpio->default_level = GPIO_PuPd_NOPULL;
        break;

        case XH_GPIO_MODE_OUT_PP: 
            p_gpio->mode = GPIO_Mode_OUT;
            p_gpio->default_level = GPIO_PuPd_NOPULL;
        break;

        default:
            break;
    }
    if(mode == XH_GPIO_MODE_AIN)
    GPIO_Config(p_gpio->port,p_gpio->pin,
                    p_gpio->mode, GPIO_AF_ANA);
    else
    GPIO_Config(p_gpio->port,p_gpio->pin,
                    p_gpio->mode, GPIO_AF_GPIO);
        
}
