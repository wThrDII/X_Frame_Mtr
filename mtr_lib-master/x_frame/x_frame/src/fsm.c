/**
 * @brief 多级状态机处理
 * @file fsm.c
 * @version 1.0
 * @author hzf
 * @date 2022-6-13 19:33:31
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-6-13 19:33:31
 *   Author      : hzf
 *   Modification: Created file
 */
 
#include "x_frame.h"
 
 
static fsm_core_t fsm_core = {.fsm_item_head = LIST_HEAD_INIT(fsm_core.fsm_item_head)};
 
/**
 * @brief 子类状态机挂载到正确的链表上
 *
 * @param [in] fsm_item_t *p_src_item  : 子类状态机指针
 * @param [in] list_t *p_parent_head   : 状态机链表头
 * @param [out] None
 * 
 * @return xf_ret
 *          XF_OK : 挂载成功
 *          其它 : 挂载失败
 * 
 * @history
 * 1.Date         : 2022-6-13 19:33:57
 *   Author       : hzf
 *   Modification : Created function
 */
static xf_ret __child_fsm_mount(fsm_item_t *p_src_item, list_t *p_parent_head)
{
    list_t  *p_state_head = NULL;
    fsm_item_t  *p_item_node = NULL;
    fsm_item_t  *p_child_node = NULL;
    reg_fsm_state_t *p_state_node = NULL;
    list_t  *p_parent_list = NULL;
    list_t  *p_list_temp = NULL;
    list_t  *p_child_item_head = NULL;
    list_t  *p_child_list = NULL;

    list_for_each(p_parent_list, p_parent_head)
    {
        p_item_node = list_entry(p_parent_list, fsm_item_t, fsm_item_node);
        if(p_item_node->fsm_id == p_src_item->parent_fsm_id)
        {
            p_state_head = &p_item_node->fsm_state_head;
            list_for_each(p_list_temp, p_state_head)
            {
                p_state_node = list_entry(p_list_temp, reg_fsm_state_t, fsm_state_node);
                if(p_state_node->p_fsm_state->fsm_state_id == p_src_item->parent_state_id)
                {
                    p_child_item_head = &p_state_node->fsm_head;
                    list_for_each(p_child_list, p_child_item_head){
                        p_child_node = list_entry(p_child_list, fsm_item_t, fsm_item_node);
                        ///> 按照从小到大的顺序排序
                        if(p_src_item->fsm_id < p_child_node->fsm_id){
                            break;
                        }
                    }

                    list_add_tail(&p_src_item->fsm_item_node, p_child_list);
                    return XF_OK;
                }
            }
            break;
        }
    }

    return XF_COM_ERROR;
}

/**
 * @brief 状态机链表信息初始化
 *
 * @param [in] void  
 * @param [out] None
 * 
 * @return 
 * 
 * @history
 * 1.Date         : 2022-6-13 19:51:58
 *   Author       : hzf
 *   Modification : Created function
 */
static void fsm_core_init(void)
{
    uint8_t            i = 0;
    fsm_item_t         *p_fsm_item;
    reg_fsm_state_t    *p_reg_fsm_state;
    list_t             *p_list_head = &fsm_core.fsm_item_head;
    list_t             *cur_node = NULL;
    list_t			    *p_temp_list = NULL;
    uint8_t            find_fsm_item = 0;
    fsm_item_t         *p_fsm_temp;

    //    INIT_LIST_HEAD(&fsm_core.fsm_item_head);
    /*1:加载FSM*/
    i = 0;
    while(1){
        p_fsm_item = (fsm_item_t *)auto_reg_get_func_cont_by_index(AUTO_REG_FSM, i);
        i++;
        if(p_fsm_item == NULL){
            break;
        }
        list_for_each(cur_node, p_list_head)
        {
            p_fsm_temp = list_entry(cur_node, fsm_item_t, fsm_item_node);
            if(p_fsm_item->fsm_id == p_fsm_temp->fsm_id)
            {
                DBG_E_BLOCK(50, "fsm id 0x%x is already used", p_fsm_item->fsm_id);
                while(1);
            }
        }
        INIT_LIST_HEAD(&p_fsm_item->fsm_state_head);
        list_add(&p_fsm_item->fsm_item_node, p_list_head);
        fsm_core.total_fsm_num++;
         
        //--------------goto init state
        p_fsm_item->state_is_change = 1;
        p_fsm_item->next_state = p_fsm_item->init_state;
    }

    /*2:加载FSM  state*/
    i = 0;
    while(1){
        p_reg_fsm_state = (reg_fsm_state_t *)auto_reg_get_func_cont_by_index(AUTO_REG_FSM_STATE, i);
        i++;
        find_fsm_item = 0;
        if(p_reg_fsm_state == NULL){
            break;
        }
        list_for_each(cur_node, p_list_head)
        {
            p_fsm_item = list_entry(cur_node, fsm_item_t, fsm_item_node);
            if(p_fsm_item->fsm_id == p_reg_fsm_state->p_fsm_state->fsm_id)
            {
                find_fsm_item = 1;
                break;
            }
        }

        if(find_fsm_item == 1)
        {
            INIT_LIST_HEAD(&p_reg_fsm_state->fsm_head);
            list_add(&p_reg_fsm_state->fsm_state_node, &p_fsm_item->fsm_state_head);
        }
    }

    xf_ret ret = -1;

    list_for_each_safe(cur_node, p_temp_list, p_list_head)
    {
        p_fsm_item = list_entry(cur_node, fsm_item_t, fsm_item_node);
        if(p_fsm_item->parent_fsm_id != FSM_NO_PARENT){
            list_del(cur_node);
            fsm_core.total_fsm_num--;
            ret = __child_fsm_mount(p_fsm_item, p_list_head);
            if(ret != XF_OK){
                DBG_E_LIMIT(10, "child fsm mount failed, id:%d", p_fsm_item->fsm_id);
            }
        }
    }

}

REG_INIT_MOD_BASIC(fsm_core_init);

/**
 * @brief 通过状态值查找状态机节点数据
 *
 * @param [in] fsm_item_t *p_fsm_item  ：状态机指针
 * @param [in] uint32_t state_num      ：状态值
 * @param [out] None
 * 
 * @return reg_fsm_state_t  ：节点指针
 * 
 * @history
 * 1.Date         : 2022-6-13 19:52:58
 *   Author       : hzf
 *   Modification : Created function
 */
static reg_fsm_state_t *__find_state_by_state_num(fsm_item_t *p_fsm_item, uint32_t state_num)
{
    reg_fsm_state_t *p_reg_fsm_state;
    static reg_fsm_state_t  suspend_state = {.fsm_head = LIST_HEAD_INIT(suspend_state.fsm_head)};
    list_t         *p_list_head = NULL;
    list_t          *cur_node = NULL;

    if(FSM_SUSPEND_STATE == state_num)
    {
        return &suspend_state;
    }

    p_list_head = &p_fsm_item->fsm_state_head;
    list_for_each(cur_node, p_list_head)
    {
        p_reg_fsm_state = list_entry(cur_node, reg_fsm_state_t, fsm_state_node);
        if(p_reg_fsm_state->p_fsm_state->fsm_state_id == state_num)
        {
            return p_reg_fsm_state;
        }

    }
    return NULL;
}

/**
 * @brief 状态机规则处理
 *
 * @param [in] list_t *p_fsm_list_head  ：状态机链表头指针
 * @param [in] uint32_t cur_time_ms     ：当前时间戳，单位：ms
 * @param [out] None
 * 
 * @return void
 * 
 * @history
 * 1.Date         : 2022-6-13 19:57:34
 *   Author       : hzf
 *   Modification : Created function
 */
static void __fsm_rule_handle(list_t *p_fsm_list_head, uint32_t cur_time_ms)
{
    fsm_item_t *p_head_item = NULL;
    reg_fsm_state_t *p_head_state = NULL;
    list_t  *p_head_item_node = NULL;

    list_for_each(p_head_item_node, p_fsm_list_head)
    {
        p_head_item = list_entry(p_head_item_node, fsm_item_t, fsm_item_node);
        if( (cur_time_ms - p_head_item->last_exec_time) >= p_head_item->fsm_run_time)
        {
            p_head_item->last_exec_time = cur_time_ms;

            //DBG_I_LIMIT(1000,"fsm:%s  state %d ", p_fsm_item->fsm_name, p_fsm_item->cur_state);
            if(p_head_item->fsm_item_in_func != NULL)   {p_head_item->fsm_item_in_func();}

            if(p_head_item->state_is_change)
            {
                p_head_item->state_is_change = 0;
                p_head_item->last_state = p_head_item->cur_state;
                p_head_item->cur_state  = p_head_item->next_state;

                if(p_head_item->fsm_is_ready==1)
                {
                    //执行退出函数
                    p_head_state = __find_state_by_state_num(p_head_item, p_head_item->last_state);
                    if(p_head_state == NULL)
                    {
                        DBG_E("fsm:%s  state %d is err", p_head_item->fsm_name, p_head_item->last_state);
                        break;
                    }

                    if(!list_empty(&p_head_state->fsm_head))
                    {
                        __fsm_rule_handle(&p_head_state->fsm_head, cur_time_ms);
                    }
             
                    if((p_head_state->p_fsm_state != NULL) && (p_head_state->p_fsm_state->fsm_out_func != NULL))
                    {
                        p_head_state->p_fsm_state->fsm_out_func(p_head_item, p_head_item->p_fsm_param_buf, p_head_item->fsm_param_len);
                    }
                }

                //执行进入新状态的函数
                p_head_state = __find_state_by_state_num(p_head_item, p_head_item->cur_state);
                if(p_head_state == NULL)
                {
                    DBG_E("fsm:%s  state %d is err", p_head_item->fsm_name, p_head_item->cur_state);
                    break;
                }
                if((p_head_state->p_fsm_state != NULL) && (p_head_state->p_fsm_state->fsm_in_func != NULL))
                {
                    p_head_state->p_fsm_state->fsm_in_func(p_head_item, p_head_item->p_fsm_param_buf, p_head_item->fsm_param_len);
                }
            }
            else
            {
                //执行稳定状态运行函数
                p_head_state = __find_state_by_state_num(p_head_item, p_head_item->cur_state);
                if(p_head_state == NULL)
                {
                    DBG_E("fsm:%s  state %d is err", p_head_item->fsm_name, p_head_item->cur_state);
                    break;
                }
                if((p_head_state->p_fsm_state != NULL) && (p_head_state->p_fsm_state->fsm_exe_func != NULL))
                {
                    p_head_state->p_fsm_state->fsm_exe_func(p_head_item, p_head_item->p_fsm_param_buf, p_head_item->fsm_param_len);
                }

                if(!list_empty(&p_head_state->fsm_head))
                {
                    __fsm_rule_handle(&p_head_state->fsm_head, cur_time_ms);
                }
            }
            p_head_item->fsm_is_ready = 1;// 执行第一次之后此标志位ok，避免不能调用out函数

            if(p_head_item->fsm_item_out_func != NULL)  {p_head_item->fsm_item_out_func();}
        }
    }
}

/**
 * @brief 状态机循环任务执行回调
 *
 * @param [in] void  
 * @param [out] None
 * 
 * @return 
 * 
 * @history
 * 1.Date         : 2022-6-13 19:58:22
 *   Author       : hzf
 *   Modification : Created function
 */
void __fsm_core_run(void)
{
    __fsm_rule_handle(&fsm_core.fsm_item_head, get_sys_ms());
}

REG_TASK(ID_TASK, 1, __fsm_core_run);

/**
 * @brief 挂起当前链表下的所有状态机
 *
 * @param [in] list_t *p_fsm_list_head  ：需要挂起的状态机链表头指针
 * @param [out] None
 * 
 * @return void
 * 
 * @history
 * 1.Date         : 2022-6-13 19:58:49
 *   Author       : hzf
 *   Modification : Created function
 */
void __fsm_suspend_all(list_t *p_fsm_list_head)
{
    list_t *p_node = NULL;
    fsm_item_t *p_item = NULL;
    reg_fsm_state_t *p_state = NULL;

    list_for_each(p_node, p_fsm_list_head)
    {
        p_item = list_entry(p_node, fsm_item_t, fsm_item_node);
        p_item->state_is_change = 1;
        p_item->next_state = FSM_SUSPEND_STATE;
        p_state = __find_state_by_state_num(p_item, p_item->cur_state);
        if(NULL != p_state)
        {
            if(!list_empty(&p_state->fsm_head))
            {
                __fsm_suspend_all(&p_state->fsm_head);
            }
        }
    }
}

/**
 * @brief 恢复被挂起的状态机链表
 *
 * @param [in] list_t *p_fsm_list_head  ：需要恢复的状态机链表头指针
 * @param [out] None
 * 
 * @return 
 * 
 * @history
 * 1.Date         : 2022-6-13 19:59:33
 *   Author       : hzf
 *   Modification : Created function
 */
void __fsm_resume_all(list_t *p_fsm_list_head)
{
    list_t *p_node = NULL;
    fsm_item_t *p_item = NULL;
    reg_fsm_state_t *p_state = NULL;

    list_for_each(p_node, p_fsm_list_head)
    {
        p_item = list_entry(p_node, fsm_item_t, fsm_item_node);
        if(!p_item->state_is_change){
            p_item->state_is_change = 1;
            p_item->next_state = (p_item->fsm_is_ready) ? (p_item->last_state) : (p_item->init_state);
        }
        p_state = __find_state_by_state_num(p_item, p_item->cur_state);
        if(NULL != p_state)
        {
            if(!list_empty(&p_state->fsm_head))
            {
                __fsm_resume_all(&p_state->fsm_head);
            }
        }
    }
}

/**
 * @brief 状态机状态切换
 *
 * @param [in] list_t *p_fsm_list_head  ：状态机链表头指针
 * @param [in] uint32_t fsm_id          ：状态机ID
 * @param [in] uint32_t event           ：状态机状态值
 * @param [in] uint8_t *pbuf            ：处理buf
 * @param [in] uint32_t len             ：buf长度
 * @param [out] None
 * 
 * @return 
 * 
 * @history
 * 1.Date         : 2022-6-13 20:0:9
 *   Author       : hzf
 *   Modification : Created function
 */
bool __fsm_event_send(list_t *p_fsm_list_head, uint32_t fsm_id, uint32_t event, uint8_t *pbuf, uint32_t len)
{
    fsm_item_t *p_fsm_item;
    reg_fsm_state_t *p_reg_cur_state;
    list_t          *cur_node = NULL;
    reg_fsm_state_t *p_reg_next_state = NULL;
    list_t *p_state_node = NULL;    
    list_t *p_state_head = NULL;

    list_for_each(cur_node, p_fsm_list_head)
    {
        p_fsm_item = list_entry(cur_node, fsm_item_t, fsm_item_node);
        if(p_fsm_item->fsm_id  == fsm_id)
        {
            p_reg_cur_state = __find_state_by_state_num(p_fsm_item, p_fsm_item->cur_state);
            if(p_reg_cur_state == NULL)
            {
                DBG_E("fsm:%s  state %d is err", p_fsm_item->fsm_name, p_fsm_item->cur_state);
                return false;
            }
            p_fsm_item->fsm_param_len = 0;
            if(p_reg_cur_state->p_fsm_state->fsm_cond_func != NULL)
            {
                if( p_reg_cur_state->p_fsm_state->fsm_cond_func(p_fsm_item, event, pbuf, len) )
                {
                    p_fsm_item->state_is_change = 1;
                    __fsm_suspend_all(&p_reg_cur_state->fsm_head);

                    p_reg_next_state = __find_state_by_state_num(p_fsm_item, p_fsm_item->next_state);
                    if(NULL != p_reg_next_state)
                    {
                        if(!list_empty(&p_reg_next_state->fsm_head))
                        {
                            __fsm_resume_all(&p_reg_next_state->fsm_head);
                        }
                    }
                }
             
            }

            return true;
        }

        p_state_head = &p_fsm_item->fsm_state_head;
        list_for_each(p_state_node, p_state_head)
        {
            p_reg_cur_state = list_entry(p_state_node, reg_fsm_state_t, fsm_state_node);
            if(true == __fsm_event_send(&p_reg_cur_state->fsm_head, fsm_id, event, pbuf, len))
            {
                return true;
            }
        }
    }

    return false;
}

/**
 * @brief 对外的状态机处理事件接口
 *
 * @param [in] uint32_t  fsm_id  ：状态机ID
 * @param [in] uint32_t event    ：事件编号
 * @param [in] uint8_t *pbuf     ：处理buf
 * @param [in] uint32_t len      ：buf长度
 * @param [out] None
 * 
 * @return 
 * 
 * @history
 * 1.Date         : 2022-6-13 20:1:5
 *   Author       : hzf
 *   Modification : Created function
 */
void real_fsm_send_event( uint32_t  fsm_id, uint32_t event, uint8_t *pbuf, uint32_t len)
{
    __fsm_event_send(&fsm_core.fsm_item_head, fsm_id, event, pbuf, len);
}

/**
 * @brief 状态机重置状态，从挂起后进入的新状态
 *
 * @param [in] list_t *p_fsm_list_head  ：状态机链表头指针
 * @param [in] uint32_t fsm_id          ：状态机指针
 * @param [in] uint32_t new_state       ：新状态编号
 * @param [out] None
 * 
 * @return 
 * 
 * @history
 * 1.Date         : 2022-6-13 20:1:49
 *   Author       : hzf
 *   Modification : Created function
 */
bool __fsm_new_state_set(list_t *p_fsm_list_head, uint32_t fsm_id, uint32_t new_state)
{
    fsm_item_t *p_fsm_item = NULL;
    list_t *p_node = NULL;
    reg_fsm_state_t *p_reg_state = NULL;
    list_t *p_state_node = NULL;    
    list_t *p_state_head = NULL;

    list_for_each(p_node, p_fsm_list_head)
    {
        p_fsm_item = list_entry(p_node, fsm_item_t, fsm_item_node);
        if(p_fsm_item->fsm_id  == fsm_id)
        {          
            p_fsm_item->next_state = new_state;
            p_fsm_item->state_is_change = 1;
            return true;
        }

        p_state_head = &p_fsm_item->fsm_state_head;
        list_for_each(p_state_node, p_state_head)
        {
            p_reg_state = list_entry(p_state_node, reg_fsm_state_t, fsm_state_node);
            if(true == __fsm_new_state_set(&p_reg_state->fsm_head, fsm_id, new_state))
            {
                return true;
            }
        }
    }

    return false;
}

/**
 * @brief 对外的状态机重置接口
 *
 * @param [in] uint32_t fsm_id     ：状态机ID
 * @param [in] uint32_t new_state  ：新状态
 * @param [out] None
 * 
 * @return 
 * 
 * @history
 * 1.Date         : 2022-6-13 20:3:0
 *   Author       : hzf
 *   Modification : Created function
 */
void real_fsm_reset_state(uint32_t fsm_id, uint32_t new_state)
{
    if(false == __fsm_new_state_set(&fsm_core.fsm_item_head, fsm_id, new_state))
    {
        DBG_E_LIMIT(10, "fsm :%d new state :%d set err!", fsm_id, new_state);
    }
}
 
/*************************************************************
** Descriptions:        根据状态机id获取状态机指针
** Input parameters:    fsm_id：状态机id
*************************************************************/
/**
 * @brief 获取以及状态机ID的指针
 *
 * @param [in] uint32_t  fsm_id  ：一级状态机ID
 * @param [out] None
 * 
 * @return 
 * 
 * @history
 * 1.Date         : 2022-6-13 20:3:27
 *   Author       : hzf
 *   Modification : Created function
 */
fsm_item_t* fsm_get_obj( uint32_t  fsm_id)
{
    fsm_item_t *p_fsm_item;
    list_t         *p_list_head = NULL;
    list_t          *cur_node = NULL;
    p_list_head = &fsm_core.fsm_item_head;
    list_for_each(cur_node, p_list_head)
    {
        p_fsm_item = list_entry(cur_node, fsm_item_t, fsm_item_node);
        if(p_fsm_item->fsm_id  == fsm_id)
        {          
            return p_fsm_item;
        }
    }
    return NULL;
}
 
 
 

