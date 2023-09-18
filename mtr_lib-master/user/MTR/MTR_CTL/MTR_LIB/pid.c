#include "pid.h"

void Pid_Init(pid_t *pid)
{
    
}
void Pid_Reset(pid_t *pid)
{
    pid->Integral = 0;
}
void Pid_SetKpKiKd(pid_t *pid,mtr_type Kp,mtr_type Ki,mtr_type Kd)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
}
void Pid_SetOutLimit(pid_t *pid,mtr_type out_max,mtr_type out_min)
{
    pid->OutputMaxLim = out_max;
    pid->OutputMinLim = out_min;
    pid->IntegralMaxLim = _MUL(out_max,pid->Freq,GLOBAL_Q);
    pid->IntegralMinLim = _MUL(out_min,pid->Freq,GLOBAL_Q);
}
void Pid_SetTager(pid_t *pid,mtr_type tag)
{
    pid->Tag = tag;
}
void Pid_SetCur(pid_t *pid,mtr_type cur)
{
    pid->Cur = cur;
}
mtr_type Pid_Calculate(pid_t *pid,mtr_type Cur,mtr_type Tag)
{
    pid->Cur        =       Cur;
    pid->Tag        =       Tag;
    pid->Err        =       pid->Tag - pid->Cur;
    pid->POut       =       _MUL(pid->Kp,pid->Err,GLOBAL_Q);
    pid->Integral   +=      pid->POut;
    pid->Integral   =       pid->Integral>pid->IntegralMaxLim?pid->IntegralMaxLim:pid->Integral;
    pid->Integral   =       pid->Integral<pid->IntegralMinLim?pid->IntegralMinLim:pid->Integral;
    if(pid->Freq==16384){
        pid->IOut   =       _MUL(pid->Ki,_DIV_16384(pid->Integral),GLOBAL_Q);
    }else{
        pid->IOut   =       _MUL(pid->Ki,_DIV(pid->Integral,pid->Freq),GLOBAL_Q);
    }
    pid->DOut       =       _MUL(pid->Kd,pid->POut,GLOBAL_Q);
    pid->Output     =       pid->POut + pid->IOut + pid->DOut;
    pid->Output     =       pid->Output>pid->OutputMaxLim?pid->OutputMaxLim:pid->Output;
    pid->Output     =       pid->Output<pid->OutputMinLim?pid->OutputMinLim:pid->Output;
    return pid->Output;
}
