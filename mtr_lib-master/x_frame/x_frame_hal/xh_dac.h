/**
 * @brief
 * @file xh_dac.h
 * @version 1.0
 * @author liuxiang
 * @date 2022-07-20 14:14:58
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-20 14:14:58 <td>1.0       <td>liuxiang      <td>Created function
 */

#ifndef __xh_dac_H__
#define __xh_dac_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "xh_core.h"

    typedef void (*xh_dac_set_obj)(void *var, uint16_t value);

    typedef struct _xh_dac_t
    {
        xh_comm_t core;
        xh_dac_set_obj dac_set;
    } xh_dac_t, *p_xh_dac_t;

#define REG_XH_DAC_DRV(id, var, prio, auto_init, init, deinit, dac_set) \
    REG_XH_DEV(xh_dac_t, id, var, prio, auto_init, init, deinit,        \
               (xh_dac_set_obj)dac_set)

#define EXT_XH_DAC_DRV(id) EXT_XH_DEV(xh_dac_t, id)

    /**
     * @brief DAC设置输出
     *
     * @param [in] id       驱动ID
     * @param [in] value    设定值
     *
     * @details 特殊说明:
     * @par eg:12位DAC，输出1.65V电压
     * @code
     *    xh_dac_set(DRV_DAC, 2048);
     * @endcode
     */
    static inline void xh_dac_set(void *const id, uint16_t value)
    {
        xh_dac_t *p = (xh_dac_t *)id;
        p->dac_set(p->core.var, value);
    }

#ifdef __cplusplus
}
#endif
#endif //__xh_dac_H__\

