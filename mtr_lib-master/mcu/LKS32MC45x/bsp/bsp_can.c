#include "bsp_can.h"





#define TX_IRQ_TIMEOUT_MS			10				//发送中断未触发重启发送超时时间

#define BSP_CAN_S_WHILE_MAX_TIMES			100				//While最大循环次数




void bsp_can_init(P_BSP_CAN_S_OBJ p_obj)
{
SYS_WR_PROTECT = 0x7a83;   /*使能系统寄存器写操作*/
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = p_obj->rx_pin;
    GPIO_Init(p_obj->rx_port, &GPIO_InitStruct);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = p_obj->tx_pin;
    GPIO_Init(p_obj->tx_port, &GPIO_InitStruct);
    GPIO_PinAFConfig(p_obj->rx_port,p_obj->GPIO_rx_PinSource,GPIO_AF_CAN);
    GPIO_PinAFConfig(p_obj->tx_port,p_obj->GPIO_tx_PinSource,GPIO_AF_CAN);


    if (p_obj->def_baud == 1000000) {
        CAN_Initl(0x0B,0x01,0x01,0x0C); //CAN波特率1000K
    } else if (p_obj->def_baud == 500000) {
        CAN_Initl(0x17,0x01,0x01,0x0C); //CAN波特率500K
    } else if (p_obj->def_baud == 100000) {
        CAN_Initl(0x63,0x01,0x01,0x0C); //CAN波特率100K
    }

    ID0_Filter(0x02,0xFFFFFFFF,1);  //开启IDO接收滤波，接收ID=0x02,扩展帧
//    ID2_Filter(0x03,0x000,0);  //开启ID2接收滤波，接收ID=0x03,扩展帧
//    ID3_Filter(0x06,0x000,0);  //开启ID3接收滤波，接收ID=0x06,扩展帧
//    ID5_Filter(0x04,0x000,1);  //开启ID4接收滤波，接收ID=0x04,扩展帧
//    ID11_Filter(0x08,0x000,0); //开启ID11接收滤波，接收ID=0x08,扩展帧
//    ID14_Filter(0x07,0x000,1); //开启ID14接收滤波，接收ID=0x07,扩展帧
block_ms(10);
SYS_WR_PROTECT = 0x0;    /* 关闭系统寄存器写操作*/
    NVIC_SetPriority(p_obj->nvic_irq, p_obj->pre_priority); 
    NVIC_EnableIRQ(p_obj->nvic_irq);
    p_obj->retrain_cnt_all = 0;
	CAN_CFG_STAT &= ~(1 << 7);      
}


/**
 * @brief 从fifo中取出数据并发送数据包
 * 
 * @param [in] p_obj       参数指针  
 * 
 * @return true 
 *  @retval 发送成功
 * @return false 
 *  @retval 发送失败
 * 
 * @details 
 */
static inline bool bsp_can_transmit_fifo_message(P_BSP_CAN_S_OBJ p_obj)
{
    static uint8_t times = 0;
    while (1) {
        if(!bkfifo_is_empty(p_obj->p_s_bkfifo)) {
            BSP_CAN_S_PACK_OBJ pack;
            bkfifo_de(p_obj->p_s_bkfifo, &pack);
            memcpy(&p_obj->tx_msg.id, &pack.id, 4);
            memcpy(p_obj->tx_msg.data, pack.data, pack.len);
            p_obj->tx_msg.len = pack.len;
            p_obj->tx_msg.pack_type = pack.pack_type;
            //whlie次数判断
			if (times >= BSP_CAN_S_WHILE_MAX_TIMES) {
                times = 0;
				return false;
			}
            //长度判断
			if (p_obj->tx_msg.len > 8) {
                times++;
				continue;
			}
            //标准帧-判断标准ID
			if (p_obj->tx_msg.pack_type == 0)
			{
				//11位的标准ID禁止高7位全部为1
				if((p_obj->tx_msg.id & 0x7F0) == 0x7F0)
				{
                    times++;
					continue;
				}
			}
			//扩展帧-判断扩展ID
			if (p_obj->tx_msg.pack_type == 1)
			{
				//28位的标准ID禁止高7位全部为1
				if((p_obj->tx_msg.id & 0X1FC00000) == 0X1FC00000)
				{
                    times++;
					continue;
				}
			}
            My_CAN_Send_Msg(p_obj->tx_msg.id,p_obj->tx_msg.pack_type,0, p_obj->tx_msg.data,p_obj->tx_msg.len);
            times = 0;
            return true;
        } else {
			times = 0;
			p_obj->is_sending = false;
			return false;
		}
    }
	
}

/**
 * @brief 将CAN帧暂存到发送fifo中
 * 
 * @param [in] p_obj         参数指针
 * @param [in] pack_type     数据包类型
 * @param [in] id            can id
 * @param [in] p_data        需要发送的数据指针
 * @param [in] len           需要发送的数据长度，1 ~ 8
 * 
 * @return true 
 *  @retval 缓存成功
 * @return false 
 *  @retval 缓存失败
 * 
 * @details 
 */
static inline bool bsp_can_push_to_tx_fifo(P_BSP_CAN_S_OBJ p_obj, uint32_t pack_type, uint32_t id, uint8_t *p_data, uint8_t len)
{
	if(!bkfifo_is_full(p_obj->p_s_bkfifo)) {
		BSP_CAN_S_PACK_OBJ pack;
		pack.id = id;
		memcpy(pack.data, p_data, len);
		pack.len = len;
        if (pack_type == 0) {
			pack.pack_type = 0;
		} else {
			pack.pack_type = 1;
		}
		bkfifo_en(p_obj->p_s_bkfifo, &pack);
		return true;
	}
	return false;
}

/**
 * @brief can 发送数据包
 * 
 * @param [in] p_obj         参数指针
 * @param [in] pack_type     数据包类型
 * @param [in] id            can id
 * @param [in] p_data        需要发送的数据指针
 * @param [in] len           需要发送的数据长度，1 ~ 8         
 * 
 * @return true 
 *  @retval 发送成功
 * @return false 
 *  @retval 发送失败
 * 
 * @details 
 */
bool bsp_can_s_send_pack(P_BSP_CAN_S_OBJ p_obj, uint32_t pack_type, uint32_t id, uint8_t *p_data, uint32_t len)
{
	if(p_obj->is_sending == false){
		bsp_can_push_to_tx_fifo(p_obj, pack_type, id, p_data, len);
		if (bsp_can_transmit_fifo_message(p_obj) == true) {
            p_obj->is_sending = true;
            p_obj->last_tx_irq_timestamp = get_sys_ms();
		    return true;    
        } else {
            return false;    
        }	
	}
	return bsp_can_push_to_tx_fifo(p_obj, pack_type, id, p_data, len);
}




/**
 * @brief can 重发函数
 * 
 * @param [in] p_obj        参数指针 
 * 
 * 
 * @details 
 */
static inline void bsp_can_retrain_send(P_BSP_CAN_S_OBJ p_obj)
{
	if (p_obj->retrain_cnt_all < BSP_CAN_MAX_RETRAIN) {
        My_CAN_Send_Msg(p_obj->tx_msg.id,1,0, p_obj->tx_msg.data,p_obj->tx_msg.len);
		p_obj->retrain_cnt_all++;
	} else {
		p_obj->retrain_cnt_all = 0;
        bsp_can_transmit_fifo_message(p_obj);
	}
}
/**
 * @brief can 定时处理函数，防止一些异常状态出现
 * 
 * @param [in] p_obj        参数指针 
 * 
 * 
 * @details 
 */
void bsp_can_time_task(P_BSP_CAN_S_OBJ p_obj)
{
  if(check_time_out(p_obj->can_rece_irq_time,500)){
		p_obj->can_rece_irq_mark = false;
	}else{
		p_obj->can_rece_irq_mark = true;
	}
	if(p_obj->is_time_delay == true){
		/* 连续重发n次失败，则延时重发 */
		uint32_t ms_delay = p_obj->retrain_cnt >> 3;
		ms_delay = X_FRAME_MIN(ms_delay, 10);
		if(!check_time_out(p_obj->last_retrain_timestamp, ms_delay))
			return;
		p_obj->last_retrain_timestamp = get_sys_ms();
		
		p_obj->is_time_delay = false;

        bsp_can_retrain_send(p_obj);

		p_obj->last_tx_irq_timestamp = get_sys_ms();
	}
	else if(p_obj->is_sending)
	{
		/* 这段代码防异常发不出数据。*/
		if(check_time_out(p_obj->last_tx_irq_timestamp, TX_IRQ_TIMEOUT_MS))
		{
			bsp_can_retrain_send(p_obj);
			p_obj->last_tx_irq_timestamp = get_sys_ms();
		}
	}
}
/**
 * @brief 注册can中断回调函数
 * 
 * @param [in] p_obj         参数指针 
 * @param [in] irq_cb        回调函数
 * 
 * @return true 
 *  @retval 注册成功
 * @return false 
 *  @retval 注册失败
 * 
 * @details 
 */
bool bsp_can_reg_irq_cb(P_BSP_CAN_S_OBJ p_obj, xh_can_irq_cb_obj irq_cb)
{
    uint16_t cir = 0;
    for(cir = 0;cir < BSP_CAN_MAX_RX_IRQ_NUM;cir++){
        if(p_obj->rx_irq_cb[cir] == irq_cb){
            return true;
        }
    }
    for(cir = 0;cir < BSP_CAN_MAX_RX_IRQ_NUM;cir++){
        if(p_obj->rx_irq_cb[cir] == NULL){
            p_obj->rx_irq_cb[cir] = irq_cb;
            return true;
        }
    }
    return false;
}




void bsp_can_irq_action(P_BSP_CAN_S_OBJ p_obj)
{
  if(CAN_RTIF & BIT3)/*PTB 发送中断标志*/
    {
        CAN_RTIF = BIT3;
        p_obj->last_tx_irq_timestamp = get_sys_ms();
        bsp_can_transmit_fifo_message(p_obj);
    }
    if(CAN_RTIF & BIT2)/*STB 发送中断标志*/
    {
        CAN_RTIF = BIT2; 

    }


	/*****************接收设备一直无应答，则取消重发发送*****************/
 if(CAN_ERRINT & BIT0)/*取消发送中断标志*/
 {
	 CAN_ERRINT = BIT0;
	if(CAN_TECNT > 127)   /*被动错误*/
	{
		if(CAN_EALCAP & 0x80) /* 应答错误*/
		{
//		   CAN_TCMD |= BIT3; /*取消PTB发送*/
		}
	}
 }
 if(CAN_RTIF & BIT7)/*接收到有效帧标志*/
 {
    CAN_RTIF = BIT7;
	CAN_Receive_Msg(&( can_par.id),&( can_par.len),&(can_par.ide),&(can_par.rtr),can_par.RX) ;

    p_obj->rx_msg.len = can_par.len;

	 

    /* 防止数据长度错误，导致后续程序执行指针越界 */
    if(p_obj->rx_msg.len <= 8){
		for (uint8_t i = 0; i < p_obj->rx_msg.len; i++) {
			p_obj->rx_msg.data[i] = can_par.RX[i];
		}
		p_obj->rx_msg.pack_type = can_par.ide;	
        p_obj->rx_msg.id = can_par.id;
		
        p_obj->can_rece_irq_time = get_sys_ms();
        uint16_t cir = 0;
        for(;cir < BSP_CAN_MAX_RX_IRQ_NUM;cir++){
            if(p_obj->rx_irq_cb[cir] != NULL){
                p_obj->rx_irq_cb[cir](p_obj->rx_msg.pack_type, p_obj->rx_msg.id, p_obj->rx_msg.data, p_obj->rx_msg.len);
            }
        }
    }


 }
 
 if(CAN_RTIF & BIT4)
 {
	  CAN_ERRINT = BIT4; /*被动错误中断标志*/
 }
 
 if(CAN_RTIF & BIT5)
 {
   CAN_RTIF = BIT5;
 }
 if(CAN_RTIF & BIT6)
 {
   CAN_RTIF = BIT6;
 }
  if(CAN_RTIF & BIT1)
 {
   CAN_RTIF = BIT1;
 }
 
  CAN_RTIF = 0xff;
}








/**
 * @brief 设置 can 波特率
 * 
 * @param [in] p_obj         参数指针
 * @param [in] baudrate      波特率, 单位 bit/s
 * 
 * 
 * @details 
 */
void bsp_can_set_baudrate(P_BSP_CAN_S_OBJ p_obj, uint32_t baudrate)
{
    p_obj->def_baud = baudrate;
    bsp_can_init(p_obj);
}
/**
 * @brief can 反初始化
 * 
 * @param [in] p_obj         参数指针
 * 
 * 
 * @details 
 */
void bsp_can_deinit(P_BSP_CAN_S_OBJ p_obj)
{
	SYS_SoftResetMODULE(p_obj->sys_module);
}
