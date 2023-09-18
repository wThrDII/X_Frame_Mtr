#ifndef __INC_FSM_H__
#define __INC_FSM_H__
#include "stdint.h"
#include "stddef.h"
#include "../TYPE/mtr_type.h"
#include "MTR_CTRL.h"
// #define UNCHANGE_STATE 0u
typedef void (*__fun)(void *P);
typedef int  (*__exe_fun)(void *P);

typedef struct
{
    uint8_t fsm_state_id;
    __fun fsm_in_func;
    __exe_fun fsm_exe_func;
    __fun fsm_out_func;
} mtr_fsm_item_obj_t;

typedef struct
{
    const char *fsm_name;
    const mtr_fsm_item_obj_t *p_fsm_item_tab;
    uint8_t max_status;
    uint8_t init_status;
    uint8_t action;
    uint8_t last_state;
    uint8_t cur_state;
    uint8_t next_state;
    void *P;
} mtr_fsm_obj_t;

// #define MTR_REG_FSM_TAB(id, ...)                                 \
//     const mtr_fsm_item_obj_t fsm_item_tab_##id[] = {__VA_ARGS__}; \
//     mtr_fsm_obj_t fsm_obj_##id = {#id, fsm_item_tab_##id, ((uint8_t)(sizeof(fsm_item_tab_##id) / sizeof(fsm_item_obj_t))), 0,0, 0, 0, 0};

#define MTR_FSM_STATE_WEAK_FUN_DECL(state)                                          \
void state##_in(void *P);                                               \
int  state##_exe(void *P);                                              \
void state##_out(void *P);
 
#define MTR_FSM_STATE_TAB_ITEM(state)   {state,state##_in,state##_exe,state##_out}

// #define MTR_RUN_FSM(id)                 mtr_fsm_fun(&fsm_obj_##id)
// #define MTR_EMIT_FSM(id, necx_state)    mtr_fsm_emit_event(&fsm_obj_##id, necx_state)
// #define MTR_FSM_OBJ(id)                  (&fsm_obj_##id)

void mtr_fsm_fun(mtr_fsm_obj_t *p_fsm);
void mtr_fsm_emit_event(mtr_fsm_obj_t *p_fsm, uint32_t event);

#endif
