/**
 * @brief
 * @file bsp_gpio.h
 * @version 1.0
 * @author yaqi.wang
 * @date 2023-01-12 15:59:40
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 *
 * @details 文件详细描述，包含使用场景，依赖项等
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2023-01-12 15:59:40      <td>yaqi.wang      <td>创建文件
 */
#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "lks32mc45x.h"
#include "lks32mc45x_lib.h"
#include "xh_io.h"

#define GPIO_IN_MODE    0x00    ///< 数字输入模式
#define GPIO_OUT_MODE   0x01    ///< 数字输出模式
#define GPIO_ANA_MODE   0x02    ///< 模拟输入模式
#define GPIO_INOUT_MODE 0x03    ///< 双向模式，通常需要配置为开漏

    
typedef enum {RESET = 0, SET = !RESET} FlagStatus;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrStatus;

    typedef struct _BSP_GPIO_PARAM
    {
        GPIO_TypeDef* port;
        uint16_t pin;
        GPIO_Mode_TypeDef mode;
        GPIO_PuPd_TypeDef default_level;
    } BSP_GPIO_PARAM, *P_BSP_GPIO_PARAM;

    typedef struct _BSP_GPIO_GROUP
    {
        P_BSP_GPIO_PARAM p_gpio_param;
        uint16_t gpio_num;
    } BSP_GPIO_GROUP, *P_BSP_GPIO_GROUP;

#define REG_XH_BSP_IO_GROUP(dev_id, prio, gpio_param...)                                                         \
    BSP_GPIO_PARAM g_bsp_gpio_param##dev_id[] = {gpio_param};                                                    \
    BSP_GPIO_GROUP _##dev_id = {                                                                                 \
        g_bsp_gpio_param##dev_id,                                                                                \
        sizeof(g_bsp_gpio_param##dev_id) / sizeof(g_bsp_gpio_param##dev_id[0]),                                  \
    };                                                                                                           \
    REG_XH_GPIO_DRV(dev_id, &_##dev_id, prio, DEV_AUTO_INIT, bsp_gpio_init, NULL, bsp_gpio_read, bsp_gpio_write, \
                    bsp_gpio_change_mode);

    extern void bsp_gpio_init(P_BSP_GPIO_GROUP p_gpio_group);
    extern void bsp_gpio_write(P_BSP_GPIO_GROUP p_gpio_group, uint16_t ch, uint32_t bit_value);
    extern uint32_t bsp_gpio_read(P_BSP_GPIO_GROUP p_gpio_group, uint16_t ch);
    extern void bsp_gpio_change_mode(P_BSP_GPIO_GROUP p_gpio_group, uint16_t ch, uint32_t mode);

#ifdef __cplusplus
}
#endif
#endif /* bsp_gpio_h */
