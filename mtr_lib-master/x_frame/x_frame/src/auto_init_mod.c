#include "x_frame.h"


#define MAX_INIT_MOD_CNT    (64)

typedef struct{
    XF_AUTO_INIT_REG_T *p_reg;
    list_t node;
}XF_AUTO_INIT_PARAM_T;

#if USE_REG_SECTION==1

/*
** Descriptions:        自动初始化模块
** Input parameters:    init_type：初始化类型
*/
void hal_init(void)
{
    list_t head = LIST_HEAD_INIT(head);
    uint32_t count = 0;
    XF_AUTO_INIT_PARAM_T list[MAX_INIT_MOD_CNT];
    XF_AUTO_INIT_REG_T *p_reg = NULL;
    XF_AUTO_INIT_PARAM_T *p_param = NULL;
    list_t *pos = NULL;
    list_t *p_head = &head;
    
    while(1){
        p_reg = (XF_AUTO_INIT_REG_T *)auto_reg_get_func_cont_by_index(AUTO_REG_INIT_MOD, count);
        if(p_reg == NULL){
            break;
        }

        if( count < MAX_INIT_MOD_CNT ){
            list[count].p_reg = p_reg;
            list_for_each(pos, p_head){
                p_param = list_entry(pos, XF_AUTO_INIT_PARAM_T, node);
                if(p_reg->init_priority < p_param->p_reg->init_priority){
                    break;
                }
            }
            list_add_tail(&list[count].node, pos);
        }else{
            while(1);
        }
        
        count++;
    }

    list_for_each(pos, p_head){
        p_param = list_entry(pos, XF_AUTO_INIT_PARAM_T, node);
        if(p_param->p_reg->pf_init_func){
            p_param->p_reg->pf_init_func();
        }
    }
}

#endif


