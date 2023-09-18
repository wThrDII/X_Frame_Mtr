#include "hal.h"
#include "bsp_gpio.h"
#include "bsp_sys.h"
#include "bsp_uart.h"
#include "bsp_adc.h"
#include "bsp_mcpwm.h"
#include "bsp_flash.h"
#include "bsp_can.h"
#include "can_s.h"
#include "x_frame.h"
/*----------------------------------------------------------------------*/

///<驱动初始化优先级表
enum{
    PEI_DRV_SYSTEM,
    PEI_DRV_FLASH,
    PEI_DRV_ADC,
    PEI_DRV_GPIO,
    PEI_DRV_UART0,
    PEI_DRV_MCPWM,
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
REG_FLASH_PART(DRV_FLASH, FLASH_ADDR, FLASH_SIZE, FLASH_BLOCK_SIZE,
                {"loader",   SYS_LOADER_PART,     SYS_LOADER_PART_SIZE  },
                {"app",      SYS_APP_PART,        SYS_APP_PART_SIZE     },
                {"boot_cfg", SYS_BOOT_CFG_PART,   SYS_BOOT_CFG_PART_SIZE},
                {"rec",      USER_REC_PART,       USER_REC_PART_SIZE    },
                {"app_cfg",  SYS_APP_CFG_PART,    SYS_APP_CFG_PART_SIZE },
                {"product",  SYS_PRODUCT_PART,    SYS_PRODUCT_PART_SIZE },
                {"hw_ver",   SYS_HW_VER_PART,     SYS_HW_VER_PART_SIZE  },
//                {"bl_back", SYS_BACKUP_PART, SYS_APP_PART_SIZE},
                );
/* FWDG */

/* VIR_LINK*/

/* UART */
REG_XH_BSP_UART(DRV_UART0,PEI_DRV_UART0,UART0,0,GPIO4,GPIO_PinSource_1,GPIO4,GPIO_PinSource_2,921600,USART_WL_8BIT,USART_STB_1BIT,USART_PM_EVEN,512,512,NULL,NULL)

/* BQ79616 */


/* GPIO */
REG_XH_BSP_IO_GROUP(DRV_GPIO,PEI_DRV_GPIO,
    {GPIO4, GPIO_PinSource_4, GPIO_Mode_OUT, GPIO_PuPd_NOPULL},  //LED1->RED
    {GPIO4, GPIO_PinSource_3, GPIO_Mode_OUT, GPIO_PuPd_NOPULL},  //LED2->BLUE
)
/* EXTI */
/* CAN */
REG_BSP_CAN_DRV(DRV_CAN,PEI_DRV_CAN,CAN,GPIO4,GPIO_Pin_4,GPIO_PinSource_4,GPIO4,GPIO_Pin_3,GPIO_PinSource_3,0,0,500000,100)
//REG_CAN_S_DRV(DRV_CAN_S,PEI_DRV_CAN_S,DRV_SYSTEM,DRV_CAN,false,HOST_ADDR,100,200)

/* ADC REGURE */


/* ADC */
REG_XH_ADC(DRV_ADC, PEI_DRV_ADC);

/* ADC INSERTED */

/* DAC INSERTED */

/* MCPWM FAIL*/

/* MCPWM */
REG_XH_MCPWM(DRV_MCPWM,PEI_DRV_MCPWM,MCPWM0)
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



