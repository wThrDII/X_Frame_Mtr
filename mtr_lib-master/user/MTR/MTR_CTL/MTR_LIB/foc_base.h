#ifndef __INC_FOC_BASE_H__
#define __INC_FOC_BASE_H__
#include "../TYPE/mtr_type.h"
#include "../DSP/mtr_dsp.h"

void Foc_ConstClarke(math_vec3_t *pInVec, math_vec2_t *pOutVec);
void Foc_IClarke(math_vec2_t *pInVec, math_vec3_t *pOutVec);
void Foc_Park(math_vec2_t *pInVec, math_vec2_t *pOutVec, math_vec2_t *Phasor);
void Foc_IPark(math_vec2_t *pInVec, math_vec2_t *pOutVec, math_vec2_t *Phasor);
void Foc_Svgen3S(math_vec3_t *v_abc, math_vec3_t *pwm, mtr_type dead_cpst);

// void Foc_GetUdqBySet(MTR_CTRL_typedef *p_mtr);
void Foc_VqLimit(int32_t vq, math_vec2_t *vdq, mtr_type VS_MAX);
void Foc_PwmTrans(math_vec3_t *Uabc_pwm, math_vec3_t *Uabc_out, uint32_t MTR_PWM_PERIOD);

#endif
