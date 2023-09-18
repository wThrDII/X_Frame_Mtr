

#ifndef __BSP_IIC_SIM__H
#define __BSP_IIC_SIM__H
#include "x_frame.h"
#include "xh_i2c.h"
#include "xh_io.h"

/******************
*模拟IIC结构体
******************/
typedef struct _BSP_IIC_SIM_PARAM{
    void * const drv_sda_gpio;      //SDA_GPIO驱动ID
    uint32_t sda_ch;                //SDA通道号
    void * const drv_scl_gpio;      //SCL_GPIO驱动ID
    uint32_t scl_ch;                //SDA通道号
    uint8_t i2c_salve_address;     // i2c 从机地址
    uint16_t reg_len;                //寄存器地址占的字节数量

}BSP_IIC_SIM_PARAM, *P_BSP_IIC_SIM_PARAM;
/******************
*IIC消息结构体
******************/
typedef struct _BSP_IIC_SIM_MSG{
    uint8_t addr;
    uint8_t flags;
    uint16_t len;
    uint8_t *buf;
}BSP_IIC_SIM_MSG, *P_BSP_IIC_SIM_MSG;
/*************************************************************
** Function name:       IIC_IO_INIT
** Descriptions:        模拟IIC初始化
** Input parameters:    x: IIC结构体变量名
**                      xSetSDAOutput：设置SDA输出模式
**                      xSetSCLOutput: 设置SCL输出模式
**                      xSetSDAInput：设置SDA输入模式
**                      xSetSDAOutLeave：设置SDA输出电平
**                      xSetSCLOutLeave：设置SCL输出电平
**                      xGetSDAInputLeave: 获取SDA输入电平
**                      xIICDelay: 微妙延时函数
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define REG_XH_IIC_SIM_PARAM(dev_id,prio,drv_sda_gpio,sda_gpio_ch,drv_scl_gpio,scl_gpio_ch,i2c_salve_address,reg_len)         \
BSP_IIC_SIM_PARAM _##dev_id = {                                                                                            \
    PTR_TYPE_XH_DEV(drv_sda_gpio),                                                                                  \
    sda_gpio_ch,                                                                                                    \
    PTR_TYPE_XH_DEV(drv_scl_gpio),                                                                                  \
    scl_gpio_ch,                                                                                                    \
    i2c_salve_address,                                                                                              \
    reg_len,                                                                                                        \
};                                                                                                                  \
REG_XH_I2C_DRV(dev_id, &_##dev_id, prio, DEV_AUTO_INIT, NULL, NULL, bsp_i2c_sim_write, bsp_i2c_sim_read,                    \
                NULL, bsp_i2c_sim_set_slave_addr)    




extern uint8_t bsp_i2c_sim_write(P_BSP_IIC_SIM_PARAM p_iic_sim, uint8_t *tx_data, uint32_t reg_addr, uint32_t len);
extern uint8_t bsp_i2c_sim_read(P_BSP_IIC_SIM_PARAM p_iic_sim, uint8_t *rx_data, uint32_t reg_addr, uint32_t len);
extern uint8_t bsp_i2c_sim_set_slave_addr(P_BSP_IIC_SIM_PARAM p_iic_sim, uint8_t slave_addr);






#endif

