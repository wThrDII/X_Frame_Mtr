/********************** Copyright(c)***************************
** ECOFLOW 
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               comm_cmd.c
** Created by:              
** Created date:            
** Version:                 
** Descriptions:            通用指令
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2022-04-19
** Last version:            
** Description:             通用指令
**
*************************************************************/
#include "x_frame.h"


#include "xh_sys.h"

/**
 * @brief 读取app 信息
 *
 * @param [in] p_app_info
 *
 * @return true
 * @retval
 * @return false
 * @retval
 *
 * @details 特殊说明:  固件前部1K固件信息区，
 *                    详细内容参见 app_inf_t 结构体
 */
bool read_app_info(app_info_t *p_app_info)
{
    part_item_t *p_app_part;
    uint32_t check_val = 1;
    uint8_t *p_start_ptr = 0;
    p_app_part = get_flash_item_by_part_id(SYS_APP_PART);
    if (p_app_part == NULL)
    {
        DBG_E("flash part app-part is not exist");
        return false;
    }
    // 1-check app_info_section
    app_info_t *p_info = (app_info_t *)p_app_part->part_address;
    DBG_I("p_app_part->part_address:0x%x", p_app_part->part_address);
    //    show_app_info(p_app_info);
    // show_buf((uint8_t*)p_app_info,sizeof(app_info_t));
    if (p_info->app_info_chk_type >= u_chk_err)
    {
        DBG_E("app_info_check_type is err");
        return false;
    }
    if (p_info->app_chk_type >= u_chk_err)
    {
        DBG_E("app_check_type is err");
        return false;
    }
    if ((p_info->app_info_block_len) < sizeof(app_info_t))
    {
        DBG_E("app_info_BLOCK_size is err");
        return false;
    }
    if ((p_info->app_info_real_len) != sizeof(app_info_t))
    {
        DBG_E("app_info_size is err");
        return false;
    }
    p_start_ptr = (uint8_t *)&p_info->app_info_ver;
    check_val = calc_chk_val(p_info->app_info_chk_type, p_start_ptr, sizeof(app_info_t) - 4);
    // show_buf(p_start_ptr,sizeof(app_info_t)-4);
    if (check_val != p_info->app_info_chk_val)
    {
        DBG_E("app_info is err[calc:_val:0x%x real:0x%x] size:%d", check_val, p_info->app_info_chk_val, sizeof(app_info_t) - 4);
        return false;
    }
    memcpy(p_app_info, p_info, sizeof(app_info_t));
    return true;
}

/**
 * @brief 读取app拓展信息
 *
 * @param [in] p_ex_app_info
 *
 * @return true
 * @retval
 * @return false
 * @retval
 *
 * @details 特殊说明:
 */
bool read_app_info_ex(app_info_ex_t *p_ex_app_info)
{
    part_item_t *p_app_part;
    uint32_t check_val = 1;
    uint8_t *p_start_ptr = 0;
    p_app_part = get_flash_item_by_part_id(SYS_APP_PART);
    if (p_app_part == NULL)
    {
        DBG_E("flash part app-part is not exist");
        return false;
    }
    //紧跟app_info_t后面
    app_info_ex_t *p_info = (app_info_ex_t *)(p_app_part->part_address + sizeof(app_info_t));
    if (p_info->app_info_ex_real_len > 2048 - sizeof(app_info_t) - 4)
    {
        DBG_E("app info ex size is err,max_len %d, read len %d", 2048 - sizeof(app_info_t) - 4, p_info->app_info_ex_real_len);
        return false;
    }
    p_start_ptr = (uint8_t *)&p_info->app_info_ex_ver;
    check_val = calc_chk_val(u_chk_sum, p_start_ptr, sizeof(app_info_t) - 4);
    // show_buf(p_start_ptr,sizeof(app_info_t)-4);
    if (check_val != p_info->app_info_ex_chk_val)
    {
        DBG_E("app_info is err[calc:_val:0x%x real:0x%x] size:%d", check_val, p_info->app_info_ex_chk_val, sizeof(app_info_t) - 4);
        return false;
    }
    uint16_t read_len = X_FRAME_MIN(p_info->app_info_ex_real_len, sizeof(app_info_ex_t));
    memset(p_ex_app_info, 0, sizeof(app_info_ex_t));
    memcpy(p_ex_app_info, p_info, read_len);
    return true;
}
/*************************************************************
** Descriptions:        获取boot配置信息
*************************************************************/
int32_t read_boot_cfg(boot_cfg_t *p_boot_cfg)
{
    uint32_t chk_val;
    uint32_t start_addr = (uint32_t)p_boot_cfg;
    uint32_t end_addr = (uint32_t)start_addr + (sizeof(boot_cfg_t) - sizeof(p_boot_cfg->boot_cfg_chk_val));
    part_item_t *p_boot_cfg_part = NULL;
    
    p_boot_cfg_part = get_flash_item_by_part_id(g_sys_boot_cfg_part_id);
    if (p_boot_cfg_part == NULL) {
        DBG_E("flash part boot cfg is not exist");
        return -2;
    }

    chk_val =  calc_chk_val(u_chk_sum, (uint8_t *)start_addr, (end_addr - start_addr));
    memcpy(p_boot_cfg, (void *)p_boot_cfg_part->part_address, sizeof(boot_cfg_t));

    if (chk_val != p_boot_cfg->boot_cfg_chk_val || (p_boot_cfg->boot_param_len != sizeof(boot_cfg_t) ) ){
        DBG_I("real_chk:0x%x   boot_chk:0x%x", chk_val, p_boot_cfg->boot_cfg_chk_val);
        return -1;
    }
    else {
        return 0;
    }
}

/*************************************************************
** Descriptions:        保存boot配置信息
*************************************************************/
void save_boot_cfg(boot_cfg_t *p_boot_cfg)
{
    uint32_t start_addr = (uint32_t)p_boot_cfg;
    uint32_t end_addr = (uint32_t)start_addr + (sizeof(boot_cfg_t) - sizeof(p_boot_cfg->boot_cfg_chk_val));
    part_item_t *p_boot_cfg_part = NULL;
    p_boot_cfg_part = get_flash_item_by_part_id(g_sys_boot_cfg_part_id);
    if (p_boot_cfg_part == NULL)
    {
        DBG_E("flash part boot cfg is not exist");
        return;
    }
    p_boot_cfg->boot_cfg_chk_val = calc_chk_val(u_chk_sum, (uint8_t *)start_addr, (end_addr - start_addr));
    flash_part_erase(g_sys_boot_cfg_part_id);
    flash_part_write(g_sys_boot_cfg_part_id, 0, (uint8_t *)p_boot_cfg, sizeof(boot_cfg_t));
}

/*************************************************************
    COMM CMD
*************************************************************/

/*************************************************************
** Descriptions:        获取硬件版本信息
*************************************************************/
int32_t get_hw_version_func(hw_info_t *p_hw_info)
{
    uint16_t chk_val = 0;
    flash_part_read(g_sys_hw_ver_part_id, 0, (uint8_t *)p_hw_info, sizeof(hw_info_t));
    chk_val = calc_chk_val(u_chk_crc16, (uint8_t *)p_hw_info + 2, sizeof(hw_info_t) - 2);
    if ((chk_val != p_hw_info->crc_chk_val))
    {
        return -1;
    }
    return 0;
}

/*************************************************************
** Descriptions:        获取cpu id信息
*************************************************************/
int32_t get_cpu_id_info(hal_frame_info_t *p_frame_info)
{
    uint8_t cpu_id[CPUID_BYTES] = {0};
    if(gx_xh_sys_id != NULL){
        xh_sys_get_cpuid(gx_xh_sys_id, cpu_id, CPUID_BYTES);
    }
    protocol_send_ack(p_frame_info, (uint8_t *)&cpu_id, CPUID_BYTES);
    return 0;
}

/*************************************************************
** Descriptions:        获取 SN 信息
*************************************************************/
int32_t read_sn_info(sn_info_t *p_sn_info)
{
    uint8_t i = 0;
    uint8_t *p_u8_str = (uint8_t *)p_sn_info;
    uint32_t chk_val=0;	
    flash_part_read(g_sys_product_part_id, 0, (uint8_t *)p_sn_info, sizeof(sn_info_t));
    for (i = 4; i < (sizeof(sn_info_t) - 4); i++) {
        chk_val += p_u8_str[i];
    }
    if (chk_val != p_sn_info->chk_val) {
        memset(p_sn_info, 0, sizeof(sn_info_t));
        return -1;
    } 
    return 0;
}
/*************************************************************
** Descriptions:        发送 SN 信息
*************************************************************/
int32_t get_sn_info(hal_frame_info_t *p_frame_info)
{
    sn_info_t sn_info = {0};
    DBG_I("get_sn_info, src:%d, dest:%d, is_ack:%d", p_frame_info->src, p_frame_info->dest, p_frame_info->is_ack);
    read_sn_info(&sn_info);
    sn_info.model = 0;
    protocol_send_ack(p_frame_info, (uint8_t *)&sn_info, sizeof(sn_info_t));
    return 0;
}


/*************************************************************
** Descriptions:        设置sn信息
*************************************************************/
int32_t set_sn_info(hal_frame_info_t *p_frame_info)
{
    sn_info_t  sn_info = {0};
    uint8_t     *p_u8_str = (uint8_t *)&sn_info;
    uint8_t i  = 0;
    uint8_t ret = 0;
    //show_buf((uint8_t*)p_frame_info->pdata,p_frame_info->data_len);
    if(p_frame_info->data_len == SN_MAX_LEN) {
    if(gx_xh_sys_id != NULL){
        xh_sys_get_cpuid(gx_xh_sys_id, sn_info.cpu_id, CPUID_BYTES);
    }
        sn_info.product_type   = g_product_type;
        sn_info.product_detail = g_product_detail;
        sn_info.chk_val = 0;
        memcpy(sn_info.sn, p_frame_info->pdata, SN_MAX_LEN);
        for(i = 4; i < (sizeof(sn_info_t) -4); i++) {
            sn_info.chk_val += p_u8_str[i];
        }
        flash_part_erase(g_sys_product_part_id);
        flash_part_write(g_sys_product_part_id, 0, (uint8_t *)&sn_info, sizeof(sn_info_t));
        flash_part_read(g_sys_product_part_id, 0, (uint8_t *)&sn_info, sizeof(sn_info_t));
        //show_buf((uint8_t*)p_sn_info,sizeof(sn_info_t));
        protocol_send_ack(p_frame_info, (uint8_t *)&ret, sizeof(uint8_t));
        return 0;
    }
    ret = 1;
    protocol_send_ack(p_frame_info, (uint8_t *)&ret, sizeof(uint8_t));
    return 1;
}

/*************************************************************
** Descriptions:        设置产品名称
*************************************************************/
int32_t set_product_name(hal_frame_info_t *p_frame_info)
{
    user_rec_info user_rec = {0};
    uint8_t res = 0;
    uint8_t *p_user_data = (uint8_t *)&user_rec + sizeof(user_rec.chk_val);
    if (sizeof(product_name_obj) <= p_frame_info->data_len)
    {
        product_name_obj *p_cmd = (product_name_obj *)p_frame_info->pdata;
        flash_part_read(g_sys_rec_part_id, 0, (uint8_t *)&user_rec, sizeof(user_rec));
        uint32_t chk_val = calc_chk_val(u_chk_crc16, p_user_data, sizeof(user_rec) - sizeof(user_rec.chk_val));
        if (chk_val != user_rec.chk_val)
        {
            memset(p_user_data, 0, sizeof(user_rec) - sizeof(user_rec.chk_val));
        }
        else
        {
            memset(&user_rec.product_name, 0, sizeof(user_rec.product_name));
        }
        memcpy(&user_rec.product_name, p_cmd, sizeof(user_rec.product_name));
        user_rec.chk_val = calc_chk_val(u_chk_crc16, p_user_data, sizeof(user_rec) - sizeof(user_rec.chk_val));
        flash_part_erase(g_sys_rec_part_id);
        flash_part_write(g_sys_rec_part_id, 0, (uint8_t *)&user_rec, sizeof(user_rec));
    }
    else
    {
        res = 1;
    }
    protocol_send_ack(p_frame_info, (uint8_t *)&res, sizeof(res));
    return 0;
}

/*************************************************************
** Descriptions:        获取产品名称和长度
*************************************************************/
int32_t read_product_name(product_name_obj *p_product_name)
{
    user_rec_info user_rec = {0};
    uint8_t *p_user_data = (uint8_t *)&user_rec + sizeof(user_rec.chk_val);
    flash_part_read(g_sys_rec_part_id, 0, (uint8_t *)&user_rec, sizeof(user_rec_info));

    uint32_t chk_val = calc_chk_val(u_chk_crc16, p_user_data, sizeof(product_name_obj));
    if (chk_val != user_rec.chk_val) 
    {
        memset(p_product_name, 0, sizeof(product_name_obj));
        return -1;
    } else {
        memcpy(p_product_name, &user_rec.product_name, sizeof(product_name_obj));
        return 0;
    }
}
/*************************************************************
** Descriptions:        发送产品名称
*************************************************************/
int32_t get_product_name(hal_frame_info_t *p_frame_info)
{
    product_name_obj ack = {0};
    read_product_name(&ack);
    protocol_send_ack(p_frame_info, (uint8_t *)&ack, sizeof(ack));
    return 0;
}

/*************************************************************
** Descriptions:        设置产品信息指令使能，默认使能状态，
**						主要用于固件升级搬运固件过程关闭版本
**						信息获取功能，防止重复升级
*************************************************************/
static bool product_info_cmd_enable = true;
void set_product_info_cmd_enable(bool is_enable)
{
	product_info_cmd_enable = is_enable;
}

/*************************************************************
** Descriptions:        获取产品信息
*************************************************************/
int32_t action_product_info(hal_frame_info_t *p_frame_info)
{
	if(false == product_info_cmd_enable)
	{
		return 0;
	}
    uint8_t ret = 0;
    product_info app_product_info = {0};
    hw_info_t hw_info = {0};
    ret = get_hw_version_func(&hw_info);
    if (ret != 0)
    {
        memset(&app_product_info, 0xff, sizeof(product_info));
        protocol_send_ack(p_frame_info, (uint8_t *)&app_product_info, sizeof(product_info));
        return 0;
    }

    app_product_info.app_main_ver = hw_info.app_main_ver;
    app_product_info.app_sub_ver = hw_info.app_sub_ver;
    app_product_info.app_dbg_ver = 0;
    app_product_info.app_test_ver = 0;
    app_product_info.loader_ver = hw_info.loader_ver;

    app_product_info.product_type = g_product_type;
    app_product_info.product_detail = g_product_detail;

    memset(app_product_info.comp_time, 0, sizeof(app_product_info.comp_time));
    strncpy((char *)app_product_info.comp_time, COMP_DATE " " COMP_TIME, APP_COMP_SIZE - 1);

    app_info_ex_t ex_app_info;
    if (read_app_info_ex(&ex_app_info) == true)
    {
        app_product_info.pack_main_ver = ex_app_info.pack_version[3];
        app_product_info.pack_sub_ver = ex_app_info.pack_version[2];
        app_product_info.pack_dbg_ver = ex_app_info.pack_version[1];
        app_product_info.pack_test_ver = ex_app_info.pack_version[0];
    }

#if PROGRAM_TYPE == PROGRAM_APL

		app_product_info.app_main_ver = g_app_main_ver;
		app_product_info.app_sub_ver =  g_app_sub_ver;
		app_product_info.app_dbg_ver =  g_app_dbg_ver;
		app_product_info.app_test_ver = g_app_test_ver;
		app_product_info.loader_ver = hw_info.loader_ver;
		
        app_product_info.lib_hal_main_ver = g_hal_drv_main_ver;
        app_product_info.lib_hal_sub_ver = g_hal_drv_sub_ver;
        app_product_info.lib_hal_dbg_ver = g_hal_drv_dbg_ver;
        app_product_info.lib_hal_test_ver = g_hal_drv_test_ver;

        app_product_info.lib_xframe_main_ver = XFRAME_MAIN_VER;
        app_product_info.lib_xframe_sub_ver = XFRAME_SUB_VER;
        app_product_info.lib_xframe_dbg_ver = XFRAME_DBG_VER;
        app_product_info.lib_xframe_test_ver = XFRAME_TEST_VER;
    
#endif
    protocol_send_ack(p_frame_info, (uint8_t *)&app_product_info, sizeof(product_info));
    return 0;
}
X_FRAME_WEAK uint8_t _sys_info_action_list[][2] = {{0, 0}};
X_FRAME_WEAK uint8_t _sys_info_list_cnt = 0;

int32_t action_sys_product_info(hal_frame_info_t *p_frame_info)
{
    if (_sys_info_list_cnt > 0)
    {
        hal_frame_info_t info = {0};
        memcpy(&info, p_frame_info, sizeof(hal_frame_info_t));

        uint8_t i;
        for (i = 0; i < _sys_info_list_cnt; i++)
        {
            p_frame_info->link_id = _sys_info_action_list[i][0];
            p_frame_info->dest = _sys_info_action_list[i][1];
            protocol_route_send_by_id(p_frame_info);
        }
        action_product_info(&info);
    }
    else
    {
        action_product_info(p_frame_info);
    }
    return 0;
}

X_FRAME_WEAK void cmd_dev_reset_cb(void)
{
    DBG_I("please achieve func <cmd_dev_reset_cb>, type: void cmd_dev_reset_cb(void)");
}

int32_t cmd_dev_reset(hal_frame_info_t *p_frame_info)
{
    protocol_send_ack(p_frame_info, NULL, 0);
	cmd_dev_reset_cb();
    block_ms(100);		/* 延时保证指令发送完成 */
    if(gx_xh_sys_id != NULL){
        xh_sys_reset(gx_xh_sys_id);
    }
    return 1;
}


/*************************************************************
** Descriptions:        获取时间
*************************************************************/
int32_t get_unix_time(hal_frame_info_t *p_frame_info)
{
    rtc_time_t time = {0};
    time.unix_time = real_get_calendar_sec();
    time.time_zone = get_time_zone();
    protocol_send_ack(p_frame_info, (uint8_t *)&time, sizeof(rtc_time_t));
    return 0;
}

/*************************************************************
** Descriptions:        设置时间
*************************************************************/
int32_t set_unix_time(hal_frame_info_t *p_frame_info)
{
    uint16_t read_len = X_FRAME_MIN(sizeof(rtc_time_t), p_frame_info->data_len);
    rtc_time_t time = {0};
    memcpy(&time, p_frame_info->pdata, read_len);
    real_set_calendar_sec(time.unix_time);
    set_time_zone(time.time_zone);

    uint8_t sta = 0;

    /* 广播数据不做回复 */
    if (p_frame_info->dest != 0)
    {
        protocol_send_ack(p_frame_info, (uint8_t *)&sta, sizeof(sta));
    }
    return 0;
}

int32_t set_silence_state(hal_frame_info_t *p_frame_info)
{
    uint8_t enable = *(uint8_t *)p_frame_info->pdata;
    protocol_send_silence_set(enable);

    /* 广播数据不做回复 */
    if (p_frame_info->dest != 0)
    {
        protocol_send_ack(p_frame_info, (uint8_t *)&enable, sizeof(enable));
    }
    return 0;
}

//REG_CMDSET_ACTION(COMM_CMD_SET, CMD_ID_GET_GPUID, 		    get_cpu_id_info);	
//REG_CMDSET_ACTION(COMM_CMD_SET, CMD_ID_GET_SN_INFO, 		get_sn_info);	
//REG_CMDSET_ACTION(COMM_CMD_SET, CMD_ID_SET_SN_INFO, 		set_sn_info);	
//REG_CMDSET_ACTION(COMM_CMD_SET, CMD_ID_PRODUCT_INFO, 	    action_product_info);  
//REG_CMDSET_ACTION(COMM_CMD_SET, CMD_ID_DEV_RESET, 	        cmd_dev_reset);        
//REG_CMDSET_ACTION(COMM_CMD_SET, CMD_ID_SET_PRODUCT_NAME,    set_product_name);   
//REG_CMDSET_ACTION(COMM_CMD_SET, CMD_ID_GET_PRODUCT_NAME,    get_product_name);   
//REG_CMDSET_ACTION(COMM_CMD_SET, CMD_ID_GET_UNIX_TIME,       get_unix_time);      
//REG_CMDSET_ACTION(COMM_CMD_SET, CMD_ID_SET_UNIX_TIME,       set_unix_time);




