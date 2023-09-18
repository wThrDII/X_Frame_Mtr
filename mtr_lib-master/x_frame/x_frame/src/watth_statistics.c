/********************** Copyright(c)***************************
** ECOFLOW 
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               watth_statistics.c
** Created by:              Chen JunTong
** Created date:            2022-04-6
** Version:                 V1.0.0
** Descriptions:            能量统计
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2022-04-6
** Last version:            V1.0.0
** Description:             能量统计
**
*************************************************************/
#include "x_frame.h"


#define WATTH_FLG_PUSH					(0x01<<0)

X_FRAME_WEAK float hd_get_watt(uint16_t watt_type)
{
	DBG_E_LIMIT(2000, "please achieve func <hd_get_watt>, type: float drv_get_watth(uint16_t watt_type)");
	return 0;
}

typedef struct _WATTH_STATISTICS_OBJ{
	uint32_t unix_time;
	X_CALENDAR_DATE unix_date;
	bool is_date_ready;
	list_t head;
}WATTH_STATISTICS_OBJ, *P_WATTH_STATISTICS_OBJ;

static WATTH_STATISTICS_OBJ ws_obj = {.head = LIST_HEAD_INIT(ws_obj.head)};
static list_t* p_last_exe_list = &ws_obj.head;
static list_t ws_push_list_head = LIST_HEAD_INIT(ws_push_list_head);

/*************************************************************
** Descriptions:        链表初始化
*************************************************************/
static void ws_item_list_init(void)
{
	watth_statistics_item_t* p_item = NULL;
    uint32_t i = 0;
//    INIT_LIST_HEAD(&ws_obj.head);
    while(1) {
        p_item = (watth_statistics_item_t*)auto_reg_get_func_cont_by_index(AUTO_REG_WATTH_STATISTICS, i);
        if(p_item == NULL) {
            break;
        }
		DBG_I_BLOCK(10, "watth static cal item: watth type: %d",p_item->watth_type);
        INIT_LIST_HEAD(&p_item->list);
        list_add_tail(&p_item->list, &ws_obj.head);
		i++;
	}
}

/*************************************************************
** Descriptions:        链表初始化
*************************************************************/
static void ws_push_list_init(void)
{
	watth_statistics_push_run_t* p_item = NULL;
    uint32_t i = 0;
//    INIT_LIST_HEAD(&ws_push_list_head);
    while(1) {
        p_item = (watth_statistics_push_run_t*)auto_reg_get_func_cont_by_index(AUTO_REG_WATTH_STATISTICS_PUSH, i);
        if(p_item == NULL) {
            break;
        }
		DBG_I_BLOCK(10, "watth static push item: link id: %d, dest: %d ",p_item->link_id, p_item->dest);
        INIT_LIST_HEAD(&p_item->list);
        list_add_tail(&p_item->list, &ws_push_list_head);
		i++;
	}
}

/*************************************************************
** Descriptions:        能量统计初始化
*************************************************************/
static void watth_statistics_init(void)
{
	ws_item_list_init();
	ws_push_list_init();
}

REG_INIT_MOD_BASIC(watth_statistics_init);

/*************************************************************
** Descriptions:        历史数据初始化
*************************************************************/
static bool update_date_info(void)
{
	ws_obj.unix_time = get_calendar_sec();
	get_calendar_date(&ws_obj.unix_date);
	if(ws_obj.unix_time < 1640995200u)			//1640995200对应2022-1-1 0:0:0
		return false;
	return true;
}

/*************************************************************
** Descriptions:        设置所有的推送标记
*************************************************************/
static void set_all_push_mark(uint8_t week)
{
	watth_statistics_push_run_t *p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &ws_push_list_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, watth_statistics_push_run_t, list);
		p_item->week_mark[week] = false;
	}
}

/*************************************************************
** Descriptions:        历史数据初始化
*************************************************************/
static void history_init(watth_statistics_item_t *p_item)
{
	watth_week_item_t week_item = {0};
	X_CALENDAR_DATE date;
	FLASH_PARAM_STA_ID sta = real_flash_param_read(p_item->param_name,(uint8_t*)&week_item,sizeof(week_item));
	p_item->day_watth = 0;
	p_item->hour_watth = 0;
    calendar_sec_to_date(&date,week_item.item_arry[ws_obj.unix_date.week].unix_time);
    if(sta == FLASH_PARAM_SUCCESS){
        p_item->all_watth = week_item.all_watth;
    }else{
        p_item->all_watth = 0;
    }
    if(sta != FLASH_PARAM_SUCCESS
        || ws_obj.unix_date.day != date.day
        || ws_obj.unix_date.month != date.month
        || ws_obj.unix_date.year != date.year){
        memset(&date,0,sizeof(date));
        date.year = ws_obj.unix_date.year;
        date.month = ws_obj.unix_date.month;
        date.day = ws_obj.unix_date.day;
        p_item->w_item.unix_time = calendar_date_to_sec(&date);
        memset(&p_item->w_item.watth,0,sizeof(p_item->w_item.watth));
    }else{
        memcpy(&p_item->w_item,&week_item.item_arry[ws_obj.unix_date.week],sizeof(watth_item_t));
        uint16_t hour = 0;
        for(; hour < 24;hour++){
            p_item->day_watth += p_item->w_item.watth[hour];
        }
        p_item->hour_watth = p_item->w_item.watth[ws_obj.unix_date.hour];
    }
	memcpy(&p_item->date,&ws_obj.unix_date,sizeof(X_CALENDAR_DATE));
	p_item->w_item.watth_type = p_item->watth_type;
	p_item->is_history_init = true;
	p_item->last_sec = ws_obj.unix_date.sec;
	p_item->last_hour = ws_obj.unix_date.hour;
}

/*************************************************************
** Function name:       load_watth_updata
** Descriptions:        负载能量统计更新
** Input parameters:    p_load_param：负载参数指针
**                      返回整数类型的负载参数指针
** Returned value:      None
** Remarks:             None
*************************************************************/
static void load_watth_updata(watth_statistics_item_t *p_item)
{
	if(p_item->date.year == ws_obj.unix_date.year
		&& p_item->date.month == ws_obj.unix_date.month
		&& p_item->date.day == ws_obj.unix_date.day){
		return;
	}
	watth_week_item_t week_item = {0};
	FLASH_PARAM_STA_ID sta = flash_param_read(p_item->param_name,(uint8_t*)&week_item,sizeof(week_item));
	if(sta != FLASH_PARAM_SUCCESS){
		memset(&week_item,0,sizeof(week_item));
	}
	memcpy(&week_item.item_arry[p_item->date.week],&p_item->w_item,sizeof(watth_item_t));
    week_item.all_watth = p_item->all_watth;
	flash_param_save(p_item->param_name,(uint8_t*)&week_item,sizeof(week_item));
	set_all_push_mark(p_item->date.week);
	uint16_t hour = 0;
	for(; hour < 24;hour++){
		p_item->w_item.watth[hour] = 0;
	}
	X_CALENDAR_DATE date = {0};
	date.year = ws_obj.unix_date.year;
	date.month = ws_obj.unix_date.month;
	date.day = ws_obj.unix_date.day;
	p_item->w_item.unix_time = calendar_date_to_sec(&date);
	p_item->w_item.watth_type = p_item->watth_type;
	p_item->day_watth = 0;
	p_item->hour_watth = 0;
	p_item->last_sec = ws_obj.unix_date.sec;
    memcpy(&p_item->date,&ws_obj.unix_date,sizeof(X_CALENDAR_DATE));
}

/*************************************************************
** Descriptions:        保存所有能量统计数据
*************************************************************/
void real_watth_statistics_save_all_watth(void)
{
    watth_statistics_item_t *p_watth_item;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &ws_obj.head;
    watth_week_item_t week_item = {0};
    list_for_each(cur_list, tag_list_head) {
        p_watth_item = list_entry(cur_list, watth_statistics_item_t, list);
        if(p_watth_item->is_history_init == false){
            continue;
        }
        memset(&week_item,0,sizeof(week_item));
        FLASH_PARAM_STA_ID sta = flash_param_read(p_watth_item->param_name,(uint8_t*)&week_item,sizeof(week_item));
        if(sta != FLASH_PARAM_SUCCESS && sta != FLASH_PARAM_NO_DATA){
            continue;
        }
        memcpy(&week_item.item_arry[p_watth_item->date.week],&p_watth_item->w_item,sizeof(watth_item_t));
        week_item.all_watth = p_watth_item->all_watth;
        flash_param_save(p_watth_item->param_name,(uint8_t*)&week_item,sizeof(week_item));
	}
}

/*************************************************************
** Descriptions:        获取累计能量统计数据
*************************************************************/
float real_get_watth_statistics_all_watth(uint16_t watt_type)
{
    watth_statistics_item_t *p_watth_item;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &ws_obj.head;
    list_for_each(cur_list, tag_list_head) {
        p_watth_item = list_entry(cur_list, watth_statistics_item_t, list);
        if(watt_type != p_watth_item->watth_type){
            continue;
        }
        if(p_watth_item->is_history_init == false){
            return 0;
        }
        return p_watth_item->all_watth;
	}
    return 0;
}

/*************************************************************
** Descriptions:        计算能量统计值
*************************************************************/
void cal_watth_statistics(watth_statistics_item_t *p_item)
{
	if(ws_obj.unix_date.sec == p_item->last_sec){
		return;
	}
	uint32_t elapse_sec = 0;
	if(p_item->last_sec > ws_obj.unix_date.sec){
		elapse_sec = ws_obj.unix_date.sec + 60 - p_item->last_sec;
	}else{
		elapse_sec = ws_obj.unix_date.sec - p_item->last_sec;
	}
	p_item->last_sec = ws_obj.unix_date.sec;
	float type_watt = hd_get_watt(p_item->watth_type);
	float cur_watth = type_watt * elapse_sec / 3600.0f;
	if(ws_obj.unix_date.hour != p_item->last_hour){
		p_item->hour_watth = 0;
		p_item->last_hour = ws_obj.unix_date.hour;
	}
    p_item->all_watth += cur_watth;
	p_item->hour_watth += cur_watth;
	p_item->day_watth += cur_watth;
	p_item->w_item.watth[ws_obj.unix_date.hour] = p_item->hour_watth;
}

/*************************************************************
** Descriptions:        能量统计任务
*************************************************************/
void watth_statistics_task(void)
{
	if(update_date_info() == false){
		return;
	}
    list_t      *cur_node = NULL;
    watth_statistics_item_t  *p_item;
    cur_node = p_last_exe_list->next;
    if(cur_node == &ws_obj.head){
		if(cur_node->next == &ws_obj.head){
			return;
		}
		cur_node = cur_node->next;
	}
    p_item = list_entry(cur_node,watth_statistics_item_t,list);
    p_last_exe_list = cur_node;
	if(p_item->is_history_init == false){
		history_init(p_item);
	}else{
		load_watth_updata(p_item);
		cal_watth_statistics(p_item);
	}
}

REG_TASK(VH_TASK,100,watth_statistics_task)

/*************************************************************
** Descriptions:        搜索log_item指针
*************************************************************/
static watth_statistics_push_run_t *search_ws_push_item(uint32_t link_id,uint8_t dest)
{
	watth_statistics_push_run_t *p_item = NULL;
    list_t *cur_list = NULL;
    list_t *tag_list_head = &ws_push_list_head;
    list_for_each(cur_list, tag_list_head) {
        p_item = list_entry(cur_list, watth_statistics_push_run_t, list);
		if(p_item->link_id == link_id && p_item->dest == dest){
			return p_item;
		}
	}
	return NULL;
}

/*************************************************************
** Descriptions:        获取需要推送的日期
*************************************************************/
static uint8_t get_push_day(watth_statistics_push_run_t *p_push_item,uint32_t link_id,uint8_t dest)
{
	uint16_t push_day = ws_obj.unix_date.week;
	uint16_t search_num = 0;
	while(search_num < 7){
		if(p_push_item->week_mark[push_day] == false){
			//protocol_set_item_push_interval(link_id,dest,x_watth_statistics_push_cmd,1000);
			p_push_item->next_push_decrase = 0;
			break;
		}
		push_day = (push_day + 1) % 7;
		search_num++;
	}
	if(search_num == 7){
		//protocol_set_item_push_interval(link_id,dest,x_watth_statistics_push_cmd,30 * 1000);
		p_push_item->next_push_decrase = 30;
	}
	return push_day;
}

/*************************************************************
** Descriptions:        能量推送指令
*************************************************************/
uint16_t x_watth_statistics_push_cmd(uint8_t is_first,uint32_t link_id,uint8_t dest)
{
	watth_statistics_push_run_t *p_push_item;
    p_push_item = search_ws_push_item(link_id,dest);
	if(p_push_item == NULL){
		return 0;
	}
	if(is_first == true){
		p_push_item->next_push_decrase = 0;
		p_push_item->cur_list = ws_obj.head.next;
		memset(p_push_item->week_mark,0,sizeof(p_push_item->week_mark));
	}
	if(p_push_item->flg & WATTH_FLG_PUSH)
		p_push_item->next_push_decrase = 0;
	if(p_push_item->next_push_decrase)
	{
		--(p_push_item->next_push_decrase);
	}
	if(0 != p_push_item->next_push_decrase)
		return 0;
	
	uint8_t push_day;
	push_day = get_push_day(p_push_item,link_id,dest);
	if(p_push_item->cur_list == &ws_obj.head){
		p_push_item->cur_list = p_push_item->cur_list->next;
		p_push_item->week_mark[push_day] = true;
		push_day = get_push_day(p_push_item,link_id,dest);
	}
	
	watth_statistics_item_t  *p_watth_item;
	watth_week_item_t week_item;
	p_watth_item = list_entry(p_push_item->cur_list,watth_statistics_item_t,list);
	watth_statistics_push_cmd_t push_cmd;
	push_cmd.seq = p_push_item->seq;
    
    hal_frame_info_t frame_info = {0};
	frame_info.cmd_func = p_push_item->cmd_func;
	frame_info.cmd_id = p_push_item->cmd_id;
	frame_info.dest = dest;
	frame_info.link_id = link_id;
    
	if(push_day != ws_obj.unix_date.week){
		FLASH_PARAM_STA_ID sta = real_flash_param_read(p_watth_item->param_name,(uint8_t*)&week_item,sizeof(week_item));
		if(sta != FLASH_PARAM_SUCCESS 
            || week_item.item_arry[push_day].unix_time == 0
            || week_item.item_arry[push_day].watth_type != p_watth_item->watth_type){
			p_push_item->week_mark[push_day] = true;
			p_push_item->cur_list = ws_obj.head.next;
			return 0;
		}
		memcpy(&push_cmd.item,&week_item.item_arry[push_day],sizeof(push_cmd.item));
        DBG_I("watth static history push, link id: %d, dest: %2x, type: %d, day: %d ", frame_info.link_id, frame_info.dest, push_cmd.item.watth_type, push_cmd.item.unix_time);
	}else{
        /* 当天未初始化的时候不推送数据 */
        if(p_watth_item->is_history_init == false){
            p_push_item->week_mark[push_day] = true;
			p_push_item->cur_list = ws_obj.head.next;
            DBG_W("watth static is not init, item type: %d", p_watth_item->watth_type);
            return 0;
        }
//        if(p_watth_item->watth_type != p_watth_item->w_item.watth_type){
//            DBG_E("watth static cur push,is_init %d, item type: %d, cal type: %d ",p_watth_item->is_history_init, p_watth_item->watth_type, p_watth_item->w_item.watth_type);
//        }
		memcpy(&push_cmd.item,&p_watth_item->w_item,sizeof(push_cmd.item));
        DBG_I("watth static cur push, link id: %d, dest: %2x, type: %d, day: %d ", frame_info.link_id, frame_info.dest, push_cmd.item.watth_type, push_cmd.item.unix_time);
	}
	p_push_item->flg |= WATTH_FLG_PUSH;
	frame_info.data_len = sizeof(push_cmd);
	frame_info.pdata = (uint8_t*)&push_cmd;
	
	return protocol_send_by_id(&frame_info);
}

/*************************************************************
** Descriptions:        能量推送指令应答
*************************************************************/
int32_t x_watth_statistics_push_ack(hal_frame_info_t *p_frame)
{
	watth_statistics_push_ack_t ack = {0};
	uint8_t de_num = X_FRAME_MIN(p_frame->data_len,sizeof(ack));
	watth_statistics_push_run_t *p_push_item;
    p_push_item = search_ws_push_item(p_frame->link_id,p_frame->src);
	if(p_push_item == NULL){
		DBG_E("watth static ack item is null, link id: %d , src: %d", p_frame->link_id, p_frame->src);
		return 0;
	}
	if(p_push_item->cur_list == NULL){
		return 0;
	}
    memcpy(&ack,p_frame->pdata,de_num);
	watth_statistics_item_t *p_watth_item = list_entry(p_push_item->cur_list,watth_statistics_item_t,list);
	if(ack.seq == p_push_item->seq && ack.watth_type == p_watth_item->watth_type){
		p_push_item->seq++;
		p_push_item->cur_list = p_push_item->cur_list->next;
		p_push_item->flg &= ~WATTH_FLG_PUSH;
		DBG_I("watth static ack success");
	}else{
		DBG_W("watth static push ack err,link_id %2x, src %2x, send seq: %d, rec seq: %d , send watth type: %d, rece  watth type: %d", p_frame->link_id, p_frame->src, p_push_item->seq, ack.seq, p_watth_item->watth_type, ack.watth_type);
	}
    return 0;                                                
}






