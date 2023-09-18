#ifndef __MTR_CTRL_H__
#define __MTR_CTRL_H__

#include "MTR_LIB\foc_base.h"
#include "MTR_LIB\pid.h"
#include "MTR_LOG\MTR_logic.h"
#include "MTR_LOG\mtr_fsm.h"
typedef struct _MTR_CTRL_
{
    uint32_t PWM_PERIOD;
    mtr_type Vs_Max;////32767->0.66  28388->0.577
    // mtr_type SVPWM_Ratio;
    void (*PWM_OUT)(math_vec3_t *);
    void (*ADC_GET)(hal_adc_data_t *);
    pid_t     *DAxisCurrentPid;
    pid_t     *QAxisCurrentPid;
    pid_t     *SpeedHzPid;
    pid_t     *PositionPid;
    
    hal_adc_data_t ADC_Value;
    math_vec3_t Iabc;
    math_vec3_t Uabc;
    math_vec2_t Iab;
    math_vec2_t Uab;
    math_vec2_t Idq;
    math_vec2_t Udq;

    math_vec2_t Udq_out;
    math_vec2_t Uab_out;
    math_vec3_t Uabc_out;
    math_vec3_t Uabc_pwm;
    math_vec2_t SetIdq;

    mtr_type  Udc;
    mtr_type  Angle;
    mtr_type  SpeedHz;
	
	
} MTR_CTRL_typedef;

#define REG_MTR_CTRL(id,PWM_PERIOD,svpwm_ratio,PWM_OUT,ADC_GET,DAxisCurrentPid,QAxisCurrentPid)     \
         MTR_CTRL_typedef MTR_obj_##id = {PWM_PERIOD,(svpwm_ratio/0.66)*32767,PWM_OUT,ADC_GET,      \
         DAxisCurrentPid,QAxisCurrentPid};                                                          \
         const mtr_fsm_item_obj_t mtr_item_tab_##id[] = {   MTR_FSM_STATE_TAB_ITEM(STATE_INIT),         \
                                                            MTR_FSM_STATE_TAB_ITEM(STATE_BIAS_GET),     \
                                                            MTR_FSM_STATE_TAB_ITEM(STATE_PARAM_ID),     \
                                                            MTR_FSM_STATE_TAB_ITEM(STATE_IDLE),         \
                                                            MTR_FSM_STATE_TAB_ITEM(STATE_RUN_SPEED),    \
                                                            MTR_FSM_STATE_TAB_ITEM(STATE_RUN_POSITION), \
                                                            MTR_FSM_STATE_TAB_ITEM(STATE_STOP),         \
                                                            MTR_FSM_STATE_TAB_ITEM(STATE_ABNORMAL),     \
         };                                                                                             \
        mtr_fsm_obj_t mtr_fsm_obj_##id = {#id, mtr_item_tab_##id, ((uint8_t)(sizeof(mtr_item_tab_##id) / sizeof(mtr_fsm_item_obj_t))), 0,0, 0, 0, 0,&MTR_obj_##id};



#define MTR_CTRL_OBJ(id)                (&MTR_obj_##id)        
#define RUN_MTR_CTRL(id)                MTR_CTRL_RUN(&MTR_obj_##id)
#define RUN_MTR_LOGIC(id)               mtr_fsm_fun(&mtr_fsm_obj_##id)


void MTR_INIT(void);
void MTR_CTRL_RUN(MTR_CTRL_typedef *P_MTR);
void MTR_CTRL_VF(MTR_CTRL_typedef *P_MTR,math_vec2_t VF);


#endif
