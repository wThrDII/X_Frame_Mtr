/********************** Copyright(c)***************************
** ECOFLOW
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** Created by:              Chen JunTong
** Created date:            2021-03-26
** Version:                 V1.0.0
** Descriptions:            时钟库
**
**------File Info----------------------------------------------
** File name:               rtc_hal.c
** Latest modified date:    2021-03-26
** Latest version:          V1.0.0
** Description:             时钟库
**
*************************************************************/


#include "x_frame.h"
#include "time.h"
#include "xh_rtc.h"

#define RTC_MIN_SEC             (60)
#define RTC_HOUR_SEC            (60*RTC_MIN_SEC)
#define RTC_DAY_SEC(time)     	(24*RTC_HOUR_SEC)


X_FRAME_WEAK X_CALENDAR_OBJ g_x_calendar_obj = {.drv_rtc = NULL};

/*************************************************************
** Descriptions:        时钟初始化
*************************************************************/
static void _calendar_init(void)
{

    g_x_calendar_obj.zone = xh_rtc_get_zone(g_x_calendar_obj.drv_rtc);

}

REG_INIT_MOD_BASIC(_calendar_init);


/*************************************************************
** Descriptions:        日期转换成秒数
*************************************************************/
uint32_t real_calendar_date_to_sec(P_X_CALENDAR_DATE p_date)
{
    struct tm tm_tim = {0};
    tm_tim.tm_year = p_date->year - 1900;
    tm_tim.tm_mon = p_date->month - 1;
    tm_tim.tm_mday = p_date->day;
    tm_tim.tm_hour = p_date->hour;
    tm_tim.tm_min = p_date->min;
    tm_tim.tm_sec = p_date->sec;
    return mktime(&tm_tim);
}

/*************************************************************
** Descriptions:        秒数转换成日期
*************************************************************/
void real_calendar_sec_to_date(P_X_CALENDAR_DATE p_date, uint32_t sec)
{
    struct tm *local;
    time_t secs = sec;
    local = localtime(&secs);

    p_date->year = local->tm_year + 1900;
    p_date->month = local->tm_mon + 1;
    p_date->day = local->tm_mday;
    p_date->hour = local->tm_hour;
    p_date->min = local->tm_min;
    p_date->sec = local->tm_sec;
    p_date->week = local->tm_wday;
}

/*************************************************************
** Descriptions:        设置RTC日期
*************************************************************/
void real_set_calendar_date(P_X_CALENDAR_DATE p_date)
{
	uint64_t sec = real_calendar_date_to_sec(p_date);
    if(g_x_calendar_obj.drv_rtc == NULL){
        return;
    }
    xh_rtc_set_cnt(g_x_calendar_obj.drv_rtc, sec);
}

/*************************************************************
** Descriptions:        设置RTC计数值
*************************************************************/
void real_set_calendar_sec(uint32_t sec)
{	
    if(g_x_calendar_obj.drv_rtc == NULL){
        return;
    }
    xh_rtc_set_cnt(g_x_calendar_obj.drv_rtc, sec);

}

/*************************************************************
** Descriptions:        获取日期
*************************************************************/
bool real_get_calendar_date(P_X_CALENDAR_DATE p_date)
{
	uint64_t sec = 0;
    if(g_x_calendar_obj.drv_rtc == NULL){
        return false;
    }
    sec = xh_rtc_get_cnt(g_x_calendar_obj.drv_rtc);

	real_calendar_sec_to_date(p_date,sec);
	return true;
}

/*************************************************************
** Descriptions:        获取秒数
*************************************************************/
uint32_t real_get_calendar_sec(void)
{
	uint64_t sec = 0;
    if(g_x_calendar_obj.drv_rtc == NULL){
        return 0;
    }
    sec = xh_rtc_get_cnt(g_x_calendar_obj.drv_rtc);

	return sec;
}

/*************************************************************
** Descriptions:        设置时区
*************************************************************/
bool real_set_time_zone(int8_t zone)
{
    if(zone != g_x_calendar_obj.zone)
    {
        g_x_calendar_obj.zone = zone;
        xh_rtc_set_zone(g_x_calendar_obj.drv_rtc,g_x_calendar_obj.zone);
    }
    return true;
}

/*************************************************************
** Descriptions:        获取时区
*************************************************************/
int real_get_time_zone(void)
{
    return g_x_calendar_obj.zone;
}

/*************************************************************
** Descriptions:        日期比较
** Return:				0:  p_date_1 = p_date_2
**						1:  p_date_1 > p_date_2
**						2:  p_date_1 < p_date_2, 
*************************************************************/
uint8_t real_calendar_date_cmp(P_X_CALENDAR_DATE p_date_1,P_X_CALENDAR_DATE p_date_2)
{
	uint32_t date_1_sec = real_calendar_date_to_sec(p_date_1);
	uint32_t date_2_sec = real_calendar_date_to_sec(p_date_2);
	if(date_1_sec == date_2_sec){
		return 0;
	}else if(date_1_sec > date_2_sec){
		return 1;
	}else{
		return 2;
	}
}



