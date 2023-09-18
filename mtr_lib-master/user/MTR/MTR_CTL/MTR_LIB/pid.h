#ifndef __INC__PID_H__
#define __INC__PID_H__
#include "stdint.h"
#include "../TYPE/IQmath.h"
#include "../TYPE/mtr_type.h"
typedef struct
{
    mtr_type    Freq;
	mtr_type    Kp;
	mtr_type    Ki;
	mtr_type    Kd;
	mtr_type    IntegralMaxLim;
	mtr_type    IntegralMinLim;
	mtr_type    OutputMaxLim;
	mtr_type    OutputMinLim;
	mtr_type    Tag;
	mtr_type    Cur;
	mtr_type    Err;
	mtr_type    LastErr;
	mtr_type    IErr;
	mtr_type    DVal;
	mtr_type    POut;
	mtr_type    IOut;
	mtr_type    DOut;
	mtr_type    Integral;
	mtr_type    Output;
    mtr_type    OutputLast;
}pid_t;

#define REG_PID(id,freq,Kp,Ki,Kd,out_max,out_min)  \
         pid_t pid_obj_##id = {freq,Kp,Ki,Kd,out_max*freq,out_min*freq,out_max,out_min};
#define PID_OBJ(id)         (&pid_obj_##id)
void Pid_Init(pid_t *pid);
void Pid_Reset(pid_t *pid);
void Pid_SetKpKiKd(pid_t *pid,mtr_type Kp,mtr_type Ki,mtr_type Kd);
void Pid_SetOutLimit(pid_t *pid,mtr_type Out_max,mtr_type Out_min);
void Pid_SetTager(pid_t *pid,mtr_type Tag);
void Pid_SetCur(pid_t *pid,mtr_type Cur);
mtr_type Pid_Calculate(pid_t *pid,mtr_type Cur,mtr_type Tag);

#endif /* __INC__PID_H__ */
