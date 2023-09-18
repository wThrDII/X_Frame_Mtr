/**
 * @brief RTC驱动接口
 * @file xh_rtc.h
 * @version 1.1
 * @author Xiao Shulong
 * @date 2022-07-08 19:25:17
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-08 19:25:17 <td>1.0       <td>Xiao Shulong      <td>Created function
 * <tr><td>2022-07-20 16:25:58 <td>1.1       <td>Xiao Shulong      <td>增加时区标志位的设置和获取
 */

#ifndef xh_rtc_h
#define xh_rtc_h
#ifdef __cplusplus
extern "C" {
#endif

#include "xh_core.h"

typedef void (*xh_rtc_set_cnt_obj)(void *var,uint32_t cnt);
typedef uint32_t (*xh_rtc_get_cnt_obj)(void *var);
typedef uint32_t (*xh_rtc_get_erro_obj)(void *var);
typedef bool (*xh_rtc_set_zone_obj)(void *var,int8_t zone);
typedef int8_t (*xh_rtc_get_zone_obj)(void *var);

typedef struct _xh_rtc_t{
    xh_comm_t core;
    xh_rtc_set_cnt_obj      set_cnt;
    xh_rtc_get_cnt_obj      get_cnt;
    xh_rtc_get_erro_obj     get_erro;
    xh_rtc_set_zone_obj     set_zone;
    xh_rtc_get_zone_obj     get_zone;
}xh_rtc_t, *p_xh_rtc_t;

/* 驱动宏定义实现 */
#define REG_XH_RTC_DRV(id,var, prio,auto_init,init,deinit,updata,get_cnt,get_erro,set_zone,get_zone)    \
            REG_XH_DEV(xh_rtc_t,id,var,prio,auto_init,init,deinit,                  \
            (xh_rtc_set_cnt_obj)updata,                                             \
            (xh_rtc_get_cnt_obj)get_cnt,                                            \
            (xh_rtc_get_erro_obj)get_erro,                                          \
            (xh_rtc_set_zone_obj)set_zone,                                          \
            (xh_rtc_get_zone_obj)get_zone,                                          \
        )

#define EXT_XH_RTC_DRV(id)        EXT_XH_DEV(xh_rtc_t,id)

/**
 * @brief 设置计数值
 *
 * @param [in] id            驱动ID
 * @param [in] cnt           计数值
 *
 * @details 一般是由框架调用
 * @par eg:RTC写入345662
 * @code
 *    xh_rtc_set_cnt(DRV_RTC,345662);
 * @endcode
 */
static inline void xh_rtc_set_cnt(void *const id,uint32_t cnt)
{
    xh_rtc_t *p = (xh_rtc_t *)id;
    p->set_cnt(p->core.var,cnt);
}

/**
 * @brief 获取计数值
 *
 * @param [in] id            驱动ID
 *
 * @return uint32_t
 *  @retval RTC计数值
 * @details 一般是由框架调用
 * @par eg:获取RTC计数值
 * @code
 *    uint32_t cnt;
 *    cnt = xh_rtc_get_cnt(DRV_RTC);
 * @endcode
 */
static inline uint32_t xh_rtc_get_cnt(void *const id)
{
    xh_rtc_t *p = (xh_rtc_t *)id;
    return p->get_cnt(p->core.var);
}

/**
 * @brief 获取RTC错误标志
 * @param [in] id            驱动ID
 * @return uint32_t
 *  @retval 错误标志
 *          1：RTC初始化错误
 *          0：RTC初始化成功
 * @details 在外部晶振未起振情况下会产生错误标志
 * @par eg:获取RTC错误标志
 * @code
 * uint32_t rtc_flag;
 * void func(void)
 * {
 *      rtc_flag = xh_rtc_get_erro(DRV_RTC);
 *      if(rtc_flag)
 *      {
 *          ///< 外部晶振起振失败
 *      }
 *      else
 *      {
 *          ///< 初始化成功
 *      }
 * }
 * @endcode
 */
static inline uint32_t xh_rtc_get_erro(void *const id)
{
    xh_rtc_t *p = (xh_rtc_t *)id;
    return p->get_erro(p->core.var);
}

/**
 * @brief 设置时区标志
 * @param [in] id            驱动ID
 * @param [in] zone          时区
 * @return true
 *  @retval 成功
 * @return false
 *  @retval 失败
 * @details 使用了第二块备份域
 * @par eg:设置时区为8
 * @code
 * int8_t zone = 8;///< 东八区
 * void func(void)
 * {
 *    xh_rtc_set_zone(DRV_RTC,zone);
 * }
 * @endcode
 */
static inline bool xh_rtc_set_zone(void *const id,int8_t zone)
{
    xh_rtc_t *p = (xh_rtc_t *)id;
    if(NULL != p->set_zone){
        p->set_zone(p->core.var,zone);
    }
    return false;
}

/**
 * @brief 获取时区标志
 * @param [in] id            驱动ID
 * @return int8_t
 *  @retval 时区标志
 * @details 东八区为8，西八区为-8
 * @par eg:获取时区
 * @code
 * int8_t zone;
 * void func(void)
 * {
 *    zone = xh_rtc_get_zone(DRV_RTC);
 * }
 * @endcode
 */
static inline int8_t xh_rtc_get_zone(void *const id)
{
    xh_rtc_t *p = (xh_rtc_t *)id;
    if(NULL != p->set_zone){
        return p->get_zone(p->core.var);
    }
    return 0;
}

#ifdef __cplusplus
}
#endif
#endif /* xh_rtc_h */


