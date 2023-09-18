/**
 * @file bsp_can.**/ 
 
 #ifndef BSP_CAN_H
 #define BSP_CAN_H
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
#include "lks32mc45x.h"
#include "lks32mc45x_lib.h"

#include "x_frame.h"
#include "xh_can.h"

#define BSP_CAN_MAX_RX_IRQ_NUM     4
#define BSP_CAN_MAX_RETRAIN     	200

typedef struct _BSP_CAN_S_PACK_OBJ {
    uint32_t id;
    uint8_t pack_type;
    uint8_t len;
    uint8_t data[8];
}BSP_CAN_S_PACK_OBJ ,*P_BSP_CAN_S_PACK_OBJ;

typedef struct _BSP_CAN_S_OBJ{
    
    P_BKFIFO_OBJ p_s_bkfifo;
		uint32_t sys_module;
    IRQn_Type   nvic_irq;

    GPIO_TypeDef* rx_port;       /* 接收端口 */
    uint16_t    rx_pin;        /* 接收端口引脚 */
	uint32_t    GPIO_rx_PinSource;
    GPIO_TypeDef* tx_port;       /* 发送端口 */
    uint16_t    tx_pin;        /* 发送端口引脚 */
	uint32_t    GPIO_tx_PinSource;
    uint32_t    def_baud;       //波特率

    uint8_t     pre_priority;
    uint8_t     sub_priority;

    xh_can_irq_cb_obj rx_irq_cb[BSP_CAN_MAX_RX_IRQ_NUM];
    BSP_CAN_S_PACK_OBJ		rx_msg;
    BSP_CAN_S_PACK_OBJ		tx_msg;


    uint8_t is_sending;
    uint8_t is_time_delay;
    uint32_t retrain_cnt;				    /* 重发计数 */
    uint32_t retrain_cnt_all;				/* 同一消息的重发总计数 */
    uint32_t last_retrain_timestamp;		/* 最后重传时间戳 */
    uint32_t last_tx_irq_timestamp;			/* 发送中断时间戳 */
    uint32_t can_rece_irq_time;			    /* can接收中断时间 */
    uint8_t  can_rece_irq_mark;				/* can中断接收标记 */
}BSP_CAN_S_OBJ, *P_BSP_CAN_S_OBJ;




#define REG_BSP_CAN_DRV(dev_id,prio,periph,rx_port,rx_pin,rx_pin_source,tx_port,tx_pin,tx_pin_source,pre_priority,sub_priority,def_baud,s_cache_num)  \
        DEF_BKFIFO_OBJ(g_bsp_can_s_bkfifo_##dev_id,BSP_CAN_S_PACK_OBJ,s_cache_num)                              \
		BSP_CAN_S_OBJ _##dev_id =                                                                           \
		{                                                                                                   \
			&g_bsp_can_s_bkfifo_##dev_id,																																			\
			SYS_MODULE_##periph,                                                                                   \
            periph##_IRQn,                                                                                  \
            rx_port,                                                                                        \
            rx_pin,                                                                                         \
            rx_pin_source,                                                                                  \
            tx_port,                                                                                        \
            tx_pin,                                                                                         \
            tx_pin_source,                                                                                  \
			def_baud,                                                                                       \
			pre_priority,                                                                                   \
			sub_priority,                                                                                   \
		};                                                                                                  \
    void CAN_IRQHandler(void)                                                                               \
    {                                                                                                       \
        bsp_can_irq_action(&_##dev_id);                                                                     \
    }                                                                                                       \
		static void can_s_task_##dev_id(void){bsp_can_time_task(&_##dev_id);}						            						\
		REG_TASK(VH_TASK,0,can_s_task_##dev_id);															    															\
    REG_XH_CAN_DRV(dev_id,&_##dev_id,prio,DEV_AUTO_INIT,bsp_can_init,bsp_can_deinit,bsp_can_s_send_pack,bsp_can_reg_irq_cb,bsp_can_set_baudrate);
	







void bsp_can_init(P_BSP_CAN_S_OBJ p_obj);
bool bsp_can_s_send_pack(P_BSP_CAN_S_OBJ p_obj, uint32_t pack_type, uint32_t id, uint8_t *p_data, uint32_t len);
void bsp_can_irq_action(P_BSP_CAN_S_OBJ p_obj);
void bsp_can_time_task(P_BSP_CAN_S_OBJ p_obj);
bool bsp_can_reg_irq_cb(P_BSP_CAN_S_OBJ p_obj, xh_can_irq_cb_obj irq_cb);
void bsp_can_irq_rx_action(P_BSP_CAN_S_OBJ p_obj);
void bsp_can_set_baudrate(P_BSP_CAN_S_OBJ p_obj, uint32_t baudrate);
void bsp_can_deinit(P_BSP_CAN_S_OBJ p_obj);


#ifdef __cplusplus
}
#endif

#endif  // BSP_CAN_H
 
