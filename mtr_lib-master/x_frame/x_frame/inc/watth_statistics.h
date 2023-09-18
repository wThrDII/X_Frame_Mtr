/**
 * @brief watth_statistics.c.xtemp header file
 * @file watth_statistics.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 16:56:51
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 16:56:51
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __WATTH_STATISTICS_H__
#define __WATTH_STATISTICS_H__

/**
 * @brief 注册能量统计
 *
 * @param [in]  watth_type      能量类型,用于区分不同类型能量   
 * @param [in]  part_id         flash分区的ID 
 * 
 * @details 
 * @par 示例:注册一个能量类型为1的能量统计,统计数据保存到flash中的SYS_APP_CFG_PART区域
 * @code
 *   REG_WATTH_STATISTICS(1,SYS_APP_CFG_PART)
 * @endcode
 */
#define	REG_WATTH_STATISTICS(watth_type,part_id)																					\
			REG_FLASH_NORMAL_PARAM(part_id, ws_save_##watth_type, NULL, NULL, sizeof(watth_week_item_t))									\
			watth_statistics_item_t	watth_statistics_##watth_type = {watth_type,X_UID_VAL(ws_save_##watth_type)};					\
			AUTO_REG_ITEM(reg_watth_statistics_##watth_type,AUTO_REG_WATTH_STATISTICS,(void*)&watth_statistics_##watth_type);

/**
 * @brief 注册能量统计的推送链路
 *
 * @param [in]  link        推送的链路
 * @param [in]  dest        推送的目的地址
 * @param [in]  priority    推送的优先级，越小，优先级越高
 *
 * @details 
 * @par 示例:注册能量统计推送链路，链路ID为LINK_UART1，推送的目的地址为0x21，推送优先级为1
 * @code
 *   REG_WATTH_STATISTICS_PUSH_ITEM(LINK_UART1,0x21,1)
 * @endcode
 */
#define REG_WATTH_STATISTICS_PUSH_ITEM(link,dest,priority)					\
			REG_PROTOCOL_PUSH_ITEM(link, dest, 1000, priority, x_watth_statistics_push_cmd)				\
			watth_statistics_push_run_t g_ws_push_run##link##_##dest = {link,dest,FUNC_CMDSET_REC,CMD_ID_WATTH_PUSH};				\
			AUTO_REG_ITEM(reg_g_g_ws_push_run##link##_##dest,AUTO_REG_WATTH_STATISTICS_PUSH,(void*)&g_ws_push_run##link##_##dest);

/**
 * @brief 注册能量统计应答链路
 *
 * @param [in]  link        应答链路ID 
 *
 * @details 
 * @par 示例:注册能量统计应答链路，应答链路ID为LINK_UART1
 * @code
 *   REG_WATTH_STATISTICS_PUSH_ACK(LINK_UART1)
 * @endcode
 */
#define REG_WATTH_STATISTICS_PUSH_ACK(link)			            	REG_LINK_ACTION(link, FUNC_CMDSET_REC, CMD_ID_WATTH_PUSH, x_watth_statistics_push_ack)			
		
/**
 * @brief 获取累计能量统计数据
 *
 * @param [in]  watt_type        能量类型,用于区分不同类型能量 
 *
 * @return float 
 * @retval 返回获取的能量统计累计值
 * @details 
 * @par 示例:获取能量类型为1的能量统计累计值，返回给变量watth
 * @code
 *   float watth;
 *   uint16_t watth_type = 1;
 *   watth = get_watth_statistics_all_watth(watth_type);
 * @endcode
 */
#define get_watth_statistics_all_watth(watt_type)				    real_get_watth_statistics_all_watth(watt_type)

/**
 * @brief 保存所有能量统计数据
 *
 * @param [in]  无
 *  
 * @details 
 * @par 示例:保存所有能量统计数据，写入flash中保存
 * @code
 *   watth_statistics_save_all_watth();
 * @endcode
 */
#define watth_statistics_save_all_watth()						    real_watth_statistics_save_all_watth() 


float hd_get_watt(uint16_t watt_type);

#pragma pack(1)

typedef struct {
	uint32_t unix_time;       			/* unix 时间, 自动填充 */
    uint32_t watth_type;				/* 能量类型 */
	uint16_t watth[24];					/* 一天24小时的功率 */
}watth_item_t;

typedef struct {
	watth_item_t item_arry[7];
    double all_watth;
}watth_week_item_t;

typedef struct {
	uint32_t watth_type;
	_X_UID_TYPE param_name;
	float day_watth;
	float hour_watth;
    double all_watth;
	watth_item_t w_item;
	uint32_t last_sec;
	X_CALENDAR_DATE date;
	bool is_history_init;
	uint8_t last_hour;
	list_t	list;
}watth_statistics_item_t;

typedef struct {
	uint8_t seq;
	watth_item_t item;
}watth_statistics_push_cmd_t;

typedef struct {
	uint8_t sta;
	uint8_t seq;
	uint32_t watth_type;				/* 能量类型 */
}watth_statistics_push_ack_t;

typedef struct {
	uint32_t link_id;
	uint8_t dest;
	uint8_t cmd_func;
	uint8_t cmd_id;
	
	uint8_t seq;
	
	uint8_t flg;									/* 标记 */
	uint8_t next_push_decrase;		/* 下次推送递减计数器 */
	list_t	*cur_list;
	bool week_mark[7];
	list_t list;
}watth_statistics_push_run_t;

#pragma pack()

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

float hd_get_watt(uint16_t watt_type);
uint16_t x_watth_statistics_push_cmd(uint8_t is_first,uint32_t link_id,uint8_t dest);
int32_t x_watth_statistics_push_ack(hal_frame_info_t *p_frame);
float real_get_watth_statistics_all_watth(uint16_t watt_type);
void real_watth_statistics_save_all_watth(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __WATTH_STATISTICS_H__
