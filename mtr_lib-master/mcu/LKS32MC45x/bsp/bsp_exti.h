/**
 * @brief
 * @file bsp_exti.h
 * @version 1.0
 * @author yaqi.wang
 * @date 2023-01-16 14:27:53
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 *
 * @details 文件详细描述，包含使用场景，依赖项等
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2023-01-16 14:27:53      <td>yaqi.wang      <td>创建文件
 */

#ifndef __BSP_EXTI_H__
#define __BSP_EXTI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "lks32mc45x_lib.h"
#include "x_frame.h"
#include "xh_exti.h"

typedef void(*irq_cb)(void);

//#define GPIO_EXTI_MODE_NEGEDGE 1 ///< 下降沿触发
//#define GPIO_EXTI_MODE_POSEDGE 2 ///< 上升沿触发
//#define GPIO_EXTI_MODE_EDGE 3    ///< 上升沿、下降沿均触发

typedef enum
{
    EXTI_TRIG_FALLING = 1,  ///< 下降沿触发
    EXTI_TRIG_RISING = 2,  ///< 上升沿触发
    EXTI_TRIG_BOTH = 3,     ///< 上升沿、下降沿均触发
}GPIO_EXTI_TRIG_MODE;

    typedef struct _BSP_EXTI_GPIO_SOURCE
    {
        GPIO_TypeDef *port;
        uint16_t pin;
        uint8_t port_source;
        uint8_t pin_source;
    } BSP_EXTI_GPIO_SOURCE, *P_BSP_EXTI_GPIO_SOURCE;

    typedef struct _BSP_EXTI
    {
        P_BSP_EXTI_GPIO_SOURCE exti_source;
        IRQn_Type nvic_irq;
        EXTI_TypeDef* exti_prot;
        GPIO_EXTI_TRIG_MODE trig_mode;
        void (*exti_cb)(void);
    } BSP_EXTI, *P_BSP_EXTI;

/**
 * @brief
 *
 *
 * @details 特殊说明:
 *  Function name:          DEF_BSP_EXTI
 *  Input parameters:       dev_id: 驱动id
 *                          port：GPIO 端口
 *                          pin：GPIO 引脚
 *                          vic_irq：中断通道
 *                          trig_mode：触发模式
 *                          exti_cb：中断回调函数
 * @par eg:
 * @code
 *
 * @endcode
 */
#define REG_XH_EXTI(dev_id, prio, port, pin, exti, trig_mode, exti_cb)         \
    BSP_EXTI_GPIO_SOURCE bsp_exti_gpio_source_##dev_id = {port, pin, 0}; \
    BSP_EXTI _##dev_id = {                                                                                 \
        &bsp_exti_gpio_source_##dev_id,                                                                    \
        EXTI_IRQn,                                                                                   \
        exti,                                                                               \
        trig_mode,                                                                                         \
        exti_cb,                                                                                           \
    };                                                                                                     \
    REG_XH_EXTI_DRV(dev_id, &_##dev_id, prio, DEV_AUTO_INIT, bsp_exti_gpio_init, NULL, bsp_exti_reg_irq)
    
    extern void bsp_exti_gpio_init(P_BSP_EXTI p_param_exti);
    extern void exti_irq_action(P_BSP_EXTI exti);
    extern void bsp_exti_reg_irq(P_BSP_EXTI exti, uint8_t *pDest);
#ifdef __cplusplus
}
#endif

#endif //__BSP_EXTI_H__
