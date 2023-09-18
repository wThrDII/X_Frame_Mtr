#include "mtr_fsm.h"
void mtr_fsm_fun(mtr_fsm_obj_t *p_fsm)
{
    if (p_fsm->init_status == 0)
    {
        p_fsm->init_status = 1;
        if(p_fsm->p_fsm_item_tab[p_fsm->cur_state].fsm_in_func!=NULL) p_fsm->p_fsm_item_tab[p_fsm->cur_state].fsm_in_func(p_fsm->P);
    }

    if (p_fsm->action == 1)
    {
        p_fsm->action = 0;
        if(p_fsm->p_fsm_item_tab[p_fsm->cur_state].fsm_out_func!=NULL) p_fsm->p_fsm_item_tab[p_fsm->cur_state].fsm_out_func(p_fsm->P);
        p_fsm->cur_state = p_fsm->next_state;
        if(p_fsm->p_fsm_item_tab[p_fsm->cur_state].fsm_in_func!=NULL) p_fsm->p_fsm_item_tab[p_fsm->cur_state].fsm_in_func(p_fsm->P);
    }
    else
    {
        int event = 0;
        if(p_fsm->p_fsm_item_tab[p_fsm->cur_state].fsm_exe_func!=NULL) event = p_fsm->p_fsm_item_tab[p_fsm->cur_state].fsm_exe_func(p_fsm->P);
        if(event!=0) mtr_fsm_emit_event(p_fsm,event);
    }
}

void mtr_fsm_emit_event(mtr_fsm_obj_t *p_fsm, uint32_t event)
{
    if (event < p_fsm->max_status)
    {
        p_fsm->next_state = event;
        p_fsm->action = 1;
    }
}
