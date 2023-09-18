/**
 * @brief ADC驱动接口
 * @file xh_adc.h
 * @version 3.0
 * @author Xiao Shulong
 * @date 2022-07-08 17:57:31
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-08 17:57:31 <td>1.0       <td>Xiao Shulong      <td>Created function
 * <tr><td>2022-07-15 18:53:31 <td>2.0       <td>Xiao Shulong      <td>修改ADC读接口
 * <tr><td>2022-10-08 20:53:42 <td>1.0       <td>peixiang.qiu      <td>增加ADC读所有数据接口,ADC注册中断回调函接口
 */

#ifndef xh_adc_h
#define xh_adc_h
#ifdef __cplusplus
extern "C"
{
#endif

#include "xh_core.h"

    typedef void (*xh_adc_start_obj)(void *var);
    typedef int32_t (*xh_adc_read_val_obj)(void *var, uint32_t index);
    typedef uint32_t (*xh_adc_read_all_val_obj)(void *var, void *p_buf, uint32_t buf_len);
    typedef void (*xh_adc_enable_obj)(void *var);
    typedef void (*xh_adc_disable_obj)(void *var);
    typedef void (*xh_adc_set_buf_obj)(void *var, void *p_buf, uint32_t len);
    typedef void (*xh_adc_reg_irq_obj)(void *val, void (*irq_cb)(void));
    typedef struct _xh_adc_t
    {
        xh_comm_t core;
        xh_adc_start_obj adc_start;
        xh_adc_read_val_obj adc_read;
        xh_adc_read_all_val_obj adc_read_all;
        xh_adc_enable_obj adc_enable;
        xh_adc_disable_obj adc_disable;
        xh_adc_set_buf_obj adc_set_buf;
        xh_adc_reg_irq_obj reg_irq;
    } xh_adc_t, *p_xh_adc_t;

/**
 * @brief 注册ADC驱动
 *
 * @param [in] id            驱动ID
 * @param [in] var           驱动参数变量
 * @param [in] prio          初始化优先级
 * @param [in] auto_init     自动初始化
 * @param [in] init          初始化函数
 * @param [in] deinit        反初始化函数
 * @param [in] adc_start     开始转换函数
 * @param [in] adc_read      读数据函数
 * @param [in] adc_read_all  读所有数据函数
 * @param [in] adc_enable    使能
 * @param [in] adc_disable   失能
 * @param [in] adc_set_buf   更改缓存区
 *
 * @details
 */
#define REG_XH_ADC_DRV(id, var, prio, auto_init, init, deinit, adc_start, adc_read, adc_read_all, adc_enable,       \
                       adc_disable, adc_set_buf, reg_irq)                                                           \
    REG_XH_DEV(xh_adc_t, id, var, prio, auto_init, init, deinit, (xh_adc_start_obj)adc_start,                       \
               (xh_adc_read_val_obj)adc_read, (xh_adc_read_all_val_obj)adc_read_all, (xh_adc_enable_obj)adc_enable, \
               (xh_adc_disable_obj)adc_disable, (xh_adc_set_buf_obj)adc_set_buf, (xh_adc_reg_irq_obj)reg_irq)

#define EXT_XH_ADC_DRV(id) EXT_XH_DEV(xh_adc_t, id)

    /**
     * @brief ADC开始转换
     *
     * @param [in] id   驱动ID
     *
     * @details 特殊说明:
     * 软件触发模式下，调用该函数启动ADC转换
     * @par eg:启动ADC转换
     * @code
     *      xh_adc_start(DRV_ADC)
     * @endcode
     */
    static inline void xh_adc_start(void *const id)
    {
        xh_adc_t *p = (xh_adc_t *)id;
        p->adc_start(p->core.var);
    }

    /**
     * @brief ADC读数据
     *
     * @param [in] id   驱动ID
     * @param [in] index    索引
     *
     * @return uint32_t
     *  @retval 返回读取的通道值
     * @details 特殊说明:
     * @par eg:读取驱动ID为DRV_ADC，通道名为ADC_CH_NAME的ADC通道
     * @code
     *      uint16_t adc_value[4] = {0};
     *      void adc_task(void)
     *      {
     *          adc_value[0] = xh_adc_read_value(DRV_ADC, ADC_CH_NAME);
     *      }
     *      REG_TASK(1,1,adc_task)
     * @endcode
     */
    static inline int32_t xh_adc_read_value(void *const id, uint32_t index)
    {
        xh_adc_t *p = (xh_adc_t *)id;
        return p->adc_read(p->core.var, index);
    }

    /**
     * @brief ADC读所有通道数据
     *
     * @param [in] id       驱动ID
     * @param [in] p_buf    读缓存区,LKS32平台数据类型为int16_t,其他平台数据类型为uint16_t
     * @param [in] buf_len  用户定义的数组缓存区大小，若小于通道个数，则只读用户定义的缓存个数
     *
     * @return uint32_t
     *  @retval 读数据成功返回0，否则返回1
     * @details 特殊说明:该接口适用于ARM平台的ADC注入组和TI平台
     * @par eg:在一个1ms的任务中读取驱动ID为DRV_ADC的所有通道数据存到adc_value[4]中
     * @code
     *    uint16_t adc_value[4] = {0};
     *    uint32_t state;
     *    void adc_task(void)
     *    {
     *      state = xh_adc_read_all_value(DRV_ADC, adc_value, 4);
     *    }
     *    REG_TASK(1,1,adc_task)
     * @endcode
     */
    static inline uint32_t xh_adc_read_all_value(void *const id, void *p_buf, uint32_t buf_len)
    {
        xh_adc_t *p = (xh_adc_t *)id;
        return p->adc_read_all(p->core.var, p_buf, buf_len);
    }

    /**
     * @brief   ADC使能
     *
     * @param [in] id   驱动ID
     *
     * @details 特殊说明:
     * @par eg:使能驱动ID为DRV_ADC的ADC
     * @code
     *      xh_adc_enable(DRV_ADC);
     * @endcode
     */
    static inline void xh_adc_enable(void *const id)
    {
        xh_adc_t *p = (xh_adc_t *)id;
        p->adc_enable(p->core.var);
    }

    /**
     * @brief ADC失能
     *
     * @param [in] id   驱动ID
     *
     * @details 特殊说明:
     * @par eg:失能驱动ID为DRV_ADC的ADC
     * @code
     *      xh_adc_disable(DRV_ADC);
     * @endcode
     */
    static inline void xh_adc_disable(void *const id)
    {
        xh_adc_t *p = (xh_adc_t *)id;
        p->adc_disable(p->core.var);
    }

    /**
     * @brief ADC更改缓存区
     *
     * @param [in] id       驱动ID
     * @param [in] p_buf    更改的缓存区指针,LKS32平台数据类型为int16_t,其他平台数据类型为uint16_t
     * @param [in] len      读取数据的个数
     *
     * @details 特殊说明:该接口适用于ARM平台的ADC规则组
     *
     * @par eg:通道个数为10，读取通道的数据放到adc_value[10]中
     * @code
     *      uint16_t adc_value[10] = {0};
     *      void test_init(void){
     *          xh_adc_set_buf(DRV_ADC, adc_value, 10);
     *      }
     *      REG_INIT_MOD(test_init)
     * @endcode
     */
    static inline void xh_adc_set_buf(void *const id, void *p_buf, uint32_t len)
    {
        xh_adc_t *p = (xh_adc_t *)id;
        p->adc_set_buf(p->core.var, p_buf, len);
    }

    /**
     * @brief ADC注册中断回调函数
     *
     * @param [in] id            驱动ID
     * @param [in] irq_cb        回调函数
     *
     * @details 特殊说明:
     * @par eg:注册一个ADC中断回调函数，函数名为adc_irq,在中断中记录进入中断的次数count
     * @code
     *      void adc_irq(void){
     *          static uint8_t count = 0;
     *          count ++;
     *      }
     *      void test_init(void){
     *          xh_adc_reg_irq(DRV_ADC, adc_irq);
     *      }
     *      REG_INIT_MOD(test_init)
     *
     * @endcode
     */
    static inline void xh_adc_reg_irq(void *id, void (*irq_cb)(void))
    {
        xh_adc_t *p = (xh_adc_t *)id;
        p->reg_irq(p->core.var, irq_cb);
    }

#ifdef __cplusplus
}
#endif
#endif /* xh_adc_h */
