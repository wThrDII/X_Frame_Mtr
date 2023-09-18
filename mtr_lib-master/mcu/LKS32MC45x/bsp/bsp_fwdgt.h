/**
 * @brief 看门狗驱动
 * @file bsp_fwdgt.h
 * @version 1.0
 * @author boyu.qin 
 * @date 2023-01-11 10:37:29
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 * 
 * @details 文件详细描述，包含使用场景，依赖项等
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2023-01-11 10:37:29      <td>boyu.qin      <td>创建文件
 */

#ifndef __BSP_FWDGT_H__
#define __BSP_FWDGT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "lks32mc45x_iwdg.h"
#include "xh_fwdgt.h"

    typedef struct _BSP_FWDGT
    {
        uint16_t timeout;
    } BSP_FWDGT, *P_BSP_FWDGT;

/**
 * @brief   看门狗驱动注册宏
 *
 * @param [in] dev_id        ///< 设备ID
 * @param [in] prio          ///< 优先级
 * @param [in] timeout       ///< 复位时间
 * @details 特殊说明:
 * @par eg:
 * @code
 *
 * @endcode
 */
#define REG_XH_FWDG(dev_id, prio, timeout)                                                                      \
    BSP_FWDGT _##dev_id = {timeout};                                                                            \
    REG_XH_WDG_DRV(dev_id, &_##dev_id, prio, DEV_AUTO_INIT, bsp_fwdgt_init, bsp_fwdgt_deinit, bsp_reload_fwdgt, \
                   bsp_get_fwdgt_is_reset)

extern void bsp_fwdgt_init(P_BSP_FWDGT fwdgt);
extern uint32_t bsp_get_fwdgt_is_reset(P_BSP_FWDGT fwdgt);
extern void bsp_reload_fwdgt(P_BSP_FWDGT fwdgt);
extern void bsp_fwdgt_deinit(P_BSP_FWDGT fwdgt);

#ifdef __cplusplus
}
#endif

#endif //__BSP_FLASH_H__

