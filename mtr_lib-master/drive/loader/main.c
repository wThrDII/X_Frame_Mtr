/********************** Copyright(c)***************************
** ECOFLOW
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               main.c
** Created by:              Chen JunTong
** Created date:            2021-03-19
** Version:                 V1.0.0
** Descriptions:            主函数
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2021-03-19
** Last version:            V1.0.0
** Description:             主函数
**
*************************************************************/
#define DEF_DBG_I
#define DEF_DBG_W
#define DEF_DBG_E
#include "main.h"


int main(void)
{
    //此时打开总中断（由于程序执行到此处前，关闭了所有中断）
    __enable_irq(); 
    hal_init();
    DBG_I("\r\n \r\n");
    DBG_I("================================================================");
    DBG_I("EF 01 S1 is START !!!  %s   %s  (%s)", __DATE__, __TIME__, (USE_REG_SECTION > 0) ? "USE_REG_SECTION" : "USE_REG_ARRY_TAB");
    DBG_I("================================================================\r\n \r\n");
    while(1) {
		task_sch(VH_TASK);
		task_sch(HI_TASK);
		task_sch(LW_TASK);
        task_sch(ID_TASK);
    }
}



