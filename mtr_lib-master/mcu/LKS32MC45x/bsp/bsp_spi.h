/**
 * @brief spi驱动
 * @file bsp_spi.h
 * @version 1.0
 * @author wending.wei 
 * @date 2023-01-31 10:50:22
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2023-01-31 10:50:22 <td>1.0       <td>wending.wei      <td>Created function
 */
#ifndef BSP_SPI_H
#define BSP_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lks32mc45x.h"
#include "lks32mc45x_lib.h"
#include "x_frame.h"
#include "xh_spi.h"

/**
 * @brief spi 片选脚组
 * @details
 */
typedef struct _BSP_SPI_CS_GPIO_GROUP
{
    // cs group
    GPIO_TypeDef*   cs_port;
    uint32_t        cs_pin;
} BSP_SPI_CS_GPIO_GROUP, *P_BSP_SPI_CS_GPIO_GROUP;

/**
 * @brief spi引脚配置组
 * @details 0:sck 1:miso 2:mosi 
 */
typedef struct _BSP_SPI_GPIO_GROUP
{
    GPIO_TypeDef*   port;
    uint32_t        pin;
    uint32_t        clock;
} BSP_SPI_GPIO_GROUP, *P_BSP_SPI_GPIO_GROUP;

/**
 * @brief spi功能配置
 * @details 
 */
typedef struct _BSP_SPI_SPI_PARAMS
{
    SPI_TypeDef*       spi_periph;             ///< spi号 
    uint32_t           spi_clock;              ///< 时钟源
    uint8_t            spi_nvic_irq;           ///< 中断源     
    uint8_t            data_len;               ///< 数据长度   
    uint8_t            clock_phase;            ///< 相位
    uint8_t            clock_polarity;         ///< 极性
    uint16_t           prescale;               ///< 分频
    uint32_t           pre_priority;           ///< 抢占优先级
    uint32_t           sub_priority;           ///< 抢占子优先级
} BSP_SPI_SPI_PARAMS, *P_BSP_SPI_SPI_PARAMS;

typedef struct _BSP_SPI
{
    P_BSP_SPI_GPIO_GROUP     gpio_group;        ///< spi功能脚配置
    P_BSP_SPI_CS_GPIO_GROUP  cs_group;          ///< 片选脚配置
    uint32_t                 cs_pin_num;        ///< 片选脚的总数
    P_BSP_SPI_SPI_PARAMS     params_s;          ///< spi配置

    uint16_t *               tx_buffer;         ///< spi数据缓冲区
    uint16_t *               rx_buffer;         ///< spi数据缓冲区
    uint8_t *                tx_data_8;
    uint16_t *               tx_data_16;
    uint32_t                 tx_data_len;
    uint8_t *                rx_data_8;
    uint16_t *               rx_data_16;
    uint32_t                 rx_data_len;
    uint8_t                  flag_is_block;     ///< 是否阻塞标志位
} BSP_SPI, *P_BSP_SPI;

/**
 * @brief 注册SPI功能
 *
 * @param [in]  dev_id                  驱动ID 
 * @param [in]  prio                    驱动初始化优先级
 * @param [in]  spi_periph              spi号
 * @param [in]  sck_port                sck引脚对应的组
 * @param [in]  sck_pin                 sck引脚号
 * @param [in]  miso_port               miso引脚对应的组
 * @param [in]  miso_pin                miso引脚号
 * @param [in]  mosi_port               mosi引脚对应的组
 * @param [in]  mosi_pin                mosi引脚号
 * @param [in]  data_len                单次发送数据长度
 * @param [in]  clock_polarity_phase    spi模式      
 * @param [in]  prescale                spi时钟分频
 * @param [in]  pre_priority            中断优先级
 * @param [in]  sub_priority            中断子优先级
 * @param [in]  buffer_len              buff大小
 * @param [in]  flag_is_block           阻塞模式标志位
 * @param [in]  cs_gpio_param...        片选引脚配置组 
 *
 * @details 提示一些注意事项或必要的技术细节
 * @par 示例: 注册spi功能
 * @code
 *   REG_XH_SPI(DRV_SPI,PEI_DRV_SPI,SPI1,GPIOB,GPIO_PIN_13,GPIOB,GPIO_PIN_14,GPIOB,GPIO_PIN_15,SPI_FRAMESIZE_8BIT,SPI_CK_PL_LOW_PH_1EDGE ,SPI_PSC_128,2,2,20,1,
 *       {GPIOA,GPIO_PIN_1},  //CS_1
 *   )
 * @endcode
 */
#define REG_XH_SPI(dev_id, prio, spi_periph, sck_port, sck_pin, miso_port, miso_pin, mosi_port, mosi_pin, data_len,    \
                   clock_polarity,clock_phase, prescale, pre_priority, sub_priority, buffer_len, flag_is_block,        \
                   cs_gpio_param...)                                                                                   \
        uint16_t tx_buffer_##dev_id[buffer_len];                                                                       \
        uint16_t rx_buffer_##dev_id[buffer_len];                                                                       \
        BSP_SPI_GPIO_GROUP bsp_spi_gpio_group##dev_id[] = {                                                            \
            {sck_port, sck_pin, NULL},                                                                                 \
            {miso_port, miso_pin, NULL},                                                                               \
            {mosi_port, mosi_pin, NULL},                                                                               \
        };                                                                                                              \
        BSP_SPI_CS_GPIO_GROUP cs_group_##dev_id[] = {cs_gpio_param};                                                   \
        BSP_SPI_SPI_PARAMS bsp_spi_params##dev_id = {                                                                  \
            spi_periph,                                                                                                \
            0,                                                                                                         \
            0,                                                                                                         \
            data_len,                                                                                                  \
            clock_polarity,                                                                                            \
            clock_phase,                                                                                               \
            prescale,                                                                                                  \
            pre_priority,                                                                                              \
            sub_priority,                                                                                              \
        };                                                                                                             \
        BSP_SPI _##dev_id = {                                                                                          \
            bsp_spi_gpio_group##dev_id,                                                                                \
            cs_group_##dev_id,                                                                                         \
            sizeof(cs_group_##dev_id) / sizeof(cs_group_##dev_id[0]),                                                  \
            &bsp_spi_params##dev_id,                                                                                   \
            tx_buffer_##dev_id,                                                                                        \
            rx_buffer_##dev_id,                                                                                        \
            NULL,                                                                                                      \
            NULL,                                                                                                      \
            0,                                                                                                         \
            NULL,                                                                                                      \
            NULL,                                                                                                      \
            0,                                                                                                         \
            flag_is_block,                                                                                             \
        };                                                                                                             \
        void spi_periph##_IRQHandler(void)                                                                             \
        {                                                                                                              \
            _bsp_spi_irq_handler(&_##dev_id);                                                                          \
        }                                                                                                              \
        REG_XH_SPI_DRV(dev_id, &_##dev_id, prio, DEV_AUTO_INIT, bsp_spi_init, bsp_spi_deinit, bsp_spi_rw_data,         \
                        bsp_spi_write_data, bsp_spi_read_data, bsp_spi_get_is_busy, bsp_spi_set_cs_level);


extern void bsp_spi_init(P_BSP_SPI spi);
extern void bsp_spi_deinit(P_BSP_SPI spi);
extern void _bsp_spi_irq_handler(P_BSP_SPI spi);
extern void bsp_spi_set_cs_level(P_BSP_SPI spi, uint32_t cs_index, uint32_t level);
extern uint8_t bsp_spi_get_is_busy(P_BSP_SPI spi);

extern void bsp_spi_rw_data(P_BSP_SPI spi, void *tx_data, void *rx_data, uint32_t len);
extern void bsp_spi_write_data(P_BSP_SPI spi, void *tx_data, uint32_t len);
extern void bsp_spi_read_data(P_BSP_SPI spi, void *rx_data, uint32_t len);





#ifdef __cplusplus
}
#endif

#endif  // BSP_SPI_H
