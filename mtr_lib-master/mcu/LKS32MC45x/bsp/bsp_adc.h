/**
 * @brief ADC驱动
 * @file bsp_adc.h
 * @version 1.0
 * @author peixiang.qiu
 * @date 2023-01-08 21:35:06
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2023-01-08 21:35:06 <td>1.0       <td>peixiang.qiu      <td>Created function
 */

#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__
#ifdef __cplusplus
extern
{
#endif
#include "lks32mc45x.h"
#include "lks32mc45x_lib.h"
#include "stdbool.h"
#include "stdint.h"
#include "xh_adc.h"

#define ADC_GAIN_LOW_DAT_3_3 0xAA  ///< 低增益，量程3.3
#define ADC_GAIN_HIGH_DAT_2_2 0xAB ///< 高增益，量程2.2
#define ADC_CHN_NULL 16
#define DMA_NULL ((DMA_RegTypeDef *)0xFF)
#define ADC_IRQ_DISABLE 0xFF

#define ADC_IE_SF1_AND_SF2 (BIT0 | BIT1) ///< 第一段和第二段采样完成中断标志

    typedef void (*adc_irq_cb)(void);

    typedef struct _BSP_ADC_CH_PARAM
    {
        uint8_t p_adc_ch;
        uint8_t n_adc_ch;
        uint16_t gain; ///< 增益
    } BSP_ADC_CH_PARAM, *P_BSP_ADC_CH_PARAM;

    typedef struct _BSP_ADC_DMA_PARAMS
    {
        DMA_RegTypeDef *dma_ch;
        uint8_t dma_req_en; // DMA请求
        int16_t *mem_addr;
        uint32_t mem_len;
    } BSP_ADC_DMA_PARAMS, *P_BSP_ADC_DMA_PARAMS;

    typedef struct _BSP_ADC
    {
        ADC_TypeDef *adc_periph;
        uint32_t sys_module;
        //				uint32_t* adc_data_reg;
        uint32_t trigger_source;
        uint32_t continue_mode;
        uint8_t adc_ovsr; ///< 过采样率，触发一次进行2^n次采样
        uint8_t adc_tcnt; ///< 触发次数,触发几次进行一次采样
        uint8_t adc_nsmp; ///< 触发模式，单段或双段
        uint8_t adc_s1;   ///< 第一段触发个数
        uint8_t adc_s2;   ///< 第二段触发个数
        uint8_t priority; // 中断优先级
        uint32_t TRIM_VAL_ADC_DC0;
        uint32_t TRIM_VAL_ADC_AMC0;
        uint32_t TRIM_VAL_ADC_DC1;
        uint32_t TRIM_VAL_ADC_AMC1;
        P_BSP_ADC_CH_PARAM p_ch_param;
        P_BSP_ADC_DMA_PARAMS p_dma_param;
        uint16_t ch_num;

        IRQn_Type nvic_irq;
        uint16_t adc_irq_type;
        adc_irq_cb adc_cb; // ADC中断回调函数
    } BSP_ADC, *P_BSP_ADC;

//#define REG_XH_ADC(dev_id, prio, adc_periph, trigger_source, continue_mode, adc_ovsr, adc_tcnt, adc_nsmp, adc_s1,  \
//                   adc_s2, dma_ch, priority, adc_irq_type, adc_param...)                                           \
//    BSP_ADC_CH_PARAM gbsp_adc_ch_param_##dev_id[] = {adc_param};                                                   \
//    int16_t g_bsp_adc_value_##dev_id[sizeof(gbsp_adc_ch_param_##dev_id) / sizeof(gbsp_adc_ch_param_##dev_id[0])]; \
//    BSP_ADC_DMA_PARAMS gbsp_adc_param_##dev_id = {                                                                 \
//        dma_ch,                                                                                                    \
//        DMA_REQ_##adc_periph,                                                                                      \
//        g_bsp_adc_value_##dev_id,                                                                                  \
//        sizeof(gbsp_adc_ch_param_##dev_id) / sizeof(gbsp_adc_ch_param_##dev_id[0]),                                \
//    };                                                                                                             \
//    BSP_ADC _##dev_id = {                                                                                          \
//        adc_periph,                                                                                                \
//        SYS_MODULE_##adc_periph,                                                                                   \
//        trigger_source,                                                                                            \
//        continue_mode,                                                                                             \
//        adc_ovsr,                                                                                                  \
//        adc_tcnt,                                                                                                  \
//        adc_nsmp,                                                                                                  \
//        adc_s1,                                                                                                    \
//        adc_s2,                                                                                                    \
//        priority,                                                                                                  \
//        TRIM_VAL_##adc_periph##_DC0,                                                                               \
//        TRIM_VAL_##adc_periph##_AMC0,                                                                              \
//        TRIM_VAL_##adc_periph##_DC1,                                                                               \
//        TRIM_VAL_##adc_periph##_AMC1,                                                                              \
//        gbsp_adc_ch_param_##dev_id,                                                                                \
//        &gbsp_adc_param_##dev_id,                                                                                  \
//        sizeof(gbsp_adc_ch_param_##dev_id) / sizeof(gbsp_adc_ch_param_##dev_id[0]),                                \
//        adc_periph##_IRQn,                                                                                         \
//        adc_irq_type,                                                                                              \
//        NULL,                                                                                                      \
//    };                                                                                                             \
//    void adc_periph##_IRQHandler(void)                                                                             \
//    {                                                                                                              \
//        bsp_adc_irp_handle(&_##dev_id);                                                                            \
//    }                                                                                                              \
//    REG_XH_ADC_DRV(dev_id, &_##dev_id, prio, DEV_AUTO_INIT, bsp_adc_init, bsp_adc_deinit, bsp_adc_set_start,       \
//                   bsp_adc_read_value, bsp_adc_read_all_value, bsp_adc_enable, bsp_adc_disable,                    \
//                   bsp_adc_change_memory_address, bsp_adc_reg_irq)

#define REG_XH_ADC(dev_id, prio)                                                                       \
    BSP_ADC _##dev_id = {};                                                                                        \
    REG_XH_ADC_DRV(dev_id, &_##dev_id, prio, DEV_AUTO_INIT, bsp_adc_init, bsp_adc_deinit, bsp_adc_set_start,       \
                   bsp_adc_read_value, bsp_adc_read_all_value, bsp_adc_enable, bsp_adc_disable,                    \
                   bsp_adc_change_memory_address, bsp_adc_reg_irq)

    extern void bsp_adc_init(P_BSP_ADC adc);
    extern void bsp_adc_deinit(P_BSP_ADC adc);
    extern void bsp_adc_set_start(P_BSP_ADC adc);
    extern void bsp_adc_enable(P_BSP_ADC adc);
    extern void bsp_adc_disable(P_BSP_ADC adc);
    extern void bsp_adc_change_memory_address(P_BSP_ADC adc, int16_t *memory_address, uint32_t num);
    extern int32_t bsp_adc_read_value(P_BSP_ADC p_adc_group, uint32_t index);
    extern uint32_t bsp_adc_read_all_value(P_BSP_ADC adc, int16_t *p_adc_value, uint32_t buf_len);
    extern void bsp_adc_reg_irq(P_BSP_ADC adc, void (*irq_cb)(void));

    extern void bsp_adc_irp_handle(P_BSP_ADC adc); // 非用户API
#ifdef __cplusplus
}
#endif
#endif //__BSP_ADC_H__
