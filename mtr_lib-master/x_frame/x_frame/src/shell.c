#include "x_frame.h"
#include "xh_stream.h"


#define SHELL_WRITE	PRF
#define SHELL_BUF_SIZE  300
#define SHELL_MAX_ARGS  8
#define SHELL_X_MAX_ITEM  12
#define SHELL_MAX_ITEM    32


typedef struct
{
    uint8_t  shell_init_flag;

    uint8_t  shell_x_item ;
    shell_cmd_t  *sg_shell_x_item[SHELL_X_MAX_ITEM];

    uint8_t  shell_item;
    shell_cmd_t  *sg_shell_item[SHELL_MAX_ITEM];

    list_t	 shell_dev_head;
} shell_obj_t;



static shell_obj_t shell_obj = {.shell_dev_head = LIST_HEAD_INIT(shell_obj.shell_dev_head)};
static int32_t parse_line (uint8_t *line, uint8_t *argv[], uint8_t *p_args, uint8_t max_len);
static uint8_t 	*shell_dec_line	(linebuf_t   *p_lb);
static shell_cmd_t *find_cmd		(uint8_t *pcmd);
static uint8_t find_EOF(uint8_t *pbuf, uint32_t len);

static uint8_t shell_action(uint8_t *pbuf_start, uint8_t max_len)
{
    uint8_t  cur_args = 0;
    uint8_t  read_len = 0;
    uint8_t *shell_args[SHELL_MAX_ARGS];
    shell_cmd_t  *cur_cmd = NULL;

    read_len = parse_line(pbuf_start, shell_args, &cur_args, max_len);

    /*寻找对应的cmd，并且执行*/

    if(cur_args)
    {
        cur_cmd = find_cmd(shell_args[0]);
        if(!cur_cmd)
        {
            SHELL_WRITE("can not find the cmd: %s", shell_args[0]);
        }
        else
        {
            cur_cmd->cmd_func(cur_args, shell_args);
        }
    }
	
    return read_len;
}

static void shell_run_on_tick(void)
{
    list_t *cur_list;
    list_t *tag_list_head = &shell_obj.shell_dev_head;
    debug_obj_t *p_dev_obj = NULL;

    if(tag_list_head->next == tag_list_head){
        current_task_delete();
        return;
    }
    
    list_for_each(cur_list, tag_list_head)
    {
        uint16_t lb_rest_len = 0;
        uint32_t real_len = 0;
        int32_t  ret = 0;
        uint8_t  temp_buf[SHELL_BUF_SIZE];
        uint8_t *pbuf_start = NULL;
        uint8_t  read_len = 0;
        uint32_t cur_len = 0;

        p_dev_obj = list_entry(cur_list, debug_obj_t, shell_debug_node);

        /*1:从fifo中读取数据到linebuf*/
        lb_rest_len = lb_get_rest_len(p_dev_obj->p_line_buf);
        real_len = xh_stream_read(p_dev_obj->debug_dev, temp_buf, lb_rest_len);
        //DBG_I("hal_app_read  dev:%d read:%d len:%d ",p_dev_obj->debug_dev,lb_rest_len,real_len);
        if( (ret >= 0) && (real_len > 0) )
        {
            lb_push(p_dev_obj->p_line_buf, temp_buf, real_len);
        }

        /*2:不断的解析linebuf*/
        do
        {
            cur_len = lb_get_data_len(p_dev_obj->p_line_buf);
            if(find_EOF(p_dev_obj->p_line_buf->pbuf, cur_len))
            {
                pbuf_start = shell_dec_line(p_dev_obj->p_line_buf);
                read_len = shell_action(pbuf_start, cur_len);
            }
            else
            {
                break;
            }
            /*3:移除已经处理的过的linebuf数据*/
            lb_move(p_dev_obj->p_line_buf, read_len);
        }
        while(real_len);
    }
}

REG_TASK(VH_TASK, 100, shell_run_on_tick);

static int32_t  is_ascii(uint8_t ch)
{
    if( ( (ch > 'a') && (ch < 'z') ) ||
        ( (ch > 'A') && (ch < 'Z') ) ||
        ( (ch > '0') && (ch < '9') )
      )
    {
        return 1;
    }
    return 0;
}


static uint8_t *shell_dec_line(linebuf_t   *p_lb)
{
    uint16_t 	buf_len = 0;
    uint8_t	*cur_ptr = NULL;
    uint16_t 	cur_index = 0;

    buf_len = lb_get_data_len(p_lb);
    cur_ptr = lb_get_r_ptr(p_lb);

    while(cur_index < buf_len)
    {
        if(is_ascii(cur_ptr[cur_index]))
        {
            return &cur_ptr[cur_index];
        }
        cur_index++;
        p_lb->r_index++;
    }

    return NULL;
}


static int32_t parse_line (uint8_t *line, uint8_t *argv[], uint8_t *p_args, uint8_t max_len)
{
    int nargs = 0;
    int16_t read_len = 0;
    while ((nargs < SHELL_MAX_ARGS) && (read_len < max_len))
    {
        /* skip any white space */
        while ( ((*line == ' ') || (*line == '\t') || (*line == '\r') || (*line == '\n')) && (read_len < max_len))
        {
            ++line;
            read_len++;
        }

        if (*line == '\0')  	/* end of line, no more args	*/
        {
            argv[nargs] = NULL;
            *p_args = nargs;
            return (read_len);
        }

        if(read_len < max_len)
        {
            argv[nargs++] = line;	/* begin of argument string	*/
        }


        /* find end of string */
        while (*line && (*line != ' ') && (*line != '\t') && (*line != '\r') && (*line != '\n') && (read_len < max_len) )
        {
            ++line;
            read_len++;
        }

        *p_args = nargs;

        if ((*line == '\0') )  	/* end of line, no more args	*/
        {
            argv[nargs] = NULL;
            return (read_len);
        }

        if(read_len < max_len)
        {
            *line++ = '\0';		/* terminate current arg	 */
            read_len++;
        }
    }

    //SHELL_WRITE ("** Too many args (max. %d) **\n", SHELL_MAX_ARGS);
    return (read_len);
}


static uint8_t find_EOF(uint8_t *pbuf, uint32_t len)
{
    uint32_t i = 0;

    for(i = 0; i < len; i++)
    {
        if((pbuf[i] == '\r') || ((pbuf[i] == '\n')))
        {
            return 1;
        }
    }

    return 0;
}

static void init_shell_item(void)
{
    uint8_t i = 0;
	debug_obj_t *p_debug_obj = NULL;
	
	for(i = 0;i < MAX_DRV_NUM;i++)
	{
		p_debug_obj = auto_reg_get_func_cont_by_index(AUTO_REG_TYPE_DBG_DEV, i);	//shell中也寻找了一遍
		
		if(p_debug_obj != NULL)
		{
			list_add_tail(&p_debug_obj->shell_debug_node, &shell_obj.shell_dev_head);
		}
		else
		{
			break;
		}
	}
    shell_obj.shell_x_item   = auto_reg_get_func_type_cont(AUTO_REG_TYPE_SHELL_X, (void **)shell_obj.sg_shell_x_item, SHELL_X_MAX_ITEM);
    shell_obj.shell_item     = auto_reg_get_func_type_cont(AUTO_REG_TYPE_SHELL  , (void **)shell_obj.sg_shell_item  , SHELL_MAX_ITEM  );
}

REG_INIT_MOD_BASIC(init_shell_item);

static shell_cmd_t *find_cmd(uint8_t *pcmd)
{
    shell_cmd_t *pShell_cmd = NULL;
    uint32_t i = 0;

    for(i = 0; i < shell_obj.shell_item; i++)
    {
        pShell_cmd = (shell_cmd_t *)(shell_obj.sg_shell_item[i]);
        if(strcmp((const char *)pShell_cmd->name, (const char *)pcmd) == 0)
        {
            return pShell_cmd;
        }
    }

    for(i = 0; i < shell_obj.shell_x_item; i++)
    {
        pShell_cmd = (shell_cmd_t *)(shell_obj.sg_shell_x_item[i]);
        if(strcmp((const char *)pShell_cmd->name, (const char *)pcmd) == 0)
        {
            return pShell_cmd;
        }
    }
    return NULL;
}


int32_t shell_helpx(uint8_t argc, uint8_t *argv[])
{
    shell_cmd_t *pShell_cmd = NULL;
    uint32_t i = 0;

    /*help 后面还有参数，说明是显示具体函数的帮助信息所有就不要显示list信息了*/
    if(argc == 1)
    {
        SHELL_WRITE("[sys reg cmd list]:");
        SHELL_WRITE(" --name--    ----usestage----");
    }

    for(i = 0; i < shell_obj.shell_x_item; i++)
    {
        pShell_cmd = (shell_cmd_t *)shell_obj.sg_shell_x_item[i];
        if(argc > 1)
        {
            if(strcmp((const char *)pShell_cmd->name, (const char *)argv[1]) == 0)
            {
                SHELL_WRITE("%-10s  :\r\n  %s", pShell_cmd->name, pShell_cmd->help);
                return RET_OK;
            }
        }
        else
        {
            SHELL_WRITE("%-15s  : %-15s", pShell_cmd->name, pShell_cmd->usage);
        }
    }

    if(argc > 1)
    {
        SHELL_WRITE("Not find the shell cmd: %s", argv[1]);
    }

    return RET_OK;
}

REG_SHELL_CMD_X(helpx, 0, shell_helpx, "help to list sys_cmds",
                "this is the dtail help for help \r\n \
    help - list all func.\r\n \
    help - cmd_name  show the detail info for cmd");


int32_t shell_help(uint8_t argc, uint8_t *argv[])
{
    shell_cmd_t *pShell_cmd = NULL;
    uint32_t i = 0;

    /*help 后面还有参数，说明是显示具体函数的帮助信息所有就不要显示list信息了*/
    if(argc == 1)
    {
        SHELL_WRITE("[user reg cmd list]:");
        SHELL_WRITE(" --name--    ----usestage----");
    }

    for(i = 0; i < shell_obj.shell_item; i++)
    {
        pShell_cmd = (shell_cmd_t *)shell_obj.sg_shell_item[i];
        if(argc > 1)
        {
            if(strcmp((const char *)pShell_cmd->name, (const char *)argv[1]) == 0)
            {
                SHELL_WRITE("%-10s  :\r\n  %s", pShell_cmd->name, pShell_cmd->help);
                return RET_OK;
            }
        }
        else
        {
            SHELL_WRITE("%-15s  : %-15s", pShell_cmd->name, pShell_cmd->usage);
        }
    }

    if(argc > 1)
    {
        SHELL_WRITE("Not find the shell cmd:%s", argv[1]);
    }

    return RET_OK;
}

REG_SHELL_CMD(help, 0, shell_help, "help to list user_cmds",
              "this is the dtail help for help \r\n \
    help - list all func.\r\n \
    help - cmd_name  show the detail info for cmd");


#define MAX_SHOW_BUF_LEN 200
static uint8_t show_buf_buf[MAX_SHOW_BUF_LEN];
void real_show_buf(uint8_t *pbuf, uint8_t len)
{
	uint8_t i = 0;
	uint32_t index = 0;
	uint32_t cnt = 0;
	uint8_t *p_buf = show_buf_buf;
	cnt = sprintf((char*)&p_buf[index],"\r\n"); index+=cnt;
    for(i = 0; i < len; i++){
        if(index + 4 > (MAX_SHOW_BUF_LEN-2)) {
            break;
        }
        cnt = sprintf((char*)&p_buf[index],"%02x ", pbuf[i]);  
        index+=cnt;
        if(i % 16 == 15){
            cnt = sprintf((char*)&p_buf[index],"\r\n");
            index+=cnt;
        }
    }
    cnt = sprintf((char*)&p_buf[index],"\r\n"); index+=cnt;
  
  DBG_RAW_SER("%s",p_buf);
}

int32_t action_shell_in(hal_frame_info_t* p_frame_info)
{
	shell_action(p_frame_info->pdata,p_frame_info->data_len);
    return 0;
}

REG_SILENCE_CMD(COMM_CMD_SET, CMD_ID_SHELL_OUT, 1);





