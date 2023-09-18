#include "MTR_CTRL.h"
#include "DSP\mtr_dsp.h"
#include "stdio.h"
void MTR_INIT(void)
{

}

void MTR_CTRL_RUN(MTR_CTRL_typedef *P_MTR)
{
    math_vec2_t Phasor = {0};
    math_vec3_t TMP = {0};

    P_MTR->ADC_GET(&P_MTR->ADC_Value);
    // Foc_Getadc(P_MTR);
//    Foc_GetUdqBySet(P_MTR);
    Foc_ConstClarke(&P_MTR->Iabc, &P_MTR->Iab);
//    MTR_DSP_SINCOS(P_MTR->Angle, Phasor.Value[0], Phasor.Value[1]);
    Foc_Park(&P_MTR->Iab, &P_MTR->Idq, &Phasor);
    
    P_MTR->Udq_out.Value[0] = Pid_Calculate(P_MTR->DAxisCurrentPid,P_MTR->Idq.Value[0],P_MTR->SetIdq.Value[0]);
    P_MTR->Udq_out.Value[1] = Pid_Calculate(P_MTR->QAxisCurrentPid,P_MTR->Idq.Value[1],P_MTR->SetIdq.Value[1]);
    
    Foc_VqLimit(P_MTR->Udq_out.Value[1], &P_MTR->Udq_out, P_MTR->Vs_Max);
    Foc_IPark(&P_MTR->Udq_out, &P_MTR->Uab_out, &Phasor);
    Foc_IClarke(&P_MTR->Uab_out, &TMP);
    Foc_Svgen3S(&TMP, &P_MTR->Uabc_out, 0);
    Foc_PwmTrans(&P_MTR->Uabc_pwm,&P_MTR->Uabc_out,P_MTR->PWM_PERIOD);
    P_MTR->PWM_OUT(&P_MTR->Uabc_pwm);
}

void MTR_CTRL_VF(MTR_CTRL_typedef *P_MTR,math_vec2_t VF)
{
    P_MTR->Udq_out = VF;
}
