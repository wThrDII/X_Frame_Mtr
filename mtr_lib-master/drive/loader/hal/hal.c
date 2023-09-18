#include "hal.h"
#include "bsp_gpio.h"
#include "bsp_sys.h"
#include "bsp_uart.h"
#include "bsp_flash.h"
#include "bsp_fwdgt.h"
#include "bsp_can.h"
#include "can_s.h"
#include "x_frame.h"
/*----------------------------------------------------------------------*/

///<驱动初始化优先级表
enum{
    PEI_DRV_SYSTEM,
    PEI_DRV_FWDGT,
    PEI_DRV_FLASH,
    PEI_DRV_UART0,
    PEI_DRV_CAN,
    PEI_DRV_CAN_S,
    PEI_DRV_HAL,
    PEI_DRV_MAX,
};

/* SYSTEM */
__attribute__((weak)) uint32_t g_vect_tab_offset = 0;
REG_XH_BSP_SYS(DRV_SYSTEM,PEI_DRV_SYSTEM,g_vect_tab_offset,RUN_XF_SYS_TIME(1))
DEF_XF_XH_SYS_ID(DRV_SYSTEM)
/* FLSAH */
REG_XH_BSP_FLASH(DRV_FLASH,PEI_DRV_FLASH)

/* FWDG */
bool g_bsp_fwdgt_enable = WATCHDOG_ENABLE;
REG_XH_FWDG(DRV_FWDGT,PEI_DRV_FWDGT,3000)
DEF_XF_XH_IWDG_ID(DRV_FWDGT)

/* VIR_LINK*/

/* UART */
REG_XH_BSP_UART(DRV_UART0,PEI_DRV_UART0,UART0,0,GPIO4,GPIO_PinSource_1,GPIO4,GPIO_PinSource_2,921600,USART_WL_8BIT,USART_STB_1BIT,USART_PM_EVEN,512,512,NULL,NULL)
/* BQ79616 */


/* GPIO */

/* EXTI */
/* CAN */
REG_BSP_CAN_DRV(DRV_CAN,PEI_DRV_CAN,CAN,GPIO2,GPIO_Pin_4,GPIO_PinSource_4,GPIO2,GPIO_Pin_3,GPIO_PinSource_3,0,0,500000,100)
REG_CAN_S_DRV(DRV_CAN_S,PEI_DRV_CAN_S,DRV_SYSTEM,DRV_CAN,false,HOST_ADDR,100,200)
/* ADC REGURE */

/* ADC */

/* ADC INSERTED */

/* DAC INSERTED */

/* MCPWM FAIL*/

/* MCPWM */

/* PWM */

/* TIMER_FREQ */

/* RTC */

/* I2C */

/* IIC */

/* I2C SIM*/


/* SPI */

/* SPI_DMA */

/* SPI_FLASH */

/* 外部中断事件函数 */
void EXTI_IRQHandler(void)
{
}



