/**
 * @brief BSP初始化
 * @file xh_hal.h
 * @version 1.0
 * @author Xiao Shulong
 * @date 2022-07-08 19:06:45
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-08 19:06:45 <td>1.0       <td>Xiao Shulong      <td>Created function
 */
#ifndef xh_hal_h
#define xh_hal_h
#ifdef __cplusplus
extern "C"
{
#endif

#include "xh_core.h"

    typedef char const *(*xh_hal_get_commit_id_obj)(void *var);
    typedef char const *(*xh_hal_get_srf_ver_obj)(void *var);

    typedef struct _xh_hal_init_t
    {
        xh_comm_t core;
        xh_hal_get_commit_id_obj get_commit_id;
        xh_hal_get_srf_ver_obj get_srf_ver;
    } xh_hal_init_t, *p_xh_hal_init_t;

/* 驱动宏定义实现 */
#define REG_XH_HAL_DRV(id, var, prio, auto_init, init, deinit, get_commit_id, get_srf_ver) \
    REG_XH_DEV(xh_hal_init_t, id, var, prio, auto_init, init, deinit,                      \
               (xh_hal_get_commit_id_obj)get_commit_id,                                    \
               (xh_hal_get_srf_ver_obj)get_srf_ver, )

#define EXT_XH_HAL_DRV(id) EXT_XH_DEV(xh_hal_init_t, id)

    /**
     * @brief 获取commit id
     *
     * @param [in] id               设备ID
     *
     * @return char const*
     *  @retval commit id 字符串指针
     * @details 特殊说明:           无
     * @par eg:获取commit id
     * @code
     * char *commit_id;
     * commit_id = xh_hal_get_commit_id(DRV_HAL)
     * @endcode
     */
    static inline char const *xh_hal_get_commit_id(void *const id)
    {
        xh_hal_init_t *p = (xh_hal_init_t *)id;
        return p->get_commit_id(p->core.var);
    }
    /**
     * @brief 获取软件平台版本号
     *
     * @param [in] id               设备ID
     *
     * @return char const*
     *  @retval 版本号字符串指针
     * @details 特殊说明:           无
     * @par eg: 获取版本号
     * @code
     * char *ver;
     * ver = xh_hal_get_srf_ver(DRV_HAL);
     * @endcode
     */
    static inline char const *xh_hal_get_srf_ver(void *const id)
    {
        xh_hal_init_t *p = (xh_hal_init_t *)id;
        return p->get_srf_ver(p->core.var);
    }

#ifdef __cplusplus
}
#endif
#endif /* xh_hal_h */
