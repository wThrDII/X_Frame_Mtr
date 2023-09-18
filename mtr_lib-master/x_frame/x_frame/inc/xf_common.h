/**
 * @brief 通用宏定义
 * @file xf_common.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 15:43:50
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 15:43:50
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __XF_COMMON_H__
#define __XF_COMMON_H__

#define SWAP_U16(a)  ((a>>8)|((a & 0x00ff)<<8))		/* 交换16位数据高低位 */
#define SWAP_U32(a)  ((a >> 24) | ((a & 0x00ff0000) >> 8) | ((a & 0x0000ff00) << 8) | (a << 24))	/* 交换32位数据的高低位 */

#define ARRY_ITEMS_NUM(n)	(sizeof(n)/sizeof(n[0]))	/* 计算数组元素个数 */

#define ALIGN(n)    __attribute__((aligned (n)))
#define PACK_8      __attribute__((aligned(8)))
#define PACK_4      __attribute__((aligned(4)))

#define STRIP_2N(num)       ((num <= 2) ? (2) : \
                             (num <= 4) ? (4) : \
                             (num <= 8) ? (8) : \
                             (num <= 16) ? (16) : \
                             (num <= 32) ? (32) : \
                             (num <= 64) ? (64) : \
                             (num <= 128) ? (128) : \
                             (num <= 256) ? (256) : \
                             (num <= 512) ? (512) : \
                             (num <= 1024) ? (1024) : \
                             (num <= 2048) ? (2048) : \
                             (num <= 4096) ? (4096) : \
                             (num <= 8192) ? (8192) : \
                             (num <= 16384) ? (16384) : \
                             (num <= 32768) ? (32768) : \
                             (num <= 65536) ? (65536) : \
                             (0xFFFFFFFFFF))

#define COMP_TIME 	__TIME__		/* 程序编译的时间，字符串 */
#define COMP_DATE 	__DATE__		/* 程序编译的日期，字符串 */

#define X_FRAME_WEAK    __attribute__((weak))
#define X_FRAME_WEAK_EXT(func)

#define X_FRAME_MIN(a,b) (((a) < (b)) ? (a) : (b))
#define X_FRAME_MAX(a,b) (((a) < (b)) ? (b) : (a))

#define X_FRAME_BIT_SET(var,bit)	(var |= 0x01U << (bit))
#define X_FRAME_BIT_RESET(var,bit)	(var &= ~(0x01U << (bit)))

#define X_MAKE_VERSION(main_ver, sub_ver, dbg_ver, test_ver)        (((main_ver)<<24) | ((sub_ver)<<16) | ((dbg_ver)<<8) | ((test_ver)))
#define X_GET_MAIN_VERSION(ver)                                     (((ver)>>24)&0xFF)
#define X_GET_SUB_VERSION(ver)                                      (((ver)>>16)&0xFF)
#define X_GET_DBG_VERSION(ver)                                      (((ver)>>8)&0xFF)
#define X_GET_TEST_VERSION(ver)                                     (((ver))&0xFF)

#define _X_UID_TYPE				void* const
#define _X_UID_VAL(id)			((void*)&g_32_##id)
#define _X_UID_VAR_VAL(id)		(g_32_##id)
#define _DEF_X_UID(id)			void* g_32_##id;	_X_UID_TYPE id = _X_UID_VAL(id);
#define _EXT_X_UID(id)			extern void* g_32_##id; extern _X_UID_TYPE id;

#define X_UID_TYPE              _X_UID_TYPE					/* UID 类型 */
#define DEF_X_UID(id)			_DEF_X_UID(id)				/* 定义UID */
#define EXT_X_UID(id)			_EXT_X_UID(id)				/* 声明UID */
#define X_UID_VAL(id)			_X_UID_VAL(id)				/* UID 唯一值 */
#define X_UID_VAR_VAL(id)		_X_UID_VAR_VAL(id)			/* UID 变量值 */

typedef int xf_ret;
#define XF_OK			(0)
#define XF_COM_ERROR	(-1)
#define XF_NOT_FOUND    (-2)

/* 提示符，说明是可重写函数 */
#define API_WEAK
#define RET_OK    0

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __XF_COMMON_H__
