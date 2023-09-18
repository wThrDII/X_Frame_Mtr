/********************** Copyright(c)***************************
** ECOFLOW 
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** Created by:              Xiao ShuLong
** Created date:            2022-04-12
** Version:                 V1.0.0
** Descriptions:            接口弱定义
**
**------File Info----------------------------------------------
** File name:               hal_api_week.c
** Latest modified date:    2022-04-12
** Latest version:          V1.0.0
** Description:             接口弱定义
**
*************************************************************/
#define DEF_DBG_I
#define DEF_DBG_W
#define DEF_DBG_E
#include "x_frame.h"

#define DBG_FUNC_WEEK_ERR_INFO()  DBG_E_LIMIT(1000, "please achieve func!!!")

X_FRAME_WEAK void hal_bsp_init(void)
{
}

/*************************************************************
    SYS
*************************************************************/

X_FRAME_WEAK void sys_reset(void)
{
    DBG_FUNC_WEEK_ERR_INFO();
}

X_FRAME_WEAK void sys_jump_addr(uint32_t addr)
{
    DBG_FUNC_WEEK_ERR_INFO();
}

X_FRAME_WEAK uint32_t sys_get_cpuid(uint8_t *p_data, uint32_t len)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}
X_FRAME_WEAK uint32_t sys_get_bsp_ver(void)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}
/*************************************************************
    WATCHDOG
*************************************************************/

X_FRAME_WEAK void watchdog_feed(uint32_t watchdog_id)
{
    DBG_FUNC_WEEK_ERR_INFO();
}

X_FRAME_WEAK bool watchdog_get_reset_sta(uint32_t watchdog_id)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}
/*************************************************************
    IRQ
*************************************************************/
X_FRAME_WEAK void irq_reg_cb_init(uint32_t irq_id,void (*cb)(void))
{
    DBG_FUNC_WEEK_ERR_INFO();
}
X_FRAME_WEAK void irq_set_freq(uint32_t irq_id,uint32_t freq)
{
    DBG_FUNC_WEEK_ERR_INFO();
}
/*************************************************************
    IO
*************************************************************/

X_FRAME_WEAK void io_set_sta(uint32_t io_id,bool sta)
{
    DBG_FUNC_WEEK_ERR_INFO();
}

X_FRAME_WEAK void io_action_cfg(uint32_t io_id,uint16_t repeat_num,bool fir_sta,char *p_action)
{
    DBG_FUNC_WEEK_ERR_INFO();
}

X_FRAME_WEAK bool io_get_sta(uint32_t io_id)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}

/*************************************************************
    CHA 
*************************************************************/

X_FRAME_WEAK uint32_t stream_write(uint32_t cha_id,uint8_t *p_data,uint32_t len)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}

X_FRAME_WEAK uint32_t stream_read(uint32_t cha_id,uint8_t *p_data,uint32_t len)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}

/*************************************************************
    FLASH
*************************************************************/

X_FRAME_WEAK bool flash_erase(uint32_t flash_id,uint32_t addr,uint32_t len)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}

X_FRAME_WEAK bool flash_write(uint32_t flash_id,uint32_t addr,uint8_t *p_data,uint32_t len)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}

X_FRAME_WEAK bool flash_read(uint32_t flash_id,uint32_t addr,uint8_t *p_data,uint32_t len)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}

/*************************************************************
    AD
*************************************************************/

X_FRAME_WEAK uint16_t ad_read_raw_val(uint32_t dev_id)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}

X_FRAME_WEAK void ad_read_multi_val(uint16_t *p_buf , uint32_t start , uint8_t num)
{
    DBG_FUNC_WEEK_ERR_INFO();
}

X_FRAME_WEAK void ad_set_regular_dma_init(uint16_t *buf)
{
    DBG_FUNC_WEEK_ERR_INFO();
}
/*************************************************************
    TEMP
*************************************************************/

X_FRAME_WEAK int32_t temp_get_val(uint32_t temp_id)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}

/*************************************************************
    AC
*************************************************************/

X_FRAME_WEAK float ac_get_vol_val(uint32_t vol_id)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}

X_FRAME_WEAK float ac_get_cur_val(uint32_t cur_id)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}

X_FRAME_WEAK float ac_get_s_watt_val(uint32_t watt_id)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}

X_FRAME_WEAK float ac_get_p_watt_val(uint32_t watt_id)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}

X_FRAME_WEAK float ac_get_q_watt_val(uint32_t watt_id)
{
    DBG_FUNC_WEEK_ERR_INFO();
    return 0;
}


/*************************************************************
    PWM
*************************************************************/


X_FRAME_WEAK void pwm_set_duty_cycle(uint32_t pwm_id,uint16_t duty_cycle)
{
    DBG_FUNC_WEEK_ERR_INFO();
}


X_FRAME_WEAK void pwm_timer_en(uint32_t pwm_id,uint8_t en)
{
    DBG_FUNC_WEEK_ERR_INFO();
}

/*************************************************************
    BUCK BOOST PWM
*************************************************************/

X_FRAME_WEAK void buck_boost_set_dutycycle(uint32_t buck_boost_id,uint16_t duty_cycle)
{
    DBG_FUNC_WEEK_ERR_INFO();
}

X_FRAME_WEAK void buck_boost_set_deadtime(float deadtime)
{
    DBG_FUNC_WEEK_ERR_INFO();
}

X_FRAME_WEAK void buck_boost_timer_en(uint8_t en)
{
    DBG_FUNC_WEEK_ERR_INFO();
}

X_FRAME_WEAK void buck_boost_set_ch_sta(uint32_t buck_boost_id,uint32_t state)
{
    DBG_FUNC_WEEK_ERR_INFO();
}
