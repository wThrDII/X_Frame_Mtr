/**
 * @file loader_module.h.xtemp
 * @author jim.xiao (jim.xiao@ecoflow.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef LOADER_MODULE_H
#define LOADER_MODULE_H

#if PROGRAM_TYPE == PROGRAM_APL

#define REG_FW_BACKUP_PART_ID(backup_part_id)      \
    FW_DL_OBJ g_fw_dl_info = {                      \
        backup_part_id,                             \
        PROGRAM_TYPE,                               \
    };

#elif PROGRAM_TYPE == PROGRAM_LOADER

struct _loader_program_info;
typedef struct _loader_program_info loader_program_info_t;

struct _loader_program_info{
    uint8_t         jump_part;      //下一跳固件分区
    uint8_t         backup_part;    //备份域id
    uint32_t        erase_addr;     //擦除地址
    uint32_t        program_addr;   //编程地址
    uint32_t        fw_size;        //固件大小，包括1K区域
    uint8_t         program_part;   //编程域id
    uint32_t        timestamp;      //时间戳
    boot_cfg_t      boot_cfg;
};

/**
 * 注册固件更新模块，将检查备份域内容，有内容则更新。
 */
#define REG_FW_BACKUP_PART_ID(backup_part_id)                                  \
	FW_DL_OBJ g_fw_dl_info = {                      							\
        backup_part_id,                             							\
        PROGRAM_TYPE,                               							\
    };																			\
    loader_program_info_t g_loader_program_info = {                             \
        SYS_APP_PART,                                                           \
        backup_part_id,                                                         \
    };                                                                          \
    loader_program_info_t * g_ptr_loader_program_info = &g_loader_program_info; \

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief 固件下载事件通知
 * 
 * @param dl_obj 
 * @param ev 
 * @param param 
 * @return int 
 */
API_WEAK int firmware_download_event_notify(P_FW_DL_OBJ dl_obj, uint32_t ev, void * param);


#ifdef __cplusplus
}
#endif
#endif /* LOADER_MODULE_H */
