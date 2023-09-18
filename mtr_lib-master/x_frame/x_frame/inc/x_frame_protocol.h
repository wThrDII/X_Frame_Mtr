/********************** Copyright(c)***************************
** ECOFLOW 
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               x_frame_protocol.h
** Created by:              Chen JunTong
** Created date:            2021-04-23
** Version:                 V1.0.0
** Descriptions:            x frame 协议相关配置
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2021-04-23
** Last version:            V1.0.0
** Description:             x frame 协议相关配置
**
*************************************************************/
#ifndef x_frame_protocol_h
#define x_frame_protocol_h
#ifdef __cplusplus
extern "C" {
#endif

//通用命令集 --- update  shell  cfg_param
#define COMM_CMD_SET              	0x01
//----心跳包
#define CMD_ID_HEART_BEAT           0x00
#define CMD_ID_PING                 0x01
#define CMD_ID_LINK_CFG             0x02
#define CMD_ID_HW_INFO              0x03
#define CMD_ID_SW_INFO              0x04
#define CMD_ID_PRODUCT_INFO         0x05

#define CMD_ID_SYS_PRODUCT_INFO     0x06

//查询是否在bootloader中
#define ISP_CMD_ID_IS_BOOTING       0x07
//在
#define ISP_CMD_ID_GO               0x08
#define ISP_CMD_ID_GO_ALL           0x09


//----升级传输的子命令ID
#define CMD_ID_UPDATE_PREPARE       0x10
#define CMD_ID_UPDATE_START         0x11
#define CMD_ID_UPDATE_TRANS         0x12
#define CMD_ID_UPDATE_CHECK         0x13
#define CMD_ID_DEV_RESET            0x14



//----shell的子命令ID
#define CMD_ID_SHELL_IN             0x21
#define CMD_ID_SHELL_OUT            0x22


//----参数配置的子命令ID
#define CMD_ID_GET_PARAM_CNT        0x31
#define CMD_ID_GET_PARAM_ITEM       0x32
#define CMD_ID_WR_PARAM_ITEM        0x33
#define CMD_ID_RD_PARAM_ITEM        0x34
#define CMD_ID_RS_PARAM_ITEM        0x35
#define CMD_ID_RS_ALL_PARAM_ITEM    0x36
#define CMD_ID_WR_PARAM_NAME_ITEM   0x37
#define CMD_ID_RD_PARAM_NAME_ITEM   0x38
#define CMD_ID_RS_PARAM_NAME_ITEM   0x39

//--------------------------sn_info 
#define CMD_ID_GET_GPUID          	0x40 
#define CMD_ID_GET_SN_INFO        	0x41 
#define CMD_ID_SET_SN_INFO        	0x42

#define CMD_ID_GET_PCBA_SN_INFO     0x43 
#define CMD_ID_SET_PCBA_SN_INFO     0x44

//----产品名称
#define CMD_ID_SET_PRODUCT_NAME     0x45
#define CMD_ID_GET_PRODUCT_NAME     0x46

//----UNIX TIME
#define CMD_ID_GET_UNIX_TIME        0x52
#define CMD_ID_SET_UNIX_TIME        0x53

#define CMD_ID_SILENCE              0xFF

//----数据记录指令集
#define FUNC_CMDSET_REC            	0xFE

#define CMD_ID_FLASH_PARAM_RD       0x01

#define CMD_ID_LOG_PUSH             0x10

#define CMD_ID_WATTH_PUSH           0x20

#define CMD_ID_BBOX_UPLOAD_SET      0x30
#define CMD_ID_BBOX_REPLENISH       0x31
#define CMD_ID_BBOX_STOP_AND_ERASE  0x32

#define CMD_ID_BBOX_NORMAL_UPLOAD   0x33
#define CMD_ID_BBOX_RECORD_UPLOAD   0x34
#define CMD_ID_BBOX_CONFIG_UPLOAD   0x35
#define CMD_ID_BBOX_FINISH_UPLOAD   0x36

//----数据记录指令集
#define REC_CMDSET_REC            	0xff
#define REC_CMDID_REC_CFG        	0x01
#define REC_CMDID_REC_CAP_CFG       0x02

#define REC_CMDID_START_CAP                0x10
#define REC_CMDID_PUSH_CAP_BUF_STA         0x11
#define REC_CMDID_GET_CAP_BUF_CFG          0x12
#define REC_CMDID_GET_CAP_BUF_DAT          0x13


#ifdef __cplusplus
}
#endif
#endif /* x_frame_protocol_h */







