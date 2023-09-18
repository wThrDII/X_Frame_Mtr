/**
 * @brief I2C驱动接口
 * @file xh_i2c.h
 * @version 1.0
 * @author zain.zhou
 * @date 2022-09-06 17:48:50
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-09-06 17:48:50 <td>1.0       <td>zain.zhou      <td>Created function
 */

#ifndef xh_i2c_h
#define xh_i2c_h
#ifdef __cplusplus
extern "C"
{
#endif

#include "xh_core.h"

    typedef uint8_t (*xh_i2c_write_obj)(void *id, uint8_t *p_buffer, uint32_t w_addr, uint32_t len);
    typedef uint8_t (*xh_i2c_read_obj)(void *id, uint8_t *p_buffer, uint32_t r_addr, uint32_t len);
    typedef uint8_t (*xh_i2c_get_is_busy_obj)(void *id);
    typedef uint8_t (*xh_i2c_set_slave_addr_obj)(void *id, uint8_t slave_addr);

    typedef struct _xh_i2c_t
    {
        xh_comm_t core;
        xh_i2c_write_obj i2c_write;
        xh_i2c_read_obj i2c_read;
        xh_i2c_get_is_busy_obj i2c_get_is_busy;
        xh_i2c_set_slave_addr_obj i2c_set_slave_addr;
    } xh_i2c_t, *p_xh_i2c_t;

/* 驱动宏定义实现 */
#define REG_XH_I2C_DRV(id, var, prio, auto_init, init, deinit, i2c_write, i2c_read, i2c_get_is_busy, i2c_set_slave_addr) \
    REG_XH_DEV(xh_i2c_t, id, var, prio, auto_init, init, deinit,                                        \
               (xh_i2c_write_obj)i2c_write,                                                             \
               (xh_i2c_read_obj)i2c_read,                                                               \
               (xh_i2c_get_is_busy_obj)i2c_get_is_busy,                                                 \
               (xh_i2c_set_slave_addr_obj)i2c_set_slave_addr, )

#define EXT_XH_I2C_DRV(id) EXT_XH_DEV(xh_i2c_t, id)

    /**
     * @brief 阻塞式I2C读函数
     *
     * @param [in] id            驱动ID
     * @param [in] p_buffer      写入的数据
     * @param [in] w_addr        写入设备的起始地址
     * @param [in] len           写入长度
     *
     * @return uint8_t
     *  @retval
     * @details 特殊说明:
     * @par eg:使用设备ID:DRV_I2C 往从设备 地址为0x00写入8个字节
     * @code
     *    uint8_t str[8] = {0xAA,1,2,3,4,5,6,7};
     *    xh_i2c_write(DRV_I2C,str,0x00,8);
     * @endcode
     */
    static inline uint8_t xh_i2c_write(void *const id, uint8_t *p_buffer, uint32_t w_addr, uint32_t len)
    {
        xh_i2c_t *p = (xh_i2c_t *)id;
        return p->i2c_write(p->core.var, p_buffer, w_addr, len);
    }
    /**
     * @brief 阻塞式I2C写函数
     *
     * @param [in] id            驱动ID
     * @param [in] p_buffer      读取的数据
     * @param [in] r_addr        读的地址
     * @param [in] len           读的长度
     *
     * @return uint8_t
     *  @retval
     * @details 特殊说明:
     * @par eg:使用设备ID:DRV_I2c 从设备 地址为0x00读取8个字节到数组str
     * @code
     *    uint8_t str[8] = {0};
     *    xh_i2c_write(DRV_I2C,str,0x00,8);
     * @endcode
     */
    static inline uint8_t xh_i2c_read(void *const id, uint8_t *p_buffer, uint32_t r_addr, uint32_t len)
    {
        xh_i2c_t *p = (xh_i2c_t *)id;
        return p->i2c_read(p->core.var, p_buffer, r_addr, len);
    }
    /**
     * @brief  获取IIC是否忙
     * 
     * @param [in] id           驱动ID       
     * 
     * @return uint8_t 
     *  @retval 是否忙 1: 忙 0: 空闲
     * @details 特殊说明: 
     * @par eg: 获取IIC是否为忙状态
     * @code
     * uint8_t is_busy;
     * while (1) {
     *      is_busy = xh_i2c_get_is_busy(DRV_I2C);
     *      if (is_busy == 0) {
     *          break;
     *      }
     * }
     * @endcode
     */
    static inline uint8_t xh_i2c_get_is_busy(void *const id)
    {
        xh_i2c_t *p = (xh_i2c_t *)id;
        return p->i2c_get_is_busy(p->core.var);
    }
    /**
     * @brief 修改I2C设备对应的从机设备地址
     *
     * @param [in] id            IIC设备ID
     * @param [in] slave_addr    IIC从机地址
     *
     * @details 特殊说明: 无
     * @par eg: 修改设备ID:DRV_I2C 的从机设备地址为 A0
     * @code
     *    xh_i2c_set_slave_addr(DRV_I2C,0xA0);
     * @endcode
     */
    static inline void xh_i2c_set_slave_addr(void *id, uint8_t slave_addr)
    {
        xh_i2c_t *p = (xh_i2c_t *)id;
        p->i2c_set_slave_addr(p->core.var, slave_addr);
    }

#ifdef __cplusplus
}

#endif
#endif /* xh_i2c_h */
