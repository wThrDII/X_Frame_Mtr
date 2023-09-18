/********************** Copyright(c)***************************
** ECOFLOW
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               bsp_sys.h
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
#ifndef bsp_sys_h
#define bsp_sys_h
#ifdef __cplusplus
extern "C" {
#endif
#include "x_frame.h"
#include "lks32mc45x.h"
#include "xh_sys.h"
#include "system_lks32mc45x.h"

typedef struct _bsp_sys_obj
{
    uint32_t vector_tab_offset;
}bsp_sys_obj,*p_bsp_sys_obj;

/*************************************************************
** Function name:       REG_XH_BSP_SYS
** Descriptions:        定义系统中断处理函数
** Input parameters:    handle_action：需要执行的中断动作，不定长
** Returned value:      None
** Remarks:             None
*************************************************************/
#define REG_XH_BSP_SYS(dev_id,prio,vector_tab_offset,irq_act...)             \
        bsp_sys_obj bsp_sys_##dev_id;                                   \
        void bsp_sys_init(p_bsp_sys_obj p_bsp_sys)                      \
        {                                                               \
            bsp_sys_init_offset(vector_tab_offset);                     \
        }                                                               \
        void SysTick_Handler(void)                                      \
        {                                                               \
            irq_act                             	                    \
        }                                                               \
        REG_XH_SYS_DRV(dev_id,&bsp_sys_##dev_id,prio,DEV_AUTO_INIT,bsp_sys_init,NULL,bsp_sys_get_cpuid,bsp_sys_reset,bsp_sys_jump_to)


extern void bsp_sys_init_offset(uint32_t offset);
extern void bsp_sys_reset(void* var);
extern uint16_t bsp_sys_get_cpuid( void *var ,uint8_t *pbuf, uint32_t length);
extern void bsp_sys_jump_to(uint32_t address);

#ifdef __cplusplus
}
#endif
#endif /* bsp_sys_h */
