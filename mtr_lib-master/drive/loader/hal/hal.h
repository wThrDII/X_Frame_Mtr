/********************** Copyright(c)***************************
** ECOFLOW
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               hal.h
** Created by:              Chen JunTong
** Created date:            2022-03-01
** Version:                 V1.0.0
** Descriptions:            接口文档
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2022-03-01
** Last version:            V1.0.0
** Description:             接口文档
**
*************************************************************/
#ifndef hal_h
#define hal_h
#ifdef __cplusplus
extern "C" {
#endif
#include "chip_cfg.h"
// #include "cmsis_armclang.h"
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

#include "xh_hal.h"
#include "xh_io.h"
#include "xh_sys.h"
#include "xh_fwdgt.h"
#include "xh_stream.h"
#include "xh_flash.h"
#include "xh_iic.h"
#include "xh_i2c.h"
//#include "xh_spi_dma.h"
#include "xh_adc.h"
#include "xh_dac.h"
#include "xh_pwm.h"
#include "xh_mcpwm.h"
#include "xh_time_cnt.h"
#include "xh_can.h"
#include "xh_can_s.h"
#include "xh_exti.h"

#define BSP_COMMIT_ID "082d7870186ef8f58d2b5bdd361733760f16a691"
#define BSP_SRF_VER "1.0.0.27"

EXT_XH_SYS_DRV( DRV_SYSTEM );
EXT_XH_WDG_DRV( DRV_FWDGT );
EXT_XH_FLASH_DRV( DRV_FLASH );
EXT_XH_STREAM_DRV( DRV_UART0 ); //xh_stream.h | TX:GPIO2 GPIO_PinSource_5 | RX:GPIO2 GPIO_PinSource_6 | 115200
EXT_XH_CAN_DRV ( DRV_CAN);
EXT_XH_CAN_S_DRV ( DRV_CAN_S);
    





#ifndef BSP_MCPWM_CH_0
#define BSP_MCPWM_CH_0 0
#endif
#ifndef BSP_MCPWM_CH_1
#define BSP_MCPWM_CH_1 1
#endif
#ifndef BSP_MCPWM_CH_2
#define BSP_MCPWM_CH_2 2
#endif
#ifndef BSP_MCPWM_CH_3
#define BSP_MCPWM_CH_3 3
#endif




#undef BSP_MCPWM_CH_0
#undef BSP_MCPWM_CH_1
#undef BSP_MCPWM_CH_2
#undef BSP_MCPWM_CH_3

/**@warning   在.c文件中加入以下内容
*********************************************
#include "chip_cfg.h"
#define WATCHDOG_ENABLE	       (0)	   /// 看门狗开关  1：开启 0：关闭
uint32_t g_vect_tab_offset = VECTOR_TAB_OFFSET;
bool g_bsp_fwdgt_enable = WATCHDOG_ENABLE;
*********************************************
 */

#ifdef __cplusplus
}
#endif
#endif /* hal_h */



