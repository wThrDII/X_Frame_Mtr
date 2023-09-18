/**
 * @brief 
 * @file xh_cmpss.h
 * @version 1.0
 * @author peixiang.qiu 
 * @date 2022-12-16 15:01:35
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-12-16 15:01:35 <td>1.0       <td>peixiang.qiu      <td>Created function
 */

#ifndef __xh_cmpss_h__
#define __xh_cmpss_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include "xh_core.h"

    typedef void (*xh_cmpss_set_dac_h_obj)(void *val, uint32_t value);
    typedef void (*xh_cmpss_set_dac_l_obj)(void *val, uint32_t value);
    typedef uint32_t (*xh_cmpss_get_latch_l_obj)(void *val);
    typedef uint32_t (*xh_cmpss_get_latch_h_obj)(void *val);

    typedef struct _xh_cmpss_t
    {
        xh_comm_t core;
        xh_cmpss_set_dac_h_obj set_dac_h;
        xh_cmpss_set_dac_l_obj set_dac_l;
        xh_cmpss_get_latch_l_obj get_latch_l;
        xh_cmpss_get_latch_h_obj get_latch_h;
    } xh_cmpss_t, *p_xh_cmpss_t;

/* 驱动宏定义实现 */

#define REG_XH_CMPSS_DRV(id, var, prio, auto_init, init, deinit, set_dac_h, set_dac_l, get_latch_l, get_latch_h) \
    REG_XH_DEV(xh_cmpss_t, id, var, prio, auto_init, init, deinit, (xh_cmpss_set_dac_h_obj)set_dac_h,            \
               (xh_cmpss_set_dac_l_obj)set_dac_l, (xh_cmpss_get_latch_l_obj)get_latch_l,                         \
               (xh_cmpss_get_latch_h_obj)get_latch_h)

#define EXT_XH_CMPSS_DRV(id) EXT_XH_DEV(xh_cmpss_t, id)



    typedef enum
    {
        XH_CMPSS_TRIG_END = 2, ///< CMPSS曾触发过封波
        XH_CMPSS_TRIG_ING = 3, ///< CMPSS封波中
    } XH_CMPSS_TRIG_STA;

    /**
     * @brief 设置CMPSS DAC高比较值
     *
     * @param [in] id            驱动ID
     * @param [in] value         比较值
     *
     * @details value：比较值 0~4095 对应0~3.3v
     * @par eg:设置DAC高比较值为2048
     * @code
     * #include "hal.h"
     * uint32_t dach = 2048;///< 取值范围 0~4095
     * void set_dacl(void)
     * {
     *     ///< DRV_CMPSS1在hal中声明的驱动ID,为CMPSS驱动ID
     *      xh_cmpss_set_dac_h(DRV_CMPSS1,dach);
     * }
     *
     * @endcode
     */
    static inline void xh_cmpss_set_dac_h(void *id, uint32_t value)
    {
        xh_cmpss_t *p = (xh_cmpss_t *)id;
        p->set_dac_h(p->core.var, value);
    }

    /**
     * @brief 设置CMPSS DAC低比较值
     *
     * @param [in] id            驱动ID
     * @param [in] value         比较值
     *
     * @details value：比较值 0~4095 对应0~3.3v
     * @par eg:设置DAC低比较值为2048
     * @code
     * #include "hal.h"
     * uint32_t dacl = 2048;///< 取值范围 0~4095
     * void set_dacl(void)
     * {
     *     ///< DRV_CMPSS1在hal中声明的驱动ID,为CMPSS驱动ID
     *      xh_cmpss_set_dac_l(DRV_CMPSS1,dacl);
     * }
     * @endcode
     */
    static inline void xh_cmpss_set_dac_l(void *id, uint32_t value)
    {
        xh_cmpss_t *p = (xh_cmpss_t *)id;
        p->set_dac_l(p->core.var, value);
    }

    /**
     * @brief 获取CMPSS低比较触发的状态，优化等级为2耗时 0.58us
     *
     * @param [in] id        驱动ID
     *
     * @return uint32_t
     *  @retval 返回CMPSS状态，等于XH_CMPSS_TRIG_ING，CMPSS正在触发低封波
     *          等于XH_CMPSS_TRIG_END，CMPSS曾触发低封波，如果当前不触发封波，读取后会清除状态
     * @details 特殊说明: 
     * @par eg: CMPSS驱动ID为DRV_CMPSS1,用于控制驱动ID为DRV_EPWM1和DRV_EPWM2的EPWM封波，读取CMPSS低触发状态
     * @code
     *      uint32_t flag = 0；
     *      flag = xh_cmpss_get_state_l(DRV_CMPSS1); //注意是CMPSS驱动ID
     * @endcode
     */
    static inline uint32_t xh_cmpss_get_state_l(void *id)
    {
        xh_cmpss_t *p = (xh_cmpss_t *)id;
        return p->get_latch_l(p->core.var);
    }

    /**
     * @brief 获取CMPSS高比较触发的状态，优化等级为2耗时 0.58us
     *
     * @param [in] id        驱动ID
     *
     * @return uint32_t
     *  @retval 返回CMPSS状态，等于XH_CMPSS_TRIG_ING，CMPSS正在触发高封波
     *          等于XH_CMPSS_TRIG_END，CMPSS曾触发高封波，读取后会清除状态
     * @details 特殊说明: 
     * @par eg: CMPSS驱动ID为DRV_CMPSS1,用于控制驱动ID为DRV_EPWM1和DRV_EPWM2的EPWM封波，读取CMPSS高触发状态
     * @code
     *      uint32_t flag = 0；
     *      flag = xh_cmpss_get_state_h(DRV_CMPSS1);
     * @endcode
     */
    static inline uint32_t xh_cmpss_get_state_h(void *id)
    {
        xh_cmpss_t *p = (xh_cmpss_t *)id;
        return p->get_latch_h(p->core.var);
    }
#ifdef __cplusplus
}
#endif
#endif /* __xh_cmpss_h__ */
