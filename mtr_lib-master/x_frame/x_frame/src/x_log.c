/********************** Copyright(c)***************************
** ECOFLOW 
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               x_log.c
** Created by:              Chen JunTong
** Created date:            2022-04-6
** Version:                 V1.0.0
** Descriptions:            日志模块
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2022-04-6
** Last version:            V1.0.0
** Description:             日志模块
**
*************************************************************/
#include "x_frame.h"


X_FRAME_WEAK X_UID_TYPE x_log_mod;
X_FRAME_WEAK log_mod_obj_t	g_log_mod_obj_t;

static list_t log_list_head = LIST_HEAD_INIT(log_list_head);

#define X_LOG_VER		1

/*************************************************************
** Descriptions:        添加日志
*************************************************************/
void real_x_log_add(log_mod_item_t *p_item)
{
	if(g_log_mod_obj_t.max_log_num == 0){
		return;
	}
	p_item->unix_time = get_calendar_sec();
	p_item->ms = get_sys_ms();
	flash_param_save(x_log_mod,(uint8_t*)p_item,sizeof(log_mod_item_t));
	log_mod_push_run_t *p_push_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &log_list_head;
    list_for_each(cur_list, tag_list_head) {
        p_push_item = list_entry(cur_list, log_mod_push_run_t, list);
		if(p_push_item->push_index < g_log_mod_obj_t.max_log_num){
			p_push_item->push_index++;
		}
	}
}

/*************************************************************
** Descriptions:        读取日志
*************************************************************/
uint16_t real_x_log_read(log_mod_item_t *p_log_arry, uint32_t index, uint32_t num)
{
	uint32_t read_num = 0;
	for(;read_num < num;read_num++){
		if(flash_queue_param_read_history(x_log_mod,index + read_num,(uint8_t*)&p_log_arry[read_num],sizeof(log_mod_item_t)) != FLASH_PARAM_SUCCESS){
			break;
		}
	}
	return read_num;
}

/*************************************************************
** Descriptions:        链表初始化
*************************************************************/
static void log_mod_init(void)
{
	log_mod_push_run_t *p_item = NULL;
    uint32_t i = 0;
//    INIT_LIST_HEAD(&log_list_head);
    while(1) {
        p_item = (log_mod_push_run_t *)auto_reg_get_func_cont_by_index(AUTO_REG_LOG_PUSH, i);
        if(p_item == NULL) {
            break;
        }
        INIT_LIST_HEAD(&p_item->list);
        list_add_tail(&p_item->list, &log_list_head);
		i++;
	}
}
REG_INIT_MOD_BASIC(log_mod_init);

/*************************************************************
** Descriptions:        日志初始化
*************************************************************/
//void real_log_mod_init(void)
//{
//	list_init();
//}


/*************************************************************
** Descriptions:        搜索log_item指针
*************************************************************/
static log_mod_push_run_t *search_log_item(uint32_t link_id,uint8_t dest)
{
	log_mod_push_run_t *p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &log_list_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, log_mod_push_run_t, list);
		if(p_item->link_id == link_id && p_item->dest == dest){
			return p_item;
		}
	}
	return NULL;
}

/*************************************************************
** Descriptions:        填充日志缓存
*************************************************************/
static void fill_log_cmd_buf(log_mod_push_cmd_t *p_cmd,uint8_t *p_buf)
{
	uint16_t log_head_num = sizeof(log_mod_push_cmd_t) - sizeof(log_mod_item_t*);
	memcpy(p_buf,(uint8_t *)p_cmd + sizeof(log_mod_item_t*),log_head_num);
	memcpy(p_buf + log_head_num,p_cmd->log_mod_item,sizeof(log_mod_item_t) * p_cmd->num);
}

/*************************************************************
** Descriptions:        上传日志指令
*************************************************************/
uint16_t x_log_mod_push_cmd(uint8_t is_first,uint32_t link_id,uint8_t dest)
{
	log_mod_push_run_t *p_item;
    p_item = search_log_item(link_id,dest);
	if(p_item == NULL){
		return 0;
	}
	if(is_first == true){
		p_item->push_index = g_log_mod_obj_t.max_log_num;
	}
	if(p_item->push_index == 0){
        DBG_I("x_log no push,link id:%d, dest: %2x",link_id,dest);
		return 0;
	}
	uint32_t read_num = 0;
	log_mod_item_t log_item[5];
	while(read_num == 0 && p_item->push_index != 0){
		int read_cnt;
		read_cnt = X_FRAME_MIN(p_item->push_index, 5);
		read_num = real_x_log_read(log_item,p_item->push_index - read_cnt,read_cnt);
		if(read_num != 0){
			break;
		}
		if(p_item->push_index >= 5){
			p_item->push_index -= 5;
		}else{
			p_item->push_index = 0;
		}
	}
    
	if(read_num == 0){
		return 0;
	}
    DBG_I("x_log push link id:%d, dest: %2x, num: %d, seq: %d",link_id,dest,read_num, p_item->cur_seq);
	log_mod_push_cmd_t cmd;
	cmd.ver = X_LOG_VER;
	cmd.seq = p_item->cur_seq;
	cmd.num = read_num;
	cmd.log_mod_item = log_item;
	/* send data */
	uint8_t buf[sizeof(log_item) + sizeof(log_mod_push_cmd_t) - sizeof(log_mod_item_t *)] = {0};
	fill_log_cmd_buf(&cmd, buf);
	hal_frame_info_t frame_info = {0};
	frame_info.cmd_func = g_log_mod_obj_t.cmd_func;
	frame_info.cmd_id = g_log_mod_obj_t.cmd_id;
	frame_info.dest = dest;
	frame_info.link_id = link_id;
	frame_info.data_len = sizeof(buf);
	frame_info.pdata = buf;
	return protocol_send_by_id(&frame_info);
}

/*************************************************************
** Descriptions:        日志应答
*************************************************************/
int32_t x_log_mod_push_ack(hal_frame_info_t *p_frame)
{
	log_mod_push_ack_t ack = {0};
	uint8_t de_num = X_FRAME_MIN(p_frame->data_len,sizeof(ack));
	log_mod_push_run_t *p_item;
	p_item = search_log_item(p_frame->link_id,p_frame->src);
	if(p_item == NULL){
		return 0;
	}
    memcpy(&ack,p_frame->pdata,de_num);
	if(ack.seq == p_item->cur_seq){
        if(p_item->push_index >= ack.num){
            DBG_I("x_log push ack success!!! link id: %d, src: %2x, send seq: %d, ack seq: %d, push index: %d, ack num: %d",p_frame->link_id,p_frame->src,p_item->cur_seq,ack.seq,p_item->push_index,ack.num);
            p_item->push_index -= ack.num;
        }else{
            DBG_W("x_log push ack success!!! link id: %d, src: %2x, send seq: %d, ack seq: %d, push index: %d, ack num: %d",p_frame->link_id,p_frame->src,p_item->cur_seq,ack.seq,p_item->push_index,ack.num);
            p_item->push_index = 0;
        }
		
        p_item->cur_seq++;
	}else{
        DBG_W("x_log push ack error!!! link id: %d, src: %2x, send seq %d, ack seq ",p_frame->link_id,p_frame->src,p_item->cur_seq,ack.seq);
    }
    return 0;                                                
}




