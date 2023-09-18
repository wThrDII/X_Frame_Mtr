

#include "bsp_iic_sim.h"
#define GLOBAL_LEVAL 1
#if GLOBAL_LEVAL
void iic_delay(uint8_t us)
{
    uint16_t i = 0;

    for (i = 0; i < 10 * us; i++) {
        __asm(" NOP");
    }
}
#else
void iic_delay(uint8_t us)
{
    uint16_t i = 0;
    for (i = 0; i < 50 * us; i++) {
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
        __asm(" NOP");
    }
}
#endif
/********************************************************************************************************************
                                                      模拟IIC时序相关代码
********************************************************************************************************************/

/*************************************************************
** Function name:       IICStart
** Descriptions:        IIC起始信号
** Input parameters:    p_iic_sim:IIC结构体指针
** Output parameters:   None
** Returned value:      None
*************************************************************/
void IICStart(P_BSP_IIC_SIM_PARAM p_iic_sim)
{
    //START:when CLK is high,DATA change form high to low
    xh_io_mode(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,XH_GPIO_MODE_OUT_OD);
    xh_io_write(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,1);
    xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,1);
    iic_delay(4);
    xh_io_write(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,0);
    iic_delay(4);
    xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,0);
}
/*************************************************************
** Function name:       IICStop
** Descriptions:        IIC停止信号
** Input parameters:    p_iic_sim:IIC结构体指针
** Output parameters:   None
** Returned value:      None
*************************************************************/
void IICStop(P_BSP_IIC_SIM_PARAM p_iic_sim)
{
    //STOP:when CLK is high DATA change form low to high
	xh_io_mode(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,XH_GPIO_MODE_OUT_OD);
	xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,0);
	xh_io_write(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,0);
 	iic_delay(4);
    xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,1);
    xh_io_write(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,1);
	iic_delay(4);
}
/*************************************************************
** Function name:       IICWaitAck
** Descriptions:        IIC等待响应
** Input parameters:    p_iic_sim:IIC结构体指针
** Output parameters:   None
** Returned value:      None
*************************************************************/
uint8_t IICWaitAck(P_BSP_IIC_SIM_PARAM p_iic_sim)
{
	uint8_t ucErrTime=0;
//  xh_io_mode(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,XH_GPIO_MODE_IN_FLOATING);
//	xh_io_write(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,1);iic_delay(1);
	xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,1);iic_delay(2);
	xh_io_mode(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,XH_GPIO_MODE_IN_FLOATING);
	while(xh_io_read(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IICStop(p_iic_sim);
			return 1;
		}
	}
	xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,0);
	
	return 0;
}
/*************************************************************
** Function name:       IICAck
** Descriptions:        IIC响应
** Input parameters:    p_iic_sim:IIC结构体指针
** Output parameters:   None
** Returned value:      None
*************************************************************/
void IICAck(P_BSP_IIC_SIM_PARAM p_iic_sim)
{
	xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,0);
	xh_io_mode(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,XH_GPIO_MODE_OUT_OD);
	xh_io_write(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,0);
	iic_delay(2);
	xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,1);
	iic_delay(2);
	xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,0);
}
/*************************************************************
** Function name:       IICNAck
** Descriptions:        IIC不响应
** Input parameters:    p_iic_sim:IIC结构体指针
** Output parameters:   None
** Returned value:      None
*************************************************************/
void IICNAck(P_BSP_IIC_SIM_PARAM p_iic_sim)
{
	xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,0);
	xh_io_mode(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,XH_GPIO_MODE_OUT_OD);
	xh_io_write(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,1);
	iic_delay(2);
	xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,1);
	iic_delay(2);
	xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,0);
}
/*************************************************************
** Function name:       IICSendByte
** Descriptions:        IIC不响应
** Input parameters:    p_iic_sim:IIC结构体指针 txd:发送数据
** Output parameters:   None
** Returned value:      None
*************************************************************/
void IICSendByte(P_BSP_IIC_SIM_PARAM p_iic_sim,uint8_t txDate)
{
    uint8_t t;
	xh_io_mode(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,XH_GPIO_MODE_OUT_OD);
    xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,0);
    for(t=0;t<8;t++)
    {
        xh_io_write(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,(txDate&0x80)>>7);
        txDate<<=1;
		iic_delay(2);
		xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,1);
		iic_delay(2);
		xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,0);
		iic_delay(2);
    }
}
/*************************************************************
** Function name:       IICReadByte
** Descriptions:        IIC不响应
** Input parameters:    p_iic_sim:IIC结构体指针 isACK:是否响应
** Output parameters:   None
** Returned value:      None
*************************************************************/
uint8_t IICReadByte(P_BSP_IIC_SIM_PARAM p_iic_sim,uint8_t isACK)
{
	uint8_t i,receive=0;
    xh_io_write(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,1);
	xh_io_mode(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch,XH_GPIO_MODE_OUT_OD);
    iic_delay(2);
    for(i=0;i<8;i++ )
	{
        xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,0);
        iic_delay(2);
		xh_io_write(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,1);
        receive<<=1;
        if(xh_io_read(p_iic_sim->drv_sda_gpio,p_iic_sim->sda_ch))receive++;
		iic_delay(1);
    }
    if (!isACK)
        IICNAck(p_iic_sim);
    else
        IICAck(p_iic_sim);
    return receive;
}
/*************************************************************
** Function name:       IICReadRegiister
** Descriptions:        读取寄存器数据
** Input parameters:    p_iic_sim:IIC结构体指针 isACK:是否响应
** Output parameters:   None
** Returned value:      None
*************************************************************/
void IICTransfer(P_BSP_IIC_SIM_PARAM p_iic_sim,BSP_IIC_SIM_MSG *p_iic_simMsg,uint8_t len)
{
    int i = 0;
    int j = 0;
    xh_io_mode(p_iic_sim->drv_scl_gpio,p_iic_sim->scl_ch,XH_GPIO_MODE_OUT_PP);
    for(i = 0; i < len; i++) {
        if (p_iic_simMsg[i].flags == 0){ //写
            IICStart(p_iic_sim);
            IICSendByte(p_iic_sim,p_iic_simMsg[i].addr|0x00);
            if (IICWaitAck(p_iic_sim)) {
                return;
            }
            for(j = 0; j < p_iic_simMsg[i].len; j++) {
                IICSendByte(p_iic_sim,p_iic_simMsg[i].buf[j]);
                IICWaitAck(p_iic_sim);
            }
        } else if (p_iic_simMsg[i].flags == 1) {
            IICStart(p_iic_sim);
            IICSendByte(p_iic_sim,p_iic_simMsg[0].addr|0x01);
            if (IICWaitAck(p_iic_sim)) {
                return;
            }
            for(j = 0; j < p_iic_simMsg[i].len; j++) {
                if (j == p_iic_simMsg[i].len - 1) {
                    p_iic_simMsg[i].buf[j] = IICReadByte(p_iic_sim,0);
                } else {
                    p_iic_simMsg[i].buf[j] = IICReadByte(p_iic_sim,1);
                }
            }
        }
    }
    IICStop(p_iic_sim);
}
/*************************************************************
** Function name:       IICReadRegs
** Descriptions:        IIC读取寄存器
** Input parameters:    p_iic_sim SIIC_Struct结构体指针
**                      reg:寄存器地址
**                      addr:IIC设备地址
**                      len:长度
**                      val:结果存放数组
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void IICReadRegs(P_BSP_IIC_SIM_PARAM p_iic_sim,uint8_t reg,uint8_t addr,uint16_t len,uint8_t *val)
{
    BSP_IIC_SIM_MSG msg[2];
    msg[0].addr = addr;
    msg[0].flags = 0;
    msg[0].buf = &reg;
    msg[0].len = 1;

    msg[1].addr = addr;
    msg[1].flags = 1;
    msg[1].buf = val;
    msg[1].len = len;
    IICTransfer(p_iic_sim,msg,2);
}
/*************************************************************
** Function name:       IICWriteRegs
** Descriptions:        IIC写寄存器
** Input parameters:    p_iic_sim SIIC_Struct结构体指针
**                      reg:寄存器地址
**                      addr:IIC设备地址
**                      len:长度
**                      val:写入数据存放数组
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void IICWriteRegs(P_BSP_IIC_SIM_PARAM p_iic_sim,uint8_t reg,uint8_t addr,uint16_t len,uint8_t *val)
{
    BSP_IIC_SIM_MSG msg;
    uint8_t buf[256];
    buf[0] = reg;
    memcpy(&buf[1],val,len);
    msg.addr = addr;
    msg.flags = 0;
    msg.buf = buf;
    msg.len = len+1;
    IICTransfer(p_iic_sim,&msg,1);
}
/*************************************************************
** Function name:       IICReadOneReg
** Descriptions:        IIC读取一个寄存器
** Input parameters:    p_iic_sim SIIC_Struct结构体指针
**                      reg:寄存器地址
**                      addr:IIC设备地址
** Output parameters:   读取结果
** Returned value:      None
** Remarks:             None
*************************************************************/
uint8_t IICReadOneReg(P_BSP_IIC_SIM_PARAM p_iic_sim,uint8_t reg,uint8_t addr)
{
    uint8_t res = 0;
    IICReadRegs(p_iic_sim,reg,addr,1,&res);
    return res;
}
/*************************************************************
** Function name:       IICWriteOneReg
** Descriptions:        IIC写一个寄存器
** Input parameters:    p_iic_sim SIIC_Struct结构体指针
**                      reg:寄存器地址
**                      addr:IIC设备地址
**                      value:要写入的值
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void IICWriteOneReg(P_BSP_IIC_SIM_PARAM p_iic_sim,uint8_t reg,uint8_t addr,uint8_t value)
{
    IICWriteRegs(p_iic_sim,reg,addr,1,&value);
}



/*************************************************************
** Function name:       IICReadRegsUint16
** Descriptions:        IIC读取寄存器(寄存器地址为16位)
** Input parameters:    p_iic_sim SIIC_Struct结构体指针
**                      reg:寄存器地址
**                      addr:IIC设备地址
**                      len:长度
**                      val:结果存放数组
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void IICReadRegs16(P_BSP_IIC_SIM_PARAM p_iic_sim,uint16_t reg,uint8_t addr,uint16_t len,uint8_t *val)
{
    BSP_IIC_SIM_MSG msg[2];
    uint8_t regAddr[2];
    regAddr[0] = reg >> 8;
    regAddr[1] = reg & 0xFF;
    msg[0].addr = addr;
    msg[0].flags = 0;
    msg[0].buf = regAddr;
    msg[0].len = 2;

    msg[1].addr = addr;
    msg[1].flags = 1;
    msg[1].buf = val;
    msg[1].len = len;
    IICTransfer(p_iic_sim,msg,2);
}
/*************************************************************
** Function name:       IICWriteRegsUint16
** Descriptions:        IIC写寄存器(寄存器地址为16位)
** Input parameters:    p_iic_sim SIIC_Struct结构体指针
**                      reg:寄存器地址
**                      addr:IIC设备地址
**                      len:长度
**                      val:写入数据存放数组
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void IICWriteRegs16(P_BSP_IIC_SIM_PARAM p_iic_sim,uint16_t reg,uint8_t addr,uint16_t len,uint8_t *val)
{
    BSP_IIC_SIM_MSG msg;
    uint8_t buf[256];
    uint8_t regAddr[2];
    regAddr[0] = reg >> 8;
    regAddr[1] = reg & 0xFF;
    buf[0] = regAddr[0];
    buf[1] = regAddr[1];
    memcpy(&buf[2],val,len);
    msg.addr = addr;
    msg.flags = 0;
    msg.buf = buf;
    msg.len = len+2;
    IICTransfer(p_iic_sim,&msg,1);
}
/*************************************************************
** Function name:       IICReadOneRegUint16
** Descriptions:        IIC读取一个寄存器(寄存器地址为16位)
** Input parameters:    p_iic_sim SIIC_Struct结构体指针
**                      reg:寄存器地址
**                      addr:IIC设备地址
** Output parameters:   读取结果
** Returned value:      None
** Remarks:             None
*************************************************************/
uint8_t IICReadOneReg16(P_BSP_IIC_SIM_PARAM p_iic_sim,uint16_t reg,uint8_t addr)
{
    uint8_t res = 0;
    IICReadRegs16(p_iic_sim,reg,addr,1,&res);
    return res;
}
/*************************************************************
** Function name:       IICWriteOneRegUint16
** Descriptions:        IIC写一个寄存器(寄存器地址为16位)
** Input parameters:    p_iic_sim SIIC_Struct结构体指针
**                      reg:寄存器地址
**                      addr:IIC设备地址
**                      value:要写入的值
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void IICWriteOneReg16(P_BSP_IIC_SIM_PARAM p_iic_sim,uint16_t reg,uint8_t addr,uint8_t value)
{
    IICWriteRegs16(p_iic_sim,reg,addr,1,&value);
}






uint8_t bsp_i2c_sim_write(P_BSP_IIC_SIM_PARAM p_iic_sim, uint8_t *tx_data, uint32_t reg_addr, uint32_t len)
{
    if(p_iic_sim->reg_len == 1) {
        IICWriteRegs(p_iic_sim,(uint16_t)reg_addr,p_iic_sim->i2c_salve_address,(uint16_t)len,tx_data);
    } else if (p_iic_sim->reg_len == 2) {
        IICWriteRegs16(p_iic_sim,(uint16_t)reg_addr,p_iic_sim->i2c_salve_address,(uint16_t)len,tx_data);
    }
	return 1; 
}
uint8_t bsp_i2c_sim_read(P_BSP_IIC_SIM_PARAM p_iic_sim, uint8_t *rx_data, uint32_t reg_addr, uint32_t len)
{
    if(p_iic_sim->reg_len == 1) {
        IICReadRegs(p_iic_sim,(uint16_t)reg_addr,p_iic_sim->i2c_salve_address,(uint16_t)len,rx_data);  
    } else if (p_iic_sim->reg_len == 2) {
        IICReadRegs16(p_iic_sim,(uint16_t)reg_addr,p_iic_sim->i2c_salve_address,(uint16_t)len,rx_data);             
    } 
	return 1;
}
uint8_t bsp_i2c_sim_set_slave_addr(P_BSP_IIC_SIM_PARAM p_iic_sim, uint8_t slave_addr)
{
    p_iic_sim->i2c_salve_address = slave_addr;
	return 1;
}





