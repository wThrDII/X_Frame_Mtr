/**
 * @brief
 * @file bsp_uart.c
 * @version 1.0
 * @author yaqi.wang
 * @date 2023-01-09 15:16:39
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 *
 * @details 文件详细描述，包含使用场景，依赖项等
 * 基于LKS32MC453RCT8开发
 * https://www.linkosemi.com/LKS45Series/list.aspx
 * 数据手册 P11
 * UART0_RX/TX: [P0_1,P0_4,P0_14,P4_12]/[P0_2,P0_5,P0_13,P2_12]
 * UART1_RX/TX: [P0_11,P1_5,P2_5]/[P0_12,P1_6,P2_6,P3_6]
 * UART2_RX/TX: [P1_8,P1_10,P1_12,P2_15]/[P1_9,P1_11,P2_14,P4_14]
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2023-01-09 15:16:39      <td>yaqi.wang      <td>创建文件
 */

#include "bsp_uart.h"

//static void uart_remap(P_BSP_UART_PARAM p_uart_param)
//{
//    uint8_t a = 1;
//}

void bsp_uart_init(P_BSP_UART_PARAM p_uart_param)
{
    UART_InitTypeDef uart1_config;
    GPIO_InitTypeDef uartx_gpio_config;
    
    UART_StructInit(&uart1_config);
    
    //这些是默认配置，简单的应用只配置波特率即可
    uart1_config.DUPLEX       = DISABLE;   // 半双工模式使能，tx_data
    uart1_config.MD_EN        = DISABLE;   // Multi-drop 使能
    uart1_config.CK_EN        = DISABLE;   // 数据校验使能
    uart1_config.CK_TYPE      = p_uart_param->uart_check_bits;         // 奇偶校验配置       0:偶校验（EVEN）;1: 奇校验（ODD）
    uart1_config.BIT_ORDER    = 0;         // 数据发送顺序配置   0:LSB;1:MSB
    uart1_config.STOP_LEN     = p_uart_param->uart_stop_bits;         // 停止位长度配置     0:1-Bit;1:2-Bit
    uart1_config.BYTE_LEN     = p_uart_param->uart_data_bits;         // 数据长度配置       0:8-Bit;1:9-Bit
    uart1_config.BAUDRATE     = 115200;      // 波特率
    uart1_config.ADR          = 0;         // 多机通讯时的从机地址
    uart1_config.TX_BUF_EMPTY = DISABLE;   // 发送缓冲区空 DMA 请求使能
    uart1_config.RX_DONE      = DISABLE;   // 接收完成 DMA 请求使能
    uart1_config.TX_DONE      = DISABLE;   // 发送完成 DMA 请求使能
    uart1_config.TXD_INV      = DISABLE;   // TXD 输出极性取反
    uart1_config.RXD_INV      = DISABLE;   // RXD 输入极性取反
    uart1_config.IE           = 0;   // 中断配置
    uart1_config.BAUDRATE     = p_uart_param->uart_baud;
    
    UART_Init(p_uart_param->uart_peri_port,&uart1_config);
    
//    UART_NVIC_Enable(UART1,UART_IRQEna_SendOver,1);
    UART_NVIC_Enable(p_uart_param->uart_peri_port,UART_IRQEna_RcvOver,1);
//    UART_NVIC_Enable(p_uart_param->uart_peri_port,UART_IRQEna_SendBuffEmpty,1);
    UART_NVIC_Enable(p_uart_param->uart_peri_port,UART_IRQEna_CheckError,1);

    
    GPIO_StructInit(&uartx_gpio_config);
    uartx_gpio_config.GPIO_Pin = (1 << p_uart_param->uart_tx_pin); // 要配置的PIN
    uartx_gpio_config.GPIO_Mode     =   GPIO_Mode_OUT;       // GPIO模式:输入、输出、模拟
    uartx_gpio_config.GPIO_PuPd     =   GPIO_PuPd_NOPULL;   // 上拉/下拉
    uartx_gpio_config.GPIO_PODEna   =   DISABLE;            // 开漏使能
    uartx_gpio_config.GPIO_PFLT     =   DISABLE;            // 滤波使能
    GPIO_Init(p_uart_param->uart_tx_port,&uartx_gpio_config);
    
    GPIO_StructInit(&uartx_gpio_config);
    uartx_gpio_config.GPIO_Pin = (1 << p_uart_param->uart_rx_pin); // 要配置的PIN
    uartx_gpio_config.GPIO_Mode     =   GPIO_Mode_IN;      // GPIO模式:输入、输出、模拟
    uartx_gpio_config.GPIO_PuPd     =   GPIO_PuPd_NOPULL;   // 上拉/下拉
    uartx_gpio_config.GPIO_PODEna   =   DISABLE;            // 开漏使能
    uartx_gpio_config.GPIO_PFLT     =   DISABLE;            // 滤波使能
    GPIO_Init(p_uart_param->uart_rx_port,&uartx_gpio_config);    
    
    GPIO_PinAFConfig(p_uart_param->uart_tx_port,p_uart_param->uart_tx_pin,GPIO_AF_UART);
    GPIO_PinAFConfig(p_uart_param->uart_rx_port,p_uart_param->uart_rx_pin,GPIO_AF_UART);    
    
    NVIC_SetPriority(p_uart_param->uart_irq_src, p_uart_param->uart_irq_priority);
    NVIC_EnableIRQ (p_uart_param->uart_irq_src);
}

//void bsp_uart_deinit(P_BSP_UART_PARAM p_uart_param) 
//{
//    uint8_t a = 1;
//}

uint32_t bsp_uart_send(P_BSP_UART_PARAM p_uart_param, uint8_t *pbuf, uint32_t len) 
{
    uint16_t real_len;
    if(len > 1)
    {
        real_len = kfifo_push_in(p_uart_param->p_uart_tx_fifo, &pbuf[1],len-1);   
    }
    UART_NVIC_Enable(p_uart_param->uart_peri_port,UART_IRQEna_SendOver,1);
    UART_SendData(p_uart_param->uart_peri_port,(uint32_t)pbuf[0]);
    return real_len;
}

uint32_t bsp_uart_read(P_BSP_UART_PARAM p_uart_param, uint8_t *p_dest, uint32_t len) 
{
    return kfifo_pull_out(p_uart_param->p_uart_rx_fifo, p_dest, len);
}

void uart_irq_action(P_BSP_UART_PARAM p_uart_param) 
{
    uint8_t data;
    //发送中断
    if(UART_GetIRQFlag(p_uart_param->uart_peri_port,UART_IRQEna_SendOver) ) 
    {
        if(kfifo_pull_out(p_uart_param->p_uart_tx_fifo, &data, 1)) 
        {
            UART_SendData(p_uart_param->uart_peri_port,(uint32_t)data);
        } 
        else
        {
            UART_NVIC_Enable(p_uart_param->uart_peri_port,UART_IRQEna_SendOver,0);
        }  
            UART_ClearIRQFlag(p_uart_param->uart_peri_port,UART_IRQEna_SendOver);           
    }
    //接收中断
    if(UART_GetIRQFlag(p_uart_param->uart_peri_port,UART_IF_RcvOver)) {        
        data = UART_ReadData(p_uart_param->uart_peri_port);
        kfifo_push_in(p_uart_param->p_uart_rx_fifo, &data, 1);
        UART_ClearIRQFlag(p_uart_param->uart_peri_port,UART_IF_RcvOver);
    }
}

uint32_t uart_r_buf_num(P_BSP_UART_PARAM p_uart_param) 
{
    return kfifo_get_data_len(p_uart_param->p_uart_rx_fifo);
}

uint32_t uart_w_buf_num(P_BSP_UART_PARAM p_uart_param) 
{
    return kfifo_get_data_len(p_uart_param->p_uart_tx_fifo);
}

void uart_set_baudrate(P_BSP_UART_PARAM p_uart_param, uint32_t baudrate) 
{
    UART_InitTypeDef uartx_config;  
    UART_StructInit(&uartx_config);
    
    //这些是默认配置，简单的应用只配置波特率即可
    uartx_config.CK_TYPE      = p_uart_param->uart_check_bits;         // 奇偶校验配置       0:偶校验（EVEN）;1: 奇校验（ODD）
    uartx_config.STOP_LEN     = p_uart_param->uart_stop_bits;         // 停止位长度配置     0:1-Bit;1:2-Bit
    uartx_config.BYTE_LEN     = p_uart_param->uart_data_bits;         // 数据长度配置       0:8-Bit;1:9-Bit
    uartx_config.BAUDRATE     = baudrate;      // 波特率

    UART_Init(p_uart_param->uart_peri_port,&uartx_config);
    UART_NVIC_Enable(p_uart_param->uart_peri_port,UART_IRQEna_RcvOver,1);
}
