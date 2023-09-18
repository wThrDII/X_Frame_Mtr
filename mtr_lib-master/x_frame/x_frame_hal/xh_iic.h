#ifndef xh_iic_h
#define xh_iic_h
#ifdef __cplusplus
extern "C" {
#endif

#include "xh_core.h"


typedef void (*xh_iic_write_obj)(void *id, uint32_t reg_addr, uint8_t *tx_data,uint32_t data_len);
typedef void (*xh_iic_read_obj)(void *id, uint32_t reg_addr, uint8_t *rx_data,uint32_t data_len);
typedef uint8_t (*xh_iic_is_busy_obj)(void *id);
typedef void (*xh_iic_bus_reset_obj)(void *id);
typedef uint8_t (*xh_iic_set_slave_addr_obj)(void *id, uint8_t slave_addr);


typedef struct _xh_iic_t{
    xh_comm_t               core;
    xh_iic_write_obj        iic_write;
    xh_iic_read_obj         iic_read;
    xh_iic_is_busy_obj      iic_is_busy;
    xh_iic_bus_reset_obj    iic_reset;
    xh_iic_set_slave_addr_obj  iic_set_slave_addr;
}xh_iic_t, *p_xh_iic_t;

/* 驱动宏定义实现 */
#define REG_XH_IIC_DRV(id,var,prio,auto_init,init,deinit,iic_write,iic_read,iic_is_busy,iic_reset,iic_set_slave_addr)    \
           REG_XH_DEV(xh_iic_t,id,var,prio,auto_init,init,deinit,                       \
            (xh_iic_write_obj)iic_write,                                                \
            (xh_iic_read_obj)iic_read,                                                  \
            (xh_iic_is_busy_obj)iic_is_busy,                                                \
            (xh_iic_bus_reset_obj)iic_reset,                                                \
            (xh_iic_set_slave_addr_obj)iic_set_slave_addr,                                 \
        );

#define EXT_XH_IIC_DRV(id)        EXT_XH_DEV(xh_iic_t,id)

/** 
 * @brief iic非阻塞写
 * 
 * @param [in] id            驱动id
 * @param [in] reg_addr      写地址
 * @param [in] tx_data       写的数据
 * @param [in] data_len      数据长度
 * 
 * @details 特殊说明: 
 * @par eg:使用设备ID:DRV_IIC 往从设备 地址为0x00写入8个字节 
 * @code
 *    uint8_t str[8] = {0xAA,1,2,3,4,5,6,7};
 *    xh_i2c_write(DRV_IIC,str,0x00,8);
 * @endcode
 */
static inline void xh_iic_write(void *const id,uint32_t reg_addr, uint8_t *tx_data,uint32_t data_len)
{
    xh_iic_t *p = (xh_iic_t *)id;
    p->iic_write(p->core.var,reg_addr,tx_data,data_len);
}

/**
 * @brief iic非阻塞读
 * 
 * @param [in] id            驱动id
 * @param [in] reg_addr      读的地址
 * @param [in] rx_data       读的数据缓存
 * @param [in] data_len      读的长度
 * 
 * @details 特殊说明: 
 * @par eg:使用设备ID:DRV_IIC 从设备 地址为0x00读取8个字节到数组str
 * @code
 *    uint8_t str[8] = {0};
 *    xh_i2c_write(DRV_IIC,str,0x00,8);
 * @endcode
 */
static inline void xh_iic_read(void * const id, uint32_t reg_addr, uint8_t *rx_data,uint32_t data_len)
{
    xh_iic_t *p = (xh_iic_t *)id;
    p->iic_read(p->core.var,reg_addr,rx_data,data_len);
}

/**
 * @brief iic总线状态(上一次操作是否完成)
 * 
 * @param [in] id            驱动id
 * 
 * @return uint8_t 
 *  @retval 1：总线忙(上一次操作未完成)
 *          0：总线空闲(上一次操作完成)
 * @details 特殊说明: 
 * @par eg:写入数据后获取iic总线的状态
 * @code
 *    uint8_t sta = 3;
 *    xh_i2c_write(DRV_IIC,str,0x00,8);
 *    sta = xh_iic_is_busy(DRV_IIC);
 * @endcode
 */
static inline uint8_t xh_iic_is_busy(void * const id)
{
    xh_iic_t *p = (xh_iic_t *)id;
    return p->iic_is_busy(p->core.var);
}

/**
 * @brief iic总线复位
 * 
 * @param [in] id            驱动id
 * 
 * @details 特殊说明: 
 * @par eg: 复位设备ID：DRV_IIC 总线
 * @code
 *    xh_iic_bus_reset(DRV_IIC);
 * @endcode
 */
static inline void xh_iic_bus_reset(void * const id)
{
    xh_iic_t *p = (xh_iic_t *)id;
    p->iic_reset(p->core.var);
}

/**
 * @brief 修改I2C设备对应的从机设备地址
 * 
 * @param [in] id            IIC设备ID
 * @param [in] slave_addr    IIC从机地址
 * 
 * @details 特殊说明: 无
 * @par eg: 修改设备ID:DRV_IIC 的从机设备地址为 A0 
 * @code
 *    xh_i2c_set_slave_addr(DRV_IIC,0xA0); 
 * @endcode
 */
static inline void xh_iic_set_slave_addr(void *id, uint8_t slave_addr)
{
    xh_iic_t *p = (xh_iic_t *)id;
    p->iic_set_slave_addr(p->core.var,slave_addr);
} 

#ifdef __cplusplus
}
#endif
#endif /* xh_iic_h */
