/**
 * @brief rpc.c.xtemp header file
 * @file rpc.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 16:36:12
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 16:36:12
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __RPC_H__
#define __RPC_H__

/**
 * @brief 初始化rpc(远程过程调用)
 *
 * @param [in] rpc_id 用户定义的rpc枚举号
 * @param [in] func 用户自定义的处理回调函数
 * @param [in] param_max_len 数据buff最大长度
 *
 * @details:
 * @par e.g:
 * @code
 * 		void user_func(uint32_t cmd,uint8_t* pbuf,uint32_t len);  //函数声明
 *
 *		REG_RPC_ITEM(1,user_func,150);
 *
 * @endcode
 */
#define REG_RPC_ITEM(rpc_id,func,param_max_len)  \
			static uint8_t       	rpc_buf_##rpc_id[1+param_max_len]={0} ;   \
			const rpc_item_t        rpc_item_##rpc_id={rpc_id,func,rpc_buf_##rpc_id,param_max_len} ;   \
			reg_rpc_item_t          reg_rpc_item##rpc_id  = {&rpc_item_##rpc_id};\
			AUTO_REG_ITEM(reg_rpc_##rpc_id,AUTO_REG_RPC,(void*)&reg_rpc_item##rpc_id)

/**
 * @brief 同步rpc(远程过程调用)
 *
 * @param [in] rpc_id 使用到的枚举号
 * @param [in] cmd 用户自定义命令
 * @param [in] pbuf 数据buff
 * @param [in] len 数据长度
 *
 * @details:
 * @par e.g:
 * @code
 * 		#define read 1
 * 
 *    	rpc_call_sync(1,read,user_buff,150);
 * @endcode
 */
#define rpc_call_sync(rpc_id,cmd,pbuf,len)				real_rpc_call_sync(rpc_id,cmd,pbuf,len)

/**
 * @brief 异步rpc(远程过程调用)
 * 
 * @param [in] rpc_id 使用到的枚举号        
 * @param [in] cmd 用户自定义命令         
 * @param [in] pbuf 数据buff  
 * @param [in] len 数据长度  
 *  
 * @details: 
 * @par e.g:
 * @code
 *    	#define read 1
 * 
 *    	rpc_call_async(1,read,user_buff,150);
 * @endcode
 */
#define rpc_call_async(rpc_id,cmd,pbuf,len)         	real_rpc_call_async(rpc_id,cmd,pbuf,len)

/**
 * @brief 延时rpc(远程过程调用)
 * 
 * @param [in] ms 延时时间,单位 ms
 * @param [in] rpc_id 使用到的枚举号       
 * @param [in] cmd 用户自定义命令         
 * @param [in] pbuf 数据buff  
 * @param [in] len 数据长度 
 * 
 * @details: 
 * @par e.g:
 * @code
 *    	#define read 1
 * 
 *    	rpc_call_delay(50,1,read,user_buff,150);   
 * @endcode
 */
#define rpc_call_delay(ms,rpc_id,cmd,pbuf,len)      	real_rpc_call_delay(ms,rpc_id,cmd,pbuf,len)


typedef void (*pf_rpc_action)(uint32_t cmd,uint8_t* pbuf,uint32_t len);
	
typedef struct
{
	uint32_t    		rpc_id;
	pf_rpc_action 		rpc_action;
	uint8_t*			p_param_buf;
	uint32_t			param_max_size;	
}rpc_item_t;

enum
{
	RPC_PADDING_NONE  = 0,
	RPC_PADDING_SYNC  = 1,
	RPC_PADDING_ASYNC = 2,
	RPC_PADDING_DELAY = 3,
	
	//-------------
	RPC_PADDING_ERR,
};

typedef struct
{
	const rpc_item_t *   rpc_item;
	uint32_t		     param_len;
	uint32_t		     start_time;
	uint32_t		     time_out;
	uint32_t             exec_cmd;
	uint32_t			 is_padding_type;
	list_t				 rpc_item_node;
}reg_rpc_item_t;

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

void real_rpc_call_sync(uint32_t rpc_id,uint32_t cmd,uint8_t* pbuf,uint32_t len);
void real_rpc_call_async(uint32_t rpc_id,uint32_t cmd,uint8_t* pbuf,uint32_t len);
void real_rpc_call_delay(uint32_t ms,uint32_t rpc_id,uint32_t cmd,uint8_t* pbuf,uint32_t len);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __RPC_H__
