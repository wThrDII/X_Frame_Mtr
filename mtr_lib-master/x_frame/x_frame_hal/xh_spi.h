/**
 * @brief
 * @file xh_spi.h
 * @version 1.0
 * @author zain.zhou
 * @date 2022-09-08 14:08:36
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-09-08 14:08:36 <td>1.0       <td>zain.zhou      <td>Created function
 */
#ifndef xh_spi_h
#define xh_spi_h
#ifdef __cplusplus
extern "C"
{
#endif

#include "xh_core.h"
    typedef void (*xh_spi_rw_obj)(void *var, void *tx_data, void *rx_data, uint32_t len);
    typedef void (*xh_spi_write_obj)(void *var, void *tx_data, uint32_t len);
    typedef void (*xh_spi_read_obj)(void *var, void *rx_data, uint32_t len);
    typedef uint8_t (*xh_spi_is_busy_obj)(void *var);
    typedef void (*xh_spi_set_cs_level_obj)(void *var, uint32_t cs_index, uint32_t level);

    typedef struct _xh_spi_t
    {
        xh_comm_t core;
        // xh_spi_rw_obj spi_rw;
        // xh_spi_rw_16_obj spi_rw_16;
        xh_spi_rw_obj spi_rw;
        xh_spi_write_obj psi_write;
        xh_spi_read_obj spi_read;
        xh_spi_is_busy_obj is_busy;
        xh_spi_set_cs_level_obj set_cs_level;
    } xh_spi_t, *p_xh_spi_t;

/* 驱动宏定义实现 */
#define REG_XH_SPI_DRV(id, var, prio, auto_init, init, deinit, spi_rw, psi_write, spi_read, is_busy,      \
                       set_cs_level)                                                                                 \
    REG_XH_DEV(xh_spi_t, id, var, prio, auto_init, init, deinit, (xh_spi_rw_obj)spi_rw, \
               (xh_spi_write_obj)psi_write, (xh_spi_read_obj)spi_read, (xh_spi_is_busy_obj)is_busy,                  \
               (xh_spi_set_cs_level_obj)set_cs_level)

#define EXT_XH_SPI_DRV(id) EXT_XH_DEV(xh_spi_t, id)

    /**
     * @brief xh_spi_rw SPI读写接口(共用) 发送接收数据类型为8位/16位（一个字节/两个字节）
     *
     * @param [in] id            SPI设备ID
     * @param [in] tx_data       发送数据缓存区 数据类型 8/16位
     * @param [in] rx_data       接收数据缓存区 数据类型 8/16位
     * @param [in] len           发送接收数据长度
     *
     * @details 特殊说明: 该接口既可以用于发数据也可以用于接数据，使用功能时，不能传NULL指针
     *          而且接收缓存区的大小应该不小于len值。
     * @par eg: 使用spi：DRV_SPI 发送接收8位数据
     * @code
     *      uint8_t tx_data[8] = {0xAA,1,2,3,4,5,6,7};
     *      uint8_t rx_data[8] = {0};
     *      发送数据
     *      xh_spi_rw(DRV_SPI,tx_data,rx_data,8);
     *      接收数据
     *      xh_spi_rw(DRV_SPI,tx_data,rx_data,8);
     * @endcode
     */
    static inline void xh_spi_rw(void *id, void *tx_data, void *rx_data, uint32_t len)
    {
        xh_spi_t *p = (xh_spi_t *)id;
        p->spi_rw(p->core.var, tx_data, rx_data, len);
    }
    /**
     * @brief
     *
     * @param [in] id            SPI设备ID
     * @param [in] tx_data       发送数据缓存区
     * @param [in] len           接收数据长度
     *
     * @details 特殊说明: 该接口不读取数据
     * @par eg:使用SPI:DRV_SPI 发送8/16字节数据
     * @code
     *    uint8_t tx_data[8] = {0};
     *    xh_spi_read(DRV_SPI,tx_data,8);
     * @endcode
     */
    static inline void xh_spi_write(void *id, void *tx_data, uint32_t len)
    {
        xh_spi_t *p = (xh_spi_t *)id;
        p->psi_write(p->core.var, tx_data, len);
    }

    /**
     * @brief xh_spi_read SPI读数据函数
     *
     * @param [in] id            SPI设备ID
     * @param [in] rx_data       接收数据缓存区
     * @param [in] len           接收数据长度
     *
     * @details 特殊说明: 该接口默认发送的数据为0xFF/0XFFFF
     * @par eg:使用SPI:DRV_SPI 接收8/16字节数据
     * @code
     *    uint8_t rx_data[8] = {0};
     *    xh_spi_read(DRV_SPI,rx_data,8);
     * @endcode
     */
    static inline void xh_spi_read(void *id, void *rx_data, uint32_t len)
    {
        xh_spi_t *p = (xh_spi_t *)id;
        p->spi_read(p->core.var, rx_data, len);
    }

    /**
     * @brief 获取spi总线状态
     *
     * @param [in] id            SPI设备ID
     *
     * @return uint8_t        1: 总线忙  0：总线空闲
     *  @retval
     * @details 特殊说明:
     *          GD32F30x系列的没有这个功能，不要在GD32F30x系列芯片上的工程调用该接口
     * @par eg: 获取SPI:DRV_SPI 总线状态
     * @code
     *    uint8_t sta_bus = 10;
     *    sta_bus =  xh_spi_is_busy(DRV_SPI);
     * @endcode
     */
    static inline uint8_t xh_spi_is_busy(void *id)
    {
        xh_spi_t *p = (xh_spi_t *)id;
        return p->is_busy(p->core.var);
    }
    /**
     * @brief 设置CS信号电平
     * 
     * @param [in] id       :SPI设备ID     
     * @param [in] cs_index :CS引脚索引     
     * @param [in] level    :CS引脚电平
     * 
     * @details 特殊说明: 无
     * @par eg: 设置CS引脚为低电平
     * @code
     *    xh_spi_set_cs_level(DRV_SPI,GD25Q_CS,0);
     * @endcode
     */
    static inline void xh_spi_set_cs_level(void *id, uint32_t cs_index, uint32_t level)
    {
        xh_spi_t *p = (xh_spi_t *)id;
        p->set_cs_level(p->core.var, cs_index, level);
    }

#ifdef __cplusplus
}
#endif
#endif /* xh_spi_h */
