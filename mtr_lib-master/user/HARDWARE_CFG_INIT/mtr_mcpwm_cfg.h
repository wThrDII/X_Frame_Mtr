/*
*********************************************************************************************************
*                                                  MTR_MCPWM_CFG
*                                              MTR_MCPWM_CFG MOUDULE
*
*********************************************************************************************************
*/
 
/*
*********************************************************************************************************
*                                                  MTR_MCPWM_CFG
* Filename     :mtr_mcpwm_cfg.h
*
*********************************************************************************************************
*/ 
#ifndef __MTR_MCPWM_CFG_H__
#define __MTR_MCPWM_CFG_H__
#ifdef __cplusplus
extern "C"
{
#endif

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "lks32mc45x.h"
#include "lks32mc45x_lib.h"

/*
*********************************************************************************************************
*                                                  EXTERNS
*********************************************************************************************************
*/
#ifdef MTR_MCPWM_CFG_MODULE
#define MTR_MCPWM_CFG_EXT
#else 
#define MTR_MCPWM_CFG_EXT extern
#endif

/*
*********************************************************************************************************
*                                              EXTERN FUNCTIONS
*********************************************************************************************************
*/
MTR_MCPWM_CFG_EXT void mtr_mcpwm_config(void);

/*
*********************************************************************************************************
*                                              MOUDULE END
*********************************************************************************************************
*/
#ifdef __cplusplus
}
#endif 
#endif
