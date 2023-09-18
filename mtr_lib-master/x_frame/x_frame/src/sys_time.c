/********************** Copyright(c)***************************
** ECOFLOW
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               sys_time.c
** Created by:              Chen JunTong
** Created date:            2021-03-16
** Version:                 V1.0.0
** Descriptions:            系统时间
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2021-03-16
** Last version:            V1.0.0
** Description:             系统时间
**
*************************************************************/
#include "x_frame.h"


/*增加事件触发的回调函数*/
volatile uint32_t sys_tick_ms = 0;
volatile uint64_t sys_work_time = 0;



/*************************************************************
** Function name:       get_sys_ms
** Descriptions:        获取系统时间，单位：毫秒
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint32_t get_sys_ms(void)
{
    return sys_tick_ms;
}

/*************************************************************
** Function name:       get_sys_min
** Descriptions:        获取系统时间，单位：分钟
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint32_t get_sys_min(void)
{
    return sys_tick_ms / 1000 / 60;
}

/*************************************************************
** Function name:       get_sys_s
** Descriptions:        获取系统时间，单位：秒
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint32_t get_sys_s(void)
{
    return sys_tick_ms / 1000;
}

/*************************************************************
** Function name:       get_sys_work_time_min
** Descriptions:        获取系统工作时长,单位 ms
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint64_t get_sys_work_time(void)
{
	return sys_work_time;
}

/*************************************************************
** Function name:       block_ms
** Descriptions:        阻塞一定时间
** Input parameters:    ms：阻塞的时间
** Returned value:      None
** Remarks:             None
*************************************************************/
void block_ms(uint32_t ms)
{
    uint32_t cur_ms = sys_tick_ms;
    while((sys_tick_ms - cur_ms) < ms);
}

/*************************************************************
** Function name:       check_time_out
** Descriptions:        检查是否超时
** Input parameters:    start_time：开始时间
**                      time_out：超时时间
** Returned value:      true 超时，false 未超时
** Remarks:             None
*************************************************************/
bool check_time_out(uint32_t start_time, uint32_t time_out)
{
    if((sys_tick_ms - start_time) >= time_out) {
        return true;
    }
    return false;
}

/*************************************************************
** Function name:       get_elapse_time
** Descriptions:        获取经过时间
** Input parameters:    start_time：开始时间
** Returned value:      经过时间
** Remarks:             None
*************************************************************/
uint32_t get_elapse_time(uint32_t start_time)
{
    return sys_tick_ms - start_time;
}



