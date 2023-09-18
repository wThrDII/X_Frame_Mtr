#ifndef __ADC_CFG_H__
#define __ADC_CFG_H__

 /*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#include "lks32mc45x.h"
#include "lks32mc45x_lib.h"
#include "TYPE\mtr_type.h"

/*
*********************************************************************************************************
*                                                  EXTERNS
*********************************************************************************************************
*/
#ifdef  ADC_CFG_MODULE
#define ADC_CFG_EXT
#else
#define ADC_CFG_EXT extern
#endif

/*
*********************************************************************************************************
*                                              EXTERN FUNCTIONS
*********************************************************************************************************
*/
ADC_CFG_EXT void AdcCfg_Init(void);
ADC_CFG_EXT void MTR0_ADC_GET(hal_adc_data_t *val);
ADC_CFG_EXT void MTR1_ADC_GET(hal_adc_data_t *val);
/*
*********************************************************************************************************
*                                              MOUDULE END
*********************************************************************************************************
*/
#endif
