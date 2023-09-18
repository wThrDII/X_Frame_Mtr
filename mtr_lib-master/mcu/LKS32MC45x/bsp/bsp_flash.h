/**
 * @brief flash驱动
 * @file bsp_flash.h
 * @version 1.0
 * @author boyu.qin 
 * @date 2023-01-11 10:36:04
 * @copyright Copyright (c) ECOFLOW 2017 - 2023. All rights reserved.
 * 
 * @details 文件详细描述，包含使用场景，依赖项等
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2023-01-11 10:36:04      <td>boyu.qin      <td>创建文件
 */

#ifndef __BSP_FLASH_H__
#define __BSP_FLASH_H__

#include "lks32mc45x_flash.h"
#include "xh_flash.h"

#define REG_XH_BSP_FLASH(dev_id, prio)                                                  \
        REG_XH_FLASH_DRV(dev_id, NULL, prio, false, NULL, NULL, bsp_flash_read, bsp_flash_write, NULL, NULL, bsp_flash_erase, bsp_flash_get_byte_len);

extern uint32_t bsp_flash_read(void *var, uint32_t addr, uint8_t *p_dest, uint32_t len); 
extern uint32_t bsp_flash_write(void *var, uint32_t addr, uint8_t *pbuf, uint32_t len);  
extern uint32_t bsp_flash_erase(void *var, uint32_t address, uint32_t len);              
extern uint32_t bsp_flash_get_byte_len(void *var);      

#endif //__BSP_FLASH_H__

