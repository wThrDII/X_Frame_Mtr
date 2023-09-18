/**
 * @brief 注册 flash part 的demo程序
 * @file flash_part_demo.c
 * @version 1.0
 * @author juntong.chen
 * @date 2022-11-14 15:47:12
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @details 文件详细描述，包含使用场景，依赖项等
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2022-11-14 15:47:12      <td>juntong.chen      <td>创建文件
 */
#include "hal.h"
#include "run_demo.h"
#include "x_frame.h"

#if ENABLE_FLASH_PART == 1
REG_FLASH_PART(DRV_FLASH, FLASH_ADDR, FLASH_SIZE, FLASH_BLOCK_SIZE, 
					{"loader", SYS_LOADER_PART, SYS_LOADER_PART_SIZE}, 
					{"app", SYS_APP_PART, SYS_APP_PART_SIZE}, /* app_file_info + app_bin + recorder */
					{"boot_cfg", SYS_BOOT_CFG_PART, SYS_BOOT_CFG_PART_SIZE},
					//---user part
					{"rec", USER_REC_PART, USER_REC_PART_SIZE},
					{"app_cfg", SYS_APP_CFG_PART, SYS_APP_CFG_PART_SIZE},
					{"product_info", SYS_PRODUCT_PART, SYS_PRODUCT_PART_SIZE},
					{"hw_info", SYS_HW_VER_PART, SYS_HW_VER_PART_SIZE}, 
//					{"bl_back", SYS_BACKUP_PART, SYS_APP_PART_SIZE}, 
                    );
#endif
