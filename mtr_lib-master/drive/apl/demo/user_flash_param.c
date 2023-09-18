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
* Filename     :user_flash_param.c
*
*********************************************************************************************************
*/
#define USER_FLASH_PARAM_MODULE

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "user_flash_param.h"
#include "hal.h"

//EXT_FLASH_PARAM_NAME(SUER_FLASH_PARAM)
//REG_FLASH_NORMAL_PARAM(SYS_APP_CFG_PART, SUER_FLASH_PARAM,  NULL, NULL, USER_FLASH_PARAM_MAX)

//void user_flash_param_write(void)
//{
//    flash_param_save(SUER_FLASH_PARAM, (uint8_t *)&user_flash_param.data, USER_FLASH_PARAM_MAX);
//}

//void user_flash_param_read(void)
//{
//    flash_param_read(SUER_FLASH_PARAM, (uint8_t *)&user_flash_param.data, USER_FLASH_PARAM_MAX);
//}


//void user_flash_param_func(void)
//{
//    user_flash_param_read();
//}

//REG_INIT_MOD(user_flash_param_func);
//// #define flash_param_demo
//#ifdef  flash_param_demo

//uint8_t test_param[sizeof(data_t)];
//void flash_param_test(void)
//{
//    static uint8_t init = 0;
//    switch (init)
//    {
//    case 0:
//         user_flash_param.data.flash_demo = 112;
//         flash_param_save(SUER_FLASH_PARAM, (uint8_t *)&user_flash_param.data, USER_FLASH_PARAM_MAX);
//         init = 1;
//         break;
//    case 1:
//         flash_param_read(SUER_FLASH_PARAM, test_param, sizeof(data_t));
//         init = 2;
//         break;
//    case 2:
//         break;
//    default:
//        break;
//    }
//}

//REG_TASK(0, 1, flash_param_test);

//#endif

