/***********************************************************************
 * @file xh_bq79616
 * Description:
 *
 **********************************************************************
 * File name:      xh_bq79616.h
 * Date:           2022-07-08
 * Version:        V1.0
 * Author          liuxiang
 * @history:
 * V1.0 创建文件
 ***********************************************************************/
#ifndef __xh_bq79616_H__
#define __xh_bq79616_H__
#include "stdint.h"

#include "xh_core.h"

// AFE Real-time raw datas
typedef struct
{
    uint8_t dev_addr;
    uint8_t cell_nums;
    uint8_t fault_summary;
    uint8_t dev_state;
    uint8_t bal_state;
    uint16_t tsref;
    uint16_t aux_bat_val;
    uint16_t aux_ref_val;
    uint16_t aux_ref4v2_val;
    uint16_t aux_gpio_val[8];
    uint16_t vol[16];
} xh_bq79616_info_obj;

// typedef void    (*xh_bq79616_set_ping_obj)                  (void *var,uint32_t mode);
// typedef float   (*xh_bq79616_get_vol_obj)                   (void *var,uint32_t index);
// typedef uint8_t   (*xh_bq79616_get_is_uart_error_obj)         (void *var);
// typedef uint8_t   (*xh_bq79616_get_is_hardware_error_obj)     (void *var);

typedef void (*xh_bq79616_ctrl_ping_obj)(void *var, uint32_t mode);
typedef uint32_t (*xh_bq79616_get_uart_error_obj)(void *var);
typedef uint32_t (*xh_bq79616_get_afe_fault_obj)(void *var);
typedef uint32_t (*xh_bq79616_get_dev_state_obj)(void *var);
typedef uint32_t (*xh_bq79616_get_bal_state_obj)(void *var);
typedef uint32_t (*xh_bq79616_get_tsref_obj)(void *var);
typedef uint32_t (*xh_bq79616_get_aux_bat_vol_obj)(void *var);
typedef uint32_t (*xh_bq79616_get_aux_vref_vol_obj)(void *var);
typedef uint32_t (*xh_bq79616_get_aux_ref4v2_vol_obj)(void *var);
typedef uint32_t (*xh_bq79616_get_gpio_vol_obj)(void *var, uint32_t index);
typedef uint32_t (*xh_bq79616_get_cell_vol_obj)(void *var, uint32_t index);
typedef uint32_t (*xh_bq79616_refresh_all_obj)(void *var, xh_bq79616_info_obj *p_afe_data);

typedef struct _xh_bq79616_t
{
    xh_comm_t core;
    xh_bq79616_ctrl_ping_obj ctrl_ping;
    xh_bq79616_get_uart_error_obj get_uart_error;
    xh_bq79616_get_afe_fault_obj get_afe_fault;
    xh_bq79616_get_dev_state_obj get_dev_state;
    xh_bq79616_get_bal_state_obj get_bal_state;
    xh_bq79616_get_tsref_obj get_tsref;
    xh_bq79616_get_aux_bat_vol_obj get_aux_bat_vol;
    xh_bq79616_get_aux_vref_vol_obj get_aux_vref_vol;
    xh_bq79616_get_aux_ref4v2_vol_obj get_aux_ref4v2_vol;
    xh_bq79616_get_gpio_vol_obj get_gpio_vol;
    xh_bq79616_get_cell_vol_obj get_cell_vol;
    xh_bq79616_refresh_all_obj refresh_all;
} xh_bq79616_t, *p_xh_bq79616_t;

/* 驱动宏定义实现 */
#define REG_XH_BQ79616_DRV(id, var, prio, auto_init, init, deinit, ctrl_ping, get_uart_error, get_afe_fault, get_dev_state, get_bal_state, get_tsref, get_aux_bat_vol, get_aux_vref_vol, get_aux_ref4v2_vol, get_gpio_vol, get_cell_vol, refresh_all) \
    REG_XH_DEV(xh_bq79616_t, id, var, prio, auto_init, init, deinit,                                                                                                                                                                                  \
               (xh_bq79616_ctrl_ping_obj)ctrl_ping,                                                                                                                                                                                                   \
               (xh_bq79616_get_uart_error_obj)get_uart_error,                                                                                                                                                                                         \
               (xh_bq79616_get_afe_fault_obj)get_afe_fault,                                                                                                                                                                                           \
               (xh_bq79616_get_dev_state_obj)get_dev_state,                                                                                                                                                                                           \
               (xh_bq79616_get_bal_state_obj)get_bal_state,                                                                                                                                                                                           \
               (xh_bq79616_get_tsref_obj)get_tsref,                                                                                                                                                                                                   \
               (xh_bq79616_get_aux_bat_vol_obj)get_aux_bat_vol,                                                                                                                                                                                       \
               (xh_bq79616_get_aux_vref_vol_obj)get_aux_vref_vol,                                                                                                                                                                                     \
               (xh_bq79616_get_aux_ref4v2_vol_obj)get_aux_ref4v2_vol,                                                                                                                                                                                 \
               (xh_bq79616_get_gpio_vol_obj)get_gpio_vol,                                                                                                                                                                                             \
               (xh_bq79616_get_cell_vol_obj)get_cell_vol,                                                                                                                                                                                             \
               (xh_bq79616_refresh_all_obj)refresh_all, )
#define EXT_XH_BQ79616_DRV(id) EXT_XH_DEV(xh_bq79616_t, id)

/**
 * @brief BQ设置PING
 *
 * @param [in] id           设备ID
 * @param [in] mode         模式
 * BQ79616_CTRL_WAKEUP: 唤醒
 * BQ79616_CTRL_SHUTDOWN: 关机
 * BQ79616_CTRL_ACTIVE: 运行
 * BQ79616_CTRL_RESET: 复位
 * @details 特殊说明:
 * @par eg: 唤醒BQ
 * @code
 *  xh_bq79616_ctrl_ping(DRV_BQ79616,BQ79616_CTRL_WAKEUP);
 * @endcode
 */
static inline void xh_bq79616_ctrl_ping(void *const id, uint32_t mode)
{
    xh_bq79616_t *p = (xh_bq79616_t *)id;
    p->ctrl_ping(p->core.var, mode);
}
/**
 * @brief 获取串口异常
 *
 * @param [in] id           设备ID
 *
 * @return uint32_t
 *  @retval 1:串口通信错误
 *          0:串口通讯正常
 * @details 特殊说明:   无
 * @par eg:获取串口通讯是否正常
 * @code
 * uint32_t bq79616_is_error;
 * bq79616_is_error = xh_bq79616_get_uart_error(DRV_BQ79616);
 * @endcode
 */
static inline uint32_t xh_bq79616_get_uart_error(void *const id)
{
    xh_bq79616_t *p = (xh_bq79616_t *)id;
    return p->get_uart_error(p->core.var);
}
/**
 * @brief 获取硬件错误
 *
 * @param [in] id       设备ID
 *
 * @return uint32_t
 *  @retval 硬件错误码
 * @details 特殊说明:   无
 * @par eg:获取硬件错误码
 * @code
 * uint32_t afe_error
 * afe_error = xh_bq79616_get_afe_fault(DRV_BQ79616);
 * @endcode
 */
static inline uint32_t xh_bq79616_get_afe_fault(void *const id)
{
    xh_bq79616_t *p = (xh_bq79616_t *)id;
    return p->get_afe_fault(p->core.var);
}
/**
 * @brief 获取设备状态
 *
 * @param [in] id       设备ID
 *
 * @return uint32_t
 *  @retval 设备状态
 * @details 特殊说明:   无
 * @par eg:获取设备状态
 * @code
 * uint32_t dev_status;
 * dev_status = xh_bq79616_get_dev_state(DRV_BQ79616);
 * @endcode
 */
static inline uint32_t xh_bq79616_get_dev_state(void *const id)
{
    xh_bq79616_t *p = (xh_bq79616_t *)id;
    return p->get_dev_state(p->core.var);
}
/**
 * @brief 获取电池状态
 *
 * @param [in] id       设备ID
 *
 * @return uint32_t
 *  @retval 电池状态
 * @details 特殊说明:   无
 * @par eg:获取电池状态
 * @code
 * uint32_t bal_status;
 * bal_status = xh_bq79616_get_bal_state(DRV_BQ79616);
 * @endcode
 */
static inline uint32_t xh_bq79616_get_bal_state(void *const id)
{
    xh_bq79616_t *p = (xh_bq79616_t *)id;
    return p->get_bal_state(p->core.var);
}
/**
 * @brief 获取TSREF电压 ADC值
 *
 * @param [in] id       设备ID
 *
 * @return uint32_t
 *  @retval TSREF电压ADC值
 * @details 特殊说明:   无
 * @par eg:TSREF电压
 * @code
 * uint32_t ad_tsref;
 * ad_tsref = xh_bq79616_get_tsref(DRV_BQ79616);
 * @endcode
 */
static inline uint32_t xh_bq79616_get_tsref(void *const id)
{
    xh_bq79616_t *p = (xh_bq79616_t *)id;
    return p->get_tsref(p->core.var);
}
/**
 * @brief 获取AUX电压 ADC值
 *
 * @param [in] id       设备ID
 *
 * @return uint32_t
 *  @retval AUX电压ADC值
 * @details 特殊说明:   无
 * @par eg:AUX电压
 * @code
 * uint32_t ad_aux;
 * ad_aux = xh_bq79616_get_aux_bat_vol(DRV_BQ79616);
 * @endcode
 */
static inline uint32_t xh_bq79616_get_aux_bat_vol(void *const id)
{
    xh_bq79616_t *p = (xh_bq79616_t *)id;
    return p->get_aux_bat_vol(p->core.var);
}

/**
 * @brief 获取AUX_VREF电压 ADC值
 *
 * @param [in] id       设备ID
 *
 * @return uint32_t
 *  @retval AUX_VREF电压ADC值
 * @details 特殊说明:   无
 * @par eg:AUX_VREF电压
 * @code
 * uint32_t ad_aux_vref;
 * ad_aux_vref = xh_bq79616_get_aux_vref_vol(DRV_BQ79616);
 * @endcode
 */
static inline uint32_t xh_bq79616_get_aux_vref_vol(void *const id)
{
    xh_bq79616_t *p = (xh_bq79616_t *)id;
    return p->get_aux_vref_vol(p->core.var);
}
/**
 * @brief 获取AUX_REF_4V2电压 ADC值
 *
 * @param [in] id       设备ID
 *
 * @return uint32_t
 *  @retval AUX_REF_4V2电压ADC值
 * @details 特殊说明:   无
 * @par eg:AUX_VREF_4V2电压
 * @code
 * uint32_t ad_aux_ref_4v2;
 * ad_aux_ref_4v2 = xh_bq79616_get_aux_ref4v2_vol(DRV_BQ79616);
 * @endcode
 */
static inline uint32_t xh_bq79616_get_aux_ref4v2_vol(void *const id)
{
    xh_bq79616_t *p = (xh_bq79616_t *)id;
    return p->get_aux_ref4v2_vol(p->core.var);
}
/**
 * @brief 获取gpio电压 ADC值
 *
 * @param [in] id       设备ID
 * @param [in] index    GPIO索引
 *
 * @return uint32_t
 *  @retval gpio电压ADC值
 * @details 特殊说明:   无
 * @par eg:获取gpio_1电压
 * @code
 * uint32_t ad_gpio_1;
 * ad_gpio_1 = xh_bq79616_get_gpio_vol(DRV_BQ79616,1);
 * @endcode
 */
static inline uint32_t xh_bq79616_get_gpio_vol(void *const id, uint32_t index)
{
    xh_bq79616_t *p = (xh_bq79616_t *)id;
    return p->get_gpio_vol(p->core.var, index);
}

/**
 * @brief 获取电芯电压 ADC值
 *
 * @param [in] id       设备ID
 * @param [in] index    电芯索引
 *
 * @return uint32_t
 *  @retval 电芯电压ADC值
 * @details 特殊说明:   无
 * @par eg:获取电芯_1电压
 * @code
 * uint32_t ad_cell_1;
 * ad_cell_1 = xh_bq79616_get_cell_vol(DRV_BQ79616,1);
 * @endcode
 */
static inline uint32_t xh_bq79616_get_cell_vol(void *const id, uint32_t index)
{
    xh_bq79616_t *p = (xh_bq79616_t *)id;
    return p->get_cell_vol(p->core.var, index);
}

/**
 * @brief BQ79616数据刷新
 *
 * @param [in] id       设备ID
 * @param [in] p_afe_data    结构体指针
 *
 * @return uint32_t
 *  @retval 0
 * @details 特殊说明:   无
 * @par eg:刷新BQ数据
 * @code
 * xh_bq79616_info_obj afe_data
 * xh_bq79616_refresh_all(DRV_BQ79616,&afe_data);
 * @endcode
 */
static inline uint32_t xh_bq79616_refresh_all(void *const id, xh_bq79616_info_obj *p_afe_data)
{
    xh_bq79616_t *p = (xh_bq79616_t *)id;
    return p->refresh_all(p->core.var, p_afe_data);
}

#endif //__xh_bq79616_H__
