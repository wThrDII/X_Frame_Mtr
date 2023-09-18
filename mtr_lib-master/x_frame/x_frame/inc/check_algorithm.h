/**
 * @brief check_algorithm.c header file
 * @file check_algorithm.h
 * @version 1.0
 * @author hzf
 * @date 2022-9-22 9:47:58
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-9-22 9:47:58
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __CHECK_ALGORITHM_H__
#define __CHECK_ALGORITHM_H__

enum
{
    u_chk_none = 0,
    u_chk_sum  = 1,
    u_chk_crc8 = 2,
    u_chk_crc16 = 3,
    u_chk_err,
};

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus


/**
 * @brief 校验,可设定初值
 *
 * @param [in] chk_type 选择校验类型
 *             e.g:   u_chk_none = 0,
 *                    u_chk_sum  = 1,
 *                    u_chk_crc8 = 2,
 *                    u_chk_crc16 = 3,
 *                    u_chk_err
 * @param [in] init_val  校验的初值
 * @param [in] pbuf  校验的数据指针
 * @param [in] len   校验长度
 *
 * @return uint32_t 校验结果
 *  @retval
 *      0xffff: 校验错误，没有对应的校验类型
 *      其他  : 校验结果
 * @details:
 * @par e.g:
 * @code
 *      #define USER_CHK_INIT_VAl   10
 *      uint8_t user_data[50] = {...};
 *      uint32_t chk_val;
 *
 *      calc_chk_val(u_chk_crc8,USER_CHK_INIT_VAl,user_data,50);
 * @endcode
 */
extern uint32_t calc_chk_init_val(uint8_t chk_type,uint32_t init_val,uint8_t* pbuf,uint32_t len);

/**
 * @brief 校验
 *
 * @param [in] chk_type 选择校验类型
 *             e.g:   u_chk_none = 0,
 *                    u_chk_sum  = 1,
 *                    u_chk_crc8 = 2,
 *                    u_chk_crc16 = 3,
 *                    u_chk_err,
 * @param [in] pbuf  校验的数据指针
 * @param [in] len   校验长度
 *
 * @return uint32_t 校验结果
 *  @retval
 *      0xffff: 校验错误，没有对应的校验类型
 *      其他  : 校验结果
 * @details:
 * @par e.g:
 * @code
 *      uint8_t user_data[50] = {...};
 *      uint32_t chk_val;
 *
 *      calc_chk_val(u_chk_crc8,user_data,50);
 * @endcode
 */
extern uint32_t calc_chk_val(uint8_t chk_type,uint8_t* pbuf,uint32_t len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __CHECK_ALGORITHM_H__
