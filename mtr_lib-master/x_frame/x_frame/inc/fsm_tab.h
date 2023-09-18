/**
 * @brief fsm_tab.c.xtemp header file
 * @file fsm_tab.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 16:35:8
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 16:35:8
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __FSM_TAB_H__
#define __FSM_TAB_H__

/**
 *                                        +-+动作(Action)1: fsm_in_func		:从其他状态跳转到当前状态首先要做的动作,只运行一次,例如该状态中变量的初始化,赋值,参数的读取等
 *                                        |
 *                                        +-+动作2(Action): fsm_exe_func 	:该状态主要做的动作
 *                  +----+ 状态A(State) +-+
 *                  |                     +-+动作3(Action): fsm_out_func	:从该状态退出前需要做的动作,只运行一次,,例如某些特殊参数的清零等
 *                  |                     |
 * 				    |					  |
 *                  |                     +-+转换(Transition): fsm_cond_func:事件发生以后,会调用当前状态下的变换回调函数,然后去判断是否要发生转换,转换到什么状态
 *                  | 
 *                  |
 *                  |                     +-+动作1(Action): fsm_in_func		:
 *                  |                     |	
 *                  |                     +-+动作2(Action): fsm_exe_func	:
 *  状态机(fsm_tab) +----+ 状态B(State) +-+
 *                  |                     +-+动作3(Action): fsm_out_func	:
 *                  |                     |
 * 				    |					  |
 *                  |                     +-+转换 (Transition): fsm_cond_fun:
 *                  +-----+
 *  
 * 						事件(Event) : 用户定义发生某个改变时为一个事件,事件发生会调用当前状态下的转换回调函数
 * 
 *  	!! 状态机是状态的集合，事件是独立于状态的。
 */

/**
 * @brief 定义一个新的状态机结构体,并初始化状态机中包括的各个状态对应的回调函数,状态机对应的buff
 * 
 * @param [in] id 状态机id
 * @param [in] param_len 状态机对应的buff大小
 * @param [in] init_state_id 初始化后状态机对应的用户定义状态id
 * @param [in] ...  宏定义中使用_VA_ARGS_,表示可变参数列表,因此用户根据需求按结构体 fsm_item_obj_t 顺序填写
 * 
 * @details: 可以同时初始化一个状态机中的多个状态对应的回调函数
 * @par e.g:
 * @code
 * 		#define DEMO_ID   1
 * 
 *    	REG_FSM_TAB(DEMO_ID,0,1,  											//DEMO_ID是状态机的id
 *			{1,demo_init_in ,	demo_init_exe,	NULL,	demo_init_chk_func},		
 *			{2,demo_init_in2,	demo_init_exe2,	NULL,	demo_init_chk_func2},	
 *  		);
 * @endcode
 */
#define REG_FSM_TAB(id,param_len,init_state_id,...) \
		const fsm_item_obj_t  fsm_item_tab_##id[]={__VA_ARGS__};\
		static uint8_t		  fsm_param_buf_##id[param_len+1]={0};\
		fsm_obj_t             fsm_obj_##id={#id,id,init_state_id,fsm_param_buf_##id,param_len,0,sizeof(fsm_item_tab_##id)/sizeof(fsm_item_obj_t),fsm_item_tab_##id}

/**
 * @brief 状态机运行函数,包含以下功能:
 * 			1.运行状态机中当前状态的exe
 * 			2.若状态机中的状态已经切换,首先运行上一个状态的out,然后运行当前状态的in,最后运行当前状态的exe
 * 			3.若当前状态机第一次运行,首先将当前状态机的状态进行初始化,然后运行当前状态的in，最后运行当前状态的exe
 *
 * @param [in] p_fsm 状态机的结构体指针
 *
 * @details: 此宏一般运行在Task中
 * @par e.g:
 * @code
 * 		#define DEMO_ID   1
 *		void fsm_core_run(void)
 *		{
 * 			RUN_FSM(_FSM_OBJ(DEMO_ID));
 *         ...
 *		}
 *		REG_TASK(ID_TASK, 1, fsm_core_run);
 * @endcode
 */
#define RUN_FSM(id) 		  				fsm_fun(&fsm_obj_##id)

/**
 * @brief 事件发生,通过事件id,状态机对应状态进行转换
 *
 * @param [in] p_fsm 状态机的结构体指针
 * @param [in] event 事件id
 * @param [in] pbuf 数组指针
 * @param [in] len 数组长度
 *
 * @details:
 * @par e.g:
 * @code
 * 		#define DEMO_ID   1			 	//状态机的id
 * 		#define EVENT_USER   1    		//事件id
 *
 *    	EMIT_FSM(_FSM_OBJ(DEMO_ID),EVENT_USER,NULL,0); //这里数组为空，如有需要可以自行填写
 * @endcode
 */
#define EMIT_FSM(id,event,pbuf,len)		  	fsm_emit_event(&fsm_obj_##id,event,pbuf,len)

/**
 * @brief 获取状态机结构体指针
 *
 * @param [in] id 状态机id
 *
 * @details:
 * @par e.g:
 * @code
 *    	#define DEMO_ID   1     //状态机的id
 * 		fsm_obj_t  p_fsm;
 *
 * 		p_fsm = FSM_OBJ(DEMO_ID);
 * @endcode
 */
#define FSM_OBJ(id)							(&fsm_obj_##id)

/**
 * @brief 声明状态机的结构体
 *
 * @param [in] id 状态机的id
 *
 * @details:
 * @par e.g:
 * @code
 *		#define DEMO_ID   1
 *
 * 		EXT_FSM_OBJ(DEMO_ID);
 * @endcode
 */
#define EXT_FSM_OBJ(id)						extern fsm_obj_t fsm_obj_##id;

/**
 * @brief 状态机运行函数,包含以下功能:
 * 			1.运行状态机中当前状态的exe
 * 			2.若状态机中的状态已经切换,首先运行上一个状态的out,然后运行当前状态的in,最后运行当前状态的exe
 * 			3.若当前状态机第一次运行,首先将当前状态机的状态进行初始化,然后运行当前状态的in，最后运行当前状态的exe
 *
 * @param [in] p_fsm 状态机的结构体指针
 *
 * @details: 此宏一般运行在Task中
 * @par e.g:
 * @code
 * 		#define DEMO_ID   1
 *		void fsm_core_run(void)
 *		{
 * 			fsm_fun(_FSM_OBJ(DEMO_ID));
 *         ...
 *		}
 *		REG_TASK(ID_TASK, 1, fsm_core_run);
 * @endcode
 */
#define fsm_fun(p_fsm)								real_fsm_fun(p_fsm)

/**
 * @brief 事件发生,通过事件id,状态机对应状态进行转换
 *
 * @param [in] p_fsm 状态机的结构体指针
 * @param [in] event 事件id
 * @param [in] pbuf 数组指针
 * @param [in] len 数组长度
 *
 * @details:
 * @par e.g:
 * @code
 * 		#define DEMO_ID   1			 	//状态机的id
 * 		#define EVENT_USER   1    		//事件id
 *
 *    	fsm_emit_event(_FSM_OBJ(DEMO_ID),EVENT_USER,NULL,0); //这里数组为空，如有需要可以自行填写
 * @endcode
 */
#define fsm_emit_event(p_fsm,event,pbuf,len)		real_fsm_emit_event(p_fsm,event,pbuf,len)

#define UNCHANGE_STATE  0u

typedef void (*pf_fsm_in)  		(void);
typedef void (*pf_fsm_exe) 		(void);
typedef void (*pf_fsm_out) 		(void);
typedef uint32_t (*pf_fsm_cond)	(uint32_t event, uint8_t *pbuf, uint32_t len);

typedef struct
{
	uint8_t            	fsm_state_id; 	//状态机中的状态id
    pf_fsm_in      		fsm_in_func;	//事件进入此状态前调用的回调函数
    pf_fsm_exe      	fsm_exe_func;	//事件进入此状态后调用的回调函数
    pf_fsm_out     		fsm_out_func;	//事件退出此状态后调用的回调函数
    pf_fsm_cond    		fsm_cond_func; 	//事件由当前状态切换为另一个状态的触发条件。具体切换为哪个状态，在此函数里处理
}fsm_item_obj_t;

typedef struct
{
	const char*				fsm_name;			//状态机名称
	uint8_t					fsm_id;				//状态机id
	uint8_t					fsm_init_state;		//状态机初始化时对应的状态
	uint8_t*				p_fsm_param_buf;	//状态机对应的buff指针
	uint32_t				fsm_param_buf_len;	//状态机对应的buff长度
	
	uint8_t         		fsm_is_init;		//状态机是否初始化标志位
	uint8_t         		fsm_state_num;		//状态机中状态的总数
	const fsm_item_obj_t*  	p_fsm_item_tab;		//状态机中当前状态对应的结构体指针
	uint8_t					state_is_change;	//状态是否切换标志位
	uint8_t         		last_state;			//上一个状态
    uint8_t         		cur_state;			//当前状态
    uint8_t         		next_state;			//下一个状态
	uint32_t				action_event;		//执行的事件id
	
}fsm_obj_t;


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

void real_fsm_fun(fsm_obj_t* p_fsm);
void real_fsm_emit_event(fsm_obj_t* p_fsm, uint32_t event, uint8_t *pbuf, uint32_t len);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __FSM_TAB_H__
