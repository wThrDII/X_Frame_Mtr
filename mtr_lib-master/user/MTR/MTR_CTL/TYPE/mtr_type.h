#ifndef __INC_MTR_TYPE_H__
#define __INC_MTR_TYPE_H__
#include "IQmath.h"
typedef struct
{
    mtr_type Value[2];
} math_vec2_t;

typedef struct
{
    mtr_type Value[3];
} math_vec3_t;

typedef struct
{
    math_vec3_t Iabc;
    math_vec3_t Uabc;
    math_vec3_t Iabc_Offset;
    math_vec3_t Uabc_Offset;
    mtr_type  Udc;
} hal_adc_data_t;







#endif
