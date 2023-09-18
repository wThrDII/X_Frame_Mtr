
#include "x_frame.h"
#include "xh_fwdgt.h"
#include "xh_sys.h"

#define MAX_TRANS_PACK_LEN 256 //固件包单包最大数据

//固件下载配置
FW_DL_OBJ g_fw_dl_info = {0};

X_FRAME_WEAK int32_t firmware_download_event_notify(P_FW_DL_OBJ dl_obj, uint32_t ev, void *param)
{
	return 0;
}

static uint8_t check_part_is_regist(uint32_t part_id)
{
    part_item_t *p_part;
    p_part = get_flash_item_by_part_id(part_id);
    if (p_part == NULL)
    {
        return 0;
    }
    return 1;
}
/**
 * @brief 擦除固件，每次擦除一页
 *
 * @param part_id
 * @param offset
 * @return int32_t 擦除成功，返回擦除数据量。失败返回负值
 */
static int32_t flash_part_erase_one_page(int part_id, uint32_t offset)
{
    uint32_t sector_size = flash_part_get_sector_size(part_id);
    uint32_t part_size = flash_part_get_part_size(part_id);
    if (offset < part_size)
    {
        flash_part_erase_len(part_id, offset, sector_size);
        return sector_size;
    }
    return 0;
}
/**
 * @brief 固件校验
 *
 * @param [in] part_id
 * @param [in] chk_type
 * @param [in] len
 *
 * @return uint32_t
 * @retval
 *
 * @details 特殊说明:
 */
uint32_t fw_chk_calc(int part_id, uint8_t chk_type, uint32_t len)
{
    uint32_t chk_val = 0;
    uint8_t flash_buff[512];
    uint32_t end_offset = len;
    uint32_t offset = 0;
    while (offset < end_offset)
    {
        uint32_t read_size = end_offset - offset;
        if (read_size > sizeof(flash_buff))
            read_size = sizeof(flash_buff);
        flash_part_read(part_id, offset, flash_buff, read_size);
        chk_val = calc_chk_init_val(chk_type, chk_val, flash_buff, read_size);
        offset += read_size;
    }

    return chk_val;
}
#if PROGRAM_TYPE == PROGRAM_LOADER
struct _loader_program_info;
typedef struct _loader_program_info loader_program_info_t;

struct _loader_program_info
{
    uint8_t jump_part;     //下一跳固件分区
    uint8_t download_part; //备份域id
    uint32_t erase_addr;   //擦除地址
    uint32_t program_addr; //编程地址
    uint32_t fw_size;      //固件大小，包括1K区域
    uint8_t program_part;  //编程域id
    uint32_t timestamp;    //时间戳
    boot_cfg_t boot_cfg;
};

#define DEF_BOOT_DELAY (10)
#define APP_ENTRY_OFFSET (0x400)

/* 状态机名 */
#define LOADER_FSM_ID 0xf1

//---APP校验类型
#define APP_CHK_TYPE_NONE 0x00
#define APP_CHK_TYPE_SUM 0x01
#define APP_CHK_TYPE_CRC8 0x02
#define APP_CHK_TYPE_CRC16 0x03
#define APP_CHK_TYPE_CRC32 0x04

#define DBG_ERR_BOOT_CFG (0x01 << 0)   // BOOT参数区被破坏
#define DBG_ERR_IWDG_OV (0x01 << 1)    //看门狗溢出
#define DBG_ERR_APL_BROKEN (0x01 << 2) // APL区域被破坏
#define DBG_ERR_UPG_REQ (0x01 << 3)    // APL升级请求
#define DBG_ERR_BACK_CPY (0x01 << 4)   //备份域拷贝

extern uint8_t g_boot_main_ver;
extern uint8_t g_boot_sub_ver;
extern uint8_t g_boot_dbg_ver;
extern uint8_t g_boot_test_ver;



loader_program_info_t g_loader_program_info = {0};

loader_program_info_t *g_ptr_loader_program_info = &g_loader_program_info;
/* 状态机状态 */
enum
{
    LD_S_NONE = 0,     //不切状态
    LD_S_CHECK = 1,    //固件及固件参数检查
    LD_S_UPDATE_IDLE,  //空闲态
    LD_S_UPDATE_TRANS, //固件下载中
    LD_S_GOTO_APL,     //跳转到APL
    LD_S_UPDATE_APL,   //更新APL区域
};

/* 状态机事件 */
enum
{
    EV_force_stay_in_loader = 1, //强制停留在loader中
    EV_boot_param_err,           // boot参数错误
    EV_in_loader_by_apl,         // apl请求升级
    EV_apl_err,                  // apl区域被破坏
    EV_stat_timeout,             //状态超时
    EV_recv_fw_data,             //接收到固件数据
    EV_fw_dl_finish,             //下载固件完成
    EV_backup_not_exist,         //备份域不存在
    EV_backup_fw_ready,          //备份域固件OK
    EV_update_success,           //更新固件完成
    EV_update_exit,              //更新固件失败,退出更新
};

static bool check_boot_cfg_is_ok(boot_cfg_t *p_boot_cfg);
static void reset_and_save_boot_cfg(boot_cfg_t *p_boot_cfg);
static bool loader_check_reset_by_iwdg(void);
static bool check_apl_backup_ready(uint8_t backup_part);
static bool check_fw_part_data_valid(uint8_t part_id);
static uint32_t apl_fw_chk_calc(int part_id, uint32_t begin, uint8_t chk_type, uint32_t len);
static bool check_apl_is_ok(uint8_t apl_part);
static void broken_backup_part_data(void);

static void ld_s_check_in(void);
static void ld_s_check_exe(void);
static uint32_t ld_s_check_cond(uint32_t event, uint8_t *pbuf, uint32_t len);

static void ld_s_idle_in(void);
static void ld_s_idle_exe(void);
static uint32_t ld_s_idle_cond(uint32_t event, uint8_t *pbuf, uint32_t len);

static void ld_s_trans_in(void);
static void ld_s_trans_exe(void);
static uint32_t ld_s_trans_cond(uint32_t event, uint8_t *pbuf, uint32_t len);

static void ld_s_goto_app_in(void);
static void ld_s_goto_app_exe(void);
static uint32_t ld_s_goto_app_cond(uint32_t event, uint8_t *pbuf, uint32_t len);

static void ld_s_update_app_in(void);
static void ld_s_update_app_exe(void);
static uint32_t ld_s_update_app_cond(uint32_t event, uint8_t *pbuf, uint32_t len);

REG_FSM_TAB(LOADER_FSM_ID, 0, LD_S_CHECK,
            {LD_S_CHECK, ld_s_check_in, ld_s_check_exe, NULL, ld_s_check_cond},
            {LD_S_UPDATE_IDLE, ld_s_idle_in, ld_s_idle_exe, NULL, ld_s_idle_cond},
            {LD_S_UPDATE_TRANS, ld_s_trans_in, ld_s_trans_exe, NULL, ld_s_trans_cond},
            {LD_S_GOTO_APL, ld_s_goto_app_in, ld_s_goto_app_exe, NULL, ld_s_goto_app_cond},
            {LD_S_UPDATE_APL, ld_s_update_app_in, ld_s_update_app_exe, NULL, ld_s_update_app_cond}, );

static void loader_fw_program_task(void)
{
    RUN_FSM(LOADER_FSM_ID);
}
REG_TASK(ID_TASK, 2, loader_fw_program_task);
static void change_apl_def_version(void)
{
	  uint8_t ret;
		hw_info_t hw_info;
		ret = get_hw_version_func(&hw_info);
		if(ret)
		{
			if(hw_info.app_test_ver != 0 || hw_info.app_dbg_ver != 0)
			{
				hw_info.app_test_ver = 0;
				hw_info.app_dbg_ver  = 0;
        hw_info.crc_chk_val = calc_chk_val(u_chk_crc16, (uint8_t *)&hw_info + 2, sizeof(hw_info_t) - 2);
        flash_part_erase(SYS_HW_VER_PART);
        flash_part_write(SYS_HW_VER_PART, 0, (uint8_t *)&hw_info, sizeof(hw_info_t));
			}
		}
}
static void ld_s_check_in(void)
{

    boot_cfg_t *p_boot_cfg = &(g_ptr_loader_program_info->boot_cfg);

    load_boot_cfg(p_boot_cfg);
    /* 检查BOOT参数是否正确
     * 如果只有loader程序，此区域为空
     * 如果来 来自于apl复位
     */
    if (!check_boot_cfg_is_ok(p_boot_cfg))
    {
        reset_and_save_boot_cfg(p_boot_cfg);
        EMIT_FSM(LOADER_FSM_ID, EV_boot_param_err, NULL, 0);
        return;
    }
    /* 检查看门狗复位标志 */
    if (loader_check_reset_by_iwdg())
    {
        EMIT_FSM(LOADER_FSM_ID, EV_force_stay_in_loader, NULL, 0);
        return;
    }
    /* 检查固件备份区是否有固件 */
    if (check_apl_backup_ready(SYS_BACKUP_PART))
    {
        EMIT_FSM(LOADER_FSM_ID, EV_backup_fw_ready, NULL, 0);
        return;
    }
    /* 强制留在loader中 */
    if (p_boot_cfg->need_in_loader_flag != 0)
    {
        //有备份域的情况下，清升级标志，以便升级失败的情况下，不会再进入升级。
				p_boot_cfg->need_in_loader_flag = 0;
				save_boot_cfg(p_boot_cfg);
        
        EMIT_FSM(LOADER_FSM_ID, EV_in_loader_by_apl, NULL, 0);
        return;
    }
    /* 检查Apl区域是否存在固件 并且固件是否正确 */
    if (!check_apl_is_ok(g_ptr_loader_program_info->jump_part))
    {
				change_apl_def_version();
        EMIT_FSM(LOADER_FSM_ID, EV_apl_err, NULL, 0);
        return;
    }
    g_ptr_loader_program_info->timestamp = get_sys_ms();
}

static void ld_s_check_exe(void)
{
    boot_cfg_t *p_boot_cfg = &(g_ptr_loader_program_info->boot_cfg);
    DBG_I_LIMIT(1000, "flag_wait_time_out");
    if (check_time_out(g_ptr_loader_program_info->timestamp, p_boot_cfg->boot_delay))
    {
        EMIT_FSM(LOADER_FSM_ID, EV_stat_timeout, NULL, 0);
    }
}

static uint32_t ld_s_check_cond(uint32_t event, uint8_t *pbuf, uint32_t len)
{
    uint32_t next_stat = LD_S_NONE;
    DBG_I("1:ld_s_check_cond evt:%d", event);

    switch (event)
    {
    case EV_force_stay_in_loader:
    case EV_boot_param_err:
    case EV_in_loader_by_apl:
    case EV_apl_err:
    case EV_backup_not_exist:
    case EV_update_exit:
        DBG_I("wait for apl update.");
        next_stat = LD_S_UPDATE_IDLE;
        break;

    case EV_fw_dl_finish:
    case EV_update_success:
        next_stat = LD_S_CHECK;
        break;

    case EV_backup_fw_ready:
        next_stat = LD_S_UPDATE_APL;
        break;

    case EV_stat_timeout:
        next_stat = LD_S_GOTO_APL;
        break;

    default:
        break;
    }

    return next_stat;
}

static void ld_s_idle_in(void)
{
    g_ptr_loader_program_info->timestamp = get_sys_ms();
}

static void ld_s_idle_exe(void)
{
    if (check_time_out(g_ptr_loader_program_info->timestamp, (10 * 1000)) == true)
    {
        EMIT_FSM(LOADER_FSM_ID, EV_stat_timeout, NULL, 0);
    }
}

static uint32_t ld_s_idle_cond(uint32_t event, uint8_t *pbuf, uint32_t len)
{
    uint32_t next_stat = LD_S_NONE;

    switch (event)
    {
    case EV_stat_timeout:
        next_stat = LD_S_CHECK;
        break;
    case EV_recv_fw_data:
        next_stat = LD_S_UPDATE_TRANS;
        break;

    default:
        break;
    }
    return next_stat;
}

static void ld_s_trans_in(void)
{
    g_ptr_loader_program_info->timestamp = get_sys_ms();
}

static void ld_s_trans_exe(void)
{
    if (check_time_out(g_ptr_loader_program_info->timestamp, (30 * 1000)) == true)
    {
        EMIT_FSM(LOADER_FSM_ID, EV_stat_timeout, NULL, 0);
    }
}

static uint32_t ld_s_trans_cond(uint32_t event, uint8_t *pbuf, uint32_t len)
{
    uint32_t next_stat = LD_S_NONE;
    switch (event)
    {
    case EV_stat_timeout:
        next_stat = LD_S_CHECK;
        break;
    case EV_recv_fw_data:
        g_ptr_loader_program_info->timestamp = get_sys_ms();
        break;
    case EV_fw_dl_finish:
        next_stat = LD_S_CHECK;
        break;
    default:
        break;
    }
    return next_stat;
}

static void ld_s_goto_app_in(void)
{
    // g_loader_module.flag_wait_time_out = 0;
    g_ptr_loader_program_info->timestamp = get_sys_ms();
    DBG_I("ld_s_goto_app_in");
}

static void ld_s_goto_app_exe(void)
{
    uint32_t addr = 0;
    part_item_t *p_app_part = NULL;

    //跳转到APP的命令 jump_app
    (void)addr;
    p_app_part = get_flash_item_by_part_id(g_ptr_loader_program_info->jump_part);
    if (p_app_part == NULL)
    {
        DBG_E("flash part app is not exist");
        return;
    }
    /* app信息块大小，默认1K
     * 信息块大小必须对齐到向量表边界
     */
    const uint32_t app_info_block_size = 1024;
    addr = p_app_part->part_address + app_info_block_size;
    DBG_I("jump_app,and never come back! 0x%x", addr);
	
	xh_sys_jump_to(gx_xh_sys_id,addr);//-mark xtemp 添加驱动类型
}

static uint32_t ld_s_goto_app_cond(uint32_t event, uint8_t *pbuf, uint32_t len)
{
    uint32_t next_stat = LD_S_NONE;
    return next_stat;
}

static void ld_s_update_app_in(void)
{
    if (!check_part_is_regist(SYS_BACKUP_PART))
    {
        DBG_E("flash part app is not exist");
        EMIT_FSM(LOADER_FSM_ID, EV_backup_not_exist, NULL, 0);
        return;
    }
    //获取固件大小、更新区域等信息
    app_info_t app_info = {0};
    app_tail_info_t tail_info = {0};

    flash_part_read(g_ptr_loader_program_info->download_part, 0, (uint8_t *)&app_info, sizeof(app_info));
    g_ptr_loader_program_info->fw_size = app_info.app_info_block_len + app_info.app_len;

    flash_part_read(g_ptr_loader_program_info->download_part, g_ptr_loader_program_info->fw_size - sizeof(tail_info), (uint8_t *)&tail_info, sizeof(tail_info));

    if (g_ptr_loader_program_info->fw_size > flash_part_get_part_size(SYS_APP_PART))
    {
        //固件太大
        DBG_E("part:%d no enough space to program", SYS_APP_PART);
        broken_backup_part_data();
        EMIT_FSM(LOADER_FSM_ID, EV_update_exit, NULL, 0);
        return;
    }

    g_ptr_loader_program_info->erase_addr = 0;
    g_ptr_loader_program_info->program_addr = 0;
}

static void ld_s_update_app_exe(void)
{
    /* 读取程序信息 */
    uint32_t sector_size = flash_part_get_sector_size(SYS_APP_PART);
    uint32_t part_size = flash_part_get_part_size(SYS_APP_PART);
    if (g_ptr_loader_program_info->erase_addr < part_size)
    {
        flash_part_erase_len(SYS_APP_PART, g_ptr_loader_program_info->erase_addr, sector_size);
        g_ptr_loader_program_info->erase_addr += sector_size;
    }

    /* 搬运固件 */
    if (g_ptr_loader_program_info->program_addr < g_ptr_loader_program_info->fw_size )
    {
        uint8_t pbuf[512];
        uint32_t program_len = g_ptr_loader_program_info->fw_size - g_ptr_loader_program_info->program_addr;
        program_len = X_FRAME_MIN(program_len, sizeof(pbuf));

        if (flash_part_read(g_ptr_loader_program_info->download_part, g_ptr_loader_program_info->program_addr, pbuf, program_len))
        {
            flash_part_write(SYS_APP_PART, g_ptr_loader_program_info->program_addr, pbuf, program_len);
            g_ptr_loader_program_info->program_addr += program_len;
        }
        else
        {
            DBG_E("back up part:, addr:0x%04x read len:%d fail", g_ptr_loader_program_info->download_part, g_ptr_loader_program_info->program_addr, program_len);
        }
    }
    else
    {
        //更新完成，校验完整性
        if (check_apl_is_ok(SYS_APP_PART))
        {
            broken_backup_part_data();
            EMIT_FSM(LOADER_FSM_ID, EV_update_success, NULL, 0);
        }
        else
        {
            EMIT_FSM(LOADER_FSM_ID, EV_update_exit, NULL, 0);
        }
    }
}
static uint32_t ld_s_update_app_cond(uint32_t event, uint8_t *pbuf, uint32_t len)
{
    uint32_t next_stat = LD_S_NONE;
    switch (event)
    {
    case EV_update_success:
        next_stat = LD_S_GOTO_APL;
        break;
    case EV_backup_not_exist:
    case EV_update_exit:
        next_stat = LD_S_CHECK;
        break;
    default:
        break;
    }
    return next_stat;
}

static bool check_boot_cfg_is_ok(boot_cfg_t *p_boot_cfg)
{
    uint32_t check_val = 0;
    uint32_t start_addr = (uint32_t)p_boot_cfg;
    uint32_t end_addr = (uint32_t)start_addr + (sizeof(boot_cfg_t) - sizeof(p_boot_cfg->boot_cfg_chk_val));
    part_item_t *p_boot_cfg_part = NULL;
    p_boot_cfg_part = get_flash_item_by_part_id(SYS_BOOT_CFG_PART);
    if (p_boot_cfg_part == NULL)
    {
        DBG_E("flash part boot cfg is not exist");
        return false;
    }
    check_val = calc_chk_val(u_chk_sum, (uint8_t *)start_addr, (end_addr - start_addr));
    if ((check_val != p_boot_cfg->boot_cfg_chk_val) || (p_boot_cfg->boot_param_len != sizeof(boot_cfg_t)))
    {
        DBG_I("real_chk:0x%x   boot_chk:0x%x", check_val, p_boot_cfg->boot_cfg_chk_val);
        return false;
    }
    return true;
}

static void reset_and_save_boot_cfg(boot_cfg_t *p_boot_cfg)
{
    part_item_t *p_boot_cfg_part = NULL;
    part_item_t *p_app_part = NULL;
    p_boot_cfg_part = get_flash_item_by_part_id(SYS_BOOT_CFG_PART);
    if (p_boot_cfg_part == NULL)
    {
        DBG_E("flash part boot cfg  is not exist");
        return;
    }
    p_app_part = get_flash_item_by_part_id(SYS_APP_PART);
    if (p_app_part == NULL)
    {
        DBG_E("flash part app  is not exist");
        return;
    }

    memset(p_boot_cfg, 0, sizeof(boot_cfg_t));
    p_boot_cfg->boot_param_len = sizeof(boot_cfg_t);
    p_boot_cfg->need_in_loader_flag = 0x1;
    p_boot_cfg->boot_addr = p_app_part->part_address + APP_ENTRY_OFFSET;
    p_boot_cfg->fw_addr = p_app_part->part_address;
    p_boot_cfg->boot_delay = DEF_BOOT_DELAY;
    p_boot_cfg->loader_ver = X_MAKE_VERSION(g_boot_main_ver, g_boot_sub_ver, g_boot_dbg_ver, g_boot_test_ver);
    save_boot_cfg(p_boot_cfg);
    DBG_I("reset_boot_cfg");
}

/**
 * @brief 看门狗复位判断
 *
 * @return true : 看门狗引起复位
 * @return false ： 其他原因引起复位
 */
static bool loader_check_reset_by_iwdg(void)
{
    //todo：要判断在哪个应用中
    //获取看门狗超时复位状态（判断APP是否异常死机。异常死机，则强制进入升级，防止APP死机情况下无法升级）。
    return (NULL != gx_xh_iwdg_id && xh_wdg_get_reset(gx_xh_iwdg_id)) ? true : false;
}

/**
 * 检查备份域内容，是否完整有效
 */
static bool check_apl_backup_ready(uint8_t backup_part)
{
    return check_fw_part_data_valid(backup_part);
}

/**
 * 检查存储固件的分区，固件内容是否完整有效
 */
bool check_fw_part_data_valid(uint8_t part_id)
{
    uint32_t check_val;
    uint32_t apl_start_addr;
    part_item_t *p_fw_part;
    app_info_t app_info;

    p_fw_part = get_flash_item_by_part_id(part_id);
    if (p_fw_part == NULL)
    {
        DBG_E("flash part: %d is not exist", p_fw_part);
        return false;
    }

    flash_part_read(part_id, 0, (uint8_t *)&app_info, sizeof(app_info));
    if (0xFFFFFFFF == app_info.app_info_chk_val)
    {
        DBG_I("flash part have been erased.");
        return false;
    }

    if (app_info.app_info_chk_type > APP_CHK_TYPE_CRC32)
    {
        DBG_E("app_info_check_type is err");
        return false;
    }
    if (app_info.app_chk_type > APP_CHK_TYPE_CRC32)
    {
        DBG_E("app_check_type      is err");
        return false;
    }
    if ((app_info.app_info_block_len) < sizeof(app_info_t))
    {
        DBG_E("app_info_BLOCK_size       is err");
        return false;
    }
    if ((app_info.app_info_real_len) != sizeof(app_info_t))
    {
        DBG_E("app_info_size       is err");
        return false;
    }
    // 1 - check apl-info
    check_val = calc_chk_val(app_info.app_info_chk_type, (uint8_t *)&app_info.app_info_ver, sizeof(app_info_t) - 4);
    if (check_val != app_info.app_info_chk_val)
    {
        DBG_E("app_info           is err[calc:_val:0x%x real:0x%x] size:%d", check_val, app_info.app_info_chk_val, sizeof(app_info_t) - 4);
        return false;
    }
    // 2-check apl-bin
    apl_start_addr = app_info.app_info_block_len;
    check_val = apl_fw_chk_calc(part_id, apl_start_addr, app_info.app_chk_type, app_info.app_len);
    if (check_val != app_info.app_chk_val)
    {
        DBG_E("app_bin           is err[calc:chk:0x%x rec_chk_val:0x%x]", check_val, app_info.app_chk_val);
        return false;
    }

    return true;
}

/**
 * 计算分区中，从指定偏移开始，指定长度数据的校验码。
 */
uint32_t apl_fw_chk_calc(int part_id, uint32_t begin, uint8_t chk_type, uint32_t len)
{
    uint32_t chk_val = 0;
    uint8_t flash_buff[512];
    uint32_t offset = begin;
    uint32_t end_offset = offset + len;
    while (offset < end_offset)
    {
        uint32_t read_size = end_offset - offset;
        read_size = X_FRAME_MIN(read_size, sizeof(flash_buff));
        flash_part_read(part_id, offset, flash_buff, read_size);
        chk_val = calc_chk_init_val(chk_type, chk_val, flash_buff, read_size);
        offset += read_size;
    }

    return chk_val;
}

/**
 * 检查apl分区，数据是否完整有效
 */
static bool check_apl_is_ok(uint8_t apl_part)
{
    return check_fw_part_data_valid(apl_part);
}
/**
 * 破坏备份域内容
 */
void broken_backup_part_data(void)
{
    //擦除备份区域的一页，破坏完整性。
    uint32_t sector_size = flash_part_get_sector_size(SYS_BACKUP_PART);
    flash_part_erase_len(SYS_BACKUP_PART, 0, sector_size);
}

/**
 * 固件下载回调处理
 */
int upgrade_event_notify(P_FW_DL_OBJ dl_obj, uint32_t ev, void *param)
{
    switch (ev)
    {
    case UPDATE_EV_PREPARE:
    case UPDATE_EV_TRANS_START:
    case UPDATE_EV_TRANS_DATA:
        EMIT_FSM(LOADER_FSM_ID, EV_recv_fw_data, NULL, 0);
        break;
    case UPDATE_EV_TRANS_END:
        EMIT_FSM(LOADER_FSM_ID, EV_fw_dl_finish, NULL, 0);
        break;
    case UPDATE_EV_REBOOT:
    {
        boot_cfg_t boot_cfg = {0};
        load_boot_cfg(&boot_cfg);
        if (boot_cfg.need_in_loader_flag)
        {
            boot_cfg.need_in_loader_flag = 0;
            save_boot_cfg(&boot_cfg);
        }
    }
    break;
    }

    return 0;
}
static void legalize_part(void)
{
    uint8_t ret;
    ret = check_part_is_regist(SYS_BACKUP_PART);
    if (ret)
    {
		g_loader_program_info.download_part = SYS_BACKUP_PART;
        g_fw_dl_info.download_part = SYS_BACKUP_PART;
    }
    else
    {
		g_loader_program_info.download_part = SYS_APP_PART;
        g_fw_dl_info.download_part = SYS_APP_PART;
    }
		/* 指定跳转区为SYS_APP_PART */
    g_loader_program_info.jump_part = SYS_APP_PART;
}
static void set_hw_version_func(void)
{ 
		legalize_part();		
    g_fw_dl_info.program_type = PROGRAM_LOADER;
    hw_info_t hw_info = {0};
    uint16_t chk_val = 0;
    uint32_t loader_ver = X_MAKE_VERSION(LOADER_MAIN_VER, LOADER_SUB_VER, LOADER_DBG_VER, LOADER_TEST_VER);
    flash_part_read(SYS_HW_VER_PART, 0, (uint8_t *)&hw_info, sizeof(hw_info_t));
    chk_val = calc_chk_val(u_chk_crc16, (uint8_t *)&hw_info + 2, sizeof(hw_info_t) - 2);
    if ((chk_val != hw_info.crc_chk_val) || (hw_info.loader_ver != loader_ver))
    {
        hw_info.loader_ver = loader_ver;
        hw_info.crc_chk_val = calc_chk_val(u_chk_crc16, (uint8_t *)&hw_info + 2, sizeof(hw_info_t) - 2);
        flash_part_erase(SYS_HW_VER_PART);
        flash_part_write(SYS_HW_VER_PART, 0, (uint8_t *)&hw_info, sizeof(hw_info_t));
    }
}

REG_INIT_MOD_BASIC(set_hw_version_func);

/**
 * @brief 检查升级条件是否允许继续升级
 *
 * @param [in] p_frame_info
 *
 * @return int 0 表示OK 其他为错误码
 * @retval
 *
 * @details 特殊说明:
 *          检查项目：
 *          1.检查prepare数据包是否正确
 *          2.检查是否强制升级
 *          3.检查产品大类是否一致
 *          4.检查产品细类是否一致
 *          5.检查硬件版本是否一致
 *          如果存在一项不合格则无法进行下一步升级
 */
static int fw_update_prepare_check(hal_frame_info_t *p_frame_info)
{
    int err;
    update_prepare_t update_dat = {0};
    /* 检查数据包是否正确 */
    if (p_frame_info->data_len < sizeof(update_prepare_t))
    {
        return update_sta_param_err;
    }
    memcpy(&update_dat, p_frame_info->pdata, sizeof(update_prepare_t));
    DBG_I("0x%x, 0x%x, 0x%x, 0x%x, 0x%x", update_dat.forced_upgrade_cmd, update_dat.product_type,
          update_dat.product_detail, update_dat.hw_main_ver, update_dat.hw_sub_ver);
    /* 是否强制升级 */
    if (update_dat.forced_upgrade_cmd > 0)
    {
        return update_sta_ok;
    }
		/* 检查产品大类是否一致 */
    if (update_dat.product_type != g_product_type)
    {
        return update_sta_product_type_err;
    }
    /* 检查产品细类是否一致 */
    if (update_dat.product_detail != g_product_detail)
    {
        return update_sta_product_detail_err;
    }
    /* 检查产品硬件版本是否一致 */
    hw_info_t hw_info;
    err = get_hw_version_func(&hw_info);
    /* part中无数据则说明只有loader程序，允许升级 */
    if (err == 0)
    {
        return update_sta_ok;
    }
    if ((update_dat.hw_main_ver != hw_info.app_main_ver || update_dat.hw_sub_ver != hw_info.app_sub_ver))
    {
        return update_sta_hw_version_err;
    }

    return update_sta_ok;
}
/**
 * @brief 固件更新准备指令
 *
 * @param [in] p_frame_info
 *
 * @return int32_t
 * @retval
 *
 * @details 特殊说明:
 */
int32_t fw_update_prepare(hal_frame_info_t *p_frame_info)
{
    boot_cfg_t boot_cfg;
    update_prepare_ack_t update_ack = {0};
    update_ack.sta = fw_update_prepare_check(p_frame_info);
    /* 不满足升级条件，升级失败退出 */
    if (update_sta_ok != update_ack.sta)
    {
        block_ms(10); 
        protocol_send_ack(p_frame_info, (uint8_t *)&update_ack, sizeof(update_prepare_ack_t));
        return 0;
    }
    /* 满足升级条件回复相关信息 */
    load_boot_cfg(&boot_cfg);
    update_ack.trans_per_pack = MAX_TRANS_PACK_LEN;
    update_ack.loader_ver = boot_cfg.loader_ver;
    update_ack.loader_support_product_type = g_product_type;
    update_ack.loader_support_product_detail = g_product_detail;
    block_ms(10); 
    protocol_send_ack(p_frame_info, (uint8_t *)&update_ack, sizeof(update_prepare_ack_t));

    return 0;
}
/**
 * @brief 检查下发参数是否正确满足升级
 *
 * @param [in] p_frame_info
 *
 * @return int32_t
 * @retval
 *
 * @details 特殊说明:
 *          检查项目：
 *          1.检查下发参数数据是否正确
 *          2.检查实际每数据包大小
 *          3.检查总包数是否合法
 *          4.检查固件总大小是否超过存储区
 *          如果存在一项不合格则无法进行下一步升级
 */
int32_t fw_update_cfg_check(hal_frame_info_t *p_frame_info)
{
    update_trans_cfg_t *p_update_cfg = NULL;
    /* 检查下发数据是否正确 */
    if (p_frame_info->data_len < sizeof(update_trans_cfg_t))
    {
        DBG_E("p_frame_info->data_len:%d  recv:%d", sizeof(update_trans_cfg_t), p_frame_info->data_len);
        return update_sta_param_err;
    }
    p_update_cfg = (update_trans_cfg_t *)p_frame_info->pdata;
    /* 检查实际数据包大小 */
    g_fw_dl_info.trans_per_pack = p_update_cfg->trans_per_block;
    if (g_fw_dl_info.trans_per_pack > MAX_TRANS_PACK_LEN)
    {
        return update_sta_param_err;
    }
    /* 检查总包数 */
    g_fw_dl_info.total_block_num = p_update_cfg->total_blocks;
    if (g_fw_dl_info.total_block_num == 0)
    {
        return update_sta_info_lose;
    }
    /* 总包大小 */
    g_fw_dl_info.fw_size = p_update_cfg->fw_size;
    part_item_t *p_backup_part;
    p_backup_part = get_flash_item_by_part_id(g_fw_dl_info.download_part);
    /* ENC固件包大小。此ENC长度包含 app_header的1K
     * 是否超过固件下载区域大小
     */
    if (g_fw_dl_info.fw_size > p_backup_part->part_size)
    {
        return update_sta_fw_size_too_large;
    }
    /* ENC的和校验 */
    g_fw_dl_info.fw_sum_chk_val = p_update_cfg->fw_sum_chk_val;
    /* 信息打印 */
    DBG_I("trans_per_block:%d", p_update_cfg->trans_per_block);
    DBG_I("total_blocks :%d", p_update_cfg->total_blocks);
    DBG_I("fw_file_size :0x%x", p_update_cfg->fw_size);
    DBG_I("fw_file_chk  :0x%x", p_update_cfg->fw_sum_chk_val);
    /* 升级状态标志 */
    g_fw_dl_info.upload_statu = 1;

    return update_sta_ok;
}
/**
 * @brief 升级信息下发
 *  是否需要优化此处
 * @param [in] p_frame_info
 *
 * @return int32_t
 * @retval
 *
 * @details 特殊说明:
 */
int32_t fw_update_trans_cfg(hal_frame_info_t *p_frame_info)
{
    update_trans_cfg_ack_t update_ack = {0};
    update_ack.sta = update_sta_ok;
    update_ack.sta = fw_update_cfg_check(p_frame_info);
    if (update_ack.sta != update_sta_ok)
    {
        block_ms(10); 
        protocol_send_ack(p_frame_info, (uint8_t *)&update_ack, sizeof(update_trans_cfg_ack_t));
        return 0;
    }
    /* APL中触发升级，并且为注册备份区,只有这种情况发直接跳转 */
    if (NULL != gx_xh_sys_id)  //问一下为什么使用cpuid做sn
    {
        xh_sys_get_cpuid(gx_xh_sys_id, update_ack.sn, CPUID_BYTES);
    }
    else
    {
        DBG_E("unknow sys_id. please use DEF_XF_XH_SYS_ID() to define");
    }
    update_ack.sn_len = CPUID_BYTES;
    block_ms(10); 
    protocol_send_ack(p_frame_info, (uint8_t *)&update_ack, sizeof(update_trans_cfg_ack_t));
    if (update_ack.sta == update_sta_ok)
    {
        g_fw_dl_info.recv_block_num = 0; //每次重发配置信息，则从头开始接收。应对上位机升级到一半挂掉的情形
        g_fw_dl_info.erase_addr = 0;
    }
    return 0;
}
/**
 * @brief 固件数据下发
 *
 * @param [in] p_frame_info
 *
 * @return int32_t
 * @retval
 *
 * @details 特殊说明:
 */
int32_t fw_update_trans_data(hal_frame_info_t *p_frame_info)
{
    update_trans_ack_t update_trans_ack = {0};
    uint16_t cur_recv_block;
    uint16_t fw_data_len;
    uint32_t wr_addr;
    uint8_t *p_fw_data;
    memcpy(&cur_recv_block, p_frame_info->pdata, 2);
    p_fw_data = p_frame_info->pdata + 2;
    fw_data_len = p_frame_info->data_len - 2;
    update_trans_ack.sta = update_sta_ok;
    if (g_fw_dl_info.recv_block_num < cur_recv_block)
    {
        if (cur_recv_block == 1)
        {
            part_item_t *p_backup_part;
            p_backup_part = get_flash_item_by_part_id(g_fw_dl_info.download_part);
            if (p_backup_part == NULL)
            {
                DBG_E("flash part app is not exist");
                update_trans_ack.sta = update_sta_flash_is_not_define;
            }
            else
            {
                upgrade_event_notify(&g_fw_dl_info, UPDATE_EV_TRANS_START, NULL);
            }
        }
        if (update_sta_ok == update_trans_ack.sta)
        {
            /*边擦边写。注意：每次传输的数据量必须小于页大小，否则将会擦除已更新的部分，造成错误。 */
            int32_t erase_bytes = flash_part_erase_one_page(g_fw_dl_info.download_part, g_fw_dl_info.erase_addr);
            if (erase_bytes > 0)
                g_fw_dl_info.erase_addr += erase_bytes;

            wr_addr = ((cur_recv_block - 1) * g_fw_dl_info.trans_per_pack);
            flash_part_write(g_fw_dl_info.download_part, wr_addr, p_fw_data, fw_data_len);
            update_trans_ack.sta = update_sta_ok;
            g_fw_dl_info.recv_block_num = update_trans_ack.recv_cur_block = cur_recv_block;
            upgrade_event_notify(&g_fw_dl_info, UPDATE_EV_TRANS_DATA, NULL);
        }
    }
    else
    {
        update_trans_ack.sta = update_sta_cur_block_is_been_write;
        update_trans_ack.recv_cur_block = cur_recv_block;
    }
    protocol_send_ack(p_frame_info, (uint8_t *)&update_trans_ack, sizeof(update_trans_ack_t));
    return 0;
}
/**
 * @brief 升级数据包check
 *  跳转失败 异常回复
 * @param [in] p_frame_info
 *
 * @return int32_t
 * @retval
 *
 * @details 特殊说明:
 */
int32_t fw_update_check(hal_frame_info_t *p_frame_info)
{
    int8_t update_ack = update_sta_ok;
    boot_cfg_t p_boot_cfg;
    /* 只会在升级完成后的APL中进行升级完成应答*/
    uint32_t calc_checksum;
    calc_checksum = fw_chk_calc(g_fw_dl_info.download_part, u_chk_sum, g_fw_dl_info.fw_size);
    if (calc_checksum != g_fw_dl_info.fw_sum_chk_val)
    {
        DBG_E("err chk:0x%x   recv_chk:0x%x", calc_checksum, g_fw_dl_info.fw_sum_chk_val);
        update_ack = update_sta_check_err;
    }
    else
    {
        DBG_E("download success!!");
        load_boot_cfg(&p_boot_cfg);
        p_boot_cfg.respon_seccess = 1;
        save_boot_cfg(&p_boot_cfg);
    }
    block_ms(10); 
	protocol_send_ack(p_frame_info, (uint8_t *)&update_ack, sizeof(int8_t));
    upgrade_event_notify(&g_fw_dl_info, UPDATE_EV_TRANS_END, NULL);
    if (update_sta_ok == update_ack)
    {
        upgrade_event_notify(&g_fw_dl_info, UPDATE_EV_REBOOT, NULL);
    }

    return 0;
}
#endif
#if PROGRAM_TYPE == PROGRAM_APL
static void set_hw_version_func(void)
{
    uint8_t ret;
    ret = check_part_is_regist(SYS_BACKUP_PART);
    if (ret)
    {
        g_fw_dl_info.download_part = SYS_BACKUP_PART;
    }
    else
    {
        g_fw_dl_info.download_part = SYS_APP_PART;
    }

    g_fw_dl_info.program_type = PROGRAM_LOADER;
    hw_info_t hw_info = {0};

    flash_part_read(SYS_HW_VER_PART, 0, (uint8_t *)&hw_info, sizeof(hw_info_t));
		hw_info.app_main_ver = APP_MAIN_VER;
		hw_info.app_sub_ver = APP_SUB_VER;
		hw_info.app_dbg_ver = APP_DBG_VER;
		hw_info.app_test_ver = APP_TEST_VER;

		hw_info.crc_chk_val = calc_chk_val(u_chk_crc16, (uint8_t *)&hw_info + 2, sizeof(hw_info_t) - 2);
		flash_part_erase(SYS_HW_VER_PART);
		flash_part_write(SYS_HW_VER_PART, 0, (uint8_t *)&hw_info, sizeof(hw_info_t));

}

REG_INIT_MOD_BASIC(set_hw_version_func);
/**
 * @brief 检查升级条件是否允许继续升级
 *
 * @param [in] p_frame_info
 *
 * @return int 0 表示OK 其他为错误码
 * @retval
 *
 * @details 特殊说明:
 *          检查项目：
 *          2.检查prepare数据包是否正确
 *          3.检查是否强制升级
 *          4.检查产品大类是否一致
 *          5.检查产品细类是否一致
 *          6.检查硬件版本是否一致
 *          如果存在一项不合格则无法进行下一步升级
 */
static int fw_update_prepare_check(hal_frame_info_t *p_frame_info)
{
    int err = update_sta_ok;
    update_prepare_t update_dat = {0};
    /* 检查数据包是否正确 */
    if (p_frame_info->data_len < sizeof(update_prepare_t))
    {
        return update_sta_param_err;
    }
    memcpy(&update_dat, p_frame_info->pdata, sizeof(update_prepare_t));
    DBG_I("0x%x, 0x%x, 0x%x, 0x%x, 0x%x", update_dat.forced_upgrade_cmd, update_dat.product_type,
          update_dat.product_detail, update_dat.hw_main_ver, update_dat.hw_sub_ver);
    /* 是否强制升级 */
    if (update_dat.forced_upgrade_cmd > 0)
    {
        return update_sta_ok;
    }
		/* 检查产品大类是否一致 */
    if (update_dat.product_type != g_product_type)
    {
        return update_sta_product_type_err;
    }
    /* 检查产品细类是否一致 */
    if (update_dat.product_detail != g_product_detail)
    {
        return update_sta_product_detail_err;
    }
    /* 检查产品硬件版本是否一致 */
    if (update_dat.hw_main_ver != APP_MAIN_VER || update_dat.hw_sub_ver != APP_SUB_VER)
    {
        return update_sta_hw_version_err;
    }
    
    return err;
}
/**
 * @brief 固件更新准备指令
 *
 * @param [in] p_frame_info
 *
 * @return int32_t
 * @retval
 *
 * @details 特殊说明:
 */
int32_t fw_update_prepare(hal_frame_info_t *p_frame_info)
{
	int32_t reboot = 1;
    boot_cfg_t boot_cfg;
    update_prepare_ack_t update_ack = {0};
    update_ack.sta = fw_update_prepare_check(p_frame_info);
    /* 不满足升级条件，升级失败退出 */
    if (update_sta_ok != update_ack.sta)
    {
        block_ms(10); 
        protocol_send_ack(p_frame_info, (uint8_t *)&update_ack, sizeof(update_prepare_ack_t));
        return 0;
    }
    /* 满足升级条件回复相关信息 */
    load_boot_cfg(&boot_cfg);
    update_ack.trans_per_pack = MAX_TRANS_PACK_LEN;
    update_ack.loader_ver = boot_cfg.loader_ver;
    update_ack.loader_support_product_type = g_product_type;
    update_ack.loader_support_product_detail = g_product_detail;
    firmware_download_event_notify(&g_fw_dl_info, UPDATE_EV_PREPARE, &reboot);
    block_ms(10); 
    protocol_send_ack(p_frame_info, (uint8_t *)&update_ack, sizeof(update_prepare_ack_t));

    return 0;
}
/**
 * @brief 检查下发参数是否正确满足升级
 *
 * @param [in] p_frame_info
 *
 * @return int32_t
 * @retval
 *
 * @details 特殊说明:
 *          检查项目：
 *          1.检查下发参数数据是否正确
 *          2.检查实际每数据包大小
 *          3.检查总包数是否合法
 *          4.检查固件总大小是否超过存储区
 *          如果存在一项不合格则无法进行下一步升级
 */
int32_t fw_update_cfg_check(hal_frame_info_t *p_frame_info)
{
    update_trans_cfg_t *p_update_cfg = NULL;
    /* 检查下发数据是否正确 */
    if (p_frame_info->data_len < sizeof(update_trans_cfg_t))
    {
        DBG_E("p_frame_info->data_len:%d  recv:%d", sizeof(update_trans_cfg_t), p_frame_info->data_len);
        return update_sta_param_err;
    }
    p_update_cfg = (update_trans_cfg_t *)p_frame_info->pdata;
    /* 检查实际数据包大小 */
    g_fw_dl_info.trans_per_pack = p_update_cfg->trans_per_block;
    if (g_fw_dl_info.trans_per_pack > MAX_TRANS_PACK_LEN)
    {
        return update_sta_param_err;
    }
    /* 检查总包数 */
    g_fw_dl_info.total_block_num = p_update_cfg->total_blocks;
    if (g_fw_dl_info.total_block_num == 0)
    {
        return update_sta_info_lose;
    }
    /* 总包大小 */
    g_fw_dl_info.fw_size = p_update_cfg->fw_size;
    part_item_t *p_backup_part;
    p_backup_part = get_flash_item_by_part_id(g_fw_dl_info.download_part);
    /* ENC固件包大小。此ENC长度包含 app_header的1K
     * 是否超过固件下载区域大小
     */
    if (g_fw_dl_info.fw_size > p_backup_part->part_size)
    {
        return update_sta_fw_size_too_large;
    }
    /* ENC的和校验 */
    g_fw_dl_info.fw_sum_chk_val = p_update_cfg->fw_sum_chk_val;
    /* 信息打印 */
    DBG_I("trans_per_block:%d", p_update_cfg->trans_per_block);
    DBG_I("total_blocks :%d", p_update_cfg->total_blocks);
    DBG_I("fw_file_size :0x%x", p_update_cfg->fw_size);
    DBG_I("fw_file_chk  :0x%x", p_update_cfg->fw_sum_chk_val);
    /* 升级状态标志 */
    g_fw_dl_info.upload_statu = 1;

    return update_sta_ok;
}
/**
 * @brief 升级信息下发
 *  是否需要优化此处
 * @param [in] p_frame_info
 *
 * @return int32_t
 * @retval
 *
 * @details 特殊说明:
 */
int32_t fw_update_trans_cfg(hal_frame_info_t *p_frame_info)
{
    update_trans_cfg_ack_t update_ack = {0};
    update_ack.sta = update_sta_ok;
    update_ack.sta = fw_update_cfg_check(p_frame_info);
    if (update_ack.sta != update_sta_ok)
    {
        block_ms(10); 
        protocol_send_ack(p_frame_info, (uint8_t *)&update_ack, sizeof(update_trans_cfg_ack_t));
        return 0;
    }
    /* APL中触发升级，并且为注册备份区,只有这种情况发直接跳转 */
    if (g_fw_dl_info.download_part != SYS_BACKUP_PART)
    {
        int32_t reboot = 1;
        /* 如果APL没有备份区域，则直接重启，此时会转入loader中 */
        boot_cfg_t g_boot_cfg;
        firmware_download_event_notify(&g_fw_dl_info, UPDATE_EV_PRE_REBOOT, NULL);
        load_boot_cfg(&g_boot_cfg);
        g_boot_cfg.need_in_loader_flag = 1;
        save_boot_cfg(&g_boot_cfg);
        firmware_download_event_notify(&g_fw_dl_info, UPDATE_EV_REBOOT, &reboot);
        if (reboot)
        {		
        //to do: 将直接复位改为跳转到loader。此时，需要知道跳哪个loader
            if(gx_xh_sys_id != NULL){
                xh_sys_reset(gx_xh_sys_id);
            }
        }
        return 0;
    }
    if (NULL != gx_xh_sys_id)
    {
        xh_sys_get_cpuid(gx_xh_sys_id, update_ack.sn, CPUID_BYTES);
    }
    else
    {
        DBG_E("unknow sys_id. please use DEF_XF_XH_SYS_ID() to define");
    }
    update_ack.sn_len = CPUID_BYTES;
    block_ms(10); 
    protocol_send_ack(p_frame_info, (uint8_t *)&update_ack, sizeof(update_trans_cfg_ack_t));
    if (update_ack.sta == update_sta_ok)
    {
        g_fw_dl_info.recv_block_num = 0; //每次重发配置信息，则从头开始接收。应对上位机升级到一半挂掉的情形
        g_fw_dl_info.erase_addr = 0;
    }
    return 0;
}
/**
 * @brief 固件数据下发
 *
 * @param [in] p_frame_info
 *
 * @return int32_t
 * @retval
 *
 * @details 特殊说明:
 */
int32_t fw_update_trans_data(hal_frame_info_t *p_frame_info)
{
    update_trans_ack_t update_trans_ack = {0};
    uint16_t cur_recv_block;
    uint16_t fw_data_len;
    uint32_t wr_addr;
    uint8_t *p_fw_data;
    memcpy(&cur_recv_block, p_frame_info->pdata, 2);
    p_fw_data = p_frame_info->pdata + 2;
    fw_data_len = p_frame_info->data_len - 2;
    update_trans_ack.sta = update_sta_ok;
    if (g_fw_dl_info.recv_block_num < cur_recv_block)
    {
        if (cur_recv_block == 1)
        {
            part_item_t *p_backup_part;
            p_backup_part = get_flash_item_by_part_id(g_fw_dl_info.download_part);
            if (p_backup_part == NULL)
            {
                DBG_E("flash part app is not exist");
                update_trans_ack.sta = update_sta_flash_is_not_define;
            }
            else
            {
                firmware_download_event_notify(&g_fw_dl_info, UPDATE_EV_TRANS_START, NULL);
            }
        }
        if (update_sta_ok == update_trans_ack.sta)
        {
            /*边擦边写。注意：每次传输的数据量必须小于页大小，否则将会擦除已更新的部分，造成错误。 */
            int32_t erase_bytes = flash_part_erase_one_page(g_fw_dl_info.download_part, g_fw_dl_info.erase_addr);
            if (erase_bytes > 0)
                g_fw_dl_info.erase_addr += erase_bytes;

            wr_addr = ((cur_recv_block - 1) * g_fw_dl_info.trans_per_pack);
            flash_part_write(g_fw_dl_info.download_part, wr_addr, p_fw_data, fw_data_len);
            update_trans_ack.sta = update_sta_ok;
            g_fw_dl_info.recv_block_num = update_trans_ack.recv_cur_block = cur_recv_block;
			firmware_download_event_notify(&g_fw_dl_info, UPDATE_EV_TRANS_DATA, NULL);
        }
    }
    else
    {
        update_trans_ack.sta = update_sta_cur_block_is_been_write;
        update_trans_ack.recv_cur_block = cur_recv_block;
    }
    block_ms(10); 
    protocol_send_ack(p_frame_info, (uint8_t *)&update_trans_ack, sizeof(update_trans_ack_t));
    return 0;
}
/**
 * @brief 升级数据包check
 *  跳转失败 异常回复
 * @param [in] p_frame_info
 *
 * @return int32_t
 * @retval
 *
 * @details 特殊说明:
 */
int32_t fw_update_check(hal_frame_info_t *p_frame_info)
{
    int8_t update_ack = update_sta_ok;
    bool reboot_flg = false;
    int32_t reboot = 1;
    uint32_t calc_checksum;
    calc_checksum = fw_chk_calc(g_fw_dl_info.download_part, u_chk_sum, g_fw_dl_info.fw_size);
    if (calc_checksum != g_fw_dl_info.fw_sum_chk_val)
    {
        DBG_E("err chk:0x%x   recv_chk:0x%x", calc_checksum, g_fw_dl_info.fw_sum_chk_val);
        update_ack = update_sta_check_err;
    }
    else
    {
        DBG_I("download success!!");
        reboot_flg = true;
        firmware_download_event_notify(&g_fw_dl_info, UPDATE_EV_PRE_REBOOT, NULL);
    }
    block_ms(10); 
    protocol_send_ack(p_frame_info, (uint8_t *)&update_ack, sizeof(int8_t));
    firmware_download_event_notify(&g_fw_dl_info, UPDATE_EV_TRANS_END, NULL);
    if (update_sta_ok == update_ack && reboot_flg)
    { 
        firmware_download_event_notify(&g_fw_dl_info, UPDATE_EV_REBOOT, &reboot);
        if (reboot)
        {
            block_ms(100); //等待一段时间，等待协议发送完成。
            // to do: 将直接复位改为跳转到loader。此时，需要知道跳哪个loader
        if(gx_xh_sys_id != NULL){
            xh_sys_reset(gx_xh_sys_id);
        }
        }
        return 0;
    }

    return 0;
}

#endif

