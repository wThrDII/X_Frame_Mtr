/**
 * @brief cfm.c.xtemp header file
 * @file cfm.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 16:37:13
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 16:37:13
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __CFM_H__
#define __CFM_H__

/**
 * @brief 注册阈值变量的进入条件和退出条件
 *
 * @param [in] name 阈值控制条件的变量名
 * @param [in] ...  宏定义中使用_VA_ARGS_,表示可变参数列表，因此用户根据需求按结构体顺序填写
 *
 * @details:
 * @par e.g:
 * @code
 * 		#define USER_VOL 0 
 * 		void in_vol_plug_in_func();  //函数声明,进入安全范围内，会调用的回调函数
 * 		void in_vol_plug_out_func(); //函数声明,退出安全范围内，会调用的回调函数
 * 
 *    	REG_CFM_TAB(USER_VOL,
 *	               {LARGER_THAN_IN_LEVEL, 20.0f, 100, in_vol_plug_in_func, 12.0f, 5000, in_vol_plug_out_func}) 
 *  
 * 		LARGER_THAN_IN_LEVEL表示当前参数大于in_level时，触发in_vol_plug_in_func回调函数，当前参数小于out_level时，触发in_vol_plug_out_func回调函数
 * 		LESS_THAN_IN_LEVEL表示当前参数小于in_level时，触发in_vol_plug_in_func回调函数，当前参数大于out_level时，触发in_vol_plug_out_func回调函数
 * 		
 * 		1.LARGER_THAN_IN_LEVEL，out_lecel < in_level
 *                         ———————————————+              +———————————————
 *                in_vol_plug_out_func    |              |    in_vol_plug_in_func
 *                          ——————————————+——————————————+—————————————————>
 * 					    			  out_level       in_level
 * 
 * 		2.LARGER_THAN_IN_LEVEL，out_lecel > in_level
 *                                     +——————————————————————+
 *                                     | in_vol_plug_out_func |    
 *                                     | in_vol_plug_in_func  |
 *                       ——————————————+——————————————————————+—————————————————>
 * 								  in_level               out_level 
 * 
 * 		3.LESS_THAN_IN_LEVEL，out_lecel < in_level
 *                                     +——————————————————————+
 *                                     | in_vol_plug_out_func |    
 *                                     | in_vol_plug_in_func  |
 *                       ——————————————+——————————————————————+—————————————————>
 * 								  out_level                in_level
 * 
 * 		4.LESS_THAN_IN_LEVEL，out_lecel > in_level
 *                         ———————————————+              +———————————————
 *                in_vol_plug_in_func     |              |    in_vol_plug_out_func
 *                          ——————————————+——————————————+—————————————————>
 * 					    			  in_level        out_level
 * 		
 * @endcode
 */
#define REG_CFM_TAB(name,...)   confirm_state_f_t   confirm_state_tab_##name[]={\
							__VA_ARGS__ }; uint32_t confirm_state_tab_cnt_##name = sizeof(confirm_state_tab_##name)/sizeof(confirm_state_f_t);		

/**
 * @brief 遍历阈值控制条件变量名内所有阈值控制条件
 *
 * @param [in] name 阈值控制条件的变量名
 * @param [in] var 当前阈值
 *
 * @details:
 * @par e.g:
 * @code
 *    	RUN_CFM(USER_VOL, 0.0f);
 * @endcode
 */
#define RUN_CFM(name,var) confirm_zone_chk(confirm_state_tab_##name,confirm_state_tab_cnt_##name,var)	

/**
 * @brief 返回阈值变量对应的处理指针
 *
 * @param [in] name 阈值控制条件的变量名
 *
 * @details:
 * @par e.g:
 * @code
 * 		#define USER_VOL 0
 * 
 *    	PTR_CFM_TAB(USER_VOL);
 * @endcode
 */
#define PTR_CFM_TAB(name)	 confirm_state_tab_##name

#define LARGER_THAN_IN_LEVEL 	0
#define LESS_THAN_IN_LEVEL 		1

typedef void (*pf_void_func)(void);
typedef struct
{
	uint8_t			judge_dir;   	//触发进入阈值的判断方向   0--lager than in_level  1-less than in_level
	float			in_level;	 	//触发进入条件的阈值
	uint32_t		confirm_in_ms;	//当触发进入条件后，需要保持满足触发条件的时间，才算确保进入安全阈值范围。单位为ms。
	pf_void_func	in_func;		//触发进入条件时会执行的回调函数
	
	float			out_level;		//触发退出条件的阈值
	uint32_t		confirm_out_ms;	//当触发退出条件后，需要保持满足触发条件的时间，才算确保退出安全阈值范围。单位为ms。
	pf_void_func	out_func;		//触发退出条件时会执行的回调函数。
	
	uint8_t 		state;   		//注册的参数在判断安全阈值事件中的状态  0-not init  1-normnal  2-abnormal
	uint32_t    	cur_ms;			//记录当前时间
	
}confirm_state_f_t;


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

void confirm_zone_chk(confirm_state_f_t* p_confirm,uint32_t tab_cnt,float var);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __CFM_H__
