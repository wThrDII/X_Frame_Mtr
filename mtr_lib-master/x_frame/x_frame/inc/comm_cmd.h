/**
 * @brief comm_cmd.c.xtemp header file
 * @file comm_cmd.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 17:1:27
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 17:1:27
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __COMM_CMD_H__
#define __COMM_CMD_H__

/**
 * @brief 注册链路更新功能
 *
 * @param [in] link_id 链路id
 *
 * @details:
 * @par e.g:
 * @code
 *      REG_LINK_UPDATA_ACTION(LINK_UART1);
 * @endcode
 */
#define REG_LINK_UPDATA_ACTION(link_id)                                               \
    REG_LINK_ACTION(link_id, COMM_CMD_SET, CMD_ID_UPDATE_PREPARE, fw_update_prepare); \
    REG_LINK_ACTION(link_id, COMM_CMD_SET, CMD_ID_UPDATE_CHECK, fw_update_check);     \
    REG_LINK_ACTION(link_id, COMM_CMD_SET, CMD_ID_UPDATE_START, fw_update_trans_cfg); \
    REG_LINK_ACTION(link_id, COMM_CMD_SET, CMD_ID_UPDATE_TRANS, fw_update_trans_data);

/**
 * @brief 注册链路应用基础信息功能
 *
 * @param [in] link_id 链路id
 *
 * @details:
 * @par e.g:
 * @code
 *      REG_LINK_APP_BASE_ACTION(LINK_UART1);
 * @endcode
 */
#define REG_LINK_APP_BASE_ACTION(link_id)                                                     \
    REG_LINK_ACTION(link_id, COMM_CMD_SET, CMD_ID_GET_GPUID, get_cpu_id_info);                \
    REG_LINK_ACTION(link_id, COMM_CMD_SET, CMD_ID_GET_SN_INFO, get_sn_info);                  \
    REG_LINK_ACTION(link_id, COMM_CMD_SET, CMD_ID_SET_SN_INFO, set_sn_info);                  \
    REG_LINK_ACTION(link_id, COMM_CMD_SET, CMD_ID_PRODUCT_INFO, action_product_info);         \
    REG_LINK_ACTION(link_id, COMM_CMD_SET, CMD_ID_SYS_PRODUCT_INFO, action_sys_product_info); \
    REG_LINK_ACTION(link_id, COMM_CMD_SET, CMD_ID_DEV_RESET, cmd_dev_reset);                  \
    REG_LINK_ACTION(link_id, COMM_CMD_SET, CMD_ID_SET_PRODUCT_NAME, set_product_name);        \
    REG_LINK_ACTION(link_id, COMM_CMD_SET, CMD_ID_GET_PRODUCT_NAME, get_product_name);        \
    REG_LINK_ACTION_CMD(link_id, COMM_CMD_SET, CMD_ID_GET_UNIX_TIME, get_unix_time);          \
    REG_LINK_ACTION_CMD(link_id, COMM_CMD_SET, CMD_ID_SET_UNIX_TIME, set_unix_time);          \
    REG_LINK_ACTION_CMD(link_id, COMM_CMD_SET, CMD_ID_SILENCE, set_silence_state);

/**
 * @brief :定义系统信息指令（0x01, 0x06）的路由规则。定义了之后，该模块收到系统信息指令后，会再将该指令路由到定义的所有模块。
 *
 * @param [in] item... 每组列表里面包含两个数据，链路id和目标地址
 *
 * @details:
 * @par e.g:
 * @code
 *
 *      DEF_SYS_INFO_ROUTE(
 *          SYS_INFO_ROUTE_ITEM(LINK_UART1, 0x01),
 *          SYS_INFO_ROUTE_ITEM(LINK_UART2, 0x02),
 *      )
 * @endcode
 */
#define DEF_SYS_INFO_ROUTE(item...)              \
    uint8_t _sys_info_action_list[][2] = {item}; \
    uint8_t _sys_info_list_cnt = sizeof(_sys_info_action_list) / sizeof(_sys_info_action_list[0]);

/**
 * @brief 设置信息路径组
 *
 * @param [in] link_id 链路id
 * @param [in] dest 目标地址
 *
 * @details:
 * @par e.g:
 * @code
 *      SYS_INFO_ROUTE_ITEM(LINK_UART1, 0x01);
 * @endcode
 */
#define SYS_INFO_ROUTE_ITEM(link_id, dest) \
    {                                      \
        link_id, dest                      \
    }


/**
 * @brief 获取boot的参数
 *
 * @param [in]  p_data        boot_cfg_t 结构体 
 *
 * @details 使用read_boot_cfg替换原有的接口
 * @par 示例:
 * @code
 *      boot_cfg_t  boot_cfg;
 *      
 *      load_boot_cfg(boot_cfg);
 * @endcode
 */
#define load_boot_cfg(p_data)   read_boot_cfg(p_data)

/**
 * @brief 获取硬件版本信息
 *
 * @param [in]  p_data        hw_info_t 结构体 
 *
 * @details 为了和read_boot_cfg，read_sn_info，read_product_name统一接口形式
 * @par 示例:
 * @code
 *      hw_info_t  hw_info;
 *      
 *      read_hw_version(hw_info);
 * @endcode
 */
#define read_hw_version(p_data)                 get_hw_version_func(p_data)

/**
 * @brief 产品信息指令开关
 *
 * @param [in]  is_enable       指令开关
 *
 * @details 
 *			设置产品信息指令使能，默认使能状态，
 *			主要用于固件升级搬运固件过程关闭版本
 *			信息获取功能，防止重复升级
 * @par 示例:
 * @code
 *      set_product_info_cmd_enable(true)
 * @endcode
 */
extern void set_product_info_cmd_enable(bool is_enable);

#define PROGRAM_APL 1
#define PROGRAM_APL_BOOT 2
#define PROGRAM_LOADER 3
#define SN_MAX_LEN 16
#define CPUID_BYTES 12
#define APP_COMP_SIZE 32

/**
 * @brief 固件类型。只能增加，不能修改
 *
 * flash 布局如下（由下至上对应flash低到高地址）：
 * /^\ -----------------
 *  |  APL
 *  |  -----------------
 *  |  APL_BOOT
 *  |  -----------------
 *  |  LOADER
 *  |  -----------------
 */
enum _FW_TYPE {
    FW_TYPE_APL = 0,      // APL类型
    FW_TYPE_APL_BOOT = 1, //固件下载程序
    FW_TYPE_LOADER = 2,   //固件更新程序
};

#define APL_TAIL_VER_V0 0
#define APL_TAIL_VER_V1 1

#define LOADER_TAIL_VER_V0 0
#define LOADER_TAIL_VER_V1 1

#pragma pack(1)
typedef struct {
    // tail_ver == 1时，为以下结构体。
    uint8_t reserve[3];          //保留，填0
    uint8_t hw_type;             //固件类型
    void *main_addr;             // main函数地址。loader将通过此地址决定放入哪个分区中。
    uint8_t chip_name[20];       //芯片名称
    uint8_t jlink_chip_name[20]; // jlink中使用的芯片名称。参看jflash
    // tail_ver == 0时，为以下结构体。
    uint16_t product_detail;  //产品细类
    uint16_t product_type;    //产品大类
    uint32_t app_version;     // apl版本。loader中填0
    uint32_t loader_ver;      // loader版本。apl中填0
    uint8_t time_str[16];     //编译时间
    uint8_t date_str[16];     //编译日期
    uint8_t module_addr;      //模块地址
    uint8_t product_name[18]; //产品名称
    uint8_t tail_ver;         //尾部信息版本，结构体变动后改变。当前为1
} app_tail_info_t;

typedef struct
{
    // tail_ver == 0时，为以下结构体。
    uint16_t product_detail;     //产品细类
    uint16_t product_type;       //产品大类
    uint32_t app_version;        // apl版本。loader中填0
    uint32_t loader_ver;         // loader版本。apl中填0
    uint8_t time_str[16];        //编译时间
    uint8_t date_str[16];        //编译日期
    uint8_t module_addr;         //模块地址
    uint8_t product_name[19];    //产品名称
    uint8_t chip_name[20];       //芯片名称
    uint8_t jlink_chip_name[20]; // jlink中使用的芯片名称。参看jflash
    uint32_t loader_size;        // loader分区大小
    uint32_t app_size;           // app分区大小
    uint32_t flash_size;         //内部flash大小
    uint8_t loader_info[19];     //
    uint8_t tail_ver;            //尾部信息版本，结构体变动后改变。当前为1
} loader_tail_info_t;
#pragma pack()

typedef struct
{
    uint16_t crc_chk_val; // crc16
    uint8_t app_main_ver;
    uint8_t app_sub_ver;
    uint8_t app_dbg_ver;
    uint8_t app_test_ver;
    uint32_t loader_ver;

} hw_info_t;

typedef struct
{
    uint32_t chk_val;
    uint8_t product_type;
    uint8_t reserved;
    uint8_t product_detail;
    uint8_t model;
    uint8_t sn[SN_MAX_LEN];
    uint8_t cpu_id[CPUID_BYTES];
} sn_info_t;

typedef struct {
    uint8_t len; // 名字长度，不包含结束符'\0'
    uint8_t name[32];
} product_name_obj;

typedef struct
{
    uint32_t chk_val;
    product_name_obj product_name;
} user_rec_info;

typedef struct
{
    uint16_t product_type;
    uint16_t product_detail;
    uint8_t app_main_ver;
    uint8_t app_sub_ver;
    uint8_t app_dbg_ver;
    uint8_t app_test_ver;
    uint32_t loader_ver;
    uint8_t comp_time[APP_COMP_SIZE];

    /* 新增 */
    uint8_t pack_main_ver;
    uint8_t pack_sub_ver;
    uint8_t pack_dbg_ver;
    uint8_t pack_test_ver;
    uint8_t lib_hal_main_ver;
    uint8_t lib_hal_sub_ver;
    uint8_t lib_hal_dbg_ver;
    uint8_t lib_hal_test_ver;
    uint8_t lib_xframe_main_ver;
    uint8_t lib_xframe_sub_ver;
    uint8_t lib_xframe_dbg_ver;
    uint8_t lib_xframe_test_ver;

} product_info;

#pragma pack(1)
typedef struct {
    uint32_t unix_time;
    int8_t time_zone; // 时区，范围-12 ~ 12
} rtc_time_t;
#pragma pack()

/* 1K 信息区域结构体，不能变。 */
typedef struct
{
    uint32_t app_info_chk_val;   // app_info检验码
    uint32_t app_info_ver;       // 固定 1
    uint32_t app_info_block_len; // 1024
    uint32_t app_info_real_len;  // app_info结构体大小,
                                 // loader的校验方式导致不能扩展结构体，只能固定216
    uint32_t app_info_chk_type;  // 1，app_info校验类型

    uint32_t app_len;      // app固件大小
    uint32_t app_chk_type; // app校验类型
    uint32_t app_chk_val;  // app校验码

    uint32_t app_enc_type;  // 加密类型
    uint32_t app_enc_key;   // 暂时没用
    uint32_t app_auth_type; // 暂时没用
    uint32_t app_auth_key;  // 暂时没用

    uint16_t productId; // 产品类型编号
    uint16_t moduleId;  // 模块编号
    uint32_t app_ver;

    uint8_t app_code_ver[32];        // 固件版本号，ascii字符串
    uint8_t app_code_time[32];       // 固件编译时间，ascii字符串
    uint8_t app_product_info[32];    // 暂时没用
    uint8_t app_product_company[32]; // EcoFlow Inc.
    uint8_t app_custom_info[32];     // 暂时没用
} app_info_t;

typedef struct
{
    uint32_t app_info_ex_chk_val;
    uint32_t app_info_ex_ver; // from 1
    uint32_t app_info_ex_real_len;
    uint8_t app_commit_id[41]; // 固件对应的commit id
    uint8_t pack_version[4];   // 固件大包版本
} app_info_ex_t;

typedef struct {
    uint32_t boot_param_len;
    uint32_t need_in_loader_flag;
    uint32_t boot_delay;
    uint32_t fw_addr;
    uint32_t boot_addr;
    uint32_t loader_ver;
    uint32_t respon_seccess;
    uint32_t boot_cfg_chk_val;
} boot_cfg_t;

#pragma pack(1)
typedef struct {
    int8_t sta;
    uint32_t trans_per_pack;
    uint16_t loader_support_product_type;
    uint16_t loader_support_product_detail;
    uint32_t loader_ver;
} update_prepare_ack_t;

typedef struct {
    uint8_t forced_upgrade_cmd : 1;
    uint8_t product_type : 7;

    uint8_t product_detail;
    uint8_t hw_main_ver;
    uint8_t hw_sub_ver;
} update_prepare_t;

typedef struct
{
    uint32_t trans_per_block;
    uint32_t total_blocks;
    uint32_t fw_size;
    uint32_t fw_sum_chk_val;
} update_trans_cfg_t;

typedef struct
{
    int8_t sta;
    uint8_t sn_len;
    uint8_t sn[20];
} update_trans_cfg_ack_t;

typedef struct
{
    int8_t sta;
    uint16_t recv_cur_block;
} update_trans_ack_t;

typedef struct
{
    uint32_t real_checksum;
} update_check_t;
#pragma pack()

struct _FW_DL_OBJ;
typedef struct _FW_DL_OBJ FW_DL_OBJ, *P_FW_DL_OBJ;

enum {
    UPDATE_EV_PREPARE,     //请求升级
    UPDATE_EV_TRANS_START, //传输开始
    UPDATE_EV_TRANS_DATA,  //传输数据
    UPDATE_EV_TRANS_END,   //传输结束
    UPDATE_EV_PRE_REBOOT,  //重启前状态
    UPDATE_EV_REBOOT,      //重启
};

/**
 * @brief 升级相关
 *
 */
struct _FW_DL_OBJ {
    uint8_t download_part;    //固件备份分区
    uint8_t program_type;     //程序类型
    uint32_t fw_size;         // ENC固件包大小。此ENC长度包含 app_header的1K
    uint32_t fw_sum_chk_val;  // ENC的和校验
    uint32_t trans_per_pack;  //实际数据包大小
    uint32_t total_block_num; //总包数
    uint32_t recv_block_num;  // 当前包
    uint32_t erase_addr;      //擦除地址。相对分区起始的地址，非绝对地址。
    uint32_t upload_statu;    //升级状态，1为升级中 0 为空闲
};

/**
 * @brief 升级错误码定义
 *
 */
enum _UPDATE_STAT {
    update_sta_ok = 0,
    update_sta_check_err = 1,               //校验错误
    update_sta_param_err = 2,               //参数长度错误
    update_sta_fw_size_too_large = 3,       //固件过大
    update_sta_cur_block_is_been_write = 4, //当前块已经被写
    update_sta_flash_is_not_define = 5,     // flash没有定义
    update_sta_boot_addr_err = 6,           //启动地址错误
    update_sta_addr_err = 7,                //地址错误
    update_sta_addr_not_exist = 8,          //地址不存在
    update_sta_product_type_err = 9,        //大类错误
    update_sta_product_detail_err = 10,     //细类错误
    update_sta_hw_version_err = 11,         //硬件版本错误
    update_sta_info_lose = 12,              //参数信息丢失
};

extern uint32_t g_sys_boot_cfg_part_id;
extern uint32_t g_sys_hw_ver_part_id;
extern uint32_t g_sys_product_part_id;
extern uint32_t g_sys_rec_part_id;
extern uint32_t g_app_main_ver;
extern uint32_t g_app_sub_ver;
extern uint32_t g_app_dbg_ver;
extern uint32_t g_app_test_ver;
extern uint32_t g_hal_drv_main_ver;
extern uint32_t g_hal_drv_sub_ver;
extern uint32_t g_hal_drv_dbg_ver;
extern uint32_t g_hal_drv_test_ver;
extern uint32_t g_product_type;
extern uint32_t g_product_detail;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif // __cplusplus

/**
 * @brief 固件升级外部用户接口
 *
 * @param [in] dl_obj  :固件升级相关信息
 * @param [in] ev      ：事件
 * @param [in] param   :void * 类型指针参数
 *
 * @return uint32_t
 * @retval 默认 return 0;，此返回值并未使用
 *
 * @details: 特殊说明:
 *           其中ev 参数为升级过程中状态信息
 *           UPDATE_EV_PREPARE,     //请求升级
 *           UPDATE_EV_TRANS_START, //传输开始
 *           UPDATE_EV_TRANS_DATA,  //传输数据
 *           UPDATE_EV_TRANS_END,   //传输结束
 *           UPDATE_EV_PRE_REBOOT,  //重启前状态
 *           UPDATE_EV_REBOOT,      //重启
 *           特别注意：
 *           UPDATE_EV_REBOOT：重启状态，此状态至位位置位于调用复位函数之前，此状态之后程序会立即复位。
 *           UPDATE_EV_PRE_REBOOT：重启前状态，重启前状态位于系统参数保存之前调用，系统参数保存之后会调取重启相关操作
 *           流程：UPDATE_EV_PRE_REBOOT --> 保存系统参数  -->  UPDATE_EV_REBOOT --> 复位
 * @par e.g: 当想在固件升级过程中某一个节点添加自己想执行的操作时，根据 ev 值进行操作即可。
 * @code
 * NOTICE;注意 param 参数防止空指针
 * 		int32_t firmware_download_event_notify(P_FW_DL_OBJ dl_obj, uint32_t ev, void *param);
 *    	{
 *          switch(ev)
 *          {
 *              case UPDATE_EV_PREPARE:
 *              case UPDATE_EV_TRANS_START:
 *              case UPDATE_EV_TRANS_DATA:
 *              case UPDATE_EV_TRANS_END:
 *              case UPDATE_EV_PRE_REBOOT:
 *              case UPDATE_EV_REBOOT:
 *                  break;
 *              default:
 *                  break;
 *          }
 *          return 0;
 *      }
 * @endcode
 */
X_FRAME_WEAK_EXT(int32_t firmware_download_event_notify(P_FW_DL_OBJ dl_obj, uint32_t ev, void *param))

/* 内部接口 */
int32_t fw_update_prepare(hal_frame_info_t *p_frame_info);
int32_t fw_update_check(hal_frame_info_t *p_frame_info);
int32_t fw_update_trans_cfg(hal_frame_info_t *p_frame_info);
int32_t fw_update_trans_data(hal_frame_info_t *p_frame_info);
bool read_app_info(app_info_t *p_app_info);
bool read_app_info_ex(app_info_ex_t *p_ex_app_info);
int32_t read_boot_cfg(boot_cfg_t *p_boot_cfg);
void save_boot_cfg(boot_cfg_t *p_boot_cfg);
int32_t get_hw_version_func(hw_info_t *p_hw_info);
int32_t get_cpu_id_info(hal_frame_info_t *p_frame_info);
int32_t read_sn_info(sn_info_t *p_sn_info);
int32_t get_sn_info(hal_frame_info_t *p_frame_info);
int32_t set_sn_info(hal_frame_info_t *p_frame_info);
int32_t action_product_info(hal_frame_info_t *p_frame_info);
int32_t loader_action_product_info(hal_frame_info_t *p_frame_info);
int32_t action_sys_product_info(hal_frame_info_t *p_frame_info);
uint16_t push_sn_info(uint8_t is_first, uint8_t link_id, uint8_t dest);
uint16_t push_product_info(uint8_t is_first, uint8_t link_id, uint8_t dest);
int32_t cmd_dev_reset(hal_frame_info_t *p_frame_info);
int32_t set_product_name(hal_frame_info_t *p_frame_info);
int32_t get_product_name(hal_frame_info_t *p_frame_info);
int32_t read_product_name(product_name_obj *p_product_name);
int32_t get_unix_time(hal_frame_info_t *p_frame_info);
int32_t set_unix_time(hal_frame_info_t *p_frame_info);
int32_t set_silence_state(hal_frame_info_t *p_frame_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif // __cplusplus

#endif // __COMM_CMD_H__
