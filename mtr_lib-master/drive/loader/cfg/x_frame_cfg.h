/********************** Copyright(c)***************************
** ECOFLOW 
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               x_frame_cfg_m.h
** Created by:              Chen JunTong
** Created date:            2022-01-06
** Version:                 V1.0.0
** Descriptions:            框架配置模板
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2022-01-06
** Last version:            V1.0.0
** Description:             框架配置模板
**
*************************************************************/
#ifndef x_frame_cfg_m_h
#define x_frame_cfg_m_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include "chip_cfg.h"
// <<< Use Configuration Wizard in Context Menu >>>

// <h> VERSION
// <h> APP VERSION
// <i> the software app version		 
// <o0> APP_MAIN_VER
// <o1> APP_SUB_VER
// <o2> APP_DBG_VER
// <o3> APP_TEST_VER
#define APP_MAIN_VER                                1
#define APP_SUB_VER                                 0
#define APP_DBG_VER                                 0
#define APP_TEST_VER                                1
// </h>

// <h> LOADER VERSION
// <i> the hardware app version		 
// <o0> LOADER_MAIN_VER
// <o1> LOADER_SUB_VER
// <o2> LOADER_DBG_VER
// <o3> LOADER_TEST_VER
#define LOADER_MAIN_VER                             0
#define LOADER_SUB_VER                              0
#define LOADER_DBG_VER                              0
#define LOADER_TEST_VER                             1
// </h>
// </h>

#define SYS_SW_VER  		(((LOADER_MAIN_VER)<<24)|((LOADER_SUB_VER)<<16)|(LOADER_DBG_VER<<8)|(LOADER_TEST_VER))


// <h> PRODUCT INFO
// <s.19> PRODUCT_NAME 
// <o0> PRODUCT_TYPE
// <o1> PRODUCT_DETAIL
#define PRODUCT_NAME 								"AT515"//"MH100"  //"EF-NO-MODULE" 
#define PRODUCT_TYPE 								27//42
#define PRODUCT_DETAIL 								1//14
// </h>


// <h> PROGRAM
// <o0> PROGRAM_TYPE        <1=> APP  <2=> LOADER  
// <o1> HOST_ADDR
// <o2> PC_ADDR
// <q3> TINY_FUNC_CODE
// <q4> ENABLE_PROTCOL

// <e5> ENABLE_DBG
// <q6> DISABLE_DBG_I
// <q7> DEF_DBG_I_PRINT
// <q8> DISABLE_DBG_W
// <q9> DEF_DBG_W_PRINT
// <q10> DISABLE_DBG_E
// <q11> DEF_DBG_E_PRINT
// </e> 

// <e12> ENABLE_RECORDER
// <q13> DEF_REC_DAT_FLG
// </e>
// <q14> ENABLE_PARAM_CFG
// <q15> ENABLE_FSM
// <q16> ENABLE_CFM
// <q17> ENABLE_FLASH_PART
// <q18> ENABLE_RPC
// <q19> ENABLE_SHELL
// <q20> ENABLE_FSM_TAB
// <q21> ENABLE_FLASH_PARAM
// <q22> ENABLE_UNIX_CALENDAR
// <q23> ENABLE_X_LOG
// <q24> ENABLE_WATTH_STATISTICS
// <q25> ENABLE_PROTOCOL_PUSH
// <q26> ENABLE_COMM_CMD

#define PROGRAM_TYPE	                            PROGRAM_LOADER			

#define HOST_ADDR                                   0x13    //0x3A
#define PC_ADDR                                     0X01

#define TINY_FUNC_CODE          					1		/* 最小代码 */
#define ENABLE_PROTCOL  							1

#define ENABLE_DBG              					0
#define DISABLE_DBG_I	                            0
#define DEF_DBG_I_PRINT                             1   
#define DISABLE_DBG_W	                            0
#define DEF_DBG_W_PRINT                             1  
#define DISABLE_DBG_E	                            0             
#define DEF_DBG_E_PRINT                             1

#define ENABLE_RECORDER								1
#define DEF_REC_DAT_FLG 	                        1

#define ENABLE_PARAM_CFG							1
#define ENABLE_FSM									1
#define ENABLE_CFM									1
#define ENABLE_FLASH_PART							1
#define ENABLE_RPC									1
#define ENABLE_SHELL								1
#define ENABLE_FSM_TAB								1
#define ENABLE_FLASH_PARAM							1
#define ENABLE_UNIX_CALENDAR						1
#define	ENABLE_X_LOG								1
#define ENABLE_WATTH_STATISTICS						1
#define ENABLE_PROTOCOL_PUSH						1
#define ENABLE_COMM_CMD                             1

// </h>

// <h> PLATFORM INFO
// <s.20> LOADER_INFO 
#define LOADER_INFO			    "loader_info"
// </h>

// <<< end of configuration section >>>

#define ERASE_FLASH_CMD		0		/* 擦除flash指令 */
#define DRV_ID_SYS_TICK		0		/* 系统时钟计时 */

#ifdef __cplusplus
}
#endif
#endif /* x_frame_cfg_m_h */

