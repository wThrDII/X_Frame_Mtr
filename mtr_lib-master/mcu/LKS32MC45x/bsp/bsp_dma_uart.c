#include "bsp_dma_uart.h"
#include "string.h"

static void bsp_dam_config(P_BSP_UART_DMA_PARAM p_uart_param)
{
    
    DMA_Enable();
    {
        DMA_InitTypeDef DMAInitStruct;
        DMAInitStruct.DMA_Channel_EN = ENABLE; /* DMA 通道使能*/
        DMAInitStruct.DMA_IRQ_EN = ENABLE;          /* DMA 中断使能 */
        DMAInitStruct.DMA_RMODE = DISABLE;     /* 多轮传输使能 */
        DMAInitStruct.DMA_CIRC = DISABLE;      /* 循环模式使能 */
        DMAInitStruct.DMA_SINC = ENABLE;       /* 源地址递增使能 */
        DMAInitStruct.DMA_DINC = ENABLE;       /* 目的地址递增使能 */
        DMAInitStruct.DMA_SBTW = 0;            /* 源地址访问位宽， 0:byte, 1:half-word, 2:word */
        DMAInitStruct.DMA_DBTW = 0;            /* 目的地址访问位宽， 0:byte, 1:half-word, 2:word */
        DMAInitStruct.DMA_REQ_EN = 1;          /* 通道 x 硬件 DMA 请求使能，高有效 */
        DMAInitStruct.DMA_TIMES = 100;   /* DMA 通道 x 数据搬运次数 */
        DMAInitStruct.DMA_SADR = (u32)p_uart_param->uart_peri_port;       /* DMA 通道 x 源地址 */
        DMAInitStruct.DMA_DADR = (u32)p_uart_param->dma_rx_buf;       /* DMA 通道 x 目的地址 */
        DMA_Init(DMA_CH0,&DMAInitStruct);
    }
}
void bsp_dma_uart_init(P_BSP_UART_DMA_PARAM p_uart_param)
{
    bsp_dam_config(p_uart_param);
    UART_InitTypeDef uart1_config;
    GPIO_InitTypeDef uartx_gpio_config;
    
    GPIO_StructInit(&uartx_gpio_config);
    uartx_gpio_config.GPIO_Pin = (1 << p_uart_param->uart_tx_pin); // 要配置的PIN
    uartx_gpio_config.GPIO_Mode = GPIO_Mode_OUT;                   // GPIO模式:输入、输出、模拟
    uartx_gpio_config.GPIO_PuPd = GPIO_PuPd_NOPULL;                // 上拉/下拉
    uartx_gpio_config.GPIO_PODEna = DISABLE;                       // 开漏使能
    uartx_gpio_config.GPIO_PFLT = DISABLE;                         // 滤波使能
    GPIO_Init(p_uart_param->uart_tx_port, &uartx_gpio_config);

    GPIO_StructInit(&uartx_gpio_config);
    uartx_gpio_config.GPIO_Pin = (1 << p_uart_param->uart_rx_pin); // 要配置的PIN
    uartx_gpio_config.GPIO_Mode = GPIO_Mode_IN;                    // GPIO模式:输入、输出、模拟
    uartx_gpio_config.GPIO_PuPd = GPIO_PuPd_NOPULL;                // 上拉/下拉
    uartx_gpio_config.GPIO_PODEna = DISABLE;                       // 开漏使能
    uartx_gpio_config.GPIO_PFLT = DISABLE;                         // 滤波使能
    GPIO_Init(p_uart_param->uart_rx_port, &uartx_gpio_config);

    GPIO_PinAFConfig(p_uart_param->uart_tx_port, p_uart_param->uart_tx_pin, GPIO_AF_UART);
    GPIO_PinAFConfig(p_uart_param->uart_rx_port, p_uart_param->uart_rx_pin, GPIO_AF_UART);
    
    
    UART_StructInit(&uart1_config);
    // 这些是默认配置，简单的应用只配置波特率即可
    uart1_config.DUPLEX = DISABLE;                        // 半双工模式使能，tx_data
    uart1_config.MD_EN = DISABLE;                         // Multi-drop 使能
    uart1_config.CK_EN = DISABLE;                         // 数据校验使能
    uart1_config.CK_TYPE = p_uart_param->uart_check_bits; // 奇偶校验配置       0:偶校验（EVEN）;1: 奇校验（ODD）
    uart1_config.BIT_ORDER = 0;                           // 数据发送顺序配置   0:LSB;1:MSB
    uart1_config.STOP_LEN = p_uart_param->uart_stop_bits; // 停止位长度配置     0:1-Bit;1:2-Bit
    uart1_config.BYTE_LEN = p_uart_param->uart_data_bits; // 数据长度配置       0:8-Bit;1:9-Bit
    uart1_config.BAUDRATE = 115200;                       // 波特率
    uart1_config.ADR = 0;                                 // 多机通讯时的从机地址
    uart1_config.TX_BUF_EMPTY = DISABLE;                  // 发送缓冲区空 DMA 请求使能
    uart1_config.RX_DONE = DISABLE;                       // 接收完成 DMA 请求使能
    uart1_config.TX_DONE = DISABLE;                       // 发送完成 DMA 请求使能
    uart1_config.TXD_INV = DISABLE;                       // TXD 输出极性取反
    uart1_config.RXD_INV = DISABLE;                       // RXD 输入极性取反
    uart1_config.IE = 0;                                  // 中断配置
    uart1_config.BAUDRATE = p_uart_param->uart_baud;

    UART_Init(p_uart_param->uart_peri_port, &uart1_config);

    //    UART_NVIC_Enable(UART1,UART_IRQEna_SendOver,1);
//    UART_NVIC_Enable(p_uart_param->uart_peri_port, UART_IRQEna_RcvOver, p_uart_param->uart_irq_priority);
    UART_NVIC_Enable(p_uart_param->uart_peri_port, UART_IRQEna_TX_DMA_RE, p_uart_param->uart_irq_priority);
    UART_NVIC_Enable(p_uart_param->uart_peri_port, UART_IRQEna_RX_DMA_RE, p_uart_param->uart_irq_priority);
    //    UART_NVIC_Enable(p_uart_param->uart_peri_port,UART_IRQEna_SendBuffEmpty,1);
    UART_NVIC_Enable(p_uart_param->uart_peri_port, UART_IRQEna_CheckError, p_uart_param->uart_irq_priority);
    UART_NVIC_Enable(p_uart_param->uart_peri_port, UART_IRQEna_TX_BUF_DMA_RE, p_uart_param->uart_irq_priority);

    NVIC_SetPriority(DMA_IRQn, p_uart_param->uart_irq_priority);
    NVIC_EnableIRQ(DMA_IRQn);
}

 void bsp_dma_uart_deinit(P_BSP_UART_DMA_PARAM p_uart_param)
{
//     uint8_t a = 1;
}

uint32_t bsp_dma_uart_send(P_BSP_UART_DMA_PARAM p_uart_param, uint8_t *pbuf, uint32_t len)
{
    return 0;
}

uint32_t bsp_dma_uart_read(P_BSP_UART_DMA_PARAM p_uart_param, uint8_t *p_dest, uint32_t len)
{
    return kfifo_pull_out(p_uart_param->p_uart_rx_fifo, p_dest, len);
}

void dma_uart_irq_action(P_BSP_UART_DMA_PARAM p_uart_param)
{
   
}

uint32_t dma_uart_r_buf_num(P_BSP_UART_DMA_PARAM p_uart_param)
{
    return 0;
}

uint32_t dma_uart_w_buf_num(P_BSP_UART_DMA_PARAM p_uart_param)
{
    return 0;
}

void bsp_dma_uart_set_baudrate(P_BSP_UART_DMA_PARAM p_uart_param, uint32_t baudrate)
{
    UART_InitTypeDef uartx_config;
    UART_StructInit(&uartx_config);

    // 这些是默认配置，简单的应用只配置波特率即可
    uartx_config.CK_TYPE = p_uart_param->uart_check_bits; // 奇偶校验配置       0:偶校验（EVEN）;1: 奇校验（ODD）
    uartx_config.STOP_LEN = p_uart_param->uart_stop_bits; // 停止位长度配置     0:1-Bit;1:2-Bit
    uartx_config.BYTE_LEN = p_uart_param->uart_data_bits; // 数据长度配置       0:8-Bit;1:9-Bit
    uartx_config.BAUDRATE = baudrate;                     // 波特率

    UART_Init(p_uart_param->uart_peri_port, &uartx_config);
    UART_NVIC_Enable(p_uart_param->uart_peri_port, UART_IRQEna_RcvOver, 1);
}
