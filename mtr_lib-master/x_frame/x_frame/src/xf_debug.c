/**
 * @brief debug处理
 * @file xf_debug.c.xtemp
 * @version 1.0
 * @author hzf
 * @date 2022-8-8 15:8:59
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-8-8 15:8:59
 *   Author      : hzf
 *   Modification: Created file
 */

#include "x_frame.h"
#include "xh_stream.h"


static list_t all_debug_head = LIST_HEAD_INIT(all_debug_head);
X_FRAME_WEAK X_DBG_LINK_TAB g_x_dbg_link_tab = {NULL,0};
uint8_t enable_protocol_dbg_out = 1;
uint32_t flag_enable_cpf = 0;

static void debug_dev_init(void)
{
	uint8_t i = 0;
	debug_obj_t *p_debug_obj = NULL;
	
	for(i = 0;i < MAX_DRV_NUM;i++)
	{
		p_debug_obj = auto_reg_get_func_cont_by_index(AUTO_REG_TYPE_DBG_DEV, i);	//shell中也寻找了一遍
		
		if(p_debug_obj != NULL)
		{
			list_add_tail(&p_debug_obj->debug_node, &all_debug_head);
		}
		else
		{
			break;
		}
	}
}

REG_INIT_MOD_RESOURCE(debug_dev_init);

void dbg(uint8_t enable_protocol, uint8_t log_type,uint8_t log_window,char *fmt, ...)
{
    va_list ap;
    uint16_t len = 0;
    uint8_t str[DEBUG_BUF_SIZE];
    debug_obj_t *p_dev_obj = NULL;
    list_t  *cur_node = NULL;
    static volatile uint8_t lock = 0;
	
    if(lock == 1)
    {
        return;
    }

    lock = 1;
    va_start(ap, fmt);
	/*
	+-+-+---------n-3-----------+--+
	|0|1|	...........\0		|\0|
	+-+-+-----------------------+--+
	*/
    len = vsnprintf((char*)&str[2], DEBUG_BUF_SIZE - 3, fmt, ap);
    str[len+2] = '\0';
    len = len + 1;


    list_t *tag_list_head = &all_debug_head;
	
    list_for_each(cur_node, tag_list_head)
    {
        p_dev_obj = list_entry(cur_node, debug_obj_t, debug_node);
		
		if(real_judge_link_write_dev(p_dev_obj->debug_dev) == 0)	//如果同一个设备注册为DEBUG和LINK，则DEBUG不输出
		{
		    xh_stream_write(p_dev_obj->debug_dev, (uint8_t *)&str[2], len);
		}
    }
    
	hal_frame_info_t  shell_frame_info={0};
	if(enable_protocol_dbg_out>0)
	{
		str[0]=log_type;
		str[1]=log_window;
		shell_frame_info.cmd_func =  0x01;				/* COMM_CMD_SET */
		shell_frame_info.cmd_id   =  0x22;  			/* CMD_ID_SHELL_OUT */
		shell_frame_info.src      =  x_frame_host_addr;
		shell_frame_info.dest     =  x_frame_pc_addr;
		shell_frame_info.pdata    =  (uint8_t *)str;
		shell_frame_info.data_len =  len-1;    //协议传输不用后面的\0    add log_type  log_win  sub 0x0d  0x0a
		uint16_t cir = 0;
		for(;cir < g_x_dbg_link_tab.link_num;cir++){
			shell_frame_info.link_id  =  g_x_dbg_link_tab.p_link_arry[cir];
			protocol_send_by_id(&shell_frame_info);
		}
	}
    lock = 0;
    va_end(ap);
}

REG_CFG_ITEM(en_protocol_dbg,enable_protocol_dbg_out, eu_uint32,  PARM_RW, 1.0f, 0.0f, 1.0f );



