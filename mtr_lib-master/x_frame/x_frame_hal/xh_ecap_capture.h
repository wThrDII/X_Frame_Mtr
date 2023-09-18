/**
 * @brief
 * @file xh_ecap_capture.h
 * @version 1.0
 * @author peixiang.qiu
 * @date 2022-11-10 17:02:32
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-11-10 17:02:32 <td>1.0       <td>peixiang.qiu      <td>Created function
 */
#ifndef xh_ecap_capture_h
#define xh_ecap_capture_h
#ifdef __cplusplus
    extern "C"
{
#endif
typedef enum
{
    BSP_ECAP_EVENT_VAL_1 = 0U,   //!< eCAP event 1 value index
    BSP_ECAP_EVENT_VAL_2 = 1U,   //!< eCAP event 2 value index
    BSP_ECAP_EVENT_VAl_3 = 2U,   //!< eCAP event 3 value index
    BSP_ECAP_EVENT_VAL_4 = 3U    //!< eCAP event 4 value index
}BSP_ECAP_EVENT_VAL_INDEX;

#include "xh_core.h"
    typedef void (*xh_ecap_cap_start_obj)(void *var);
    typedef uint32_t (*xh_ecap_cap_read_val_obj)(void *var, uint32_t index);
    typedef void (*xh_ecap_cap_enable_obj)(void *var);
    typedef void (*xh_ecap_cap_disable_obj)(void *var);
    typedef void (*xh_ecap_cap_set_buf_obj)(void *var, uint32_t *p_buf, uint32_t len);
    typedef struct _xh_ecap_cap_t
    {
        xh_comm_t core;
        xh_ecap_cap_start_obj ecap_cap_start;
        xh_ecap_cap_read_val_obj ecap_cap_read;
        xh_ecap_cap_enable_obj ecap_cap_enable;
        xh_ecap_cap_disable_obj ecap_cap_disable;
        xh_ecap_cap_set_buf_obj ecap_cap_set_buf;
    } xh_ecap_cap_t, *p_xh_ecap_cap_t;

#define REG_XH_ECAP_CAP_DRV(id, var, prio, auto_init, init, deinit, ecap_cap_start, ecap_cap_read,           \
                                ecap_cap_enable, ecap_cap_disable, ecap_cap_set_buf)                             \
    REG_XH_DEV(xh_ecap_cap_t, id, var, prio, auto_init, init, deinit, (xh_ecap_cap_start_obj)ecap_cap_start, \
               (xh_ecap_cap_read_val_obj)ecap_cap_read, (xh_ecap_cap_enable_obj)ecap_cap_enable,                 \
               (xh_ecap_cap_disable_obj)ecap_cap_disable, (xh_ecap_cap_set_buf_obj)ecap_cap_set_buf)

#define EXT_XH_ECAP_CAP_DRV(id) EXT_XH_DEV(xh_ecap_cap_t, id)

    /**
     * @brief ECAP开始捕获，用于单次捕获模式
     *
     * @param [in] id   驱动ID
     *
     * @details 特殊说明:
     *  调用该函数启动ECAP捕获
     * @par eg:启动ECAP捕获
     * @code
     *      xh_ecap_cap_start(DRV_ECAP)
     * @endcode
     */
    static inline void xh_ecap_cap_start(void *const id)
    {
        xh_ecap_cap_t *p = (xh_ecap_cap_t *)id;
        p->ecap_cap_start(p->core.var);
    }

    /**
     * @brief ECAP读数据
     *
     * @param [in] id   驱动ID
     * @param [in] index    索引
     *
     * @return uint32_t
     *  @retval 返回读取的数据
     * @details 特殊说明:
     * @par eg:读取驱动ID为DRV_ECAP，第一个事件捕获的值
     * @code
     *      uint16_t ecap_cap_value[4] = {0};
     *      void ecap_cap_task(void)
     *      {
     *          ecap_cap_value[0] = xh_ecap_cap_read_value(DRV_ECAP, BSP_ECAP_EVENT_VAL_1);
     *      }
     *      REG_TASK(1,1,ecap_cap_task)
     * @endcode
     */
    static inline uint32_t xh_ecap_cap_read_value(void *const id, BSP_ECAP_EVENT_VAL_INDEX index)
    {
        xh_ecap_cap_t *p = (xh_ecap_cap_t *)id;
        return p->ecap_cap_read(p->core.var, index);
    }

    /**
     * @brief   ECAP使能
     *
     * @param [in] id   驱动ID
     *
     * @details 特殊说明:
     * @par eg:使能驱动ID为DRV_ECAP的ECAP
     * @code
     *      xh_ecap_cap_enable(DRV_ECAP);
     * @endcode
     */
    static inline void xh_ecap_cap_enable(void *const id)
    {
        xh_ecap_cap_t *p = (xh_ecap_cap_t *)id;
        p->ecap_cap_enable(p->core.var);
    }

    /**
     * @brief ECAP失能
     *
     * @param [in] id   驱动ID
     *
     * @details 特殊说明:
     * @par eg:失能驱动ID为DRV_ECAP的ECAP
     * @code
     *      xh_ecap_cap_disable(DRV_ECAP);
     * @endcode
     */
    static inline void xh_ecap_cap_disable(void *const id)
    {
        xh_ecap_cap_t *p = (xh_ecap_cap_t *)id;
        p->ecap_cap_disable(p->core.var);
    }

    /**
     * @brief ECAP更改缓存区
     *
     * @param [in] id       驱动ID
     * @param [in] p_buf    更改的缓存区指针
     * @param [in] len      读取数据的个数
     *
     * @details 特殊说明:
     *
     * @par eg:读取4个事件捕获的数据放到ecap_cap_value[4]中
     * @code
     *      uint16_t ecap_cap_value[4] = {0};
     *      void test_init(void){
     *          xh_ecap_cap_set_buf(DRV_ECAP, ecap_cap_value, 4);
     *      }
     *      REG_INIT_MOD(test_init)
     * @endcode
     */
    static inline void xh_ecap_cap_set_buf(void *const id, uint32_t *p_buf, uint32_t len)
    {
        xh_ecap_cap_t *p = (xh_ecap_cap_t *)id;
        p->ecap_cap_set_buf(p->core.var, p_buf, len);
    }

#ifdef __cplusplus
}
#endif
#endif /* xh_ecap_capture_h */
