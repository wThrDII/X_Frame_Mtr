/********************** Copyright(c)***************************
** ECOFLOW 
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               chip_cfg.h
** Created by:              Chen JunTong
** Created date:            2022-01-06
** Version:                 V1.0.0
** Descriptions:            芯片 配置文件
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2022-01-06
** Last version:            V1.0.0
** Description:             芯片 配置文件
**
*************************************************************/
#ifndef chip_cfg_h
#define chip_cfg_h
#ifdef __cplusplus
extern "C" {
#endif

enum
{
	SYS_LOADER_PART=1,
	SYS_APP_PART,
    SYS_BOOT_CFG_PART,
	
	//--------user part
	USER_REC_PART,
	SYS_APP_CFG_PART,
	SYS_PRODUCT_PART,
	SYS_HW_VER_PART,
	SYS_FLASH_PARAM_PART,
};

#if GD32F305RB 

#define FLASH_ADDR                      (0x8000000)
#define FLASH_BLOCK_SIZE                (1024 * 2)
#define FLASH_SIZE                      (FLASH_BLOCK_SIZE * 64)
#define SYS_FLASH_SIZE                  (FLASH_SIZE)
/* 分区 */
#define SYS_LOADER_PART_SIZE            (12  *  FLASH_BLOCK_SIZE)   /* 24k */
#define SYS_APP_PART_SIZE 		        (46  *  FLASH_BLOCK_SIZE)   /* 1k app_file_info + app_bin */
#define SYS_BOOT_CFG_PART_SIZE          (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define USER_REC_PART_SIZE  	        (2   *  FLASH_BLOCK_SIZE)   /* 4k */
#define SYS_APP_CFG_PART_SIZE           (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_PRODUCT_PART_SIZE           (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_HW_VER_PART_SIZE            (1   *  FLASH_BLOCK_SIZE)   /* 2k */

#elif GD32F303CC || GD32F303RC || GD32F303VC || GD32F303ZC || GD32F305RC || GD32F305VC || GD32F305ZC || GD32F307RC || GD32F307VC || GD32F307ZC

#define FLASH_ADDR                      (0x8000000)
#define FLASH_BLOCK_SIZE                (1024 * 2)
#define FLASH_SIZE                      (FLASH_BLOCK_SIZE * 128)
#define SYS_FLASH_SIZE                  (FLASH_SIZE)
/* 分区 */
#define SYS_LOADER_PART_SIZE            (12  *  FLASH_BLOCK_SIZE)   /* 24k */
#define SYS_APP_PART_SIZE 		        (110  *  FLASH_BLOCK_SIZE)   /* 1k app_file_info + app_bin */
#define SYS_BOOT_CFG_PART_SIZE          (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define USER_REC_PART_SIZE  	        (2   *  FLASH_BLOCK_SIZE)   /* 4k */
#define SYS_APP_CFG_PART_SIZE           (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_PRODUCT_PART_SIZE           (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_HW_VER_PART_SIZE            (1   *  FLASH_BLOCK_SIZE)   /* 2k */

#elif GD32F303CE || GD32F303RE || GD32F303VE || GD32F303ZE || GD32F305RE || GD32F305VE || GD32F305ZE || GD32F307RE || GD32F307VE || GD32F307ZE

#define FLASH_ADDR                      (0x8000000)
#define FLASH_BLOCK_SIZE                (1024 * 2)
#define FLASH_SIZE                      (FLASH_BLOCK_SIZE * 256)
#define SYS_FLASH_SIZE                  (FLASH_SIZE)
/* 分区 */
#define SYS_LOADER_PART_SIZE            (12  *  FLASH_BLOCK_SIZE)   /* 24k */
#define SYS_APP_PART_SIZE 		        (238  *  FLASH_BLOCK_SIZE)   /* 1k app_file_info + app_bin */
#define SYS_BOOT_CFG_PART_SIZE          (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define USER_REC_PART_SIZE  	        (2   *  FLASH_BLOCK_SIZE)   /* 4k */
#define SYS_APP_CFG_PART_SIZE           (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_PRODUCT_PART_SIZE           (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_HW_VER_PART_SIZE            (1   *  FLASH_BLOCK_SIZE)   /* 2k */

#elif GD32F303CG || GD32F303RG || GD32F303VG || GD32F303ZG || GD32F305RG || GD32F305VG || GD32F305ZG || GD32F307RG || GD32F307VG || GD32F307ZG

#define FLASH_ADDR                      (0x8000000)
#define FLASH_BLOCK_SIZE                (1024 * 2)
#define FLASH_SIZE                      (FLASH_BLOCK_SIZE * 256)
#define SYS_FLASH_SIZE                  (FLASH_SIZE)
/* 分区 */
#define SYS_LOADER_PART_SIZE            (12  *  FLASH_BLOCK_SIZE)   /* 24k */
#define SYS_APP_PART_SIZE 		        (238  *  FLASH_BLOCK_SIZE)   /* 1k app_file_info + app_bin */
#define SYS_BOOT_CFG_PART_SIZE          (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define USER_REC_PART_SIZE  	        (2   *  FLASH_BLOCK_SIZE)   /* 4k */
#define SYS_APP_CFG_PART_SIZE           (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_PRODUCT_PART_SIZE           (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_HW_VER_PART_SIZE            (1   *  FLASH_BLOCK_SIZE)   /* 2k */

#elif GD32F303RI || GD32F303VI || GD32F303ZI

#define FLASH_ADDR                      (0x8000000)
#define FLASH_BLOCK_SIZE                (1024 * 2)
#define FLASH_SIZE                      (FLASH_BLOCK_SIZE * 256)
#define SYS_FLASH_SIZE                  (FLASH_SIZE)
/* 分区 */
#define SYS_LOADER_PART_SIZE            (12  *  FLASH_BLOCK_SIZE)   /* 24k */
#define SYS_APP_PART_SIZE 		        (238  *  FLASH_BLOCK_SIZE)   /* 1k app_file_info + app_bin */
#define SYS_BOOT_CFG_PART_SIZE          (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define USER_REC_PART_SIZE  	        (2   *  FLASH_BLOCK_SIZE)   /* 4k */
#define SYS_APP_CFG_PART_SIZE           (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_PRODUCT_PART_SIZE           (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_HW_VER_PART_SIZE            (1   *  FLASH_BLOCK_SIZE)   /* 2k */

#elif GD32F303RK || GD32F303VK || GD32F303ZK

#define FLASH_ADDR                      (0x8000000)
#define FLASH_BLOCK_SIZE                (1024 * 2)
#define FLASH_SIZE                      (FLASH_BLOCK_SIZE * 256)
#define SYS_FLASH_SIZE                  (FLASH_SIZE)
/* 分区 */
#define SYS_LOADER_PART_SIZE            (12  *  FLASH_BLOCK_SIZE)   /* 24k */
#define SYS_APP_PART_SIZE 		        (238  *  FLASH_BLOCK_SIZE)   /* 1k app_file_info + app_bin */
#define SYS_BOOT_CFG_PART_SIZE          (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define USER_REC_PART_SIZE  	        (2   *  FLASH_BLOCK_SIZE)   /* 4k */
#define SYS_APP_CFG_PART_SIZE           (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_PRODUCT_PART_SIZE           (1   *  FLASH_BLOCK_SIZE)   /* 2k */
#define SYS_HW_VER_PART_SIZE            (1   *  FLASH_BLOCK_SIZE)   /* 2k */

#else
    #error "please check flash part"
#endif




#if !defined __IAP_ENABLE__
#define VECTOR_TAB_OFFSET 0
#else
#define VECTOR_TAB_OFFSET (SYS_LOADER_PART_SIZE + 1024)
#endif

#if GD32F305RB
#define CHIP_NAME			    "GD32F305RB"
#define JLINK_CHIP_NAME		    "GD32F305RB"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F305RB platform"
#elif GD32F303CC
#define CHIP_NAME			    "GD32F303CC"
#define JLINK_CHIP_NAME		    "GD32F303CC"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303CC platform"
#elif GD32F303RC
#define CHIP_NAME			    "GD32F303RC"
#define JLINK_CHIP_NAME		    "GD32F303RC"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303RC platform"
#elif GD32F303VC
#define CHIP_NAME			    "GD32F303VC"
#define JLINK_CHIP_NAME		    "GD32F303VC"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303VC platform"
#elif GD32F303ZC
#define CHIP_NAME			    "GD32F303ZC"
#define JLINK_CHIP_NAME		    "GD32F303ZC"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303ZC platform"
#elif GD32F305RC
#define CHIP_NAME			    "GD32F305RC"
#define JLINK_CHIP_NAME		    "GD32F305RC"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F305RC platform"
#elif GD32F305VC
#define CHIP_NAME			    "GD32F305VC"
#define JLINK_CHIP_NAME		    "GD32F305VC"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F305VC platform"
#elif GD32F305ZC
#define CHIP_NAME			    "GD32F305ZC"
#define JLINK_CHIP_NAME		    "GD32F305ZC"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F305ZC platform"
#elif GD32F307RC
#define CHIP_NAME			    "GD32F307RC"
#define JLINK_CHIP_NAME		    "GD32F307RC"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F307RC platform"
#elif GD32F307VC
#define CHIP_NAME			    "GD32F307VC"
#define JLINK_CHIP_NAME		    "GD32F307VC"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F307VC platform"
#elif GD32F307ZC
#define CHIP_NAME			    "GD32F307ZC"
#define JLINK_CHIP_NAME		    "GD32F307ZC"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F307ZC platform"
#elif GD32F303CE
#define CHIP_NAME			    "GD32F303CE"
#define JLINK_CHIP_NAME		    "GD32F303CE"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303CE platform"
#elif GD32F303RE
#define CHIP_NAME			    "GD32F303RE"
#define JLINK_CHIP_NAME		    "GD32F303RE"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303RE platform"
#elif GD32F303VE
#define CHIP_NAME			    "GD32F303VE"
#define JLINK_CHIP_NAME		    "GD32F303VE"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303VE platform"
#elif GD32F303ZE
#define CHIP_NAME			    "GD32F303ZE"
#define JLINK_CHIP_NAME		    "GD32F303ZE"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303ZE platform"
#elif GD32F305RE
#define CHIP_NAME			    "GD32F305RE"
#define JLINK_CHIP_NAME		    "GD32F305RE"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F305RE platform"
#elif GD32F305VE
#define CHIP_NAME			    "GD32F305VE"
#define JLINK_CHIP_NAME		    "GD32F305VE"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F305VE platform"
#elif GD32F305ZE
#define CHIP_NAME			    "GD32F305ZE"
#define JLINK_CHIP_NAME		    "GD32F305ZE"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F305ZE platform"
#elif GD32F307RE
#define CHIP_NAME			    "GD32F307RE"
#define JLINK_CHIP_NAME		    "GD32F307RE"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F307RE platform"
#elif GD32F307VE
#define CHIP_NAME			    "GD32F307VE"
#define JLINK_CHIP_NAME		    "GD32F307VE"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F307VE platform"
#elif GD32F307ZE
#define CHIP_NAME			    "GD32F307ZE"
#define JLINK_CHIP_NAME		    "GD32F307ZE"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F307ZE platform"
#elif GD32F303CG
#define CHIP_NAME			    "GD32F303CG"
#define JLINK_CHIP_NAME		    "GD32F303CG"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303CG platform"
#elif GD32F303RG
#define CHIP_NAME			    "GD32F303RG"
#define JLINK_CHIP_NAME		    "GD32F303RG"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303RG platform"
#elif GD32F303VG
#define CHIP_NAME			    "GD32F303VG"
#define JLINK_CHIP_NAME		    "GD32F303VG"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303VG platform"
#elif GD32F303ZG
#define CHIP_NAME			    "GD32F303ZG"
#define JLINK_CHIP_NAME		    "GD32F303ZG"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303ZG platform"
#elif GD32F305RG
#define CHIP_NAME			    "GD32F305RG"
#define JLINK_CHIP_NAME		    "GD32F305RG"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F305RG platform"
#elif GD32F305VG
#define CHIP_NAME			    "GD32F305VG"
#define JLINK_CHIP_NAME		    "GD32F305VG"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F305VG platform"
#elif GD32F305ZG
#define CHIP_NAME			    "GD32F305ZG"
#define JLINK_CHIP_NAME		    "GD32F305ZG"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F305ZG platform"
#elif GD32F307RG
#define CHIP_NAME			    "GD32F307RG"
#define JLINK_CHIP_NAME		    "GD32F307RG"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F307RG platform"
#elif GD32F307VG
#define CHIP_NAME			    "GD32F307VG"
#define JLINK_CHIP_NAME		    "GD32F307VG"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F307VG platform"
#elif GD32F307ZG
#define CHIP_NAME			    "GD32F307ZG"
#define JLINK_CHIP_NAME		    "GD32F307ZG"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F307ZG platform"
#elif GD32F303RI
#define CHIP_NAME			    "GD32F303RI"
#define JLINK_CHIP_NAME		    "GD32F303RI"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303RI platform"
#elif GD32F303VI
#define CHIP_NAME			    "GD32F303VI"
#define JLINK_CHIP_NAME		    "GD32F303VI"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303VI platform"
#elif GD32F303ZI
#define CHIP_NAME			    "GD32F303ZI"
#define JLINK_CHIP_NAME		    "GD32F303ZI"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303ZI platform"
#elif GD32F303RK
#define CHIP_NAME			    "GD32F303RK"
#define JLINK_CHIP_NAME		    "GD32F303RK"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303RK platform"
#elif GD32F303VK
#define CHIP_NAME			    "GD32F303VK"
#define JLINK_CHIP_NAME		    "GD32F303VK"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303VK platform"
#elif GD32F303ZK
#define CHIP_NAME			    "GD32F303ZK"
#define JLINK_CHIP_NAME		    "GD32F303ZK"	//参考j-link配置
#define X_FRAME_PLATFORM 		"GD32F303ZK platform"
#endif


#ifdef __cplusplus
}
#endif
#endif /* chip_cfg_h */

 




