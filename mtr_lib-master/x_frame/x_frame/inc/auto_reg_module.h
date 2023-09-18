/**
 * @brief auto_reg_module.c.xtemp header file
 * @file auto_reg_module.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 15:34:11
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 15:34:11
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __AUTO_REG_MODULE_H__
#define __AUTO_REG_MODULE_H__


#define USE_REG_SECTION         1 //1-从section加载   0-从数组加载

#define INFO_SECTION  __attribute__ ((used,section(".info_section")))

#if USE_REG_SECTION == 1
	#define AUTO_REG_SECTION  __attribute__ ((used,section(".auto_reg_section")))
#else
	#define AUTO_REG_SECTION  
#endif

#define AUTO_REG_ITEM(name,type,p_user_data)  const auto_reg_t   __auto_reg_##name   AUTO_REG_SECTION = {#name,type,p_user_data};

enum{   
    /* 注意只允许新增，不允许插入，否则会存在库版本兼容问题 */
    AUTO_REG_TYPE_RESV			= 0x0,

	//DEV_AUTO_INIT,
	AUTO_REG_TYPE_DRIVER		= 1,
	
	AUTO_REG_INIT_MOD			= 2,
	AUTO_REG_TYPE_TASK			= 3,
	
	AUTO_REG_TYPE_DRIVER_PUSH	= 4,
	AUTO_REG_TYPE_DBG_DEV		= 5,
	AUTO_REG_PROTOCOL			= 6,
	AUTO_REG_TYPE_LINK			= 7,
	AUTU_REG_TYPE_LINK_ACTION	= 8,
	AUTO_REG_ROUTE_CFG			= 9,
	AUTO_REG_TYPE_SHELL_X		= 10,
	AUTO_REG_TYPE_SHELL			= 11,
	AUTO_REG_TYPE_PARAM_CFG		= 12,
	AUTO_REG_FSM				= 13,
	AUTO_REG_FSM_STATE			= 14,
	AUTO_REG_AUTOTEST			= 15,
	AUTO_REG_AUTOTEST_CTRL		= 16,
	AUTO_REG_FLASH_PART			= 17,
	AUTO_REG_FLASH_PARAM		= 18,
	AUTO_REG_IOT_REC			= 19,
	AUTO_REG_RPC				= 20,
	AUTO_REG_MEMORY_PART		= 21,
	AUTO_REG_TYPE_SSM			= 22,
	AUTO_REG_WATTH_STATISTICS	= 23,
	AUTO_REG_WATTH_STATISTICS_PUSH	= 24,
	AUTO_REG_LOG_PUSH			= 25,
	AUTO_REG_PROTOCOL_PUSH_ITEM	= 26,
	AUTO_REG_PROTOCOL_PUSH_GROUP	= 27,
    AUTO_REG_AT_PARSER			= 28,
    AUTO_REG_AT_HEADER			= 29,
    AUTO_REG_AT_STATUS			= 30,
    AUTO_REG_INIT_TYPE			= 31,
    AUTO_REG_TYPE_ERR			= 32,
    AUTO_REG_VH_INIT_MOD		= 33,
    AUTO_REG_HI_INIT_MOD		= 34,
    AUTO_REG_LW_INIT_MOD		= 35,
    AUTO_REG_ID_INIT_MOD		= 36,
    AUTU_REG_TYPE_CMDSET_LINK_TAB	= 37,
    AUTO_REG_XH_TYPE	        = 38,
    AUTO_REG_KEY                = 39,
    AUTO_REG_BBOX               = 40,
    AUTO_REG_BBOX_FLASH         = 41,
    AUTO_REG_SILENCE            = 42,
    AUTO_REG_LINK_MONITOR       = 43,
    AUTO_REG_LINK_ROUTE_TAB     = 44,
    AUTO_REG_REP_SOLUTION       = 45,
};

typedef struct __auto_reg_
{
    const char 		*name;
    uint8_t			type;
    void			*p_user_reg_data;
}auto_reg_t;

/*
	INFO_SECTION ROM table
	
	+---------------------------------------+
	|  name1_addr |	type1	|	data_addr1  |
	+---------------------------------------+
	|  name2_addr |	type1	|	data_addr2  |
	+---------------------------------------+
	|  name3_addr |	type2	|	data_addr3  |
	+---------------------------------------+

*/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus
/**
 * @brief 获取自动注册的参数指针
 *
 * @param [in] func_type 自动注册的类型
 * @param [in] index 获取的索引，从0开始
 *
 * @return void* 返回对应类型的初始化函数指针
 *  @retval
 *  NULL:无指针
 *  其他地址:对应类型初始化函数指针
 *
 * @details: 1.如果索引值大于自动注册段总数直接退出函数.
 *           2.返回指针的类型随着注册类型不同，可以是:
 *             <1>对应模块设计的结构体
 *             <2>对应模块设计的函数
 * @par e.g:
 * @code
 *      user_val = (type *) auto_reg_get_func_cont_by_index(AUTO_REG_TYPE_RESV, i);
 * @endcode
 */
void   *auto_reg_get_func_cont_by_index(uint8_t func_type, uint32_t index);

/**
 * @brief 获取一定数量的自动注册的参数指针
 *
 * @param [in] func_type 自动注册类型
 * @param [in] p_cont 缓存指针数组
 * @param [in] max_read_item 读取的最大个数
 *
 * @return int32_t 
 *  @retval 返回自动注册的参数指针的个数
 *
 * @details:
 * @par e.g:
 * @code
 *      user_val = auto_reg_get_func_type_cont(AUTO_REG_TYPE_RESV, );
 * @endcode
 */
int32_t auto_reg_get_func_type_cont(uint8_t func_type, void *p_cont[], uint8_t max_read_item);
void    auto_init_modules(uint32_t init_type);
/**
 * @brief 获取指定注册类型的参数个数
 *
 * @param [in] func_type 自动注册类型
 *
 * @return uint32_t 
 *  @retval 返回指定注册类型的个数
 * 
 * @details:
 * @par e.g:
 * @code
 *      user_val = auto_reg_get_num_by_func_type(AUTO_REG_TYPE_RESV);
 * @endcode
 */
uint32_t auto_reg_get_num_by_func_type(uint8_t func_type);	//获取当前类型的总数


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __AUTO_REG_MODULE_H__
