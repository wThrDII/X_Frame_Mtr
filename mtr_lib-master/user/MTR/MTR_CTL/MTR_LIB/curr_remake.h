#ifndef __CURR_REMAKE_H__
#define __CURR_REMAKE_H__

 /*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#include "../TYPE/mtr_type.h"       //该文件声明的结构体变量需要放置在mtr_type中，他们两文件之间的包含关系。 

/*
*********************************************************************************************************
*                                              DEFINES
*********************************************************************************************************
*/ 
#define SAMP_RESIS_x    1

/*
*********************************************************************************************************
*                                              GLOBAL VARIABLES
*********************************************************************************************************
*/
typedef struct 
{
    uint32_t test;
    #if      SAMP_RESIS_x == 1
    uint32_t test1;
    #elif    SAMP_RESIS_x == 2

    #elif    SAMP_RESIS_x == 3

    #endif
}curr_remake_t;



#endif



#ifndef __INC_FOC_CURRECONSTRUCT_H__
#define __INC_FOC_CURRECONSTRUCT_H__
#include "../TYPE/mtr_type.h"
#include "../MTR_Driver/MTR_Driver.h"
#include "../DSP/mtr_dsp.h"

#define   MCPWM0_PWMA_OUT(pwm1,pwm2)           {MCPWM0_TH00 =  pwm1; MCPWM0_TH01 =   pwm2;}
#define   MCPWM0_PWMB_OUT(pwm1,pwm2)           {MCPWM0_TH10 =  pwm1; MCPWM0_TH11 =   pwm2;}
#define   MCPWM0_PWMC_OUT(pwm1,pwm2)           {MCPWM0_TH20 =  pwm1; MCPWM0_TH21 =   pwm2;}
#define   MCPWM1_PWMA_OUT(pwm1,pwm2)           {MCPWM1_TH00 =  pwm1; MCPWM1_TH01 =   pwm2;}
#define   MCPWM1_PWMB_OUT(pwm1,pwm2)           {MCPWM1_TH10 =  pwm1; MCPWM1_TH11 =   pwm2;}
#define   MCPWM1_PWMC_OUT(pwm1,pwm2)           {MCPWM1_TH20 =  pwm1; MCPWM1_TH21 =   pwm2;}



#if   SAMPLING_RESISTACE == Single_Shunt_1
#define MTR0_SE1            ADC0_DAT2
#define MTR0_SE2            ADC0_DAT2
#define MTR1_SE1            ADC1_DAT2
#define MTR1_SE2            ADC1_DAT2
#elif SAMPLING_RESISTACE == Single_Shunt_3
#define MTR0_SE1            ADC0_DAT2
#define MTR0_SE2            ADC0_DAT2  
#define MTR0_SE2            ADC0_DAT2  
#define MTR1_SE1            ADC1_DAT2
#define MTR1_SE2            ADC1_DAT2  
#define MTR1_SE2            ADC1_DAT2
#endif

extern void MCPWM0_Out(struct _MTR_CTRL_ *ptr);
extern void MCPWM1_Out(struct _MTR_CTRL_ *ptr);
extern void MTR0_Adc_Get(MTR_CTRL_typedef * ptr);
extern void MTR1_Adc_Get(MTR_CTRL_typedef * ptr);
extern void CalRealVal(MTR_CTRL_typedef * ptr);
extern void MCPWM0_Set_PWM (MATH_vec3 *pwm);
extern void MCPWM1_Set_PWM (MATH_vec3 *pwm);
#endif


