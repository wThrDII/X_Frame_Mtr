/**
 * @brief LKS32MC45X芯片配置
 * @file chip_cfg.h
 * @version 1.0
 * @author boyu.qin
 * @date 2022-10-24 17:36:54
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-10-24 17:36:54 <td>1.0       <td>boyu.qin      <td>Created function
 */
#ifndef chip_cfg_h
#define chip_cfg_h
#ifdef __cplusplus
extern "C"
{
#endif


#if LKS32MC453RC

#define FLASH_ADDR                      (0x0000000)
#define FLASH_BLOCK_SIZE                (1024)
#define FLASH_SIZE                      (FLASH_BLOCK_SIZE * 256)
#define SYS_FLASH_SIZE                  (FLASH_SIZE)
/* 分区 */
#define SYS_LOADER_PART_SIZE            (32   *  FLASH_BLOCK_SIZE)   /* 32k */
#define SYS_APP_PART_SIZE 		        (80  *  FLASH_BLOCK_SIZE)   /*  1k app_file_info + app_bin */
#define SYS_BOOT_CFG_PART_SIZE          (2   *  FLASH_BLOCK_SIZE)   /* 2k */
#define USER_REC_PART_SIZE  	        (4   *  FLASH_BLOCK_SIZE)   /* 4k */
#define SYS_APP_CFG_PART_SIZE           (2   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_PRODUCT_PART_SIZE           (2   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_HW_VER_PART_SIZE            (2   *  FLASH_BLOCK_SIZE)   /* 2k */


#else
    #error "please check flash part"
#endif


#if LKS32MC453RC 
#define CHIP_NAME			    "LKS32MC453RC"
#define JLINK_CHIP_NAME		    "LKS32MC453RC"	//参考j-link配置
#define X_FRAME_PLATFORM 		"LKS32MC453RC platform"
#elif LKS32MC43X
#define CHIP_NAME			    "LKS32MC455RC"
#define JLINK_CHIP_NAME		    "LKS32MC455RC"	//参考j-link配置
#define X_FRAME_PLATFORM 		"LKS32MC455RC platform"
#endif

#define WATCHDOG_ENABLE 0 ///< 开启看门狗

#if !defined __IAP_ENABLE__
#define VECTOR_TAB_OFFSET 0 ///< 程序为ISP类型是偏移地址为0
#else
#define VECTOR_TAB_OFFSET (SYS_LOADER_PART_SIZE + 1024) ///< 程序为IAP类型时需偏移地址loader + 1k信息的大小
#endif

#ifdef __cplusplus
}
#endif
#endif /* chip_cfg_h */
