/**
 * @brief fsm.c.xtemp header file
 * @file fsm.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 16:31:57
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 16:31:57
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __FSM_H__
#define __FSM_H__

#define FSM_NO_PARENT	    (0xffffffff)
#define FSM_SUSPEND_STATE	(0xffffffff)
/**
 * @brief 定义一个子级状态机结构体,并注册状态机链表,该状态机与父级状态机形成多级状态机
 * 
 * @param [in] fsm_id  运行状态机id，范围(0 ~ 0xFFFF)
 * @param [in] fsm_run_time 运行状态的间隔,单位 ms
 * @param [in] max_param_len 状态机对应参数最大的长度
 * @param [in] init_state 状态机初始化后处于的状态
 * @param [in] in_func 进入此状态机前运行的func
 * @param [in] out_func 退出此状态机后运行的func
 * @param [in] parent_fsm_id 父级状态机id
 * @param [in] parent_state_id 父级状态机状态id
 * 
 * @details: 
 * @par e.g:
 * @code
 *      #define FSM_main   1   //父级状态机id定义
 *      #define FSM_USER   2   //子级状态机id定义
 *      #define FSM_INTERVAL  10 //运行同一个状态机的状态间隔时间,单位 ms
 *      #define STATE_USER   1  //某个状态
 *      
 *      void  fsm_in_fun(void);
 *      void  fsm_out_fun(void);
 *
 *      REG_MULTI_FSM(FSM_USER,FSM_INTERVAL,0,STATE_USER,fsm_in_fun,fsm_out_fun,FSM_main,1);
 * 
 * @endcode
 */
#define REG_MULTI_FSM(fsm_id,fsm_run_time,max_param_len,init_state,in_func,out_func,parent_fsm_id,parent_state_id)  \
            static uint8_t       fsm_##fsm_id##_buf[1+max_param_len]={0} ;   	\
			static fsm_item_t    fsm_item_##fsm_id = {#fsm_id,fsm_id,fsm_run_time,init_state,max_param_len,fsm_##fsm_id##_buf,in_func,out_func,parent_fsm_id,parent_state_id};\
			AUTO_REG_ITEM(fsm_item_##fsm_id,AUTO_REG_FSM,(void*)&fsm_item_##fsm_id)

/**
 * 
 *              +----+ fsm_item_in_func    : 每次进入对应状态机前都会先运行fsm_item_in_func
 *              |
 *              |                             +-+动作(Action)1: fsm_in_func		:从其他状态跳转到当前状态首先要做的动作,只运行一次,例如该状态中变量的初始化,赋值,参数的读取等
 *              |                             |
 *              |                             +-+动作2(Action): fsm_exe_func 	:该状态主要做的动作
 *              |       +----+ 状态A(State) +-+
 *              |       |                     +-+动作3(Action): fsm_out_func	:从该状态退出前需要做的动作,只运行一次,,例如某些特殊参数的清零等
 *              |       |                     |
 * 				|       |					  |
 *              |       |                     +-+转换(Transition): fsm_cond_func:事件发生以后,会调用当前状态下的变换回调函数,然后去判断是否要发生转换,转换到什么状态
 *              |       | 
 *              |       |
 *              |       |                     +-+动作1(Action): fsm_in_func		:
 *              |       |                     |	
 *              |       |                     +-+动作2(Action): fsm_exe_func	:
 *  状态机(fsm) +-------+----+ 状态B(State) +-+
 *              |       |                     +-+动作3(Action): fsm_out_func	:
 *              |       |                     |
 * 				|       |					  |
 *              |       |                     +-+转换 (Transition): fsm_cond_fun:
 *              |       +-----+ ...
 *              |
 *              |
 *              +-----+ fsm_item_out_func : 每次退出对应状态机后都会再运行fsm_item_out_func
 *  
 * 						事件(Event) : 用户定义发生某个改变时为一个事件,事件发生会调用当前状态下的转换回调函数
 * 
 *  	!! 状态机是状态的集合，事件是独立于状态的。
 */

/**
 * @brief 定义一个新的状态机结构体,并注册一个状态机链表
 *
 * @param [in] fsm_id  运行状态机id，范围(0 ~ 0xFFFF)
 * @param [in] fsm_run_time 运行状态的间隔,单位 ms
 * @param [in] max_param_len 状态机对应参数最大的长度
 * @param [in] init_state 状态机初始化后处于的状态
 * @param [in] in_func 进入此状态机前运行的func
 * @param [in] out_func 退出此状态机后运行的func
 *
 * @details: 每次进入状态机前都会先运行in_func,同样,退出后也会运行out_func
 * @par e.g:
 * @code
 *      #define FSM_USER   1   //状态机id定义
 *      #define FSM_INTERVAL  10 //运行同一个状态机的状态间隔时间,单位 ms
 *      #define STATE_USER   1  //某个状态
 *      
 *      void  fsm_in_fun(void);
 *      void  fsm_out_fun(void);
 *
 *      REG_FSM(FSM_USER,FSM_INTERVAL,0,STATE_USER,fsm_in_fun,fsm_out_fun);
 * @endcode
 */
#define REG_FSM(fsm_id,fsm_run_time,max_param_len,init_state,in_func,out_func)  \
            static uint8_t       fsm_##fsm_id##_buf[1+max_param_len]={0} ;   	\
			static fsm_item_t    fsm_item_##fsm_id = {#fsm_id,fsm_id,fsm_run_time,init_state,max_param_len,fsm_##fsm_id##_buf,in_func,out_func,FSM_NO_PARENT,0};\
			AUTO_REG_ITEM(fsm_item_##fsm_id,AUTO_REG_FSM,(void*)&fsm_item_##fsm_id)

/**
 * @brief 获取状态机的当前状态
 *
 * @param [in] fsm_id 状态机的id，范围(0~0xFFFF)
 *
 * @details:
 * @par e.g:
 * @code
 *      #define FSM_USER   1   //状态机id定义
 *      uint8_t user_state;
 *
 *      user_state = GET_FSM_STATE(FSM_USER);  
 * @endcode
 */
#define GET_FSM_STATE(fsm_id)      fsm_item_##fsm_id.cur_state

/**
 * @brief 注册一个状态机中的新状态
 *
 * @param [in] fsm_id 此状态处于的状态机id，范围(0 ~ 0xFFFF)
 * @param [in] state_id 状态机中的状态id，范围(0 ~ 0xFFFF)
 * @param [in] in_func 此状态in对应的func
 * @param [in] exe_func 此状态exe对应的func
 * @param [in] out_func 此状态out对应的func
 * @param [in] cond_func 此状态cond对应的func
 *
 * @details:
 * @par e.g:
 * @code
 *      #define FSM_USER   1   //状态机id定义
 *      #define STATE_USER  1  //状态id定义
 *
 *      void user_in_func(fsm_item_t *p_fsm, uint8_t *pbuf, uint32_t len);   //函数声明
 *      void user_exe_func(fsm_item_t *p_fsm, uint8_t *pbuf, uint32_t len);
 *      void user_out_func(fsm_item_t *p_fsm, uint8_t *pbuf, uint32_t len);
 *      uint32_t user_cond_func(fsm_item_t *p_fsm, uint32_t event, uint8_t *pbuf, uint32_t len);
 *
 *      REG_FSM_STATE(FSM_USER,STATE_USER,user_in_func,user_exe_func,user_out_func,user_cond_func);
 *
 * @endcode
 */
#define REG_FSM_STATE(fsm_id,state_id,in_func,exe_func,out_func,cond_func)  \
            const fsm_state_t       fsm_state##fsm_id##state_id={fsm_id,state_id,in_func,exe_func,out_func,cond_func} ;   \
			reg_fsm_state_t         reg_fsm_state##fsm_id##state_id = {&fsm_state##fsm_id##state_id};\
			AUTO_REG_ITEM(reg_fsm_state##fsm_id##state_id,AUTO_REG_FSM_STATE,(void*)&reg_fsm_state##fsm_id##state_id)

/**
 * @brief 将事件发送到状态机的当前状态下的cond_func,进行状态的切换
 *
 * @param [in] fsm_id 需要发送的状态机id，范围(0 ~ 0xFFFF)
 * @param [in] event 需要发送的状态机事件
 * @param [in] pbuf buff指针
 * @param [in] len buff长度
 * 
 * @details:
 * @par e.g:
 * @code
 *      #define FSM_USER   1   //状态机id定义
 *      #define EVENT_USER   1   //用户定义事件定义
 *
 *      fsm_send_event(FSM_USER,EVENT_USER,NULL,0); //这里buff为空，如有需要可以自行填写
 * @endcode
 */
#define fsm_send_event(fsm_id,event,pbuf,len)			real_fsm_send_event(fsm_id,event,pbuf,len)

/**
 * @brief 将状态机重置，并设置状态机退出挂起状态后状态机处于的状态
 * 
 * @param [in] fsm_id 需要重置的状态机id，范围(0 ~ 0xFFFF)
 * @param [in] 对外的状态机重置接口 状态机重置后处于的状态
 * 
 * @details: 
 * @par e.g:
 * @code
 *      #define FSM_USER   1   //状态机id定义
 *      #define INIT_STATE   1   //用户定义事件定义
 *
 *      fsm_reset_state(FSM_USER,INIT_STATE); 
 * @endcode
 */
#define fsm_reset_state(fsm_id,new_state)				real_fsm_reset_state(fsm_id,new_state)

typedef struct  _fsm_core_t
{
    uint32_t       total_fsm_num;
    list_t         fsm_item_head;

} fsm_core_t;

#define FSM_MAX_PARAM_SIZE  32

typedef void (*pf_fsm_item_func)  (void);
typedef struct _fsm_item_t
{
    const char      *fsm_name;              //状态机名称
    uint32_t        fsm_id;                 //状态机id
    uint32_t        fsm_run_time;           //状态机运行状态的间隔,单位 ms
	uint32_t 		init_state;             //状态机初始化后处于的状态
    uint32_t        fsm_param_len_max;      //状态机对应的参数最大长度
    uint8_t         *p_fsm_param_buf;       //状态机对应的参数指针
	pf_fsm_item_func	fsm_item_in_func;   //进入此状态机首先调用的回调函数
	pf_fsm_item_func	fsm_item_out_func;  //退出此状态机最后调用的回调函数
	uint32_t		parent_fsm_id;			///< 父节点的fsm id
	uint32_t 		parent_state_id;		///< 父节点的state id
	
    uint32_t        last_exec_time;         //上一个exe运行的sys_time,单位 ms
    uint32_t        fsm_param_len;          //状态机参数的长度          
    uint32_t        last_state;             //上一个状态  
    uint32_t        cur_state;              //当前状态
    uint32_t        next_state;             //下一个状态
    list_t          fsm_item_node;          //状态机    
    list_t          fsm_state_head;         //状态机当前状态的链表头
    uint8_t         fsm_is_ready;           //状态机准备标志位
    uint8_t         fsm_state_num;          //状态机中状态的总数
    uint8_t         state_is_change;        //状态机切换标志位
} fsm_item_t;

typedef void (*pf_fsm_in_func)  (fsm_item_t *p_fsm, uint8_t *pbuf, uint32_t len);
typedef void (*pf_fsm_exe_func) (fsm_item_t *p_fsm, uint8_t *pbuf, uint32_t len);
typedef void (*pf_fsm_out_func) (fsm_item_t *p_fsm, uint8_t *pbuf, uint32_t len);
typedef uint32_t (*pf_fsm_cond_func)(fsm_item_t *p_fsm, uint32_t event, uint8_t *pbuf, uint32_t len);

typedef struct _fsm_state_t
{
    uint32_t            fsm_id;         //状态机id
    uint32_t            fsm_state_id;   //状态机中的状态id
    pf_fsm_in_func      fsm_in_func;    //事件进入此状态前调用的回调函数
    pf_fsm_exe_func     fsm_exe_func;   //事件进入此状态后调用的回调函数
    pf_fsm_out_func     fsm_out_func;   //事件退出此状态后调用的回调函数
    pf_fsm_cond_func    fsm_cond_func;  //事件由当前状态切换为另一个状态的触发条件。具体切换为哪个状态，在此函数里处理
} fsm_state_t;

typedef struct _reg_fsm_state_t
{
    const fsm_state_t  *p_fsm_state;
    list_t              fsm_state_node;
    list_t				fsm_head;
} reg_fsm_state_t;


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

void real_fsm_send_event( uint32_t  fsm_id, uint32_t event, uint8_t *pbuf, uint32_t len);
void real_fsm_reset_state(uint32_t fsm_id, uint32_t new_state);
fsm_item_t* fsm_get_obj( uint32_t  fsm_id);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __FSM_H__
