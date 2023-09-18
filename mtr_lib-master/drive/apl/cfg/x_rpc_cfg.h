/********************** Copyright(c)***************************
** ECOFLOW 
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               x_rpc_cfg.h
** Created by:              Chen JunTong
** Created date:            2021-07-28
** Version:                 V1.0.0
** Descriptions:            rpc 配置文件
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2021-07-28
** Last version:            V1.0.0
** Description:             rpc 配置文件
**
*************************************************************/
#ifndef x_rpc_cfg_h
#define x_rpc_cfg_h
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "x_frame.h"


typedef enum {
    RPC_ID_BEFORE_JUMP_BOOTLOADER = 0,
    RPC_ID_RESET_DEV,
    RPC_USER_DEFINE,
}X_RPC_ID_LIST;

/* USER DEFINE */

//typedef enum {
//    ... = RPC_USER_DEFINE,
//}X_RPC_USER_ID_LIST;


#ifdef __cplusplus
}
#endif
#endif /* x_rpc_cfg_h */








