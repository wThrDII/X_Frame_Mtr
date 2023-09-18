#ifndef __INC_MTR_LOGIC_H__
#define __INC_MTR_LOGIC_H__
#include "mtr_fsm.h"
#include "mtr_ctrl.h"

enum MTR_STATE
{
    STATE_INIT = 0,
    STATE_BIAS_GET,
    STATE_PARAM_ID,
    STATE_IDLE,
    STATE_RUN_SPEED,
    STATE_RUN_POSITION,
    STATE_STOP,
    STATE_ABNORMAL,
};

extern void STATE_INIT_in(void *P);
extern int  STATE_INIT_exe(void *P);
extern void STATE_INIT_out(void *P);

extern void STATE_BIAS_GET_in(void *P);
extern int  STATE_BIAS_GET_exe(void *P);
extern void STATE_BIAS_GET_out(void *P);

extern void STATE_PARAM_ID_in(void *P);
extern int  STATE_PARAM_ID_exe(void *P);
extern void STATE_PARAM_ID_out(void *P);

extern void STATE_IDLE_in(void *P);
extern int  STATE_IDLE_exe(void *P);
extern void STATE_IDLE_out(void *P);

extern void STATE_RUN_SPEED_in(void *P);
extern int  STATE_RUN_SPEED_exe(void *P);
extern void STATE_RUN_SPEED_out(void *P);

extern void STATE_RUN_POSITION_in(void *P);
extern int  STATE_RUN_POSITION_exe(void *P);
extern void STATE_RUN_POSITION_out(void *P);

extern void STATE_STOP_in(void *P);
extern int  STATE_STOP_exe(void *P);
extern void STATE_STOP_out(void *P);

extern void STATE_ABNORMAL_in(void *P);
extern int  STATE_ABNORMAL_exe(void *P);
extern void STATE_ABNORMAL_out(void *P);

//MTR_FSM_STATE_WEAK_FUN_DECL(STATE_INIT);
//MTR_FSM_STATE_WEAK_FUN_DECL(STATE_BIAS_GET);
//MTR_FSM_STATE_WEAK_FUN_DECL(STATE_PARAM_ID);
//MTR_FSM_STATE_WEAK_FUN_DECL(STATE_IDLE);
//MTR_FSM_STATE_WEAK_FUN_DECL(STATE_RUN_SPEED);
//MTR_FSM_STATE_WEAK_FUN_DECL(STATE_RUN_POSITION);
//MTR_FSM_STATE_WEAK_FUN_DECL(STATE_STOP);
//MTR_FSM_STATE_WEAK_FUN_DECL(STATE_ABNORMAL);



void MTR_logic_clc(void);
#endif
