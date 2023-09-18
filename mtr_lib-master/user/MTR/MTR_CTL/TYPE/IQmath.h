/*
*********************************************************************************************************
*                                                  IQMATH
* Filename     :IQmath.h
*
*********************************************************************************************************
*/
#ifndef __IQMATH_H__
#define __IQMATH_H__

#include "stdint.h"

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define Q_FORMAT

#ifdef Q_FORMAT
#define mtr_type int32_t
#define mtr_type int32_t
#else
#define mtr_type float
#endif

#define _IQ30(A) (int32_t)((A)*1073741824.0f)
#define _IQ29(A) (int32_t)((A)*536870912.0f)
#define _IQ28(A) (int32_t)((A)*268435456.0f)
#define _IQ27(A) (int32_t)((A)*134217728.0f)
#define _IQ26(A) (int32_t)((A)*67108864.0f)
#define _IQ25(A) (int32_t)((A)*33554432.0f)
#define _IQ24(A) (int32_t)((A)*16777216.0f)
#define _IQ23(A) (int32_t)((A)*8388608.0f)
#define _IQ22(A) (int32_t)((A)*4194304.0f)
#define _IQ21(A) (int32_t)((A)*2097152.0f)
#define _IQ20(A) (int32_t)((A)*1048576.0f)
#define _IQ19(A) (int32_t)((A)*524288.0f)
#define _IQ18(A) (int32_t)((A)*262144.0f)
#define _IQ17(A) (int32_t)((A)*131072.0f)
#define _IQ16(A) (int32_t)((A)*65536.0f)
#define _IQ15(A) (int32_t)((A)*32768.0f)
#define _IQ14(A) (int32_t)((A)*16384.0f)
#define _IQ13(A) (int32_t)((A)*8192.0f)
#define _IQ12(A) (int32_t)((A)*4096.0f)
#define _IQ11(A) (int32_t)((A)*2048.0f)
#define _IQ10(A) (int32_t)((A)*1024.0f)
#define _IQ9(A) (int32_t)((A)*512.0f)
#define _IQ8(A) (int32_t)((A)*256.0f)
#define _IQ7(A) (int32_t)((A)*128.0f)
#define _IQ6(A) (int32_t)((A)*64.0f)
#define _IQ5(A) (int32_t)((A)*32.0f)
#define _IQ4(A) (int32_t)((A)*16.0f)
#define _IQ3(A) (int32_t)((A)*8.0f)
#define _IQ2(A) (int32_t)((A)*4.0f)
#define _IQ1(A) (int32_t)((A)*2.0f)

#define rshft_1(A) (((A) + 0x1) >> 1)
#define rshft_2(A) (((A) + 0x2) >> 2)
#define rshft_3(A) (((A) + 0x4) >> 3)
#define rshft_4(A) (((A) + 0x8) >> 4)
#define rshft_5(A) (((A) + 0x10) >> 5)
#define rshft_6(A) (((A) + 0x20) >> 6)
#define rshft_7(A) (((A) + 0x40) >> 7)
#define rshft_8(A) (((A) + 0x80) >> 8)
#define rshft_9(A) (((A) + 0x100) >> 9)
#define rshft_10(A) (((A) + 0x200) >> 10)
#define rshft_11(A) (((A) + 0x400) >> 11)
#define rshft_12(A) (((A) + 0x800) >> 12)
#define rshft_13(A) (((A) + 0x1000) >> 13)
#define rshft_14(A) (((A) + 0x2000) >> 14)
#define rshft_15(A) (((A) + 0x4000) >> 15)
#define rshft_16(A) (((A) + 0x8000) >> 16)
#define rshft_17(A) (((A) + 0x10000) >> 17)
#define rshft_18(A) (((A) + 0x20000) >> 18)
#define rshft_19(A) (((A) + 0x40000) >> 19)
#define rshft_20(A) (((A) + 0x80000) >> 20)
#define rshft_21(A) (((A) + 0x100000) >> 21)
#define rshft_22(A) (((A) + 0x200000) >> 22)
#define rshft_23(A) (((A) + 0x400000) >> 23)
#define rshft_24(A) (((A) + 0x800000) >> 24)
#define rshft_25(A) (((A) + 0x1000000) >> 25)
#define rshft_26(A) (((A) + 0x2000000) >> 26)
#define rshft_27(A) (((A) + 0x4000000) >> 27)
#define rshft_28(A) (((A) + 0x8000000) >> 28)
#define rshft_29(A) (((A) + 0x10000000) >> 29)
#define rshft_30(A) (((A) + 0x20000000) >> 30)
#define rshft_31(A) (((A) + 0x40000000) >> 31)
#define rshft_32(A) (((A) + 0x80000000) >> 32)

#define lshft(A, n) ((A) << (n))
#define lshft_1(A) ((A) << 1)
#define lshft_2(A) ((A) << 2)
#define lshft_3(A) ((A) << 3)
#define lshft_4(A) ((A) << 4)
#define lshft_5(A) ((A) << 5)
#define lshft_6(A) ((A) << 6)
#define lshft_7(A) ((A) << 7)
#define lshft_8(A) ((A) << 8)
#define lshft_9(A) ((A) << 9)
#define lshft_10(A) ((A) << 10)
#define lshft_11(A) ((A) << 11)
#define lshft_12(A) ((A) << 12)
#define lshft_13(A) ((A) << 13)
#define lshft_14(A) ((A) << 14)
#define lshft_15(A) ((A) << 15)
#define lshft_16(A) ((A) << 16)
#define lshft_17(A) ((A) << 17)
#define lshft_18(A) ((A) << 18)
#define lshft_19(A) ((A) << 19)
#define lshft_20(A) ((A) << 20)
#define lshft_21(A) ((A) << 21)
#define lshft_22(A) ((A) << 22)
#define lshft_23(A) ((A) << 23)
#define lshft_24(A) ((A) << 24)
#define lshft_25(A) ((A) << 25)
#define lshft_26(A) ((A) << 26)
#define lshft_27(A) ((A) << 27)
#define lshft_28(A) ((A) << 28)
#define lshft_29(A) ((A) << 29)
#define lshft_30(A) ((A) << 30)
#define lshft_31(A) ((A) << 31)
#define lshft_32(A) ((A) << 32)

#ifndef GLOBAL_Q
#define GLOBAL_Q 15
#endif
#define _IQmpy(A, B) ((A) * (B) >> GLOBAL_Q)
#define _IQmpyV(A, B, C) ((A) * (B) >> C)
#define ABS_C(A) (A > 0 ? A : (-A))

#if GLOBAL_Q == 15
#define _IQ(A) _IQ15(A)
#endif

#define _IQabs(A) ABS_C(A)


#ifdef Q_FORMAT
#define _FLOAT_VALUE(A)  (int32_t)(((float)A)*(1<<GLOBAL_Q))
#else
#endif

#ifdef Q_FORMAT
#define _MUL(A, B, C) ((A) * (B) >> C)
#else
#define _MUL(A, B, C) ((A) * (B))
#endif
#ifdef Q_FORMAT
#define _DIV_2(A)       ((A)>>1)
#define _DIV_16384(A)   ((A)>>14)
#else
#define _DIV_2(A)  ((A)/2)
#endif


#define _DIV(X,Y)                ((X)/(Y))
#ifndef MTR_DSP_SQRT
#define MTR_DSP_SQRT(X,Y)
#endif
#define MTR_DSP_SINCOS(Angle,Sin,Cos)   {Sin=(int)(sin(((float)Angle/32768.0f)*3.14)*32767.0f);Cos=(int)(cos(((float)Angle/32768.0f)*3.14)*32767.0f);}
#define MTR_DSP_SIN(Angle)              
#define MTR_DSP_COS(Angle)              
#define MTR_DSP_SQRT_POW(X,Y)           0;



#endif
