/********************** Copyright(c)***************************
** ECOFLOW 
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               rec_db.h
** Created by:              Chen JunTong
** Created date:            2021-03-31
** Version:                 V1.0.0
** Descriptions:            调试打印
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2021-03-31
** Last version:            V1.0.0
** Description:             调试打印
**
*************************************************************/
#include "rec_def.h"
#include "run_demo.h"
//REC_START(PC_DATA_PUSH,0xFE,0x01)

//	uint32_t data = 3;
//	data = data;
//    REC_ITEM(REC_TYPE_UINT32,data);

//REC_END()

//REC_CACHE_START(AC_ADC,0xFE,0x02,200)
//    REC_CACHE_ITEM(AC_ADC,REC_TYPE_UINT16,ac_ad_v);
//    REC_CACHE_ITEM(AC_ADC,REC_TYPE_UINT16,ac_ad_i1);
//    REC_CACHE_ITEM(AC_ADC,REC_TYPE_UINT16,ac_ad_i2);
//    REC_CACHE_ITEM(AC_ADC,REC_TYPE_UINT16,ac_ad_i3);
//                   
//    REC_CACHE_ITEM(AC_ADC,REC_TYPE_FLOAT,ac_v);
//    REC_CACHE_ITEM(AC_ADC,REC_TYPE_FLOAT,ac_i1);
//    REC_CACHE_ITEM(AC_ADC,REC_TYPE_FLOAT,ac_i2);
//    REC_CACHE_ITEM(AC_ADC,REC_TYPE_FLOAT,ac_i3);
//                  
//    REC_CACHE_ITEM(AC_ADC,REC_TYPE_FLOAT,ac_p1);
//    REC_CACHE_ITEM(AC_ADC,REC_TYPE_FLOAT,ac_p2);
//    REC_CACHE_ITEM(AC_ADC,REC_TYPE_FLOAT,ac_p3);
//REC_CACHE_END(AC_ADC)

//REC_START(DA228,0xFE,0x02)
//    REC_ITEM(REC_TYPE_INT16,da228buf[0]);
//    REC_ITEM(REC_TYPE_INT16,da228buf[1]);
//    REC_ITEM(REC_TYPE_INT16,da228buf[2]);
//REC_END()
//extern uint16_t test_rec_var;
//REC_START(TEST_PUSH, 0x20, 0x07)            //
//    REC_ITEM(REC_TYPE_UINT16, test_rec_var);
//REC_END()

//extern uint8_t REC_TEST;
//REC_START(MTR_PUSH, 0x30, 0x07)            //
//    REC_ITEM(REC_TYPE_UINT8, REC_TEST);
//REC_END()

