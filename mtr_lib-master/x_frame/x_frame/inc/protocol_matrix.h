/**
 * @brief protocol_matrix.c header file
 * @file protocol_matrix.h
 * @version 1.0
 * @author hzf
 * @date 2022-8-26 11:6:55
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-8-26 11:6:55
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __PROTOCOL_MATRIX_H__
#define __PROTOCOL_MATRIX_H__

#include "line_buf.h"

/**
 * @brief 应用层协议数据结构体
 * @details
 */
typedef struct
{
    uint8_t link_id;      // 链路ID
    uint8_t src;          // 源地址
    uint8_t dest;         // 目标地址
    uint8_t d_src;        // 动态源地址
    uint8_t d_dest;       // 动态目标地址
    uint8_t enc_type;     // 加密类型（NON_ENC_TYPE或0x00、XOR_ENC_TYPE或0x01）
    uint8_t check_type;   // 校验类型
    uint8_t version;      // 版本
    uint8_t playload_ver; // playload 版本号
    uint16_t cmd_func;    // 命令组id
    uint16_t cmd_id;      // 命令ID
    uint16_t data_len;    // 数据长度
    uint8_t need_ack;     // 是否需要应答（1：需应答）
    uint8_t is_ack;       // 本包是否为应答包（1：应答包）
    uint8_t ack_type;     // 应答类型
    uint8_t *pdata;       // 数据地址
    uint32_t seq;         // 包数记录
    uint32_t time_snap;   // 时间间隔
    uint8_t is_rw_cmd;    // 读写命令
    uint8_t is_queue;     // 队列
    uint16_t product_id;  // 产品ID
    uint32_t start_id;    // 起始ID
    uint32_t start_index; // 起始地址
    uint32_t end_id;      // 结束ID
    uint32_t end_index;   // 结束地址
} hal_frame_info_t;

//--------protocol     声明协议的函数指针类型
typedef uint16_t (*pf_get_header_len)(uint8_t *p_raw_buf, uint32_t len);
typedef uint16_t (*pf_get_data_len)(uint8_t *p_raw_buf, uint32_t len);
typedef uint8_t (*pf_check_header)(uint8_t *p_raw_buf, uint32_t len);
typedef uint8_t (*pf_check_data)(uint8_t *p_raw_buf, uint32_t len);
typedef uint16_t (*pf_pack_data)(uint8_t *p_raw_buf, hal_frame_info_t *p_frame_info);
typedef uint16_t (*pf_unpack_data)(uint8_t *p_raw_buf, hal_frame_info_t *p_frame_info);
typedef uint16_t (*pf_send_ack)(hal_frame_info_t *p_frame_info, uint8_t *pbuf, uint16_t len);
typedef int32_t (*pf_check_route)(hal_frame_info_t *p_frame, uint8_t *p_tag_link_id);
typedef void (*pf_set_send_info)(hal_frame_info_t *p_frame);

/**
 * @brief 协议结构体
 * @details
 */
typedef struct _protocol_obj_t
{
    pf_get_header_len get_head_len; // 获取包头长度函数的指针
    pf_get_data_len get_pack_len;   // 获取包长度函数的指针
    pf_check_header check_header;   // 包头校验函数的指针
    pf_check_data check_data;       // 数据校验函数的指针
    pf_pack_data pack_data;         // 数据打包函数的指针
    pf_unpack_data unpack_data;     // 数据解包函数的指针
    pf_send_ack send_ack;           // 发送应答函数的指针
    pf_check_route check_route;     // 路由检查函数的指针
    pf_set_send_info set_send_info; // 打包发送的数据的函数指针
} protocol_obj_t;

typedef struct
{
    void *protocol;
    uint32_t (*pfn_get_time_ms)(void);
    linebuf_t line_buf_obj;         ///< 源数据管理队列
    linebuf_t id_buf_obj;           ///< 数据包的id及index管理队列
    uint32_t input_read_index;      ///< 当前数据包的未读取数据索引值
    uint32_t last_id;               ///< 上一次数据包的id
    uint32_t start_id;              ///< 当前解析的协议帧的起始数据包
    uint32_t start_index;           ///< 当前解析的协议帧的起始数据包的索引值
    uint32_t unpack_time_ms;        ///< 解析head成功时的时间点，用来做超时处理，单位：ms
    uint32_t timeout;               ///< 超时时间，单位：ms
    uint8_t unpack_step;            ///< 当前解包步骤
    uint8_t rec_en:1;               ///< 协议帧信息记录使能标志
    uint8_t init_flag:1;            ///< 初始化标志
}protocol_matrix_t;

#define MATRIX_ID_DATA_LEN (sizeof(uint32_t))
#define MATRIX_ID_TOTAL_LEN (3 * sizeof(uint32_t)) ///< id管理数据组成分别为：id、len、index

/**
 * @brief 协议注册宏
 *
 * @param [in] protocol_ver            // 协议版本
 * @param [in] get_head_len            // 注册到协议结构体中的函数
 * @param [in] ...                     // 同上
 *
 * @details 特殊说明:    
 * @par eg:   //注册的函数需提前写好函数定义
 * @code
 *           REG_PROTOCOL( PROTOCOL_EF03_PLUS, get_header_len, get_data_len, check_header, check_data, pack_data_ef03, unpack_data, NULL, NULL, NULL);
 * @endcode
 */
#define REG_PROTOCOL(protocol_ver,get_head_len,get_pack_len,check_head,check_pack,pack_data,unpack_data,send_ack,check_route,set_send_info) \
            protocol_obj_t protocol_ver##_obj={get_head_len,get_pack_len,check_head,check_pack,pack_data,unpack_data,send_ack,check_route,set_send_info};


/**
 * @brief 编解码功能定义，开辟编解码需要的缓存空间
 *
 * @param [in] name                 // 功能命名
 * @param [in] protocol_ver         // 协议版本
 * @param [in] buff_len             // 缓存大小
 * @param [in] record_en            // 数据包id记录使能
 * @param [in] timeout_ms           // 解码超时时间，单位：ms
 * @param [in] get_time_ms_cb       // 获取当前时间戳回调函数
 *
 * @details 特殊说明:    
 * @par eg:
 * @code
 *           DEF_PROTOCOL_MATRIX(link_uart,PROTOCOL_EF03_PLUS,512,false,500,get_sys_ms);
 * @endcode
 */
#define DEF_PROTOCOL_MATRIX(name,protocol_ver,buff_len,record_en,timeout_ms,get_time_ms_cb) \
    extern protocol_obj_t protocol_ver##_obj;                 \
    static uint8_t dat_buff_##name[buff_len] = {0};  \
    static uint8_t id_buff_##name[(record_en) ? (buff_len * MATRIX_ID_TOTAL_LEN) : (0)];\
    protocol_matrix_t __matrix_##name = {   \
                    .protocol = (void*)&protocol_ver##_obj,   \
                    .pfn_get_time_ms = get_time_ms_cb,          \
                    .line_buf_obj = {NULL,dat_buff_##name,buff_len}, \
                    .id_buf_obj = {NULL, id_buff_##name, (record_en) ? (buff_len * MATRIX_ID_TOTAL_LEN) : (0)},   \
                    .rec_en = record_en,    \
                    .timeout = (timeout_ms) ? (timeout_ms) : (500)};

/**
 * @brief 获取编解码功能结构体数据指针
 *
 * @param [in] name                 // 功能命名
 *
 * @details 特殊说明:    
 * @par eg:
 * @code
 *          DEF_PROTOCOL_MATRIX(link_uart,PROTOCOL_EF03_PLUS,512,false,500,get_sys_ms);
 *
 *          void decode_task(void)
 *          {
 *              hal_frame_info_t frame;
 *              uint8_t read_buf[512];  ///< 用做原始数据流的缓存
 *              uint16_t read_len;
 *
 *              ...
 *              protocol_decode(PROTOCOL_MATRIX_PTR(link_uart), read_buf, read_len, 0, &frame);
 *          }
 *          
 * @endcode
 */
#define PROTOCOL_MATRIX_PTR(name) (protocol_matrix_t*)&__matrix_##name

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif // __cplusplus

/**
 * @brief  协议解码
 *
 * @param [in] matrix       协议组结构体指针
 * @param [in] data         解包后得到的数据缓存指针
 * @param [in] len          数据长度
 * @param [in] id
 * @param [in] frame        协议包参数指针
 *
 * @return int32_t          返回值：大于等于0解包成功，小于0解包失败；
 *  @retval
 * @details 特殊说明:
 * @par eg:
 * @code
 *    //  先注册链路：
 *    REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路
 *    //  使用：
 *    uint8_t recv_buf[MAX_RECV_PACK_LEN] = {0};            // MAX_RECV_PACK_LEN  接收包的最大长度，框架框架配置值为： 512
 *    void protocol_decode_test(void)
 *    {
 *        ...
 *        //
 *        uint32_t read_len = MAX_RECV_PACK_LEN;
 *        uint32_t ret = -1；
 *        hal_frame_info_t frame_info_t = {0}；
 *        link_hal_item  *p_link = NULL;
 *        p_link = find_link(LINK_UART1);
 *        real_len = xh_stream_read(p_link->read_dev, recv_buf, read_len);                  //接收协议数据包数据流
 *        frame_info.link_id = p_link->link_id;                                             //将接收数据流的link_id放入到协议包中
 *        ret = protocol_decode(p_link->matrix, recv_buf, real_len, id, &frame_info_t)；
 *        //
 *        ...
 *    }
 * @endcode
 */
extern int32_t protocol_decode(protocol_matrix_t *matrix, uint8_t *data, uint32_t len, uint32_t id, hal_frame_info_t *frame);

/**
 * @brief  协议编码
 *
 * @param [in] matrix
 * @param [in] frame     协议包参数指针
 * @param [in] buff      编码后的数据缓存指针
 *
 * @return uint16_t      包长
 * @retval
 * @details 特殊说明:
 * @par eg:
 * @code
 *    //  先注册链路：
 *    REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路
 *    //  使用：
 *    uint8_t send_buff = {0};
 *    void protocol_encode_test(void)
 *    {
 *        ...
 *        //
 *        uint16_t pack_len = 0；
 *        hal_frame_info_t p_frame_info_t = {....(结构体数据填充)}；
 *        hal_frame_info_t *p_frame_info_test = &p_frame_info_t；
 *        link_hal_item  *p_link = NULL;
 *        p_link = find_link(p_frame_info->link_id);
 *        pack_len = protocol_encode(p_link->matrix,p_frame_info_test,send_buff)；
 *        //
 *        ...
 *    }
 * @endcode
 */
extern uint16_t protocol_encode(protocol_matrix_t *matrix, hal_frame_info_t *frame, uint8_t *buff);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif // __cplusplus

#endif // __PROTOCOL_MATRIX_H__
