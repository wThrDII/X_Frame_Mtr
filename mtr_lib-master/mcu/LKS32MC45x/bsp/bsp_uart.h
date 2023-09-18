/**
 * @brief
 * @file bsp_uart.h
 * @version 1.0
 * @author yaqi.wang
 * @date 2023-01-09 15:17:27
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 *
 * @details 文件详细描述，包含使用场景，依赖项等
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2023-01-09 15:17:27      <td>yaqi.wang      <td>创建文件
 */

#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "lks32mc45x_lib.h"
#include "kfifo.h"
#include "xh_stream.h"

typedef u32 uint32_t;
typedef u16 uint16_t;
typedef u8 uint8_t;

enum
{
    USART_WL_8BIT,
    USART_WL_9BIT,
};

enum
{
    USART_STB_1BIT,
    USART_STB_2BIT,
};

enum
{
    USART_PM_EVEN,
    USART_PM_ODD,
};

typedef struct _BSP_UART_PARAM
{
    UART_TypeDef* uart_peri_port; /* 串口外设端口 */

    uint32_t uart_baud;       /* 串口波特率 */
    uint32_t uart_data_bits;  /* 串口数据位 */
    uint32_t uart_stop_bits;  /* 串口停止位 */
    uint32_t uart_check_bits; /* 串口校验位 */

    uint16_t uart_irq_priority; /* 串口优先级 */
    IRQn_Type uart_irq_src;     /* 串口中断源 */

    GPIO_TypeDef* uart_tx_port;     /* 发送端口 */
    uint32_t uart_tx_pin;      /* 发送端口引脚 */

    GPIO_TypeDef* uart_rx_port;     /* 接收端口 */
    uint32_t uart_rx_pin;      /* 接收端口引脚 */
    
    kfifo_t *p_uart_tx_fifo; /* 接收环形队列指针 */
    kfifo_t *p_uart_rx_fifo; /* 读取环形队列指针 */

    void (*send_start_cb)(void);
    void (*send_stop_cb)(void);
} BSP_UART_PARAM, *P_BSP_UART_PARAM;

#define REG_XH_BSP_UART(dev_id,prio, port, uart_irq_priority,                         \
                             tx_port,tx_pin, rx_port,rx_pin,              \
                             baud, data_bits, stop, chk,                    \
                             tx_buf_size, rx_buf_size,                      \
                             send_start_cb,send_stop_cb)                    \
        DEF_KFIFO(bsp_uart_tx_fifo_##dev_id,tx_buf_size)                    \
        DEF_KFIFO(bsp_uart_rx_fifo_##dev_id,rx_buf_size)                    \
        BSP_UART_PARAM _##dev_id =                                          \
        {                                                                   \
            port,                                                           \
            baud,                                                           \
            data_bits,                                                      \
            stop,                                                           \
            chk,                                                            \
            uart_irq_priority,                                                   \
            port##_IRQn,                                                    \
            tx_port,          tx_pin,                         \
            rx_port,          rx_pin,                         \
            &bsp_uart_tx_fifo_##dev_id,                                     \
            &bsp_uart_rx_fifo_##dev_id,                                     \
            send_start_cb,                                                  \
            send_stop_cb,                                                   \
        };                                                                  \
        extern void uart_irq_action(P_BSP_UART_PARAM p_uart_param);     \
        void port##_IRQHandler(void)                                        \
        {                                                                   \
            uart_irq_action(&_##dev_id);                                    \
        }                                                                   \
        REG_XH_STREAM_DRV(dev_id,&_##dev_id,prio,DEV_AUTO_INIT,bsp_uart_init,NULL,bsp_uart_read,bsp_uart_send,uart_set_baudrate);

       
extern void bsp_uart_init(P_BSP_UART_PARAM p_uart_param);
extern void bsp_uart_deinit(P_BSP_UART_PARAM p_uart_param);
extern uint32_t bsp_uart_send(P_BSP_UART_PARAM p_uart_param, uint8_t *pbuf, uint32_t len);
extern uint32_t bsp_uart_read(P_BSP_UART_PARAM p_uart_param, uint8_t *p_dest, uint32_t len);
extern uint32_t uart_r_buf_num(P_BSP_UART_PARAM p_uart_param);
extern uint32_t uart_w_buf_num(P_BSP_UART_PARAM p_uart_param);
extern void uart_set_baudrate(P_BSP_UART_PARAM p_uart_param,uint32_t baudrate); 
#endif //__BSP_UART_H__
