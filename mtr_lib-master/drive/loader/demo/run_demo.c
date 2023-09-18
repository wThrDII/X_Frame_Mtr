/********************** Copyright(c)***************************
** ECOFLOW
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               run_demo.c
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
///< 源文件必须定义三个宏，且定义必须在x_frame.h文件前面，否则DBG功能不能使用
#define DEF_DBG_I
#define DEF_DBG_W
#define DEF_DBG_E
#include "run_demo.h"
#include "x_frame.h"
#include "hal.h"

#include "lks32mc45x.h"
#include "lks32mc45x_lib.h"

//#if ENABLE_REG_LINK_DEMO == 1
REG_LINK(LINK_UART0, VH_TASK, DRV_UART0, PROTOCOL_EF03_PLUS, 512, true, false)
REG_LINK(LINK_CAN0 , VH_TASK, DRV_CAN_S, PROTOCOL_EF03_PLUS, 512, true, false)
//#endif

//REG_REC_LINK(LINK_UART1, LINK_CAN0)

//#if ENABLE_COMM_CMD_DEMO == 1
REG_LINK_APP_BASE_ACTION(LINK_UART0)
REG_LINK_APP_BASE_ACTION(LINK_CAN0)
//#endif

//#if ENABLE_UPDATA_DEMO == 1
REG_LINK_UPDATA_ACTION(LINK_UART0)
REG_LINK_UPDATA_ACTION(LINK_CAN0)
//#endif

uint16_t test_rec_var;
void test(void)
{
//    test_rec_var++;
//    static uint8_t init_flag = 0;
//    uint8_t tx_data[8] = {0xAA,1,2,3,4,5,6,7};
//    if (init_flag == 0) {
////        init_flag = 1;
//        My_CAN_Send_Msg(2, 0, 0, tx_data, 8);
//    } else {}
    #if 1
    uint8_t pdata[20] = {0};
    for(uint8_t i =0;i<20;i++)
    {
        pdata[i] = i;
    }
    hal_frame_info_t send_info = {0};
    
    send_info.cmd_func = 0xFE;
    send_info.cmd_id = 0xFF;
    send_info.src = HOST_ADDR;
    send_info.dest = PC_ADDR;
    send_info.d_dest = 0x01;
    send_info.d_src = 0x01;
    send_info.pdata = pdata;
    send_info.data_len = sizeof(pdata);
    send_info.link_id = LINK_CAN0;
    protocol_send_by_id(&send_info);
    #endif
}
REG_TASK(ID_TASK, 200, test)


//#define REG_REC_TASK(group,perio,task_name,def_rec_sta)                                     		\
//        uint8_t enable_##task_name = def_rec_sta;                                                  	\
//        REG_CFG_ITEM(task_name, enable_##task_name, eu_uint8, PARM_RW, def_rec_sta, 0.0f, 1.0f);  	\
//        EXT_REC(task_name);                                                                 		\
//        static void task_name(void)                                                         		\
//        {                                                                                   		\
//            if(enable_##task_name == false){                                                		\
//                return;                                                                     		\
//            }                                                                               		\
//            RUN_REC(task_name);                                                             		\
//        }                                                                                   		\
//        REG_TASK(group,perio,task_name);

//REG_REC_TASK(VH_TASK, 100, TEST_PUSH, true)

#if WATCHDOG_ENABLE ==  1
void wdg_feed_task(void)
{
    xh_wdg_reload(DRV_FWDGT);   ///< 看门狗喂狗
}
REG_TASK(VH_TASK, 300, wdg_feed_task)
#endif

#if SYSINFO_ENABLE == 1
DEF_SYS_INFO_ROUTE(
)
#endif
