/**
 * @brief 协议数据处理
 * @file protocol_ef_plus.c.xtemp
 * @version 1.0
 * @author hzf
 * @date 2022-9-5 14:35:37
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-9-5 14:35:37
 *   Author      : hzf
 *   Modification: Created file
 */


#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "string.h"
#include "check_algorithm.h"
#include "protocol_ef_plus.h"
#include "protocol_matrix.h"

static uint16_t get_header_len(uint8_t *pbuf, uint32_t len);
static uint16_t get_data_len(uint8_t *pbuf, uint32_t len);
static uint8_t check_header(uint8_t *pbuf, uint32_t len);
static uint8_t check_data(uint8_t *pbuf, uint32_t len);
static uint16_t unpack_data(uint8_t *p_raw_buf, hal_frame_info_t *p_frame_info);
static uint16_t pack_data_ef02(uint8_t *p_raw_buf, hal_frame_info_t *p_frame_info);
static uint16_t pack_data_ef03(uint8_t *p_raw_buf, hal_frame_info_t *p_frame_info);
__attribute__((weak)) uint16_t ef_plus_set_send_ack(hal_frame_info_t *p_frame_info, uint8_t *pbuf, uint16_t len);
__attribute__((weak)) void ef_plus_set_send_info(hal_frame_info_t *p_frame);

#pragma pack(1)
typedef struct
{
    uint8_t sof;              ///< byte 0
    uint8_t version : 4;      ///< 协议版本号
    uint8_t playload_ver : 4; ///< playload 版本号
    uint16_t len;             ///< byte 2 - 3
    uint8_t head_crc;         ///< byte 4
    uint8_t need_ack : 1;     ///< byte 5
    uint8_t is_ack : 1;
    uint8_t check_type : 3;
    uint8_t enc_type : 2;
    uint8_t seq_type : 1;
    uint32_t seq;     ///< byte 6 - 9
    uint16_t product; ///< byte 10 - 11
    uint8_t src;      ///< byte 12
    uint8_t dest;     ///< byte 13
    uint8_t d_src;    ///< byte 14
    uint8_t d_dest;   ///< byte 15
    uint8_t cmdfunc;  ///< byte 16
    uint8_t cmdid;    ///< byte 17
    uint8_t pdata[0]; ///< byte 18
} protocol_ef03_plus_t;

typedef struct
{
    uint8_t sof;              ///< byte 0
    uint8_t version : 4;      ///< 协议版本号
    uint8_t playload_ver : 4; ///< playload 版本号
    uint16_t len;             ///< byte 2 - 3
    uint8_t head_crc;         ///< byte 4

    uint8_t need_ack : 1; ///< byte 5
    uint8_t is_ack : 1;
    uint8_t check_type : 3;
    uint8_t enc_type : 2;
    uint8_t seq_type : 1;

    uint32_t seq;     ///< byte 6 - 9
    uint16_t product; ///< byte 10 - 11
    uint8_t src;      ///< byte 12
    uint8_t dest;     ///< byte 13
    uint8_t cmdfunc;  ///< byte 14
    uint8_t cmdid;    ///< byte 15
    uint8_t pdata[0]; ///< byte 16
} protocol_ef02_plus_t;

#pragma pack()

#define HEAD_SOF (0xAA)

#define HEAD_LEN (5)
#define EF2_BODY_LEN (sizeof(protocol_ef02_plus_t)) // 9+2 crc16
#define EF3_BODY_LEN (sizeof(protocol_ef03_plus_t)) // 9+2 crc16

REG_PROTOCOL(PROTOCOL_EF02_PLUS,
             get_header_len,
             get_data_len,
             check_header,
             check_data,
             pack_data_ef02,
             unpack_data,
             ef_plus_set_send_ack,
             NULL,
             ef_plus_set_send_info);

REG_PROTOCOL(PROTOCOL_EF03_PLUS,
             get_header_len,
             get_data_len,
             check_header,
             check_data,
             pack_data_ef03,
             unpack_data,
             ef_plus_set_send_ack,
             NULL,
             ef_plus_set_send_info);

__attribute__((weak)) uint16_t ef_plus_set_send_ack(hal_frame_info_t *p_frame_info, uint8_t *pbuf, uint16_t len)
{
    return 1;
}

__attribute__((weak)) void ef_plus_set_send_info(hal_frame_info_t *p_frame)
{
}

static uint16_t get_header_len(uint8_t *pbuf, uint32_t len)
{
    return HEAD_LEN + 1;
}

static uint16_t get_data_len(uint8_t *pbuf, uint32_t len)
{
    uint16_t data_len = (pbuf[3] << 8) + pbuf[2];
    uint16_t body_len = ((pbuf[1] & 0x0F) == VERSION_EF2) ? (EF2_BODY_LEN) : (EF3_BODY_LEN);
    return body_len + data_len + 2;
}

static uint8_t check_header(uint8_t *pbuf, uint32_t len)
{
    if ((pbuf[0] == HEAD_SOF) && ((pbuf[1] & 0x0F) == VERSION_EF2 || (pbuf[1] & 0x0F) == VERSION_EF3))
    {
        uint32_t head_chk = 0;
        head_chk = calc_chk_val(CRC8_CHECK_TYPE, pbuf, HEAD_LEN - 1);
        if (head_chk == pbuf[4])
        {
            return 1;
        }
    }
    return 0;
}

static uint8_t check_data(uint8_t *pbuf, uint32_t len)
{
    uint16_t data_len = (pbuf[3] << 8) + pbuf[2];
    uint8_t *pbuf_check = NULL;
    uint32_t all_pack_len;
    uint8_t check_type = (pbuf[5] >> 2) & 0x07;
    uint16_t body_len = ((pbuf[1] & 0x0F) == VERSION_EF2) ? (EF2_BODY_LEN) : (EF3_BODY_LEN);

    all_pack_len = body_len + data_len + 2;
    pbuf_check = &pbuf[all_pack_len - 2];
    //-------------chk
    {
        uint32_t frame_chk = 0;
        uint16_t chk = (pbuf_check[1] << 8) + pbuf_check[0];
        frame_chk = calc_chk_val(check_type, pbuf, all_pack_len - 2);
        if (frame_chk == chk)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

static void __encrypt_data(hal_frame_info_t *p_frame_info, uint8_t *p_buf, uint8_t crc8_val)
{
    uint8_t enc_type = p_frame_info->enc_type & (0x03);

    if(enc_type == XOR_ENC_TYPE){
        uint32_t i = 0;
		for(i = 0; i < p_frame_info->data_len; i++){
			p_buf[i] = p_frame_info->pdata[i] ^ p_frame_info->seq;
		}
    }else if(enc_type == XOR_ENC2_TYPE){
        uint8_t key[3] = {
            [0] = ( p_frame_info->seq ^ (~crc8_val) ) >> (p_frame_info->seq & 0x03),
            [1] = ( (~p_frame_info->seq) ^ (~crc8_val) ) >> (p_frame_info->data_len & 0x03),
            [2] = ( (~p_frame_info->seq) ^ (crc8_val) ) >> ((p_frame_info->data_len ^ p_frame_info->seq) & 0x03)
        };

        uint32_t i = 0;
        for(i = 0; i < p_frame_info->data_len; i++){
            p_buf[i] = p_frame_info->pdata[i] ^ key[i % 3];
            key[i % 3] ^= p_buf[i];
        }
    }else{
        memcpy(p_buf, p_frame_info->pdata, p_frame_info->data_len);
    }
}

static void __decrypt_data(hal_frame_info_t *p_frame_info, uint8_t *p_buf, uint8_t crc8_val)
{
    uint8_t enc_type = p_frame_info->enc_type & (0x03);

    if(enc_type == XOR_ENC_TYPE){
        uint32_t i = 0;
		for(i = 0; i < p_frame_info->data_len; i++){
			p_buf[i] = p_frame_info->pdata[i] ^ p_frame_info->seq;
		}
    }else if(enc_type == XOR_ENC2_TYPE){
        uint8_t key[6] = {
            [0] = ( p_frame_info->seq ^ (~crc8_val) ) >> (p_frame_info->seq & 0x03),
            [1] = ( (~p_frame_info->seq) ^ (~crc8_val) ) >> (p_frame_info->data_len & 0x03),
            [2] = ( (~p_frame_info->seq) ^ (crc8_val) ) >> ((p_frame_info->data_len ^ p_frame_info->seq) & 0x03)
        };

        uint32_t i = 0;
        uint8_t index;
        uint8_t temp;
        for(i = 0; i < p_frame_info->data_len; i++){
            index = i % 3;
            temp = key[index] ^ p_frame_info->pdata[i];
            p_buf[i] = p_frame_info->pdata[i] ^ key[index];
            key[index] = temp;
        }
    }
}

static uint16_t pack_data(uint8_t *p_raw_buf, hal_frame_info_t *p_frame_info)
{
    uint8_t *pcheck_buf = NULL;
    uint16_t pack_len = 0;
    uint32_t crc16_val = 0;
    uint8_t *pbuf = p_raw_buf;
    uint8_t num = 0;
    uint16_t body_len = (p_frame_info->version == VERSION_EF2) ? (EF2_BODY_LEN) : (EF3_BODY_LEN);

    pbuf[num++] = HEAD_SOF;
    pbuf[num] = p_frame_info->version & 0x0F;
    pbuf[num++] |= (p_frame_info->playload_ver & 0x0F) << 4;
    pbuf[num++] = p_frame_info->data_len & 0xff;
    pbuf[num++] = p_frame_info->data_len >> 8;
    pbuf[num++] = calc_chk_val(CRC8_CHECK_TYPE, p_raw_buf, HEAD_LEN - 1) & 0xff;

    pbuf[num] = (p_frame_info->need_ack > 0) ? 1 : 0;
    pbuf[num] |= ((p_frame_info->is_ack > 0) ? 1 : 0) << 1;
    pbuf[num] |= (p_frame_info->check_type & 0x07) << 2;
    pbuf[num++] |= (p_frame_info->enc_type & (0x03)) << 5;

    pbuf[num++] = p_frame_info->seq & 0xff;
    pbuf[num++] = (p_frame_info->seq >> 8) & 0xff;
    pbuf[num++] = (p_frame_info->seq >> 16) & 0xff;
    pbuf[num++] = p_frame_info->seq >> 24;
    pbuf[num++] = p_frame_info->product_id & 0xff; // CUR_PRODUCT_ID;
    pbuf[num++] = p_frame_info->product_id >> 8;   // CUR_PRODUCT_ID;

    //------------------data
    pbuf[num++] = p_frame_info->src;
    pbuf[num++] = p_frame_info->dest;
    if (p_frame_info->version == VERSION_EF3)
    {
        pbuf[num++] = p_frame_info->d_src;
        pbuf[num++] = p_frame_info->d_dest;
    }
    pbuf[num++] = p_frame_info->cmd_func & 0xff;
    pbuf[num++] = p_frame_info->cmd_id & 0xff;

	//-------------------enc
    __encrypt_data(p_frame_info, &pbuf[num], pbuf[4]);

    pack_len = body_len + p_frame_info->data_len + 2;
    pcheck_buf = &pbuf[pack_len - 2];
    //----------------check
    crc16_val = calc_chk_val(p_frame_info->check_type, p_raw_buf, pack_len - 2);
    pcheck_buf[0] = crc16_val & 0xff;
    pcheck_buf[1] = (crc16_val >> 8) & 0xff;
    return pack_len;
}

static uint16_t pack_data_ef02(uint8_t *p_raw_buf, hal_frame_info_t *p_frame_info)
{
    p_frame_info->version = (p_frame_info->version == VERSION_EF3) ? (VERSION_EF3) : (VERSION_EF2);
    return pack_data(p_raw_buf, p_frame_info);
}

static uint16_t pack_data_ef03(uint8_t *p_raw_buf, hal_frame_info_t *p_frame_info)
{
    p_frame_info->version = (p_frame_info->version == VERSION_EF2) ? (VERSION_EF2) : (VERSION_EF3);
    return pack_data(p_raw_buf, p_frame_info);
}

static uint16_t unpack_data(uint8_t *p_raw_buf, hal_frame_info_t *p_frame_info)
{
    uint8_t *pbuf = p_raw_buf;
    uint8_t num = 14;

    p_frame_info->enc_type = (pbuf[5] >> 5) & 0x03;
    p_frame_info->check_type = (pbuf[5] >> 2) & 0x07;

    p_frame_info->data_len = pbuf[2] + (pbuf[3] << 8);

    p_frame_info->version = pbuf[1] & 0x0F;
    p_frame_info->playload_ver = (pbuf[1] >> 4) & 0x0F;

    p_frame_info->is_ack = (pbuf[5] >> 1) & 0x01;
    p_frame_info->need_ack = pbuf[5] & 0x01;

    p_frame_info->seq = pbuf[6] + (pbuf[7] << 8) + (pbuf[8] << 16) + (pbuf[9] << 24);
    p_frame_info->product_id = pbuf[10] + (pbuf[11] << 8);
    p_frame_info->src = pbuf[12];
    p_frame_info->dest = pbuf[13];

    if (p_frame_info->version == VERSION_EF3)
    {
        p_frame_info->d_src = pbuf[num++];
        p_frame_info->d_dest = pbuf[num++];
    }

    p_frame_info->cmd_func = pbuf[num++];
    p_frame_info->cmd_id = pbuf[num++];
    
    p_frame_info->pdata				= &pbuf[num];
	
	//-------------------enc
    __decrypt_data(p_frame_info, &pbuf[num], pbuf[4]);
    return 1;
}

