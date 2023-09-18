/**
 * @brief spi驱动
 * @file bsp_spi.c
 * @version 1.0
 * @author wending.wei 
 * @date 2023-01-31 10:49:54
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2023-01-31 10:49:54 <td>1.0       <td>wending.wei      <td>Created function
 */
#include "bsp_spi.h"


/**
 * @brief spi的cs引脚组初始
 * 
 * @param [in] spi           spi结构体指针
 * 
 * @details: 内部函数
 * 
 * @par e.g: 对spi的cs组进行初始化
 * @code
 *      P_BSP_SPI _DRV_SPI;
 * 
 *      _bsp_spi_cs_group_init(_DRV_SPI);
 * @endcode
 */
static void _bsp_spi_cs_group_init(P_BSP_SPI spi)
{
    uint32_t i = 0;
    GPIO_InitTypeDef spi_cs_gpio_config;

    ///< 将所有的cs引脚初始化
    for (i = 0; i < spi->cs_pin_num; i++)
    {
       
        ///< cs引脚配置
        spi_cs_gpio_config.GPIO_Pin        = 1 << spi->cs_group[i].cs_pin;         // pin脚
        spi_cs_gpio_config.GPIO_Mode       = GPIO_Mode_OUT;                        // 模式：输出
        spi_cs_gpio_config.GPIO_PuPd       = GPIO_PuPd_NOPULL;                     // 上拉/下拉
        spi_cs_gpio_config.GPIO_PODEna     = DISABLE;                              // 开漏使能
        spi_cs_gpio_config.GPIO_PFLT       = DISABLE;                              // 滤波使能 
        GPIO_Init(spi->cs_group[i].cs_port,&spi_cs_gpio_config);  
  
        ///< cs引脚置位
        GPIO_SetBits(spi->cs_group[i].cs_port, spi->cs_group[i].cs_pin);
    }
}


/**
 * @brief spi功能引脚初始化
 * 
 * @param [in] spi           spi结构体指针 
 * 
 * @details: 内部函数
 * 
 * @par e.g: 对spi功能引脚(SCK,MISO,MOSI)初始化
 * @code
 *      P_BSP_SPI _DRV_SPI;
 * 
 *      _bsp_spi_gpio_init(_DRV_SPI);
 * @endcode
 */
static uint8_t _bsp_spi_gpio_init(P_BSP_SPI spi)
{
    uint8_t status = 0;
    GPIO_InitTypeDef spi_gpio_config;
    
    ///< 判断spi的功能引脚是否需要重映射，如果需要则进行重映射

    ///< spi的SCK引脚配置 
    spi_gpio_config.GPIO_Pin        = 1 << spi->gpio_group[0].pin;          // pin脚
    spi_gpio_config.GPIO_Mode       = GPIO_Mode_OUT;                        // 模式：输出
    spi_gpio_config.GPIO_PuPd       = GPIO_PuPd_NOPULL;                     // 上拉/下拉
    spi_gpio_config.GPIO_PODEna     = DISABLE;                              // 开漏使能
    spi_gpio_config.GPIO_PFLT       = DISABLE;                              // 滤波使能
    GPIO_Init(spi->gpio_group[0].port,&spi_gpio_config);  
    GPIO_PinAFConfig(spi->gpio_group[0].port, spi->gpio_group[0].pin, GPIO_AF_SPI);

    ///< spi的MISO引脚配置 
    spi_gpio_config.GPIO_Pin        = 1 << spi->gpio_group[1].pin;          // pin脚
    spi_gpio_config.GPIO_Mode       = GPIO_Mode_IN;                         // 模式：输入
    spi_gpio_config.GPIO_PuPd       = GPIO_PuPd_NOPULL;                     // 上拉/下拉
    spi_gpio_config.GPIO_PODEna     = DISABLE;                              // 开漏使能
    spi_gpio_config.GPIO_PFLT       = DISABLE;                              // 滤波使能
    GPIO_Init(spi->gpio_group[1].port,&spi_gpio_config);  
    GPIO_PinAFConfig(spi->gpio_group[1].port, spi->gpio_group[1].pin, GPIO_AF_SPI);

    ///< spi的MOSI引脚配置 
    spi_gpio_config.GPIO_Pin        = 1 << spi->gpio_group[2].pin;          // pin脚
    spi_gpio_config.GPIO_Mode       = GPIO_Mode_OUT;                        // 模式：输出
    spi_gpio_config.GPIO_PuPd       = GPIO_PuPd_NOPULL;                     // 上拉/下拉
    spi_gpio_config.GPIO_PODEna     = DISABLE;                              // 开漏使能
    spi_gpio_config.GPIO_PFLT       = DISABLE;                              // 滤波使能
    GPIO_Init(spi->gpio_group[2].port,&spi_gpio_config);  
    GPIO_PinAFConfig(spi->gpio_group[2].port, spi->gpio_group[2].pin, GPIO_AF_SPI);

    ///< cs引脚组初始化
    _bsp_spi_cs_group_init(spi);
		
	return status;
}

/**
 * @brief spi中断配置
 * 
 * @param [in] spi           spi结构体指针
 * 
 * @details: 内部函数
 * 
 * @par e.g: 配置spi的中断函数
 * @code
 *      P_BSP_SPI _DRV_SPI;
 *      
 *      _bsp_spi_nvic_config(_DRV_SPI); 
 * @endcode
 */
static void _bsp_spi_nvic_config(P_BSP_SPI spi)
{
    if (spi->params_s->spi_periph == SPI0) {
        NVIC_EnableIRQ(SPI0_IRQn);
    }
    else if (spi->params_s->spi_periph == SPI1) {
        NVIC_EnableIRQ(SPI1_IRQn);
    }
    else {

    }
}

/**
 * @brief spi功能配置初始化
 * 
 * @param [in] spi           spi结构体指针
 * 
 * @details: 内部函数
 * 
 * @par e.g:对spi进行配置
 * @code
 *      P_BSP_SPI _DRV_SPI;
 *      
 *      _bsp_spi_param_cofig(_DRV_SPI);   
 * @endcode
 */
static void _bsp_spi_param_cofig(P_BSP_SPI spi)
{
    SPI_InitTypeDef spi_init_struct;

    SPI_StructInit(&spi_init_struct);

    ///< spi参数配置
    ///< SPI模块使能
    spi_init_struct.EN                      = 1;     
    ///< 配置为全双工
    spi_init_struct.Duplex                  = SPI_Full;
    ///< 配置为主机
    spi_init_struct.Mode                    = SPI_Master;
    ///< 一次发送长度 8bit or 16bit
    spi_init_struct.ByteLength              = spi->params_s->data_len;
    ///< spi模式，极性和相位
    spi_init_struct.CPHA                    = spi->params_s->clock_phase;
    spi_init_struct.CPOL                    = spi->params_s->clock_polarity;
    ///< 从设备下片选信号来源    0，恒有效    1，来源于主设备
    spi_init_struct.CS                      = 0;
    ///< spi分频
    spi_init_struct.BaudRate                = spi->params_s->prescale;
    ///< spi传输方式为大端
    spi_init_struct.DataOrder               = SPI_FIRSTSEND_MSB;
    ///< SPI中断使能：0，关闭；1，开启
    spi_init_struct.IRQEna                  = DISABLE;
    ///< 传输触发选择：0，内部自动执行（仅主模式有效）；1，外部触发
    spi_init_struct.Trig                    = 0;
    ///<  SPI数据搬运方式：0，DMA搬运；1，MCU搬运
    spi_init_struct.TRANS_MODE              = SPI_DMA_DISABLE;

    ///< spi初始化
    SPI_Init(spi->params_s->spi_periph, &spi_init_struct);

    /* enable SPIx */
    SPI_Enable(spi->params_s->spi_periph);
}


/**
 * @brief spi初始化
 * 
 * @param [in] spi           spi结构体指针
 * 
 * @details: spi初始化对外接口
 * 
 * @par e.g:spi初始化
 * @code
 *      P_BSP_SPI _DRV_SPI;
 *      
 *      bsp_spi_init(_DRV_SPI); 
 * @endcode
 */
void bsp_spi_init(P_BSP_SPI spi)
{    
    ///< spi引脚初始化
    _bsp_spi_gpio_init(spi);

    ///< 判断是否是阻塞模式，如果不是，打开spi中断
    if (!spi->flag_is_block)
    {
        _bsp_spi_nvic_config(spi);
    }

    ///< spi参数配置
    _bsp_spi_param_cofig(spi);

}

/**
 * @brief spi反初始化
 * 
 * @param [in] spi           spi结构体指针
 * 
 * @details: 
 * 
 * @par e.g:spi反初始化
 * @code
 *      P_BSP_SPI _DRV_SPI;
 *      
 *      bsp_spi_deinit(_DRV_SPI);  
 * @endcode
 */
void bsp_spi_deinit(P_BSP_SPI spi)
{   
    
}


/**
 * @brief 获取SPI是否忙
 *
 * @param [in] spi           spi结构体指针
 *
 * @return uint8_t  spi的状态
 *  @retval 0 空闲
 *  @retval 1 忙 
 *
 * @details
 */
uint8_t bsp_spi_get_is_busy(P_BSP_SPI spi)
{
    return (uint8_t)(spi->params_s->spi_periph->IE&SPI_IF_TranDone);
}


/**
 * @brief spi的cs引脚状态配置
 * 
 * @param [in] spi           spi结构体指针
 * @param [in] cs_index      spi的cs引脚号
 * @param [in] level         设置的电平状态
 * 
 * @details: 
 * 
 * @par e.g: 将spi的1号cs设置为高
 * @code
 *      P_BSP_SPI _DRV_SPI;
 *      
 *      bsp_spi_set_cs_level(_DRV_SPI,1, 1); 
 * @endcode
 */
void bsp_spi_set_cs_level(P_BSP_SPI spi, uint32_t cs_index, uint32_t level)
{
    GPIO_WriteBit(spi->cs_group[cs_index].cs_port, spi->cs_group[cs_index].cs_pin, (GPIO_BitAction)level);
}



/**
 * @brief spi中断服务函数
 * 
 * @param [in] spi           spi结构体指针
 * 
 * @details: 
 * 
 * @par e.g:
 * @code
 *    
 * @endcode
 */
void _bsp_spi_irq_handler(P_BSP_SPI spi)
{
    // 发送中断
    // if (RESET != spi_i2s_interrupt_flag_get(spi->params_s->spi_periph, SPI_I2S_INT_FLAG_TBE))
    // {
    if (spi->params_s->spi_periph->IE&SPI_IF_TranDone)
    {
        if (spi->params_s->data_len == 8)
        {
            /* Send SPI_MASTER data */
            spi->params_s->spi_periph->TX_DATA = *spi->tx_data_8;
            spi->tx_data_len--;
            if (spi->tx_data_len > 0)
            {
                spi->tx_data_8++;
            }
        }
        else
        {
            spi->params_s->spi_periph->TX_DATA = *spi->tx_data_16;
            spi->tx_data_len--;
            if (spi->tx_data_len > 0)
            {
                spi->tx_data_16++;
            }
        }

        /* Disable SPI_MASTER TXE interrupt */
        if (spi->tx_data_len == 0)
        {
    //         spi_i2s_interrupt_disable(spi->params_s->spi_periph, SPI_I2S_INT_TBE);
        }
    }

    // 接收中断
    // if (RESET != spi_i2s_interrupt_flag_get(spi->params_s->spi_periph, SPI_I2S_INT_FLAG_RBNE))
    // {
    if (spi->params_s->spi_periph->IE&SPI_IF_TranDone)
    {
        if (spi->params_s->data_len == 8)
        {
            *spi->rx_data_8 = (uint8_t)spi->params_s->spi_periph->RX_DATA;
            spi->rx_data_8++;
            spi->rx_data_len++;
        }
        else
        {
            *spi->rx_data_16 = (uint16_t)spi->params_s->spi_periph->RX_DATA;
            spi->rx_data_16++;
            spi->rx_data_len++;
        }
    }
}


#define WAIT_SPISTAT(func,res) i=200;do{i--;if(i<0){return res;}}while(func)
/**
 * @brief spi发送&接收数据
 * 
 * @param [in] periph        spi号
 * @param [in] half_word     发送的半字
 * 
 * @return uint16_t 接收到的数据
 *  @retval 
 * 
 * @details: 
 * 
 * @par e.g:
 * @code
 *    
 * @endcode
 */
static uint16_t _bsp_spi_send_data(SPI_TypeDef* periph, uint16_t data)
{
    int16_t i = 0;

    uint16_t  rdata=0;

    periph->TX_DATA = data;     ///< 不能使用SPI_SendData(periph, data)，因为SPI_SendData不支持16bit

    
    WAIT_SPISTAT(0 == (periph->IE&SPI_IF_TranDone), 1); ///< 等待接收寄存器为空

    rdata = periph->RX_DATA;    ///< 不能使用SPI_ReadData(periph)，因为SPI_SendData不支持16bit

    periph->IE |= SPI_IF_TranDone;//清除发送完成标志位
    return rdata;

}




/**
 * @brief SPI发送接收数据
 * 
 * @param [in] spi           spi结构体指针
 * @param [in] tx_data       发送数据buff
 * @param [in] rx_data       接收数据buff
 * @param [in] len           数据交换长度
 * 
 * @details: 
 * 
 * @par e.g:
 * @code
 *    
 * @endcode
 */
void bsp_spi_rw_data(P_BSP_SPI spi, void *tx_data, void *rx_data, uint32_t len)
{
    uint16_t *data;
    data = tx_data;
    uint32_t i;
    for (i = 0; i < len; i++)
    {
        spi->tx_buffer[i] = data[i];
    }
    spi->tx_data_len = len;

    if (spi->params_s->data_len == 8)
    {
        spi->tx_data_8 = (uint8_t *)spi->tx_buffer;
        spi->rx_data_8 = (uint8_t *)rx_data;
    }
    else
    {
        spi->tx_data_16 = (uint16_t *)spi->tx_buffer;
        spi->rx_data_16 = (uint16_t *)rx_data;
    }

    if (spi->flag_is_block)
    {
        if (spi->params_s->data_len == 8)
        {
            for (i = 0; i < spi->tx_data_len; i++)
            {
                *spi->rx_data_8 = _bsp_spi_send_data(spi->params_s->spi_periph, *spi->tx_data_8);
                spi->tx_data_8++;
                spi->rx_data_8++;
            }
        }
        else
        {
            for (i = 0; i < spi->tx_data_len; i++)
            {
                *spi->rx_data_16 = _bsp_spi_send_data(spi->params_s->spi_periph, *spi->tx_data_16);
                spi->tx_data_16++;
                spi->rx_data_16++;
            }
        }
    }
    else
    {
        // spi_i2s_interrupt_enable(spi->params_s->spi_periph, SPI_I2S_INT_TBE);  // 发送中断
        // spi_i2s_interrupt_enable(spi->params_s->spi_periph, SPI_I2S_INT_RBNE); // 接收中断
    }
}

/**
 * @brief SPI发送数据
 * 
 * @param [in] spi           spi结构体指针
 * @param [in] tx_data       发送数据buff
 * @param [in] len           数据长度
 * 
 * @details: 
 * 
 * @par e.g:
 * @code
 *    
 * @endcode
 */
void bsp_spi_write_data(P_BSP_SPI spi, void *tx_data, uint32_t len)
{
    uint16_t *data;
    data = tx_data;
    uint32_t i;
    for (i = 0; i < len; i++)
    {
        spi->tx_buffer[i] = data[i];
    }
    spi->tx_data_len = len;

    if (spi->params_s->data_len == 8)
    {
        spi->tx_data_8 = (uint8_t *)spi->tx_buffer;
        spi->rx_data_8 = (uint8_t *)spi->rx_buffer;
    }
    else
    {
        spi->tx_data_16 = (uint16_t *)spi->tx_buffer;
        spi->rx_data_16 = (uint16_t *)spi->rx_buffer;
    }

    if (spi->flag_is_block)
    {
        if (spi->params_s->data_len == 8)
        {
            for (i = 0; i < spi->tx_data_len; i++)
            {
                *spi->rx_data_8 = _bsp_spi_send_data(spi->params_s->spi_periph, *spi->tx_data_8);
                spi->tx_data_8++;
            }
        }
        else
        {
            for (i = 0; i < spi->tx_data_len; i++)
            {
                *spi->rx_data_16 = _bsp_spi_send_data(spi->params_s->spi_periph, *spi->tx_data_16);
                spi->tx_data_16++;
            }
        }
    }
    else
    {
        // spi_i2s_interrupt_enable(spi->params_s->spi_periph, SPI_I2S_INT_TBE);  // 发送中断
        // spi_i2s_interrupt_enable(spi->params_s->spi_periph, SPI_I2S_INT_RBNE); // 接收中断
    }
}

/**
 * @brief SPI接收数据
 * 
 * @param [in] spi           spi结构体指针
 * @param [in] rx_data       接收数据buff
 * @param [in] len           数据接收长度
 * 
 * 
 * @details: 
 * 
 * @par e.g:
 * @code
 *    
 * @endcode
 */
void bsp_spi_read_data(P_BSP_SPI spi, void *rx_data, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; i++)
    {
        spi->tx_buffer[i] = 0xFFFF;
    }
    spi->tx_data_len = len;

    if (spi->params_s->data_len == 8)
    {
        spi->tx_data_8 = (uint8_t *)spi->tx_buffer;
        spi->rx_data_8 = (uint8_t *)rx_data;
    }
    else
    {
        spi->tx_data_16 = (uint16_t *)spi->tx_buffer;
        spi->rx_data_16 = (uint16_t *)rx_data;
    }

    if (spi->flag_is_block)
    {
        if (spi->params_s->data_len == 8)
        {
            for (i = 0; i < spi->tx_data_len; i++)
            {
                *spi->rx_data_8 = _bsp_spi_send_data(spi->params_s->spi_periph, *spi->tx_data_8);
                spi->tx_data_8++;
                spi->rx_data_8++;
            }
        }
        else
        {
            for (i = 0; i < spi->tx_data_len; i++)
            {
                *spi->rx_data_16 = _bsp_spi_send_data(spi->params_s->spi_periph, *spi->tx_data_16);
                spi->tx_data_16++;
                spi->rx_data_16++;
            }
        }
    }
    else
    {
        // spi_i2s_interrupt_enable(spi->params_s->spi_periph, SPI_I2S_INT_TBE);  // 发送中断
        // spi_i2s_interrupt_enable(spi->params_s->spi_periph, SPI_I2S_INT_RBNE); // 接收中断
    }
}



