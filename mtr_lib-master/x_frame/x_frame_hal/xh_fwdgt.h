/**
 * @brief 看门狗驱动接口
 * @file xh_fwdgt.h
 * @version 1.0
 * @author Xiao Shulong
 * @date 2022-07-08 19:06:14
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-08 19:06:14 <td>1.0       <td>Xiao Shulong      <td>Created function
 */
#ifndef xh_fwdgt_h
#define xh_fwdgt_h
#ifdef __cplusplus
extern "C" {
#endif

#include "xh_core.h"

typedef void (*xh_wdg_reload_obj)(void *var);
typedef uint32_t (*xh_wdg_get_reset_obj)(void *var);

typedef struct _xh_wdg_t{
    xh_comm_t core;
    xh_wdg_reload_obj      reload;
    xh_wdg_get_reset_obj   get_reset;
}xh_wdg_t, *p_wdg_t;

/* 驱动宏定义实现 */
#define REG_XH_WDG_DRV(id,var, prio,auto_init,init,deinit,reload,get_reset)    \
            REG_XH_DEV(xh_wdg_t,id,var,prio,auto_init,init,deinit,                  \
            (xh_wdg_reload_obj)reload,                                          \
            (xh_wdg_get_reset_obj)get_reset,                                    \
        );

#define EXT_XH_WDG_DRV(id)        EXT_XH_DEV(xh_wdg_t,id)

/**
 * @brief xh_wdg_reload
 * 
 * @param [in] id          驱动ID  
 * 
 * @details 特殊说明: 无
 * @par eg: 喂狗
 * @code
 *    xh_wdg_reload(DRV_FWDG)
 * @endcode
 */
static inline void xh_wdg_reload(void *const id)
{
    xh_wdg_t *p = (xh_wdg_t *)id;
    p->reload(p->core.var);
}

/**
 * @brief xh_wdg_get_reset
 * 
 * @param [in] id          驱动ID  
 * 
 * @return uint32_t 
 *  @retval 
 * @details 特殊说明: 无
 * @par eg: 获取看门狗标志位
 * @code
 *    xh_wdg_get_reset(DRV_FWDG)
 * @endcode
 */
static inline uint32_t xh_wdg_get_reset(void *const id)
{
    xh_wdg_t *p = (xh_wdg_t *)id;
    return p->get_reset(p->core.var);
}

#ifdef __cplusplus
}
#endif
#endif /* xh_fwdgt_h */
