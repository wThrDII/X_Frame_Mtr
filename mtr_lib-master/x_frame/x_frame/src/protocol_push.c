/********************** Copyright(c)***************************
** ECOFLOW 
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               protocol_push.c
** Created by:              Chen JunTong
** Created date:            2021-09-18
** Version:                 V1.0.0
** Descriptions:            协议推送
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2021-09-18
** Last version:            V1.0.0
** Description:             协议推送
**
*************************************************************/
#include "x_frame.h"


/*************************************************************
** Descriptions:        检索所有链表并执行相依动作
*************************************************************/
#define LIST_FOR_EACH_ACTION(head_list, param_type, param_point_name,action...)     \
    list_t *p_cur_list = NULL;                                                      \
    param_type *param_point_name;                                                   \
    list_for_each(p_cur_list, &head_list) {                                         \
        param_point_name = list_entry(p_cur_list, param_type, list);                \
        action                                                                      \
    }

/*************************************************************
** Function name:       AUTO_REG_LIST_HEAD_INIT
** Descriptions:        自动注册链表头初始化
** Input parameters:    reg_type：注册类型
**                      list_head：链表头
**                      list_type_point：链表类型指针
** Returned value:      None
** Remarks:             None
*************************************************************/
#define AUTO_REG_LIST_HEAD_INIT(reg_type,list_head,list_type_point)                 \
        {                                                                           \
            list_type_point p_param = NULL;                                         \
            INIT_LIST_HEAD(&list_head);                                             \
            uint16_t index = 0;                                                     \
            do {                                                                    \
                p_param = auto_reg_get_func_cont_by_index(reg_type, index);         \
                if(p_param == NULL) {                                               \
                    break;                                                          \
                }                                                                   \
                list_add(&p_param->list, &list_head);                               \
                index++;                                                            \
            } while(1);                                                            	\
        }
	
static list_t g_protocol_push_list_head;

/*************************************************************
** Function name:       push_item_head_init
** Descriptions:        项目链表头初始化
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void push_item_head_init(list_t *p_item_list_head,uint8_t link_id,uint8_t dest)                                 
{                                                                                                                                   
    INIT_LIST_HEAD(p_item_list_head);
    uint16_t index = 0;                                                                
    do {                                                                               
        P_PROTOCOL_PUSH_ITEM p_param = auto_reg_get_func_cont_by_index(AUTO_REG_PROTOCOL_PUSH_ITEM, index); 
        if(p_param == NULL) {                                                          
            break;                                                                      
        }
		index++;  
        if(link_id != p_param->link_id                                                
            || dest != p_param->dest){                                                              
            continue;                                                                  
        }                                                                              
        p_param->exec.is_first = true;                                                 
        p_param->exec.push_mark = true;                                                
        p_param->exec.push_interval = p_param->def_interval;                           
        list_t *p_last_list = p_item_list_head;                                         
        LIST_FOR_EACH_ACTION(*p_item_list_head,PROTOCOL_PUSH_ITEM,p_item,                 
            if(p_item->priority > p_param->priority){                                  
                break;                                                                 
            }                                                                          
            p_last_list = &p_item->list;                                               
        )                                                                              
        list_add(&p_param->list, p_last_list);                                                                                                   
    } while(1);                                                                        
}


/*************************************************************
** Function name:       push_item_head_init
** Descriptions:        项目链表头初始化
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void check_push_item_is_init(void)                                 
{                                                                                                                                   
    uint16_t index = 0;                                                                
    do {                                                                               
        P_PROTOCOL_PUSH_ITEM p_param = auto_reg_get_func_cont_by_index(AUTO_REG_PROTOCOL_PUSH_ITEM, index); 
        if(p_param == NULL) {                                                          
            break;                                                                      
        }
		index++;
        if(p_param->list.next == NULL){
			DBG_E_BLOCK(10, "protocol push group <link %d,dest %d> is not reg!!!",p_param->link_id,p_param->dest);
		}                                                                     
    } while(1);                                                                        
}

/*************************************************************
** Function name:       protocol_push_init
** Descriptions:        协议推送初始化
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void protocol_push_init(void)
{
    AUTO_REG_LIST_HEAD_INIT(AUTO_REG_PROTOCOL_PUSH_GROUP,g_protocol_push_list_head,P_PROTOCOL_PUSH_GROUP)
	if(g_protocol_push_list_head.next == &g_protocol_push_list_head){																										
		DBG_I_BLOCK(10, "no reg protocol push group !!!");						
	}																	
    LIST_FOR_EACH_ACTION(g_protocol_push_list_head,PROTOCOL_PUSH_GROUP,p_group,
        p_group->exec.push_mark = true;
        push_item_head_init(&p_group->item_list_head,p_group->link_id,p_group->dest);
    )
	check_push_item_is_init();
}

REG_INIT_MOD_BASIC(protocol_push_init);

/*************************************************************
** Function name:       protocol_push_task
** Descriptions:        协议推送任务
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
void protocol_push_task(void)
{
    LIST_FOR_EACH_ACTION(g_protocol_push_list_head,PROTOCOL_PUSH_GROUP,p_group,
        if(p_group->exec.run_time != get_sys_s()){
            p_group->exec.run_time = get_sys_s();
            p_group->exec.real_rate = p_group->exec.send_byte;
            p_group->exec.send_byte = 0 + p_group->exec.remain_byte;
            p_group->exec.use_ratio = p_group->exec.real_rate * 100 / p_group->rate;
        }
        p_group->exec.remain_byte = 0;
        if(p_group->exec.push_mark == false){
            continue;
        }
        if(check_time_out(p_group->exec.start_time, p_group->exec.next_time) == false){
            continue;
        }
        uint16_t data_len = 0;
        LIST_FOR_EACH_ACTION(p_group->item_list_head,PROTOCOL_PUSH_ITEM,p_item,
            if(p_item->exec.push_mark == false){
                continue;
            }
            if(check_time_out(p_item->exec.start_time,p_item->exec.push_interval) == false){
                continue;
            }
            p_item->exec.start_time = get_sys_ms();
            data_len = p_item->push_cb(p_item->exec.is_first, p_item->link_id,p_item->dest);
            p_item->exec.is_first = false;
            break;
        )
        p_group->exec.next_time = data_len * 1000 / p_group->rate;
        uint32_t remain = data_len * 1000 % p_group->rate;
        if(remain != 0){
            p_group->exec.next_time++;
        }
        p_group->exec.start_time = get_sys_ms();

        uint32_t time_remain = (p_group->exec.start_time + p_group->exec.next_time) % 1000;
        if(time_remain < p_group->exec.next_time){
            p_group->exec.remain_byte = time_remain * data_len / p_group->exec.next_time;
            data_len = data_len - p_group->exec.remain_byte;
        }
        p_group->exec.send_byte += data_len;
    )
}

REG_TASK(VH_TASK,1,protocol_push_task)

/*************************************************************
** Function name:       search_item
** Descriptions:        搜索项目
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static P_PROTOCOL_PUSH_GROUP search_group(uint8_t link_id,uint8_t dest)
{
    LIST_FOR_EACH_ACTION(g_protocol_push_list_head,PROTOCOL_PUSH_GROUP,p_group,
        if(p_group->link_id != link_id
            || p_group->dest != dest){
            continue;
        }
        return p_group;
    )
    return NULL;
}


/*************************************************************
** Function name:       search_item
** Descriptions:        搜索项目
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static P_PROTOCOL_PUSH_ITEM search_item(uint8_t link_id,uint8_t dest,void *p_push_ch)
{
    P_PROTOCOL_PUSH_GROUP p_group = search_group(link_id,dest);
    if(p_group == NULL){
        return NULL;
    }
    LIST_FOR_EACH_ACTION(p_group->item_list_head,PROTOCOL_PUSH_ITEM,p_item,
        if(p_item->push_cb != p_push_ch){
            continue;
        }
        return p_item;
    )
    return NULL;
}

/*************************************************************
** Function name:       protocol_push_init
** Descriptions:        协议推送初始化
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint32_t real_protocol_group_real_rate(uint8_t link_id,uint8_t dest)
{
    P_PROTOCOL_PUSH_GROUP p_group = search_group(link_id,dest);
    if(p_group == NULL){
        return 0;
    }
    return p_group->exec.real_rate;
}

/*************************************************************
** Function name:       protocol_group_use_ratio
** Descriptions:        协议组速率占用百分比
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint8_t real_protocol_group_use_ratio(uint8_t link_id,uint8_t dest)
{
    P_PROTOCOL_PUSH_GROUP p_group = search_group(link_id,dest);
    if(p_group == NULL){
        return 0;
    }
    return p_group->exec.use_ratio;
}


/*************************************************************
** Function name:       protocol_stop_group_push
** Descriptions:        停止组推送
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/                      
void real_protocol_stop_group_push(uint8_t link_id,uint8_t dest)
{
    P_PROTOCOL_PUSH_GROUP p_group = search_group(link_id,dest);
    if(p_group == NULL){
        return;
    }
    p_group->exec.push_mark = false;
    
}

/*************************************************************
** Function name:       protocol_start_group_push
** Descriptions:        开始组推送
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
void real_protocol_start_group_push(uint8_t link_id,uint8_t dest)
{
    P_PROTOCOL_PUSH_GROUP p_group = search_group(link_id,dest);
    if(p_group == NULL){
        return;
    }
    p_group->exec.push_mark = true;
}

/*************************************************************
** Function name:       protocol_reset_group_push
** Descriptions:        重置组推送
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
void real_protocol_reset_group_push(uint8_t link_id,uint8_t dest)
{
    P_PROTOCOL_PUSH_GROUP p_group = search_group(link_id,dest);
    if(p_group == NULL){
        return;
    }
    LIST_FOR_EACH_ACTION(p_group->item_list_head,PROTOCOL_PUSH_ITEM,p_item,
        p_item->exec.is_first = true;
        p_item->exec.push_mark = true;
        p_item->exec.push_interval = p_item->def_interval;
    )
    p_group->exec.push_mark = true;
}

/*************************************************************
** Function name:       protocol_stop_all_item_push
** Descriptions:        停止所有项目推送
** Input parameters:    None
** Returned value:      None
** Remarks:             不包括强制推送项目
*************************************************************/                      
void real_protocol_stop_all_item_push(uint8_t link_id,uint8_t dest)
{
    P_PROTOCOL_PUSH_GROUP p_group = search_group(link_id,dest);
    if(p_group == NULL){
        return;
    }
    LIST_FOR_EACH_ACTION(p_group->item_list_head,PROTOCOL_PUSH_ITEM,p_item,
        if(p_item->force_mark == true){
            continue;
        }
        p_item->exec.push_mark = false;
    )
}

/*************************************************************
** Function name:       protocol_start_all_item_push
** Descriptions:        开始所有项目推送
** Input parameters:    None
** Returned value:      None
** Remarks:             不包括强制推送项目
*************************************************************/
void real_protocol_start_all_item_push(uint8_t link_id,uint8_t dest)
{
    P_PROTOCOL_PUSH_GROUP p_group = search_group(link_id,dest);
    if(p_group == NULL){
        return;
    }
    LIST_FOR_EACH_ACTION(p_group->item_list_head,PROTOCOL_PUSH_ITEM,p_item,
        if(p_item->force_mark == true){
            continue;
        }
        if(p_item->exec.push_mark == false){
            p_item->exec.is_first = true;
        }
        p_item->exec.push_mark = true;
    )
}

/*************************************************************
** Function name:       protocol_start_all_item_push
** Descriptions:        开始所有项目推送
** Input parameters:    None
** Returned value:      None
** Remarks:             不包括强制推送项目
*************************************************************/
void real_protocol_reset_all_item_push(uint8_t link_id,uint8_t dest)
{
    P_PROTOCOL_PUSH_GROUP p_group = search_group(link_id,dest);
    if(p_group == NULL){
        return;
    }
    LIST_FOR_EACH_ACTION(p_group->item_list_head,PROTOCOL_PUSH_ITEM,p_item,
        if(p_item->force_mark == true){
            continue;
        }
        p_item->exec.is_first = true;
        p_item->exec.push_mark = true;
    )
}

/*************************************************************
** Function name:       protocol_stop_item_push
** Descriptions:        停止项目推送
** Input parameters:    None
** Returned value:      None
** Remarks:             不包括强制推送项目
*************************************************************/
void real_protocol_stop_item_push(uint8_t link_id,uint8_t dest,void *p_push_ch)
{
    P_PROTOCOL_PUSH_ITEM p_item = search_item(link_id,dest,p_push_ch);
    if(p_item == NULL){
        return;
    }
    if(p_item->force_mark == true){
        return;
    }
    p_item->exec.push_mark = false;
}

/*************************************************************
** Function name:       protocol_start_item_push
** Descriptions:        开始项目推送
** Input parameters:    None
** Returned value:      None
** Remarks:             不包括强制推送项目
*************************************************************/
void real_protocol_start_item_push(uint8_t link_id,uint8_t dest,void *p_push_ch)
{
    P_PROTOCOL_PUSH_ITEM p_item = search_item(link_id,dest,p_push_ch);
    if(p_item == NULL){
        return;
    }
    if(p_item->force_mark == true){
        return;
    }
    if(p_item->exec.push_mark == false){
        p_item->exec.is_first = true;
    }
    p_item->exec.push_mark = true;
}

/*************************************************************
** Function name:       protocol_reset_item_push
** Descriptions:        重置项目推送
** Input parameters:    None
** Returned value:      None
** Remarks:             不包括强制推送项目
*************************************************************/
void real_protocol_reset_item_push(uint8_t link_id,uint8_t dest,void *p_push_ch)
{
    P_PROTOCOL_PUSH_ITEM p_item = search_item(link_id,dest,p_push_ch);
    if(p_item == NULL){
        return;
    }
    if(p_item->force_mark == true){
        return;
    }
    p_item->exec.is_first = true;
    p_item->exec.push_mark = true;
    p_item->exec.push_interval = p_item->def_interval;
}

/*************************************************************
** Function name:       protocol_set_item_push_interval
** Descriptions:        设置推送间隔
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
void real_protocol_set_item_push_interval(uint8_t link_id,uint8_t dest,void *p_push_ch,uint32_t push_interval)
{
    P_PROTOCOL_PUSH_ITEM p_item = search_item(link_id,dest,p_push_ch);
    if(p_item == NULL){
        return;
    }
    p_item->exec.push_interval = push_interval;
}







