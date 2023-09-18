#include "x_frame.h"



typedef struct
{
	uint32_t 	rpc_num;
	list_t		rpc_item_head;
}rpc_core_t;

static rpc_core_t rec_core_obj={.rpc_item_head = LIST_HEAD_INIT(rec_core_obj.rpc_item_head)};

void real_rpc_call_sync(uint32_t rpc_id,uint32_t cmd,uint8_t* pbuf,uint32_t len)
{
	reg_rpc_item_t*  p_reg_rpc = NULL;
	list_t          *tag_list_head;
    list_t          *cur_list;
	tag_list_head = &rec_core_obj.rpc_item_head;
	list_for_each(cur_list, tag_list_head)
	{
		p_reg_rpc = list_entry(cur_list, reg_rpc_item_t, rpc_item_node);
		if(p_reg_rpc->rpc_item->rpc_id == rpc_id)
		{
			p_reg_rpc->rpc_item->rpc_action(cmd,pbuf,len);
			p_reg_rpc->is_padding_type = RPC_PADDING_NONE;
			return ;
		}
	}
}

void real_rpc_call_async(uint32_t rpc_id,uint32_t cmd,uint8_t* pbuf,uint32_t len)
{
	reg_rpc_item_t*  p_reg_rpc = NULL;
	list_t          *tag_list_head;
    list_t          *cur_list;
	tag_list_head = &rec_core_obj.rpc_item_head;
	list_for_each(cur_list, tag_list_head)
	{
		p_reg_rpc = list_entry(cur_list, reg_rpc_item_t, rpc_item_node);
		if(p_reg_rpc->rpc_item->rpc_id == rpc_id)
		{
			uint32_t real_len = 0;
			p_reg_rpc->is_padding_type = RPC_PADDING_ASYNC;
			p_reg_rpc->exec_cmd = cmd;
			if(pbuf!=NULL)
			{
				real_len = (len < p_reg_rpc->rpc_item->param_max_size )?(len):(p_reg_rpc->rpc_item->param_max_size);
				memcpy(p_reg_rpc->rpc_item->p_param_buf,pbuf,real_len);
				p_reg_rpc->param_len = real_len;
			}
			return ;
		}
	}
}

void real_rpc_call_delay(uint32_t ms,uint32_t rpc_id,uint32_t cmd,uint8_t* pbuf,uint32_t len)
{
	reg_rpc_item_t*  p_reg_rpc = NULL;
	list_t          *tag_list_head;
    list_t          *cur_list;
	tag_list_head = &rec_core_obj.rpc_item_head;
	list_for_each(cur_list, tag_list_head)
	{
		p_reg_rpc = list_entry(cur_list, reg_rpc_item_t, rpc_item_node);
		if(p_reg_rpc->rpc_item->rpc_id == rpc_id)
		{
			uint32_t real_len = 0;
			
			p_reg_rpc->is_padding_type = RPC_PADDING_DELAY;
			p_reg_rpc->exec_cmd = cmd;
			if(pbuf!=NULL)
			{
				
				real_len = (len < p_reg_rpc->rpc_item->param_max_size )?(len):(p_reg_rpc->rpc_item->param_max_size);
				//DBG_I("p_reg_rpc->rpc_item->param_max_size:%d real:%d",p_reg_rpc->rpc_item->param_max_size,real_len);
				memcpy(p_reg_rpc->rpc_item->p_param_buf,pbuf,real_len);
				p_reg_rpc->param_len = real_len;
			}
			p_reg_rpc->start_time = get_sys_ms();
			p_reg_rpc->time_out = ms;
			return ;
		}
	}
}


static void rpc_core_init(void)
{	
	uint32_t i = 0;
	reg_rpc_item_t*  p_reg_rpc = NULL;
	
	rec_core_obj.rpc_num = 0;
//	INIT_LIST_HEAD(&rec_core_obj.rpc_item_head);
	
	while(1){
        p_reg_rpc = (reg_rpc_item_t *)auto_reg_get_func_cont_by_index(AUTO_REG_RPC, i);
		i++;
        if(p_reg_rpc == NULL){
			break;
		}
		p_reg_rpc->start_time = 0;
		p_reg_rpc->is_padding_type = RPC_PADDING_NONE;
		list_add(&p_reg_rpc->rpc_item_node, &rec_core_obj.rpc_item_head);
		rec_core_obj.rpc_num++;
    }
}

REG_INIT_MOD_BASIC(rpc_core_init);

void rpc_core_run(void)
{
	reg_rpc_item_t*  p_reg_rpc = NULL;
	list_t          *tag_list_head;
    list_t          *cur_list;
	tag_list_head = &rec_core_obj.rpc_item_head;
	list_for_each(cur_list, tag_list_head)
	{
		p_reg_rpc = list_entry(cur_list, reg_rpc_item_t, rpc_item_node);
		switch(p_reg_rpc->is_padding_type)
		{
			case RPC_PADDING_ASYNC:
					p_reg_rpc->rpc_item->rpc_action(p_reg_rpc->exec_cmd,p_reg_rpc->rpc_item->p_param_buf,p_reg_rpc->param_len);
					p_reg_rpc->is_padding_type = RPC_PADDING_NONE;
				break;
			case RPC_PADDING_DELAY:
					if(check_time_out(p_reg_rpc->start_time,p_reg_rpc->time_out) == true)
					{
						p_reg_rpc->rpc_item->rpc_action(p_reg_rpc->exec_cmd,p_reg_rpc->rpc_item->p_param_buf,p_reg_rpc->param_len);
						p_reg_rpc->is_padding_type = RPC_PADDING_NONE;
					}
				break;
			
			default: break;
		}
	}
}

REG_TASK(ID_TASK, 1, rpc_core_run);



