#include "x_frame.h"



X_FRAME_WEAK X_REC_LINK_TAB g_x_rec_link_tab;

int32_t x_rec_default_send(hal_frame_info_t *p_frame_info)
{
	uint16_t cir = 0;
	
    for(;cir < g_x_rec_link_tab.link_num;cir++){
		p_frame_info->dest = x_frame_pc_addr;
        p_frame_info->link_id = g_x_rec_link_tab.p_link_arry[cir];
        protocol_send_by_id(p_frame_info);
    }
    return 0;
}

int32_t x_rec_custom_send(hal_frame_info_t *p_frame_info)
{
	return protocol_send_by_id(p_frame_info);
}

uint8_t enable_rec_dat = 0;

REG_CFG_ITEM(enable_rec_dat,enable_rec_dat             ,eu_uint8      ,PARM_RW        ,0.0         ,0.0f       ,1.0f  );

static list_t rec_init_head = LIST_HEAD_INIT(rec_init_head);
const char* get_var_name(const char* var_name)
{
    char *p;
    p = strrchr(var_name, '>');
    if(p !=NULL)
    {
       return p+1;
    }

    p = strrchr(var_name, '.');
    if(p !=NULL)
    {
       return p+1;
    }

    return var_name;
}


void add_rec_node(list_t* p_add_node)
{
    list_add(p_add_node,&rec_init_head);
}

static void rec_init(void)
{
//    INIT_LIST_HEAD(&rec_init_head);
	enable_rec_dat = x_frame_rec_dat_enable;
}

REG_INIT_MOD_BASIC(rec_init);

static list_t* p_last_exe_list = &rec_init_head;
void rec_cfg_task(void)
{
	if(enable_rec_dat == 0){
		p_last_exe_list = &rec_init_head;
		return;
	}
    list_t      *cur_node = NULL;
    rec_node_t  *p_cfg_node;
    cur_node = p_last_exe_list->next;
    if(cur_node == &rec_init_head){
		if(cur_node->next == &rec_init_head){
			return;
		}
		cur_node = cur_node->next;
	}
    p_cfg_node = list_entry(cur_node,rec_node_t,rec_init_node);
	hal_frame_info_t frame = {0};
    p_cfg_node->rec_cfg_func(&frame,x_rec_default_send);
    p_last_exe_list = cur_node;
    
}

REG_TASK(ID_TASK,2000,rec_cfg_task);

REG_SILENCE_CMD(REC_CMDSET_REC, REC_CMDID_REC_CFG, 0)



