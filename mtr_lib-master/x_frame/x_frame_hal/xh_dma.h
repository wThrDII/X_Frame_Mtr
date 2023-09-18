/**
 * @brief 本文件接口只适用于DSP
 * @file xh_dma.h
 * @version 1.0
 * @author peixiang.qiu
 * @date 2022-09-21 21:09:31
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-09-21 21:09:31 <td>1.0       <td>peixiang.qiu      <td>Created function
 * <tr><td>2022-10-23 19:22:41 <td>1.0       <td>peixiang.qiu      <td>xh_dma_set_buf接口增加buf_len参数和错误码返回值
 */

#ifndef xh_dma_h
#define xh_dma_h
#ifdef __cplusplus
extern "C"
{
#endif

#include "xh_core.h"

    typedef void (*xh_dma_enable_obj)(void *var);
    typedef void (*xh_dma_disable_obj)(void *var);
    typedef uint32_t (*xh_dma_set_buf_obj)(void *var, const void * p_buf, uint32_t buf_len);
    typedef void (*xh_dma_set_size_obj)(void *var, uint16_t burst_size, uint32_t transfer_size);
    typedef void (*xh_dma_reg_irq_obj)(void *var, void (*dma_cb)(void));

    typedef struct _xh_dma_t
    {
        xh_comm_t core;
        xh_dma_enable_obj dma_enable;
        xh_dma_disable_obj dma_disable;
        xh_dma_set_buf_obj dma_set_buf;
        xh_dma_set_size_obj dma_set_size;
        xh_dma_reg_irq_obj dma_reg_irq;
    } xh_dma_t, *p_xh_dma_t;

#define REG_XH_DMA_DRV(id, var, prio, auto_init, init, deinit, dma_enable, dma_disable, dma_set_buf, dma_set_size, dma_reg_irq) \
    REG_XH_DEV(xh_dma_t, id, var, prio, auto_init, init, deinit, (xh_dma_enable_obj)dma_enable,                                 \
               (xh_dma_disable_obj)dma_disable, (xh_dma_set_buf_obj)dma_set_buf, (xh_dma_set_size_obj)dma_set_size, (xh_dma_reg_irq_obj)dma_reg_irq)

#define EXT_XH_DMA_DRV(id) EXT_XH_DEV(xh_dma_t, id)

    /**
     * @brief DMA使能，只适用于DSP
     *
     * @param [in] id   驱动ID
     *
     * @details 特殊说明:DMA初始化完成已经使能，不需要调用此接口
     * @par eg:使能驱动ID为DRV_ADC的DMA
     * @code
     *  xh_dma_enable(DRV_ADC);
     * @endcode
     */
    static inline void xh_dma_enable(void *const id)
    {
        xh_dma_t *p = (xh_dma_t *)id;
        p->dma_enable(p->core.var);
    }

    /**
     * @brief DMA禁能，只适用于DSP
     *
     * @param [in] id   驱动ID
     *
     * @details 特殊说明:
     * @par eg:禁能驱动ID为DRV_ADC的DMA
     * @code
     *  xh_dma_disable(DRV_ADC);
     * @endcode
     */
    static inline void xh_dma_disable(void *const id)
    {
        xh_dma_t *p = (xh_dma_t *)id;
        p->dma_disable(p->core.var);
    }

    /**
     * @brief DMA更改缓存区，只适用于DSP
     *
     * @param [in] id       驱动ID
     * @param [in] p_buf    更改的缓存区指针
     * @param [in] buf_len  用户定义的数组缓存区大小，用于判断用户设置的缓存区是否过小，导致越界
     *
     * @return uint32_t
     *  @retval 设置缓存区成功返回0，否则返回1（失败原因为设置的缓存区大小小于DMA传输的大小，会导致地址越界）
     * @details 特殊说明:
     * @par eg:修改驱动ID为DRV_ADC的DMA的缓存区
     * @code
     *  uint16_t adc_val[48] = {0};
     *  uint32_t state;
     *  state = xh_dma_set_buf(DRV_ADC, &adc_val, 48);
     * @endcode
     */
    static inline uint32_t xh_dma_set_buf(void *const id, const void *p_buf, uint32_t buf_len)
    {
        xh_dma_t *p = (xh_dma_t *)id;
        return p->dma_set_buf(p->core.var, p_buf, buf_len);
    }

    /**
     * @brief 设置DMA传输大小，只适用于DSP
     *
     * @param [in] id               驱动ID
     * @param [in] burst_size       一组传输的数据个数
     * @param [in] transfer_size    传输的组数
     *
     * @details 特殊说明: burst_size的大小计算是以16位数据为单位
     * @par eg:设置驱动ID为DRV_ADC的DMA一组传输10个，传输4组
     * @code
     *    xh_dma_set_size(DRV_ADC, 10, 4);
     * @endcode
     */
    static inline void xh_dma_set_size(void *const id, uint16_t burst_size, uint32_t transfer_size)
    {
        xh_dma_t *p = (xh_dma_t *)id;
        p->dma_set_size(p->core.var, burst_size, transfer_size);
    }

    /**
     * @brief 注册DMA中断回调函数，只适用于DSP
     *
     * @param [in] id       驱动ID
     * @param [in] irq_cb   DMA回调函数
     *
     * @details 特殊说明:
     *      回调函数类型：
     *      void (*dma_cb)(void);
     * @par eg:注册一个回调函数
     * @code
     *      xh_dma_reg_irq(DRV_ADC, dma_task);
     *      void dma_task(void)
     *      {
     *      }
     * @endcode
     */
    static inline void xh_dma_reg_irq(void *const id, void (*irq_cb)(void))
    {
        xh_dma_t *p = (xh_dma_t *)id;
        p->dma_reg_irq(p->core.var, irq_cb);
    }
#ifdef __cplusplus
}
#endif
#endif /* xh_dma_h */
