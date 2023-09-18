#include "foc_base.h"
void Foc_ConstClarke            (math_vec3_t *pInVec,
                            math_vec2_t *pOutVec)
{
   pOutVec->Value[0]    =   pInVec->Value[0] - _DIV_2(pInVec->Value[1]  + pInVec->Value[2]);
   pOutVec->Value[1]    =  _MUL((pInVec->Value[1] - pInVec->Value[2]), _FLOAT_VALUE(1.732f/2.0f),GLOBAL_Q);
}

void Foc_IClarke           (math_vec2_t *pInVec,
                            math_vec3_t *pOutVec)
{
   pOutVec->Value[0]    = _MUL(pInVec->Value[0],_FLOAT_VALUE(2.0f /3.0f),GLOBAL_Q);
   pOutVec->Value[1]    = -_MUL(pInVec->Value[0], _FLOAT_VALUE(1.0f /3.0f),GLOBAL_Q)
                        +  _MUL(pInVec->Value[1], _FLOAT_VALUE(1.0f/1.732f),GLOBAL_Q);
   pOutVec->Value[2]   = -_MUL(pInVec->Value[0], _FLOAT_VALUE(1.0f /3.0f),GLOBAL_Q)
                         -_MUL(pInVec->Value[1], _FLOAT_VALUE(1.0f/1.732f),GLOBAL_Q);
}

void Foc_Park              (math_vec2_t *pInVec,
                            math_vec2_t *pOutVec,
                            math_vec2_t *Phasor)
{
   pOutVec->Value[0]    =  _MUL(pInVec->Value[0], Phasor->Value[1],GLOBAL_Q)
                        +  _MUL(pInVec->Value[1], Phasor->Value[0],GLOBAL_Q);
   pOutVec->Value[1]    =  _MUL(pInVec->Value[1], Phasor->Value[1],GLOBAL_Q)
                        -  _MUL(pInVec->Value[0], Phasor->Value[0],GLOBAL_Q);
}

void Foc_IPark             (math_vec2_t *pInVec,
                            math_vec2_t *pOutVec,
                            math_vec2_t *Phasor)
{
   pOutVec->Value[0]    =  _MUL(pInVec->Value[0], Phasor->Value[1],GLOBAL_Q)
                        -  _MUL(pInVec->Value[1], Phasor->Value[0],GLOBAL_Q);
   pOutVec->Value[1]    =  _MUL(pInVec->Value[1], Phasor->Value[1],GLOBAL_Q)
                        +  _MUL(pInVec->Value[0], Phasor->Value[0],GLOBAL_Q);
}




void Foc_Svgen3S           (math_vec3_t *v_abc,
                            math_vec3_t *pwm,
                            mtr_type    dead_cpst)
{
   mtr_type v_com        =   0;
   mtr_type v_max        =   0;
   mtr_type v_min        =   0;
   mtr_type v_a          =   v_abc->Value[0];
   mtr_type v_b          =   v_abc->Value[1];
   mtr_type v_c          =   v_abc->Value[2];
   v_max                =   v_a   > v_b    ?  v_a              : v_b;
   v_max                =   v_max > v_c    ?  v_max            : v_c;
   v_min                =   v_a   < v_b    ?  v_a              : v_b;
   v_min                =   v_min < v_c    ?  v_min            : v_c;
   v_com                =  (v_max + v_min) >> 1;
   v_a                  =   v_a   > 0      ? (v_a + dead_cpst) : (v_a - dead_cpst);
   v_b                  =   v_b   > 0      ? (v_b + dead_cpst) : (v_b - dead_cpst);
   v_c                  =   v_c   > 0      ? (v_c + dead_cpst) : (v_c - dead_cpst);
   v_a                 -=   v_com;
   v_b                 -=   v_com;
   v_c                 -=   v_com;

   pwm->Value[0]        =   v_a;
   pwm->Value[1]        =   v_b;
   pwm->Value[2]        =   v_c;
}


// void Foc_Getadc    (MTR_CTRL_typedef *p_mtr)
// {
//     p_mtr->Iabc.Value[0] = p_mtr->ADC_Value.Iabc.Value[0] - p_mtr->ADC_Value.Iabc_Offset.Value[0];
//     p_mtr->Iabc.Value[1] = p_mtr->ADC_Value.Iabc.Value[1] - p_mtr->ADC_Value.Iabc_Offset.Value[1];
//     p_mtr->Iabc.Value[2] = p_mtr->ADC_Value.Iabc.Value[2] - p_mtr->ADC_Value.Iabc_Offset.Value[2];
//     p_mtr->Udc = p_mtr->ADC_Value.Udc;
// }

// void Foc_GetAngle  (MTR_CTRL_typedef *p_mtr)
// {
        
// }

// void FocBase_GetUdqBySet(MTR_CTRL_typedef *p_mtr)
// {
//     p_mtr->Udq.Value[0] = _MUL(p_mtr->Udq_out.Value[0],( _MUL( p_mtr->Udc ,_FLOAT_VALUE(2.0f/3.0f),15)),15);
//     p_mtr->Udq.Value[1] = _MUL(p_mtr->Udq_out.Value[1],( _MUL( p_mtr->Udc ,_FLOAT_VALUE(2.0f/3.0f),15)),15);
// }

void Foc_PwmTrans                       (math_vec3_t *Uabc_pwm,math_vec3_t *Uabc_out,uint32_t MTR_PWM_PERIOD)
{
    math_vec3_t pwm                     =  {0};
    pwm.Value[0]                      = _MUL((Uabc_out->Value[0] + _FLOAT_VALUE(0.5f)), MTR_PWM_PERIOD , 15);// ((Uout + 0.5Udc)/Udc)*pwmPeriod;
    pwm.Value[1]                      = _MUL((Uabc_out->Value[1] + _FLOAT_VALUE(0.5f)), MTR_PWM_PERIOD , 15);
    pwm.Value[2]                      = _MUL((Uabc_out->Value[2] + _FLOAT_VALUE(0.5f)), MTR_PWM_PERIOD , 15);
    pwm.Value[0]                      =   pwm.Value[0] > MTR_PWM_PERIOD ? MTR_PWM_PERIOD : pwm.Value[0];
    pwm.Value[0]                      =   pwm.Value[0] < 0          ? 0          : pwm.Value[0];
    pwm.Value[1]                      =   pwm.Value[1] > MTR_PWM_PERIOD ? MTR_PWM_PERIOD : pwm.Value[1];
    pwm.Value[1]                      =   pwm.Value[1] < 0          ? 0          : pwm.Value[1];
    pwm.Value[2]                      =   pwm.Value[2] > MTR_PWM_PERIOD ? MTR_PWM_PERIOD : pwm.Value[2];
    pwm.Value[2]                      =   pwm.Value[2] < 0          ? 0          : pwm.Value[2];
    Uabc_pwm->Value[0]                =   pwm.Value[0];
    Uabc_pwm->Value[1]                =   pwm.Value[1];
    Uabc_pwm->Value[2]                =   pwm.Value[2];
}

void Foc_VqLimit                             (int32_t    vq,  math_vec2_t *vdq,mtr_type VS_MAX)//0.66 0.577
{
   uint16_t vq_max                     =    0;
   vq_max 							   =	MTR_DSP_SQRT_POW(VS_MAX,vdq->Value[0]);//0.87424242  32767为0.66调制比 *0.87424为0.577调制比
   vdq->Value[1]                      =    vq             >  vq_max ?  vq_max : vq;
   vdq->Value[1]                      =    vdq->Value[1]  < -vq_max ? -vq_max : vdq->Value[1];
}
