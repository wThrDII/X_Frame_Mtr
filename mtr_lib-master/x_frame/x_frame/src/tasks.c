#include "x_frame.h"


enum{
    TASK_STOP = 0,
    TASK_RUNNING = 1,
    TASK_DELETE = 2
};
    
static ALIGN(4) list_t task_list[MAX_TASKS_GROUP] = {   [0] = LIST_HEAD_INIT(task_list[0]),\
                                                        [1] = LIST_HEAD_INIT(task_list[1]),\
                                                        [2] = LIST_HEAD_INIT(task_list[2]),\
                                                        [3] = LIST_HEAD_INIT(task_list[3])};
static volatile task_item_t *p_cur_task_item = NULL;

static void task_init(void)
{
    uint32_t i = 0;
    task_item_t *p_task_item = NULL;
	uint32_t totle_task_num = 0;
	//DBG_I("task init in ok \r\n");
	
	totle_task_num = auto_reg_get_num_by_func_type(AUTO_REG_TYPE_TASK);
	totle_task_num = totle_task_num;		/* 防止编译警告 */
	if(totle_task_num>MAX_TASKS_NUM){
		DBG_E_BLOCK(50, "totle_task_num=%d, MAX_TASKS_NUM=%d", totle_task_num, MAX_TASKS_NUM);
	}else{
		DBG_I_BLOCK(50, "totle_task_num=%d, MAX_TASKS_NUM=%d", totle_task_num, MAX_TASKS_NUM);
	}
//    DBG_I("commit id:%s", X_FRAME_COMMIT_ID);
    
    for(i = 0; i < MAX_TASKS_NUM; i++)
    {
        p_task_item = auto_reg_get_func_cont_by_index(AUTO_REG_TYPE_TASK, i);
		if(p_task_item==NULL){return;}
        if( (p_task_item->config->group < MAX_TASKS_GROUP) )
        {
            list_add_tail(&p_task_item->list, &task_list[p_task_item->config->group]);
        }
    }
}
REG_INIT_MOD_BASIC(task_init);

void task_sch (uint8_t group)
{
    list_t *cur_node = NULL;
    list_t *pos = NULL;
    
    list_for_each_safe(cur_node, pos, &task_list[group]){
        p_cur_task_item = list_entry(cur_node, task_item_t, list);
        if(p_cur_task_item->flag_delete || p_cur_task_item->config->exe_func == NULL){
            list_del(cur_node);
            continue;
        }
        
        if(check_time_out(p_cur_task_item->last_exe_time,p_cur_task_item->config->perio) == true || (p_cur_task_item->flag_run_firs == 0)){
			p_cur_task_item->flag_run_firs = 1;
            p_cur_task_item->last_exe_time = get_sys_ms();
            p_cur_task_item->config->exe_func();

            p_cur_task_item->last_run_time = get_elapse_time(p_cur_task_item->last_exe_time);
            if(p_cur_task_item->min_run_time > p_cur_task_item->last_run_time){
                p_cur_task_item->min_run_time = p_cur_task_item->last_run_time;
            }
            
            if(p_cur_task_item->max_run_time < p_cur_task_item->last_run_time){
                p_cur_task_item->max_run_time = p_cur_task_item->last_run_time;
            }
            p_cur_task_item->tatol_run_times++;
        }
    }
    p_cur_task_item = NULL;
}

void current_task_delete(void)
{
    if(p_cur_task_item != NULL){
        p_cur_task_item->flag_delete = 1;
    }
}

void task_time_info_get(const char *name, uint32_t *min_time, uint32_t *max_time, uint32_t *last_time, uint32_t *tatol_times)
{
    uint32_t  i = 0;
    list_t    *cur_node = NULL;
    task_item_t *p_task_item;

    for(i = 0; i < MAX_TASKS_GROUP; i++){
        list_for_each(cur_node, &task_list[i]){
            p_task_item = list_entry(cur_node, task_item_t, list);
            if(!strcmp(name, p_task_item->config->func_name)){
                *min_time = p_task_item->min_run_time;
                *max_time = p_task_item->max_run_time;
                *tatol_times = p_task_item->tatol_run_times;
                *last_time = p_task_item->last_run_time;
                return;
            }
        }
    }
}


#if TINY_FUNC_CODE == 0
// int32_t list_tasks(uint8_t argc, uint8_t *argv[])
// {
//     uint32_t  i = 0;
//     uint32_t task_cnt = 0;
//     list_t    *cur_node = NULL;
//     task_item_t *p_task_item;

//     for(i = 0; i < MAX_TASKS_GROUP; i++){
//         list_for_each(cur_node, &task_list[i]){
//             task_cnt++;
//             p_task_item = list_entry(cur_node, task_item_t, list);
//             DBG_I("task[%d]-\t(T:%4d ms) group:%d : %s", i, p_task_item->config->perio, p_task_item->config->group, p_task_item->config->func_name);
//         }
//     }
//     DBG_I("cur system run %d tasks  (MAX:%d)", task_cnt, MAX_TASKS_NUM);

//     return RET_OK;
// }

// REG_SHELL_CMD_X(ls_tasks, 0, list_tasks, "list_tasks", "list_tasks.  eg ls_tasks \r\n ");

#endif



