/********************** Copyright(c)***************************
** ECOFLOW
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               bsp_sys.c
** Created by:              Chen JunTong
** Created date:            2021-03-23
** Version:                 V1.0.0
** Descriptions:            系统时间
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2021-03-23
** Last version:            V1.0.0
** Description:             系统时间
**
*************************************************************/
#include "bsp_sys.h"
#include "stddef.h"

#define BSP_CPUID_LENGTH 12

/*
M0 : 0x1ffff7AC
M3 : 0x1ffff7e8
M4 : 0x1fff7a10
*/
#define CPUID_START_ADDR         0x1fff7a10



/*************************************************************
** Function name:       bsp_sys_tick_init
** Descriptions:        系统时钟初始化
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void bsp_systick_init(void)
{
    if(SysTick_Config(SystemCoreClock / 1000)) {
        /* Capture error */
        while(1);
    }
//    /*JTAG-DP除能，SW-DP使能*/
//    rcu_periph_clock_enable(RCU_AF);
//    gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);

    NVIC_SetPriority(SysTick_IRQn, 0x00U);
//    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
}

/*************************************************************
** Function name:       bsp_sys_init
** Descriptions:        系统复位
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
void bsp_sys_init_offset(uint32_t offset)
{
//    nvic_vector_table_set(NVIC_VECTTAB_FLASH,offset);
    bsp_systick_init();
}


/*************************************************************
** Function name:       sys_reset
** Descriptions:        系统复位
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
void bsp_sys_reset(void *var)
{
    __disable_irq();
    NVIC_SystemReset();
}

/*************************************************************
** Function name:       bsp_sys_get_cpuid
** Descriptions:        获取cpuid
** Input parameters:    pbuf:缓存指针
**                      length：缓存长度
** Returned value:      返回实际获取的长度
** Remarks:             None
*************************************************************/
uint16_t bsp_sys_get_cpuid(void *var,uint8_t *pbuf, uint32_t length)
{
    uint8_t i = 0;
    if(pbuf == NULL) {
        return 0;
    }
    length = length > BSP_CPUID_LENGTH ? BSP_CPUID_LENGTH : length;
    for(i = 0; i < length; i++) {
        pbuf[i] = *(volatile uint8_t *)(CPUID_START_ADDR + i);
    }
    return length;
}

void bsp_sys_jump_to(uint32_t address)
{
    typedef void (*_func)(void);

    /* MCU peripherals re-initial. */
    xh_core_deinit();

    __disable_irq();

    /* reset systick */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

    /* disable and clean up all interrupts. */
    {
    int i;
    for (i = 0; i < 8; i++)
    {
        /* disable interrupts. */
        NVIC->ICER[i] = 0xFFFFFFFF;

        /* clean up interrupts flags. */
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    /* Set new vector table pointer */
    SCB->VTOR = address;

    /* reset register values */
    //__set_BASEPRI(0);
    __set_FAULTMASK(0);

    /* set up MSP and switch to it */
    //__set_PSP(*(uint32_t*)address);
    //__set_CONTROL(0);
    __set_MSP(*(uint32_t *)address);

    /* ensure what we have done could take effect */
    //__ISB();
    }

    /* never return */
    ((_func)(*(uint32_t *)(address + 4)))();
}
