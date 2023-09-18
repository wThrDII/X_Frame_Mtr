/********************** File Info ***************************
** File name:               bkfifo.c
** Latest modified date:    2021-12-07
** Latest version:          V1.0.0
** Description:             块状环形队列
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2021-12-07
** Version:                 V1.0.0
** Descriptions:            块状环形队列
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#include "bkfifo.h"
#include <string.h>


/*************************************************************
** Function name:       bkfifo_cur_num
** Descriptions:        返回当前消息个数
** Input parameters:    p：参数结构体
** Output parameters:   None
** Returned value:      消息个数
** Remarks:             None
*************************************************************/
uint32_t bkfifo_cur_num(P_BKFIFO_OBJ p_obj)
{
    return p_obj->en_index - p_obj->de_index;
}

/*************************************************************
** Function name:       bkfifo_free_num
** Descriptions:        返回可存储的消息个数
** Input parameters:    p：参数结构体
** Output parameters:   None
** Returned value:      可存储的消息个数
** Remarks:             None
*************************************************************/
uint32_t bkfifo_free_num(P_BKFIFO_OBJ p_obj)
{
    return p_obj->max_msg_num - (p_obj->en_index - p_obj->de_index);
}

/*************************************************************
** Function name:       bkfifo_clear
** Descriptions:        清除环形队列
** Input parameters:    p：参数结构体
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void bkfifo_clear(P_BKFIFO_OBJ p_obj)
{
    p_obj->de_index = 0;
    p_obj->en_index = 0;
    p_obj->de_total_num = p_obj->en_total_num;
}

/*************************************************************
** Function name:       bkfifo_en
** Descriptions:        环形队列入队
** Input parameters:    p：参数结构体
**                      pMsg：写入消息指针
** Output parameters:   None
** Returned value:      返回入队索引位置
** Remarks:             None
*************************************************************/
uint32_t bkfifo_en(P_BKFIFO_OBJ p_obj, void *p_msg)
{
    uint32_t en_index = p_obj->en_index;
    if(p_msg) {
        memcpy((uint8_t *)p_obj->p_msg + p_obj->en_index * p_obj->msg_size, p_msg, p_obj->msg_size);
    }
    p_obj->en_total_num++;
    p_obj->en_index = (p_obj->en_index + 1) % p_obj->max_msg_num;
    if(p_obj->en_index == p_obj->de_index){
        p_obj->de_index++;
    }
    return en_index;
}

/*************************************************************
** Function name:       bkfifo_de
** Descriptions:        环形队列消息出队
** Input parameters:    p：参数结构体
**                      pMsg：存放消息指针
** Output parameters:   None
** Returned value:      返回出队的索引位置
** Remarks:             None
*************************************************************/
uint32_t bkfifo_de(P_BKFIFO_OBJ p_obj, void *p_msg)
{
    uint32_t de_index = p_obj->de_index;
    if(p_msg) {
        memcpy(p_msg, (uint8_t *)p_obj->p_msg + p_obj->de_index * p_obj->msg_size, p_obj->msg_size);
    }
    p_obj->de_index = (p_obj->de_index + 1) % p_obj->max_msg_num;
    p_obj->de_total_num++;
    return de_index;
}

/*************************************************************
** Function name:       bkfifo_is_empty
** Descriptions:        返回队列是否为空
** Input parameters:    p：参数结构体
** Output parameters:   None
** Returned value:      true：为空，false：不为空
** Remarks:             None
*************************************************************/
bool bkfifo_is_empty(P_BKFIFO_OBJ p_obj)
{
    return p_obj->en_index == p_obj->de_index;
}

/*************************************************************
** Function name:       bkfifo_is_full
** Descriptions:        返回队列是否存满
** Input parameters:    p：参数结构体
** Output parameters:   None
** Returned value:      true：存满，false：为存满
** Remarks:             None
*************************************************************/
bool bkfifo_is_full(P_BKFIFO_OBJ p_obj)
{
    uint8_t num = 0;
    if(p_obj->en_index < p_obj->de_index){
        num = p_obj->en_index + p_obj->max_msg_num - p_obj->de_index;
    }else{
        num = p_obj->en_index - p_obj->de_index;
    }
    return num == (p_obj->max_msg_num - 1);
}
