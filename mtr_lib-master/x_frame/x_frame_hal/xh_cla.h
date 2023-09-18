/**
 * @brief
 * @file xh_cla.h
 * @version 1.0
 * @author peixiang.qiu
 * @date 2022-11-17 11:44:18
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-11-17 11:44:18 <td>1.0       <td>peixiang.qiu      <td>Created function
 */

#ifndef xh_cla_h
#define xh_cla_h
#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum
    {
        CLA_TASK = 0, ///< CLA中运行
        CLA_IRQ = 1,  ///< CPU中运行,CLA任务完成后触发CLA中断
    } XH_CLA_CB_TYPE;

#include "xh_core.h"

    typedef void (*xh_cla_enable_obj)(void *var, uint32_t index);
    typedef void (*xh_cla_disable_obj)(void *var, uint32_t index);
    typedef void (*xh_cla_start_obj)(void *var, uint32_t index);
    typedef void (*xh_cla_reg_cb_obj)(void *var, uint32_t index, uint32_t cb_type, void (*cla_cb)(void));

    typedef struct _xh_cla_t
    {
        xh_comm_t core;
        xh_cla_enable_obj cla_enable;
        xh_cla_disable_obj cla_disable;
        xh_cla_start_obj cla_start;
        xh_cla_reg_cb_obj cla_reg_cb;
    } xh_cla_t, *p_xh_cla_t;

#define REG_XH_CLA_DRV(id, var, prio, auto_init, init, deinit, cla_enable, cla_disable, cla_start, cla_reg_cb) \
    REG_XH_DEV(xh_cla_t, id, var, prio, auto_init, init, deinit, (xh_cla_enable_obj)cla_enable,                \
               (xh_cla_disable_obj)cla_disable, (xh_cla_start_obj)cla_start, (xh_cla_reg_cb_obj)cla_reg_cb)

#define EXT_XH_CLA_DRV(id) EXT_XH_DEV(xh_cla_t, id)

    /**
     * @brief   CAL使能
     *
     * @param [in] id   驱动ID
     *
     * @details 特殊说明:
     * @par eg:使能驱动ID为DRV_CAL的CAL
     * @code
     *      xh_cla_enable(DRV_CAL);
     * @endcode
     */
    static inline void xh_cla_enable(void *const id, uint32_t index)
    {
        xh_cla_t *p = (xh_cla_t *)id;
        p->cla_enable(p->core.var,index);
    }

    /**
     * @brief CAL失能
     *
     * @param [in] id   驱动ID
     *
     * @details 特殊说明:
     * @par eg:失能驱动ID为DRV_CAL的CAL
     * @code
     *      xh_cla_disable(DRV_CAL);
     * @endcode
     */
    static inline void xh_cla_disable(void *const id, uint32_t index)
    {
        xh_cla_t *p = (xh_cla_t *)id;
        p->cla_disable(p->core.var,index);
    }

    /**
     * @brief 注册CLA任务或中断回调函数
     *
     * @param [in] id          驱动ID
     * @param [in] cb_type     回调类型，CLA中断或CLA任务
     * @param [in] cb          CLA回调函数
     *
     * @details 特殊说明:
     *      回调函数类型：
     *      void (*cla_cb)(void);
     * @par eg:
     * @code
     *
     * @endcode
     */
    static inline void
        xh_cla_reg_cb(void *const id, uint32_t index, uint32_t cb_type, void (*cla_cb)(void)) // TODO:cb_type
    {
        xh_cla_t *p = (xh_cla_t *)id;
        p->cla_reg_cb(p->core.var, index,cb_type, cla_cb);
    }

    /**
     * @brief   CLA开始
     *
     * @param [in] id   驱动ID
     *
     * @details 特殊说明:
     * 软件触发模式下，调用该函数启动CLA任务
     * @par eg:启动CLA任务
     * @code
     *      xh_cla_start(DRV_CAL)
     * @endcode
     */
    static inline void xh_cla_start(void *const id, uint32_t index)
    {
        xh_cla_t *p = (xh_cla_t *)id;
        p->cla_start(p->core.var, index);
    }

#ifdef __cplusplus
}
#endif
#endif /* xh_cla_h */
