/**
 * @brief x_calendar.c.xtemp header file
 * @file x_calendar.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 16:53:6
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 16:53:6
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __X_CALENDAR_H__
#define __X_CALENDAR_H__

/**
 * @brief 注册日历驱动
 *
 * @param [in]  _drv_rtc        RTC驱动ID 
 *
 * @details 
 * @par 示例:注册一个ID为DRV_RTC的RTC日历驱动
 * @code
 *   REG_UNIX_CALENDAR(DRV_RTC) 
 * @endcode
 */
#define REG_UNIX_CALENDAR(_drv_rtc) 								X_CALENDAR_OBJ g_x_calendar_obj =  {.drv_rtc = PTR_TYPE_XH_DEV(_drv_rtc),};

#define calendar_init()

/**
 * @brief 日期转换成秒数
 *
 * @param [in]  p_date        储存日期的结构体指针
 *
 * @return uint32_t 
 * @retval 返回转换的秒数
 * @details 
 * @par 示例:把结构体date中的日期转换成秒数，放到变量time_sec中
 * @code
 *   uint32_t time_sec;
 *   X_CALENDAR_DATE date;
 *   time_sec = calendar_date_to_sec(&date);
 * @endcode
 */
#define calendar_date_to_sec(p_date)                                real_calendar_date_to_sec(p_date)

/**
 * @brief 秒数转换成日期
 *
 * @param [in]  p_date        储存日期的结构体指针
 * @param [in]  sec           要转换的秒数  
 *
 * @details 
 * @par 示例:把变量time_sec中的秒数转换成日期存到结构体date中
 * @code
 *   uint32_t time_sec = 10; 
 *   X_CALENDAR_DATE date;
 *   calendar_sec_to_date(&date, time_sec);
 * @endcode
 */
#define calendar_sec_to_date(p_date, sec)                          real_calendar_sec_to_date(p_date, sec)

/**
 * @brief 设置RTC日期
 *
 * @param [in]  p_date        储存日期的结构体指针 
 *
 * @details 
 * @par 示例:把结构体date中的日期设置到RTC中
 * @code
 *   X_CALENDAR_DATE date;
 *   date.week = 3;
 *   date.sec = 6;
 *   date.min = 6;
 *   date.hour = 5;
 *   date.day = 5;
 *   date.month = 4;
 *   date.year = 1999;
 *   set_calendar_date(&date);
 * @endcode
 */
#define set_calendar_date(p_date)                         			real_set_calendar_date(p_date)

/**
 * @brief 设置RTC计数值
 *
 * @param [in]  sec        设置的RTC计数值 
 *
 * @details 
 * @par 示例:把变量time_sec中的值设置到RTC中
 * @code
 *   uint32_t time_sec = 20;
 *   set_calendar_sec(time_sec);
 * @endcode
 */
#define set_calendar_sec(sec)                             			real_set_calendar_sec(sec)

/**
 * @brief 获取日期
 *
 * @param [in]  p_date        储存日期的结构体指针 
 * 
 * @return bool 
 * @retval 获取日期成功返回true，否则返回false
 * @details 
 * @par 示例:获取日期存到结构体date中
 * @code
 *   X_CALENDAR_DATE date;
 *   get_calendar_date(&date); 
 * @endcode
 */
#define get_calendar_date(p_date)                         			real_get_calendar_date(p_date)

/**
 * @brief 获取秒数
 *
 * @param [in]  无 
 * 
 * @return uint32_t 
 * @retval 返回获取的秒数
 * @details 
 * @par 示例:获取秒数存到变量time_sec中
 * @code
 *   uint32_t time_sec;
 *   time_sec = get_calendar_sec();
 * @endcode
 */
#define get_calendar_sec()                            				real_get_calendar_sec()

/**
 * @brief 设置时区
 *
 * @param [in]  zone        设置的时区 
 *
 * @details zone范围-12~12，对应西12区到东12区
 * @par 示例:设置时区为1
 * @code
 *   int8_t zone = 1;
 *   set_time_zone(zone);
 * @endcode
 */
#define set_time_zone(zone)										real_set_time_zone(zone)

/**
 * @brief 获取时区
 *
 * @param [in]  无   
 *
 * @return int 
 * @retval 返回时区，范围-12~12，对应西12区到东12区
 * @details 
 * @par 示例:获取时区存到变量zone中
 * @code
 *   int8_t zone;
 *   zone = get_time_zone();
 * @endcode
 */
#define get_time_zone()											real_get_time_zone()

/**
 * @brief 日期比较
 *
 * @param [in]  p_date_1        储存日期的结构体指针 
 * @param [in]  p_date_2        储存日期的结构体指针 
 *
 * @return uint8_t 
 * @retval  返回比较的结果
 *          0:  p_date_1 = p_date_2
 *			1:  p_date_1 > p_date_2
 *			2:  p_date_1 < p_date_2
 * @details 
 * @par 示例:比较日期date_1和date_2
 * @code
 *   uint8_t tim;
 *   tim = calendar_date_cmp(&date_1,&date_2); 
 * @endcode
 */
#define calendar_date_cmp(p_date_1,p_date_2)              			real_calendar_date_cmp(p_date_1,p_date_2)

typedef struct _X_CALENDAR_DATE{
	uint64_t week:3;
    uint64_t sec:6;
    uint64_t min:6;
    uint64_t hour:5;
    uint64_t day:5;
    uint64_t month:4;
    uint64_t year:12;
	uint64_t reserve:23;
}X_CALENDAR_DATE, *P_X_CALENDAR_DATE;

typedef struct _X_CALENDAR_OBJ{
    void * const drv_rtc;
    int8_t   zone;           /* 时区信息。范围-12~12，对应西12区到东12区 */
}X_CALENDAR_OBJ, *P_X_CALENDAR_OBJ;

//将时区信息表示为分钟
#define time_zone_minute(tz)         ((tz)*15)


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

void real_set_calendar_date(P_X_CALENDAR_DATE p_date);
void real_set_calendar_sec(uint32_t sec);
bool real_get_calendar_date(P_X_CALENDAR_DATE p_date);
uint32_t real_get_calendar_sec(void);
bool real_set_time_zone(int8_t zone);
int real_get_time_zone(void);
uint8_t real_calendar_date_cmp(P_X_CALENDAR_DATE p_date_1,P_X_CALENDAR_DATE p_date_2);
uint32_t real_calendar_date_to_sec(P_X_CALENDAR_DATE p_date);
void real_calendar_sec_to_date(P_X_CALENDAR_DATE p_date, uint32_t sec);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __X_CALENDAR_H__
