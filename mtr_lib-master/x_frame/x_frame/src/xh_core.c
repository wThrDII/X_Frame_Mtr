/********************** Copyright(c)***************************
** ECOFLOW
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** Created by:              Chen JunTong
** Created date:            2022-06-24
** Version:                 V1.0.0
** Descriptions:            驱动抽象层
**
**------File Info----------------------------------------------
** File name:               xh_core.c
** Latest modified date:    2022-06-24
** Latest version:          V1.0.0
** Description:             驱动抽象层
**
*************************************************************/

#include "xh_core.h"
#include "x_frame.h"

/*************************************************************
** Descriptions:        驱动抽象层初始化>
*************************************************************/
void xh_core_init(void)
{
    uint32_t i = 0;
    //定义标签，用于指示当前优先级最高的驱动。
    xh_comm_t *p_max_xh = NULL;
    //定义待初始化的驱动的数量
    uint32_t wait_init_nums = 0;

    //确定要初始化的驱动的总数量
    while(1) {
        xh_comm_t *p_xh = (xh_comm_t *)auto_reg_get_func_cont_by_index(AUTO_REG_XH_TYPE, i);
        if(p_xh == NULL) {
            break;
        }
        i++;
        if(p_xh->auto_init == false){
            continue;
        }
        wait_init_nums++;
    }
    //进行wait_init_nums轮驱动初始化，每一轮初始化一个最高优先级的驱动>
    uint16_t init_dev_num;
    for (init_dev_num = 0; init_dev_num < wait_init_nums; init_dev_num++) {
        i = 0;
        p_max_xh = NULL;
        while(1) {
            xh_comm_t *p_xh = (xh_comm_t *)auto_reg_get_func_cont_by_index(AUTO_REG_XH_TYPE, i);
            if(p_xh == NULL) {
                //本轮扫描完成则初始化优先级最高的那个驱动。前提是扫描出来有需要初始化的驱动
                if(p_max_xh != NULL) {
                    if(p_max_xh->init != NULL){
                        p_max_xh->init((p_max_xh->var));
                    }
                    p_max_xh->auto_init = false;
                }
                break;
            }
            i++;
            if(p_xh->auto_init == false){
                continue;
            }
            //如果是最先被扫描出来的
            if (p_max_xh == NULL) {
                p_max_xh = p_xh;
            } else {
                //如果新扫出来的驱动优先级更高
                if (p_xh->prio < p_max_xh->prio) {
                    p_max_xh = p_xh;
                } else if(p_max_xh->prio == p_xh->prio) {
                    // 不做处理
                    // ERROR !!! FLIE:xh_core.c FUNC:xh_core_init 存在两个优先级相同的驱动
                }
            }
        }
    }
}

/*************************************************************
** Descriptions:        驱动抽象层反初始化>
*************************************************************/
void xh_core_deinit(void)
{
    uint32_t i = 0;
    while(1) {
        xh_comm_t *p_xh = (xh_comm_t *)auto_reg_get_func_cont_by_index(AUTO_REG_XH_TYPE, i);
        if(p_xh == NULL) {
            break;
        }
        if(p_xh->deinit != NULL){
            p_xh->deinit(p_xh->var);
        }
        i++;
	}
}


