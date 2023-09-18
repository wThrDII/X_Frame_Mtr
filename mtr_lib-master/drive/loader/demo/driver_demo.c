/**
 * @brief 驱动demo工程
 * @file driver_demo.c
 * @version 1.0
 * @author juntong.chen
 * @date 2022-11-14 15:06:42
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @details 文件详细描述，包含使用场景，依赖项等
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2022-11-14 15:06:42      <td>juntong.chen      <td>创建文件
 */
#include "run_demo.h"
#include "x_frame.h"
#include "hal.h"


#if ENABLE_LED_RUN ==  1
void led_run_task(void)
{
	static uint8_t led_sta = 0;
	led_sta = !led_sta;
    xh_io_write(DRV_GPIO, LED, led_sta);    ///< 设置led灯状态
}
REG_TASK(VH_TASK, 100, led_run_task)
#endif




