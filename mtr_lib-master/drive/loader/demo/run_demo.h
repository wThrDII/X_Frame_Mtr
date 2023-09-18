/********************** Copyright(c)***************************
** ECOFLOW
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               run_demo.h
** Created by:              Chen JunTong
** Created date:            2022-01-11
** Version:                 V1.0.0
** Descriptions:            运行示例
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2022-01-11
** Last version:            V1.0.0
** Description:             运行示例
**
*************************************************************/
#ifndef run_demo_h
#define run_demo_h
#ifdef __cplusplus
extern "C" {
#endif

#define ENABLE_REG_LINK_DEMO 1                                                                   ///< 注册协议链路
#define ENABLE_FLASH_PART 1                                                                      ///< 注册flash part
#define ENABLE_COMM_CMD_DEMO (1 & ENABLE_REG_LINK_DEMO)                                          ///< 通用指令集，比如获取cpuid，sn等指令
#define ENABLE_UPDATA_DEMO (1 & ENABLE_REG_LINK_DEMO & ENABLE_COMM_CMD_DEMO & ENABLE_FLASH_PART) ///< 固件更新
#define SYSINFO_ENABLE (1 & ENABLE_REG_LINK_DEMO)                                                ///< 系统信息打印
#define ENABLE_LED_RUN 0                                                                         ///< 使能运行指示灯

typedef enum {
    LINK_UART0 = 0,
    LINK_CAN0,
} LINK_ID_LIST;

#ifdef __cplusplus
}
#endif
#endif /* run_demo_h */
