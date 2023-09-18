/********************** File Info ***************************
** File name:               bkfifo.h
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
#ifndef bkfifo_h
#define bkfifo_h

#include <stdint.h>
#include <stdbool.h>


typedef struct _BKFIFO_OBJ {
    void *p_msg;                        /* 消息buf */
    uint32_t max_msg_num;               /* 最大消息个数 */
    uint32_t msg_size;                  /* 消息单元大小 */
    volatile uint32_t de_index;         /* 出队索引 */
    volatile uint32_t en_index;         /* 入队索引 */
    volatile uint64_t en_total_num;     /* 入队次数累计 */
    volatile uint64_t de_total_num;     /* 出队次数累计 */
}BKFIFO_OBJ, *P_BKFIFO_OBJ;


#define DEF_BKFIFO_OBJ(name,type,num)                           \
        type bkfifo_msg_##name[num];                            \
        BKFIFO_OBJ name = {                                     \
            bkfifo_msg_##name, num, sizeof(type),               \
            0, 0, 1, 1,                                         \
        };

#define EXT_BKFIFO_OBJ(name) extern BKFIFO_OBJ name;

extern uint32_t bkfifo_cur_num(P_BKFIFO_OBJ p_obj);
extern uint32_t bkfifo_free_num(P_BKFIFO_OBJ p_obj);
extern bool bkfifo_is_empty(P_BKFIFO_OBJ p_obj);
extern bool bkfifo_is_full(P_BKFIFO_OBJ p_obj);
extern void bkfifo_clear(P_BKFIFO_OBJ p_obj);
extern uint32_t bkfifo_en(P_BKFIFO_OBJ p_obj, void *p_msg);
extern uint32_t bkfifo_de(P_BKFIFO_OBJ p_obj, void *p_msg);


#endif  /* bkfifo_h */
