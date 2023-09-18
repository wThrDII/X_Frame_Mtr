/********************** Copyright(c)***************************
** ECOFLOW
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               protocol_hal.c
** Created by:
** Created date:
** Version:                 V1.0.0
** Descriptions:            协议框架文件
**
**------File Info----------------------------------------------
** Last modified by:         Chen JunTong
** Last modified date:       2021-03-16
** Last version:             V1.0.1
** Description:              协议框架文件
**
*************************************************************/
#include "x_frame.h"
#include "xh_stream.h"


#define PROTOCOL_RUN_PERIO 	1
#define CMD_SET_ITEM_MAX_NUM    30
#define CUR_PRODUCT_ID		((g_product_type<<8)|g_product_detail)
#define PROTOCOL_SILENCE_TIMEOUT_MS         (30 * 1000)

static link_obj_t       sg_link_obj = {.link_all_head = LIST_HEAD_INIT(sg_link_obj.link_all_head),\
                                       .link_ctrl[0] = {0, LIST_HEAD_INIT(sg_link_obj.link_ctrl[0].link_head)},\
                                       .link_ctrl[1] = {0, LIST_HEAD_INIT(sg_link_obj.link_ctrl[1].link_head)},\
                                       .link_ctrl[2] = {0, LIST_HEAD_INIT(sg_link_obj.link_ctrl[2].link_head)},\
                                       .link_ctrl[3] = {0, LIST_HEAD_INIT(sg_link_obj.link_ctrl[3].link_head)},\
                                       .silence_head = LIST_HEAD_INIT(sg_link_obj.silence_head),\
                                      };                  /* 链路指针头 */
static uint8_t          recv_buf[MAX_RECV_PACK_LEN] = {0};
static uint8_t          send_buf[MAX_SEND_PACK_LEN] = {0};
typedef struct {
    bool        is_use;
    uint8_t     cmd_set;
    cmdset_link_tab_t *p_link_tab;
    list_t      cmdset_list;
} cmdset_action_item_t;

static cmdset_action_item_t cmdset_action_item[CMD_SET_ITEM_MAX_NUM];

X_FRAME_WEAK uint8_t ef3_get_d_dest(uint32_t link_id)
{
	return 1;
}

/* 定义系统中最大链路数目 */
#define SYSTEM_LINK_MAX			32

typedef struct{
	uint8_t					links[SYSTEM_LINK_MAX];
	uint32_t				count;
}hal_link_mark;

static bool hal_route_info_valid_judge(link_route_t *p_old, link_route_t *p_new)
{
    if(p_old->route_type == p_new->route_type){
        switch (p_old->route_type)
        {
            case ROUTE_LINK_ADDR:
            case ROUTE_BROADCAST:
				if((p_old->dest == p_new->dest) && (p_old->src_link_id == p_new->src_link_id)) {
					return true;
				}
				break;
			case ROUTE_ADDR:
				if(p_old->dest == p_new->dest) {
					return true;
				}
				break;
			case ROUTE_CMD:
				if((p_old->src_link_id == p_new->src_link_id)
					&& (p_new->cmd_set == p_old->cmd_set) && (p_new->cmd_id == p_old->cmd_id)) {
					return true;
				}
				break;
			case ROUTE_LINK:
				if(p_old->src_link_id == p_new->src_link_id) {
					return true;
				}
				break;
			default:
				return false;
        }
        return false;
    }

    return false;
}

void hal_route_info_tag_link_change(link_route_t *p_new, uint8_t version)
{
    link_route_data_t *p_route = (VERSION_EF2 == version) ? (sg_link_obj.v2_route) : (sg_link_obj.v3_route);
    if(NULL == p_route){
        return;
    }

    uint8_t i;
    for(i = 0; i < p_route->count; i++){
        if(hal_route_info_valid_judge(&p_route->arr_tab[i], p_new) == true){
            p_route->arr_tab[i].tag_link_id = p_new->tag_link_id;
            break;
        }
    }
}

void turn_hal_route_rule(uint8_t version, bool enable)
{
    link_route_data_t *p_route = (VERSION_EF2 == version) ? (sg_link_obj.v2_route) : (sg_link_obj.v3_route);
    if(NULL == p_route){
        return;
    }

    p_route->enable = enable;
}

void turn_hal_route_item_rule(link_route_t *p_new, uint8_t version, bool enable)
{
    link_route_data_t *p_route = (VERSION_EF2 == version) ? (sg_link_obj.v2_route) : (sg_link_obj.v3_route);
    if(NULL == p_route){
        return;
    }

    uint8_t i;
    for(i = 0; i < p_route->count; i++){
        if(hal_route_info_valid_judge(&p_route->arr_tab[i], p_new) == true){
            p_route->arr_tab[i].opt = enable & EN_ROUTE;
            break;
        }
    }
}

static void hal_link_mark_init(hal_link_mark *link_mark)
{
	link_mark->count = 0;
}

static int hal_link_mark_mark(hal_link_mark *link_mark, uint8_t link_id)
{
	uint32_t i;
	for(i = 0; i < link_mark->count; ++i)
	{
		if(link_mark->links[i] == link_id)
			return 0;
	}
	
	if(link_mark->count < SYSTEM_LINK_MAX)
	{
		link_mark->links[link_mark->count] = link_id;
		link_mark->count++;
		return 0;
	}
	
	return -1;
}

static bool hal_link_mark_check(hal_link_mark *link_mark, uint8_t link_id)
{
	uint32_t i;
	for(i = 0; i < link_mark->count; ++i)
	{
		if(link_mark->links[i] == link_id)
			return true;
	}
	
	return false;
}

static int32_t route_check(hal_frame_info_t *p_frame, uint8_t *p_tag_link_id)
{
	uint16_t i = 0;
	hal_link_mark link_mark;
	
	if(p_frame->dest == x_frame_host_addr && (VERSION_EF2 == p_frame->version \
                                          || p_frame->d_dest == ef3_get_d_dest(p_frame->link_id) \
                                          || p_frame->d_dest == V3_BROAD_CAST_ADDR)){
		return IS_HOST_PACK;
    }

    link_route_data_t *p_route = (VERSION_EF2 == p_frame->version) ? (sg_link_obj.v2_route) : (sg_link_obj.v3_route);
    if(NULL == p_route || false == p_route->enable){
        if(VERSION_EF3 == p_frame->version && V3_BROAD_CAST_ADDR == p_frame->dest) {
            return IS_HOST_PACK;
        }
        return UN_KNOW_PACK;
    }
    
	hal_link_mark_init(&link_mark);
	for(i = 0; i < p_route->count; i++){
		int32_t pack_type = UN_KNOW_PACK;
		if(!(p_route->arr_tab[i].opt & EN_ROUTE)){
			continue;
		}
		switch(p_route->arr_tab[i].route_type) {
			case ROUTE_LINK_ADDR:
            case ROUTE_BROADCAST:
				if((p_frame->dest == p_route->arr_tab[i].dest) && (p_frame->link_id == p_route->arr_tab[i].src_link_id)) {
					pack_type = IS_ROUTE_PACK;
				}
				break;
			case ROUTE_ADDR:
				if(p_frame->dest == p_route->arr_tab[i].dest) {
					pack_type = IS_ROUTE_PACK;
				}
				break;
			case ROUTE_CMD:
				if((p_frame->link_id == p_route->arr_tab[i].src_link_id)
					&& (p_route->arr_tab[i].cmd_set == p_frame->cmd_func) && (p_route->arr_tab[i].cmd_id == p_frame->cmd_id)) {
					pack_type = IS_ROUTE_PACK;
				}
				break;
			case ROUTE_LINK:
				if(p_frame->link_id == p_route->arr_tab[i].src_link_id) {
					pack_type = IS_ROUTE_PACK;
				}
				break;
			default:
				break;
		}
		
		if(IS_ROUTE_PACK == pack_type)
		{
			uint8_t target_link = p_route->arr_tab[i].tag_link_id;
			/* 检查链路是否已经路由过，若没有，则路由 */
			if(!hal_link_mark_check(&link_mark, target_link))
			{
				hal_link_mark_mark(&link_mark, target_link);
				
				if(p_route->arr_tab[i].func_opt != NULL){
						p_route->arr_tab[i].func_opt(p_frame);
				}
				uint8_t origin_link = p_frame->link_id;
				p_frame->link_id = p_route->arr_tab[i].tag_link_id;
				route_protocol_send_by_id(p_frame);
				p_frame->link_id = origin_link;
			}
		}
	}
	
	/*
	 * V3协议支持多设备通信。同一类型设备有相同的静态地址，及不同的动态地址。因此需要同时匹配静态与动态地址
	 * 使用DEF_EF3_GET_D_DEST 宏定义动态地址获取函数
	 * 示例：
	 #define DEF_EF3_GET_D_DEST(
			uint8_t d_addr = 1;
			hal_app_ctrl(DRV_CAN0,GET_CAN_S_D_ADDR,&d_addr,NULL,NULL);
			return d_addr;
			)
	 */
	if(VERSION_EF3 == p_frame->version && V3_BROAD_CAST_ADDR == p_frame->dest) {
        return IS_HOST_PACK;
    }

    return UN_KNOW_PACK;
}

bool protocol_send_cmd_judge(hal_frame_info_t *p_frame_info)
{
    if(sg_link_obj.silence_enable == 0){
        return true;
    }
    
    silence_item_t  *p_silence;
	list_t   *tag_list_head;
	list_t   *cur_list;
	tag_list_head = &sg_link_obj.silence_head;    
    
    list_for_each(cur_list, tag_list_head)			//找链路
    {
        p_silence = list_entry(cur_list, silence_item_t, node);
        if(p_silence->cmd_func == p_frame_info->cmd_func && p_silence->cmd_id == p_frame_info->cmd_id){
            if((1 << p_silence->group_id) & sg_link_obj.silence_enable){
                return false;
            }
            break;
        }
    }
	return true;
}

void real_protocol_send_silence_set(uint8_t enable)
{
    sg_link_obj.silence_enable = enable;
    sg_link_obj.silence_cur_time = get_sys_ms();
}

void protocol_silence_timeout_task(void)
{
    if(sg_link_obj.silence_enable > 0){
        if(check_time_out(sg_link_obj.silence_cur_time, PROTOCOL_SILENCE_TIMEOUT_MS)){
            sg_link_obj.silence_enable = 0;
        }
    }
}

REG_TASK(VH_TASK, 1000, protocol_silence_timeout_task);

uint16_t ef_plus_set_send_ack(hal_frame_info_t *p_frame_info, uint8_t *pbuf, uint16_t len)
{
    p_frame_info->dest = p_frame_info->src;
	p_frame_info->d_dest = p_frame_info->d_src;
	p_frame_info->src  = x_frame_host_addr;
	p_frame_info->d_src = ef3_get_d_dest(p_frame_info->link_id);
    p_frame_info->is_ack = 1;
    p_frame_info->pdata = pbuf;
    p_frame_info->data_len = len;
	p_frame_info->product_id = CUR_PRODUCT_ID;
	return 0;
}

void ef_plus_set_send_info(hal_frame_info_t *p_frame)
{
	p_frame->src = x_frame_host_addr;
    p_frame->d_src = ef3_get_d_dest(p_frame->link_id);
    if(p_frame->enc_type != 0xff){
        p_frame->enc_type = ENC_TYPE;
    }
    p_frame->is_ack = 0;
	p_frame->product_id = CUR_PRODUCT_ID;
    p_frame->check_type = CHECK_TYPE;
}

static link_hal_item  *find_link(uint8_t link_id)
{
	link_hal_item  *p_link;
	list_t   *tag_list_head;
	list_t   *cur_list;
	tag_list_head = &sg_link_obj.link_all_head;
    list_for_each(cur_list, tag_list_head)			//找链路
    {
        p_link = list_entry(cur_list, link_hal_item, link_all_list);
        if(p_link->link_id == link_id){
            return p_link;
        }
    }
	return NULL;
}

/*************************************************************
** Function name:       protocol_send_by_id
** Descriptions:        协议发送函数，附带写入协议产品id
** Input parameters:    p_frame_info：协议参数指针
** Returned value:      成功：写入驱动的字节数；失败：< 0
** Remarks:             none
*************************************************************/
int32_t real_protocol_send_by_id(hal_frame_info_t *p_frame_info)
{
    static volatile uint8_t lock = 0;
    link_hal_item  *p_link = NULL;                  //链路信息
    int32_t         send_len = -1;
    uint16_t        pack_len = 0;
    int32_t         ret = -1;
    
	/* 防止异步调用 */
    if(lock){ return ERR_LOCK; }
    lock = 1;

    if(false == protocol_send_cmd_judge(p_frame_info)){
        goto end;
    }
    
	p_link = find_link(p_frame_info->link_id);
    if(p_link == NULL){
        goto end;
    }

    if(p_link->matrix && p_link->matrix->protocol){
        protocol_obj_t *p_protocol = (protocol_obj_t *)p_link->matrix->protocol;
        p_protocol->set_send_info(p_frame_info);
    }
    p_frame_info->seq  = p_link->send_seq++;
    pack_len = protocol_encode(p_link->matrix, p_frame_info, send_buf);
    send_len = xh_stream_write(p_link->write_dev, send_buf, pack_len);
    ret = (send_len == pack_len) ? (0) : (-1);

end:
    lock = 0;
    return ret;
}

/*************************************************************
** Function name:       protocol_send_ack
** Descriptions:        路由发送使用，不写入产品id
** Input parameters:    p_frame_info：协议参数指针
** Returned value:      成功：写入驱动的字节数；失败：< 0
** Remarks:             none
*************************************************************/
int32_t real_protocol_send_ack(hal_frame_info_t *p_frame_info, uint8_t *pbuf, uint16_t len)
{
	static volatile uint8_t lock = 0;
    link_hal_item  *p_link = NULL;                  //链路信息
    int32_t         send_len = -1;
	uint16_t        pack_len = 0;
	int32_t         ret = -1;
    
	/* 防止异步调用 */
    if(lock){ return ERR_LOCK; }
    lock = 1;

    if(false == protocol_send_cmd_judge(p_frame_info)){
        goto end;
    }
    
	p_link = find_link(p_frame_info->link_id);
    if(p_link == NULL){
        goto end;
    }

    if(p_link->matrix && p_link->matrix->protocol){
        protocol_obj_t *p_protocol = (protocol_obj_t *)p_link->matrix->protocol;
        p_protocol->send_ack(p_frame_info, pbuf, len);
    }
    
	pack_len = protocol_encode(p_link->matrix, p_frame_info, send_buf);
    send_len = xh_stream_write(p_link->write_dev, send_buf, pack_len);
    ret = (send_len == pack_len) ? (0) : (-1);

end:
    lock = 0;
    return ret;
}

/*************************************************************
** Function name:       real_protocol_send
** Descriptions:        协议发送
** Input parameters:    p_frame_info：协议参数指针
** Returned value:      成功：写入驱动的字节数；失败：< 0
** Remarks:             none
*************************************************************/
int32_t  real_protocol_send(hal_frame_info_t *p_frame_info)
{
    link_hal_item *p_link = NULL;
    uint8_t     pack_type = ERR_ADDR_PACK;
    uint8_t     tag_link_id = 0;
    int32_t     send_len = -1;
	uint16_t pack_len = 0;

    if(false == protocol_send_cmd_judge(p_frame_info)){
        return ERR_UNKNOW;
    }
    
	p_link = find_link(p_frame_info->link_id);
    if(p_link == NULL){
        return ERR_NO_LINK;
    }
    
    /*

      路由逻辑

            链路支持路由  |     ok    |    ok    |    no    |    no     |
    ----------------------+-----------+----------+----------+-----------+
        系统以及注册路由表|     ok    |    no    |    ok    |    no     |
    ----------------------+-----------+----------+----------+-----------+
           此包的发送结果 |    route  | dis_route| dis_route| dis_route |

    */

    if(p_link->en_route & EN_ROUTE)
    {
        //---需要进行路由的包
        pack_type = route_check(p_frame_info, &tag_link_id);
        if(pack_type == IS_ROUTE_PACK)
        {
            p_frame_info->link_id = tag_link_id;
        }
        else
        {
            //DBG_I("unchange pack!!");
        }
    }
    
    if(p_link->matrix && p_link->matrix->protocol){
        protocol_obj_t *p_protocol = (protocol_obj_t *)p_link->matrix->protocol;
        p_protocol->set_send_info(p_frame_info);
    }
    
	p_frame_info->seq  = p_link->send_seq++;
    pack_len = protocol_encode(p_link->matrix, p_frame_info, send_buf);
    send_len = xh_stream_write(p_link->write_dev, send_buf, pack_len);
	return send_len;
}

/*************************************************************
** Function name:       route_protocol_send_by_id
** Descriptions:        路由发送使用，不写入产品id
** Input parameters:    p_frame_info：协议参数指针
** Returned value:      成功：写入驱动的字节数；失败：< 0
** Remarks:             none
*************************************************************/
int32_t route_protocol_send_by_id(hal_frame_info_t *p_frame_info)
{
    static volatile uint8_t lock = 0;
    link_hal_item  *p_link = NULL;
    uint16_t        pack_len = 0;
    list_t          *cur_list;
    list_t          *tag_list_head;
    int32_t         send_len = -1;
    int32_t         ret = -1;
    
    if(lock) {
        return ERR_LOCK;
    }
    lock = 1;

    if(false == protocol_send_cmd_judge(p_frame_info)){
        send_len = ERR_UNKNOW;
        goto end;
    }
    
    tag_list_head = &sg_link_obj.link_all_head;
    list_for_each(cur_list, tag_list_head) {
        p_link = list_entry(cur_list, link_hal_item, link_all_list);
        if(p_link->link_id == p_frame_info->link_id) {
            break;
        }
    }
    if(cur_list == tag_list_head) {
        send_len = ERR_NO_LINK;
        goto end;
    }

    pack_len = protocol_encode(p_link->matrix, p_frame_info, send_buf);
    send_len = xh_stream_write(p_link->write_dev, send_buf, pack_len);
    ret = (send_len == pack_len) ? (0) : (-1);

end:
    lock = 0;
    return ret;
}

/*************************************************************
** Function name:       find_and_exec_action
** Descriptions:        查找并执行对应动作
** Input parameters:    p_frame_info：协议参数指针
** Returned value:      None
** Remarks:             None
*************************************************************/
static void find_and_exec_action(hal_frame_info_t *p_frame_info)
{
    link_hal_item *p_link = NULL;
    list_t *tag_list_head = NULL;
    uint8_t en_debug      = 0;
    list_t  *cur_list;
    action_tab_t   *p_action_item = NULL;
    uint8_t flag_is_exec = 0;
	uint16_t item_num = 0;
    uint16_t cmdset_link_index = 0;
	p_link = find_link(p_frame_info->link_id);
    uint8_t cmdset_link_flag = 0;
    if(p_link == NULL){
        return;
    }
	
    tag_list_head = &p_link->action_list_head;
    en_debug      =  p_link->en_debug;
    //DBG_I("recv [cmd_set:0x%02x ][cmd_id:0x%02x ]",p_frame_info->cmd_func,p_frame_info->cmd_id);
    if(p_link->flag_init) {
        list_for_each(cur_list, tag_list_head) {
            p_action_item = list_entry(cur_list, action_tab_t, action_list);
            if((p_action_item->cfg->cmd_func   == p_frame_info->cmd_func)  &&
                (p_action_item->cfg->cmd_id     == p_frame_info->cmd_id)) {
                if(p_action_item->cfg->cmd_type == LINK_ACTION_CMD && p_frame_info->is_ack == true){
                    continue;
                }
                if(p_action_item->cfg->cmd_type == LINK_ACTION_ACK && p_frame_info->is_ack == false){
                    continue;
                }
                if(p_action_item->cfg->action_func == NULL){
                    break;
                }
                p_action_item->cfg->action_func(p_frame_info);
                flag_is_exec = 1;
                //DBG_I("action [cmd_set:%4d ][cmd_id:%4d ]: %s",p_frame_info->cmd_func,p_frame_info->cmd_id,p_action_item->func_name);
                break;
            }
        }
        if(flag_is_exec == 0){
            /* 指令集动作 */
            for(item_num = 0;item_num < CMD_SET_ITEM_MAX_NUM;item_num++){
                cmdset_link_flag = 0;
                if(cmdset_action_item[item_num].cmd_set != p_frame_info->cmd_func){
                    continue;
                }
                cmdset_link_tab_t *p_link_tab = cmdset_action_item[item_num].p_link_tab;
                if(p_link_tab == NULL){
                    break;
                }
                for(cmdset_link_index = 0;cmdset_link_index < p_link_tab->link_num;cmdset_link_index++){
                    if(p_link_tab->p_link_tab[cmdset_link_index] != p_frame_info->link_id){
                        continue;
                    }
                    cmdset_link_flag = 1;
                    break;
                }
                
                if(cmdset_link_flag == 0){
                    break;
                }
                list_for_each(cur_list, &cmdset_action_item[item_num].cmdset_list) {
                    p_action_item = list_entry(cur_list, action_tab_t, action_list);
                    if(p_action_item->cfg->cmd_id != p_frame_info->cmd_id) {
                        continue;
                    }
                    if(p_action_item->cfg->cmd_type == LINK_ACTION_CMD && p_frame_info->is_ack == true){
                        continue;
                    }
                    if(p_action_item->cfg->cmd_type == LINK_ACTION_ACK && p_frame_info->is_ack == false){
                        continue;
                    }
                    if(p_action_item->cfg->action_func == NULL){
                        break;
                    }
                    p_action_item->cfg->action_func(p_frame_info);
                    flag_is_exec = 1;
                    //DBG_I("action [cmd_set:%4d ][cmd_id:%4d ]: %s",p_frame_info->cmd_func,p_frame_info->cmd_id,p_action_item->func_name);
                    break;
                }
                break;
            }
        }
    }

    if(en_debug) {
        if(flag_is_exec == 0) {
            DBG_W("unknow pack (%s) [cmd_set:0x%02x cmd_id:0x%02x]", p_link->name, p_frame_info->cmd_func, p_frame_info->cmd_id);
        } else {
            DBG_I("recv (%s) [cmd_set:0x%02x ][cmd_id:0x%02x ] : %s", p_link->name, p_frame_info->cmd_func, p_frame_info->cmd_id, p_action_item->cfg->func_name);
        }
    }
}

static void protocol_monitor_handle(link_hal_item *p_link, hal_frame_info_t *p_frame_info)
{
    list_t *tag_list_head = &p_link->monitor_list_head;
    list_t *cur_list;
    link_monitor_data_t *cur_monitor;
    list_for_each(cur_list, tag_list_head) {
        cur_monitor = list_entry(cur_list, link_monitor_data_t, node);
        if(((uint32_t)cur_monitor->item->src + cur_monitor->item->dest + cur_monitor->item->cmd_func + cur_monitor->item->cmd_id) != 0){
            if(cur_monitor->item->src == p_frame_info->src && cur_monitor->item->dest == p_frame_info->dest\
                && cur_monitor->item->cmd_func == p_frame_info->cmd_func && cur_monitor->item->cmd_id == p_frame_info->cmd_id){
                cur_monitor->item->func(p_frame_info);
            }
        }else{
            cur_monitor->item->func(p_frame_info);
        }
    }
}

/*************************************************************
** Function name:       run_unpack
** Descriptions:        运行解包函数
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void run_unpack(link_hal_item *p_link)
{
    uint32_t read_len = MAX_RECV_PACK_LEN;
    uint32_t real_len = 0;
    int32_t  ret = 0;
    static volatile uint8_t lock = 0;
    static uint32_t id = 0;
    hal_frame_info_t frame_info = {0};

    if(lock) { return; }
    lock = 1;

    real_len = xh_stream_read(p_link->read_dev, recv_buf, read_len);
    if((ret >= 0) && (real_len > 0)) {    
        uint8_t tag_link_id = 0;
        do{
            frame_info.link_id = p_link->link_id;
            ret = protocol_decode(p_link->matrix, recv_buf, real_len, id, &frame_info);
            if(ret >= 0){
                p_link->recv_seq = frame_info.seq;
                if(!list_empty(&p_link->monitor_list_head)){
                    protocol_monitor_handle(p_link, &frame_info);
                }
                if(p_link->en_route & EN_ROUTE) {
                    uint8_t pack_type;
                    pack_type = route_check(&frame_info, &tag_link_id);
                    if((p_link->en_route & EN_HACK) || (pack_type == IS_HOST_PACK)) {
                        find_and_exec_action(&frame_info);
                    }else if(pack_type == IS_ROUTE_PACK) {
                        frame_info.link_id = tag_link_id;
                        route_protocol_send_by_id(&frame_info);
                    }
                } else {
                    find_and_exec_action(&frame_info);
                }
            }
        }while(ret >= 0);
        id++;
    }
    lock = 0;
}

/*************************************************************
** Function name:       protocol_ontick
** Descriptions:        数据包解析
** Input parameters:    prio：运行的优先级
** Returned value:      None
** Remarks:             None
*************************************************************/
int32_t  protocol_ontick(uint32_t prio)
{
    list_t      *p_link_list_head = NULL;
    link_hal_item  *p_cur_link;
    list_t    *cur_node = NULL;
    
    if(prio >= MAX_TASKS_GROUP) {
        return -1;
    }
    p_link_list_head = &sg_link_obj.link_ctrl[prio].link_head;
    list_for_each(cur_node, p_link_list_head) {
        p_cur_link = list_entry(cur_node, link_hal_item, link_list);
        run_unpack(p_cur_link);
    }
    return 0;
}

/*************************************************************
** Function name:       protocol_init
** Descriptions:        协议初始化
** Input parameters:    None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void  protocol_init(void)
{
    uint32_t i = 0, j;
    link_hal_item  *p_cur_link;
    list_t          *tag_list_head;
    list_t          *cur_list;
    uint8_t          link_found = 0;
    uint16_t        item_num = 0;
    silence_item_t  *p_silence;
    link_monitor_data_t *p_monitor;
    link_route_data_t   *p_route;
    
	i = 0;
    while(1) {
        p_cur_link = (link_hal_item *)auto_reg_get_func_cont_by_index(AUTO_REG_TYPE_LINK, i);
		i++;
        if(p_cur_link == NULL) {
            break;
        }
        if(p_cur_link->prio < MAX_PROTCOL_PRIO) {
            list_add_tail(&p_cur_link->link_all_list, &sg_link_obj.link_all_head);
            //list_add_tail(&p_cur_link->link_list, &sg_link_obj.link_ctrl[p_cur_link->link_id].link_head);
            list_add_tail(&p_cur_link->link_list, &sg_link_obj.link_ctrl[p_cur_link->prio].link_head);
            sg_link_obj.real_reg_link_num++;
            INIT_LIST_HEAD(&p_cur_link->action_list_head);
            INIT_LIST_HEAD(&p_cur_link->monitor_list_head);
        }
    }
	i = 0;
    while(1) { //
        action_tab_t *p_action_item;
        action_tab_t *p_item;
        p_item = (action_tab_t *)auto_reg_get_func_cont_by_index(AUTU_REG_TYPE_LINK_ACTION, i);
		i++;
        if(p_item == NULL) {
            break;
        }

        for(j = 0; j < p_item->cfg->link_cnt; j++){
            p_action_item = &p_item[j];
            if(p_action_item->cfg->group_type == ACTION_GROUP_LINK){
                tag_list_head = &sg_link_obj.link_all_head;
                link_found = 0;
                list_for_each(cur_list, tag_list_head) {
                    p_cur_link = list_entry(cur_list, link_hal_item, link_all_list);
                    if(p_cur_link->link_id == p_action_item->cfg->p_link_id[j]) {
                        link_found = 1;
                        break;
                    }
                }
                if(link_found == 1) {
                    if(p_action_item->cfg->cmd_type == LINK_ACTION_CMD_ACK){
                        ///> LINK_ACTION_CMD_ACK类型的回调处理优先级最低，每次都插入到链表的末尾
                        list_add_tail(&p_action_item->action_list, &p_cur_link->action_list_head);
                    }else{
                        list_add(&p_action_item->action_list, &p_cur_link->action_list_head);
                    }
                } else {
                    DBG_W("ADD action item- cmd_set:%d cmd_id:%d link_id:%d is err!!!", p_action_item->cfg->cmd_func, p_action_item->cfg->cmd_id, p_action_item->cfg->p_link_id[j]);
                }
            }else if(p_action_item->cfg->group_type == ACTION_GROUP_CMDSET){
                for(item_num = 0;item_num < CMD_SET_ITEM_MAX_NUM;item_num++){
                    if(cmdset_action_item[item_num].is_use == false){
                        cmdset_action_item[item_num].is_use = true;
                        cmdset_action_item[item_num].cmd_set = p_action_item->cfg->cmd_func;
                        INIT_LIST_HEAD(&cmdset_action_item[item_num].cmdset_list);
                        INIT_LIST_HEAD(&p_action_item->action_list);
                        list_add_tail(&p_action_item->action_list, &cmdset_action_item[item_num].cmdset_list);
                        break;
                    }
                    if(cmdset_action_item[item_num].cmd_set == p_action_item->cfg->cmd_func){
                        INIT_LIST_HEAD(&p_action_item->action_list);
                        list_add_tail(&p_action_item->action_list, &cmdset_action_item[item_num].cmdset_list);
                        break;
                    }
                    DBG_W("ADD action cmd_set item full!!! cmd_set:%d cmd_id:%d link_id:%d is err!!!", p_action_item->cfg->cmd_func, p_action_item->cfg->cmd_id, p_action_item->cfg->p_link_id[j]);
                }
            }else{
                DBG_W("ADD action type err!!! cmd_set:%d cmd_id:%d link_id:%d is err!!!", p_action_item->cfg->cmd_func, p_action_item->cfg->cmd_id, p_action_item->cfg->p_link_id[j]);
            }
        }
    }
    i = 0;
    while(1) { //
        cmdset_link_tab_t *p_link_tab = NULL;
        p_link_tab = (cmdset_link_tab_t *)auto_reg_get_func_cont_by_index(AUTU_REG_TYPE_CMDSET_LINK_TAB, i);
		i++;
        if(p_link_tab == NULL) {
            break;
        }
        for(item_num = 0;item_num < CMD_SET_ITEM_MAX_NUM;item_num++){
            if(cmdset_action_item[item_num].is_use != true){
                continue;
            }
            if(cmdset_action_item[item_num].cmd_set != p_link_tab->cmdset){
                continue;
            }
            if(cmdset_action_item[item_num].p_link_tab == NULL){
                cmdset_action_item[item_num].p_link_tab = p_link_tab;
            }else{
                DBG_W("cmdset link tab is no match!!! cmd_set:%d !!!", p_link_tab->cmdset);
            }
            break;
        }
    }

    i = 0;
    while(1){
        p_silence = (silence_item_t *)auto_reg_get_func_cont_by_index(AUTO_REG_SILENCE, i);
		i++;
        if(p_silence == NULL) {
            break;
        }
        list_add_tail(&p_silence->node, &sg_link_obj.silence_head);
    }

    i = 0;
    while(1){
        p_monitor = (link_monitor_data_t *)auto_reg_get_func_cont_by_index(AUTO_REG_LINK_MONITOR, i);
		i++;
        if(p_monitor == NULL) {
            break;
        }
        p_cur_link = find_link(p_monitor->item->link_id);
        if(p_cur_link != NULL){
            list_add_tail(&p_monitor->node, &p_cur_link->monitor_list_head);
        }
    }

    i = 0;
    while(1){
        p_route = (link_route_data_t *)auto_reg_get_func_cont_by_index(AUTO_REG_LINK_ROUTE_TAB, i);
		i++;
        if(p_route == NULL) {
            break;
        }
        if(p_route->version == VERSION_EF2){
            sg_link_obj.v2_route = p_route;
        }else if(p_route->version == VERSION_EF3){
            sg_link_obj.v3_route = p_route;
        }
    }
    
    tag_list_head = &sg_link_obj.link_all_head;
    list_for_each(cur_list, tag_list_head) {
        p_cur_link = list_entry(cur_list, link_hal_item, link_all_list);
        p_cur_link->send_seq = 0;
        p_cur_link->recv_seq = 0;
        p_cur_link->flag_init  = 1;
        DBG_I("init link  [%s]", p_cur_link->name);
    }
}

REG_INIT_MOD_RESOURCE(protocol_init);

void run_protocol_vh(void)
{
    protocol_ontick(VH_TASK);
}
void run_protocol_hi(void)
{
    protocol_ontick(HI_TASK);
}
void run_protocol_lw(void)
{
    protocol_ontick(LW_TASK);
}
void run_protocol_id(void)
{
    protocol_ontick(ID_TASK);
}

REG_TASK(VH_TASK, PROTOCOL_RUN_PERIO, run_protocol_vh);
REG_TASK(HI_TASK, PROTOCOL_RUN_PERIO, run_protocol_hi);
REG_TASK(LW_TASK, PROTOCOL_RUN_PERIO, run_protocol_lw);
REG_TASK(ID_TASK, PROTOCOL_RUN_PERIO, run_protocol_id);

/*************************************************************
** Function name:       judge_link_write_dev
** Descriptions:        判断设备是否被注册成协议链路
** Input parameters:    debug_dev：需要判断的设备
** Returned value:      None
** Remarks:             None
*************************************************************/


uint32_t real_judge_link_write_dev(void * const debug_dev)
{
    list_t *cur_node = NULL;
    list_t *tag_list_head = NULL;
    link_hal_item *p_cur_link = NULL;
    tag_list_head = &sg_link_obj.link_all_head;
    list_for_each(cur_node, tag_list_head) {
        p_cur_link = list_entry(cur_node, link_hal_item, link_all_list);
        if(p_cur_link->write_dev == debug_dev) {
           return 1;
        }
    }
   return 0;
}
#if TINY_FUNC_CODE == 0
// int32_t list_links(uint8_t argc, uint8_t *argv[])
// {
//     list_t          *tag_list_head;
//     list_t          *cur_list;
//     link_hal_item  *p_cur_link;
//     (void)p_cur_link;
//     DBG_I("*cur system reg  %d links ", sg_link_obj.real_reg_link_num);
//     tag_list_head = &sg_link_obj.link_all_head;
//     list_for_each(cur_list, tag_list_head) {
//         p_cur_link = list_entry(cur_list, link_hal_item, link_all_list);
//         DBG_I("[link_id:%d] prio:%d -  %-12s ", p_cur_link->link_id, p_cur_link->prio, p_cur_link->name);
//     }
//     return RET_OK;
// }

// REG_SHELL_CMD_X(ls_links, 0, list_links, "list_links", "list_links.  eg list_links \r\n ");


// int32_t list_action(uint8_t argc, uint8_t *argv[])
// {
//     if(argc >= 2) {
//         uint32_t link_id = 0;
//         list_t          *tag_list_head;
//         list_t          *cur_list;
//         link_hal_item  *p_cur_link;
//         uint8_t         find_link = 0;
//         sscanf((const char *)argv[1], "%d", &link_id);
//         {
//             action_tab_t     *p_action_item = NULL;
//             (void)p_action_item;
//             tag_list_head = &sg_link_obj.link_all_head;
//             list_for_each(cur_list, tag_list_head) {
//                 p_cur_link = list_entry(cur_list, link_hal_item, link_all_list);
//                 if(p_cur_link->link_id  == link_id) {
//                     find_link = 1;
//                     break;
//                 }
//             }
//             if(find_link == 0) {
//                 DBG_W("LINK_ID %d is not exist!!!", link_id);
//                 return 0;
//             }
//             if(p_cur_link->flag_init) {
//                 DBG_I("The Link ID:%d action function as follow:", link_id);
//                 tag_list_head = &p_cur_link->action_list_head;
//                 list_for_each(cur_list, tag_list_head) {
//                     p_action_item = list_entry(cur_list, action_tab_t, action_list);
//                     DBG_I("[cmd_set:0x%02x ][cmd_id:0x%02x ]: %s ", p_action_item->cmd_func, p_action_item->cmd_id, p_action_item->func_name);
//                 }
//             } else {
//                 DBG_W("LINK_ID %d is not exist!!!", link_id);
//             }
//         }
//     } else {
//         DBG_W("param is error, [ eg. ] list_action link_id ");
//     }
//     return RET_OK;
// }

// REG_SHELL_CMD_X(ls_action, 0, list_action, "list_action", "list_action.  eg list_action link_id\r\n ");
#endif





