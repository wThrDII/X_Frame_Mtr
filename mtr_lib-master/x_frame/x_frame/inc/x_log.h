/**
 * @brief x_log.c.xtemp header file
 * @file x_log.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 16:54:42
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 16:54:42
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __X_LOG_H__
#define __X_LOG_H__

/**
 * @brief 注册日志模块
 *
 * @param [in]  part_id        	   存储的flash区域ID
 * @param [in]  part_block_size    存储的flash区域的块大小 
 * @param [in]  log_max_num        存储的日志条数  
 *
 * @details 
 * @par 示例:注册日志模块，存储在SYS_APP_CFG_PART中，flsh的块大小为FLASH_BLOCK_SIZE，存储100条日志
 * @code
 *   REG_X_LOG_MOD(SYS_APP_CFG_PART,FLASH_BLOCK_SIZE,100)
 * @endcode
 */
#define	REG_X_LOG_MOD(part_id,part_block_size,log_max_num)																	\
			REG_FALSH_QUEUE_PARAM(part_id, x_log_mod, NULL, NULL, sizeof(log_mod_item_t), log_max_num, part_block_size)			\
			log_mod_obj_t g_log_mod_obj_t = {log_max_num,FUNC_CMDSET_REC,CMD_ID_LOG_PUSH};																	\
			
/**
 * @brief 注册推送项
 *
 * @param [in]  link        推送的链路 
 * @param [in]  dest        推送的目的地址 
 * @param [in]  priority    推送的优先级，越小，优先级越高 
 *
 * @details 
 * @par 示例:注册推送项，推送链路为LINK_UART1，推送的目的地址为0x21，优先级为1
 * @code
 *   REG_LOG_PUSH_ITEM(LINK_UART1,0x21,1)
 * @endcode
 */
#define	REG_LOG_PUSH_ITEM(link,dest,priority)													\
			REG_PROTOCOL_PUSH_ITEM(link, dest, 1000, priority, x_log_mod_push_cmd)				\
			log_mod_push_run_t g_log_mod_push_run_##link##_##dest = {LIST_HEAD_INIT(g_log_mod_push_run_##link##_##dest.list),link,dest};				\
			AUTO_REG_ITEM(reg_g_log_mod_push_run_##link##_##dest,AUTO_REG_LOG_PUSH,(void*)&g_log_mod_push_run_##link##_##dest);

/**
 * @brief 注册推送应答
 *
 * @param [in]  link        推送应答的链路 
 *
 * @details 
 * @par 示例:注册推送应答，应答链路为LINK_UART1
 * @code
 *   REG_LOG_PUSH_ACK(LINK_UART1)
 * @endcode
 */
#define	REG_LOG_PUSH_ACK(link)				REG_LINK_ACTION(link, FUNC_CMDSET_REC, CMD_ID_LOG_PUSH, x_log_mod_push_ack)

/**
 * @brief 写入日志
 *
 * @param [in]  p_item        储存日志的结构体指针 
 *
 * @details 
 * @par 示例:写入日志，日志编号log_num为20，缓存写入221.4
 * @code
 *   log_mod_item_t item;
 * 	 item.log_num = 20;
 * 	 item.custom_buf[0] = 221.4f;
 * 	 item.custom_buf[1] = 221.4f;
 * 	 item.custom_buf[2] = 221.4f;
 * 	 item.custom_buf[3] = 221.4f;
 * 	 x_log_add(&item);
 * @endcode
 */
#define x_log_add(p_item)									real_x_log_add(p_item)

/**
 * @brief 读取日志
 *
 * @param [in]  p_log_arry        储存日志的地址 
 * @param [in]  index             读取日志的起始位置
 * @param [in]  num               读取日志的个数
 * 
 * @details 
 * @par 示例:读取最近的50条日志，存到缓存arry_item[50]中
 * @code
 * 	 log_mod_item_t arry_item[50];
 *   read_num = _x_log_read(arry_item,0,50);	
 * @endcode
 */
#define x_log_read(p_log_arry, index, num)					real_x_log_read(p_log_arry, index, num)


#pragma pack(1)

typedef struct {
	uint32_t unix_time;       			/* unix 时间, 自动填充 */
    uint32_t ms;						/* 系统上电ms数, 自动填充  */
	uint32_t log_num;       			/* 日志编号 */
    float custom_buf[4];				/* 自定义的缓存 */
}log_mod_item_t;

typedef struct{
    log_mod_item_t *log_mod_item; // 日志内容 
    uint8_t ver;  // 版本号
	uint8_t seq;
    uint8_t num; // 日志条数
}log_mod_push_cmd_t;

typedef struct{
	uint8_t sta;
    uint8_t seq;
	uint8_t num;
}log_mod_push_ack_t;

typedef struct{
    list_t list;
	uint32_t link_id;
	uint8_t dest;
	uint8_t cur_seq;
	uint8_t push_index;
}log_mod_push_run_t;

typedef struct{
	uint32_t max_log_num;
	uint8_t cmd_func;
	uint8_t cmd_id;
}log_mod_obj_t;

#pragma pack()

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

void real_x_log_add(log_mod_item_t *p_item);
uint16_t real_x_log_read(log_mod_item_t *p_log_arry, uint32_t index, uint32_t num);
int32_t x_log_mod_push_ack(hal_frame_info_t *p_frame);
uint16_t x_log_mod_push_cmd(uint8_t is_first,uint32_t link_id,uint8_t dest);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __X_LOG_H__
