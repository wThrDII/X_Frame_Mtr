/*
*********************************************************************************************************
*                                                  USER_FLASH_PARAM
*                                              USER_FLASH_PARAM MOUDULE
*
*********************************************************************************************************
*/
 
/*
*********************************************************************************************************
*                                                  USER_FLASH_PARAM
* Filename     :user_flash_param.h
*
*********************************************************************************************************
*/ 
#ifndef __USER_FLASH_PARAM_H__
#define __USER_FLASH_PARAM_H__

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "x_frame.h"
//#include "IQmath.h"
/*
*********************************************************************************************************
*                                                  EXTERNS
*********************************************************************************************************
*/
#ifdef USER_FLASH_PARAM_MODULE
#define USER_FLASH_PARAM_EXT
#else 
#define USER_FLASH_PARAM_EXT extern
#endif
/*
*********************************************************************************************************
*                                              DEFINES
*********************************************************************************************************
*/
#define USER_FLASH_PARAM_MAX     512



/*
*********************************************************************************************************
*                                              GLOBAL VARIABLES
*********************************************************************************************************
*/
//typedef struct
//{
//	uint8_t  flash_demo;
//    uint8_t  buck_Il_bias_ok;
//    uint16_t buck_Il_bias_value;
//    float    buck_cur_kp;
//}buck_flash_t;

//typedef struct
//{
//    MATH_vec3           Ibias;             //
//    MATH_vec3           Vbias;             //
//}mtr_flash_t;
//typedef struct 
//{
//	buck_flash_t buck_flash;
//	uint8_t placeholder_1[256-sizeof(buck_flash_t)];
//	mtr_flash_t   mtr_flash;
//	uint8_t placeholder_2[256-sizeof(mtr_flash_t)];
//}data_t;


//typedef struct 
//{
//    data_t data;
//    uint8_t    rev[USER_FLASH_PARAM_MAX - sizeof(data_t)];
//}user_flash_param_t;
//USER_FLASH_PARAM_EXT user_flash_param_t user_flash_param;

/*
*********************************************************************************************************
*                                              EXTERN FUNCTIONS
*********************************************************************************************************
*/
//USER_FLASH_PARAM_EXT void user_flash_param_write(void);
//USER_FLASH_PARAM_EXT void user_flash_param_read(void);

/*
*********************************************************************************************************
*                                              MOUDULE END
*********************************************************************************************************
*/
#endif
