/**
 * @brief protocol_ef_plus.c.xtemp header file
 * @file protocol_ef_plus.h
 * @version 1.0
 * @author hzf
 * @date 2022-9-5 14:44:34
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-9-5 14:44:34
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __PROTOCOL_EF_PLUS_H__
#define __PROTOCOL_EF_PLUS_H__

#define VERSION_EF2 (2) // 协议版本2
#define VERSION_EF3 (3) // 协议版本3

#define PLAYLOAD_BYTE (0)     // playload 字节流
#define PLAYLOAD_PROTOBUF (1) // playload protobuf

#define NONE_CHECK (0xBB)

#define NON_CHECK_TYPE 0x00   // 不校验
#define SUM_CHECK_TYPE 0x01   // 和校验
#define CRC8_CHECK_TYPE 0x02  // 8位CRC校验
#define CRC16_CHECK_TYPE 0x03 // 16位CRC校验

#define NON_ENC_TYPE 0x00 // 不加密
#define XOR_ENC_TYPE 0x01 // 异或加密
#define XOR_ENC2_TYPE 0x03 // 异或加密

#define CHECK_TYPE CRC16_CHECK_TYPE
#define ENC_TYPE XOR_ENC_TYPE


#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif // __cplusplus

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif // __cplusplus

#endif // __PROTOCOL_EF_PLUS_H__
