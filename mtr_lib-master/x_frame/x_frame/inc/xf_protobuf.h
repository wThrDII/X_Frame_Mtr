
/**
 * @brief x_frame protobuf 适配层
 * @file xf_protobuf.h
 * @version 1.0
 * @author juntong.chen
 * @date 2022-10-27 14:16:51
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @details 文件详细描述，包含使用场景，依赖项等
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2022-10-27 14:16:51      <td>juntong.chen      <td>创建文件
 */
#ifndef XF_PROTOBUF_H
#define XF_PROTOBUF_H
#ifdef __cplusplus
extern "C" {
#endif

#include "protocol_hal.h"
#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

#define PROTOBUF_BUF_MAX_LEN (1024)

/**
 * @brief protobuf 解码
 * @param [in]  proto_name      proto 解码结构体名称
 * @param [in]  de_ptr          待解码数据指针
 * @param [in]  de_len          待解码数据长度
 * @param [out] proto_struct    proto 解码结构体 - 存放解码后的数据
 * @param [out] ret             解码结果<type>:bool 0:解码失败，1:解码成功
 *
 * @details de_len = 0，不论缓存数据正确与否直接会返回编码失败
 *
 * @par 示例:
 * @code
 *      proto_name proto;
 *      uint32_t ret    =   0;
 *      DECODE_PROTO(proto_name,rev_buff,rec_len,proto,ret);
 *      if(ret){
 *          ///< 解码成功，开始处理数据
 *      }
 *      else{
 *          ///< 解码失败，发送警告
 *      }
 * @endcode
 */
#define DECODE_PROTO(proto_name, de_ptr, de_len, proto_struct, ret) \
    pb_istream_t stream = pb_istream_from_buffer(de_ptr, de_len);   \
    ret = pb_decode(&stream, proto_name##_fields, &proto_struct);

/**
 * @brief protobuf 编码
 *
 * @param [in]  proto_name          proto 解码结构体名称
 * @param [in]  p_proto_struct      赋值完，待编码的 proto 结构体指针
 * @param [out] buff                编码缓存，编码完成后的数据存在的地方
 * @param [in]  buff_size           缓存长度
 * @param [out] en_len              编码后实际数据长度
 *
 * @return uint32_t 编码后数据长度
 *  @retval 0：编码失败，n:编码后数据长度
 *
 * @details 缓存长度=0；函数会直接返回编码失败
 *
 * @par 示例:
 * @code
 *      proto_name proto = {0};
 *      proto.1 = x;
 *      proto.2 = y;
 *      memcpy(&proto.3,&z,sizeof(proto.3));
 *      uint32_t en_len = 0;
 *      uint8_t send_buf[1200];
 *      uint32_t buf_len = 1200;
 *      ENCODE_PROTOBUF_PTR(proto_name,&proto,send_buf,buf_len,en_len);
 *      if(en_len)
 *      {
 *          //编码成功，按照编码长度<en_len>发送数据
 *      }
 *      else
 *      {
 *          //编码失败，发送报警
 *      }
 * @endcode
 */
#define ENCODE_PROTOBUF_PTR(proto_name, proto_struct, buff, buff_size, en_len, en_flag) \
    if (buff_size != 0) {                                                               \
        pb_ostream_t stream = pb_ostream_from_buffer(buff, buff_size);                  \
        if (pb_encode(&stream, proto_name##_fields, proto_struct)) {                    \
            en_len = stream.bytes_written; /* Success */                                \
            en_flag = 1;                                                                \
        }                                                                               \
        else {                                                                          \
            en_len = 0; /* Failure */                                                   \
            en_flag = 0;                                                                \
        }                                                                               \
    }                                                                                   \
    else {                                                                              \
        en_len = 0;                                                                     \
        en_flag = 0;                                                                    \
    }

/**
 * @brief protobuf 编码
 *
 * @param [in]  proto_name          proto 解码结构体名称
 * @param [in]  proto_struct        赋值完，待编码的 proto 结构体
 * @param [out] buff                编码缓存，编码完成后的数据存在的地方
 * @param [in]  buff_size           缓存长度
 * @param [out] en_len              编码后实际数据长度
 *
 * @return uint32_t 编码后数据长度
 *  @retval 0：编码失败，n:编码后数据长度
 *
 * @details 缓存长度=0；函数会直接返回编码失败
 *
 * @par 示例:
 * @code
 *      proto_name proto = {0};
 *      proto.1 = x;
 *      proto.2 = y;
 *      memcpy(&proto.3,&z,sizeof(proto.3));
 *      uint32_t en_len = 0;
 *      uint8_t send_buf[1200];
 *      uint32_t buf_len = 1200;
 *      ENCODE_PROTO(proto_name,proto,send_buf,buf_len,en_len);
 *      if(en_len)
 *      {
 *          //编码成功，按照编码长度<en_len>发送数据
 *      }
 *      else
 *      {
 *          //编码失败，发送报警
 *      }
 * @endcode
 */
#define ENCODE_PROTO(proto_name, proto_struct, buff, buff_size, en_len, en_flag) \
    ENCODE_PROTOBUF_PTR(proto_name, &proto_struct, buff, buff_size, en_len, en_flag)

/**
 * @brief 基于 源函数 生成 proto 解码函数
 *
 * @param [in] proto_name           proto 的结构体名称
 * @param [in] func                 源函数
 * @param [in] proto_func_name      proto 函数名称
 *
 * @details 内部使用
 */
#define __GENERATE_PROTOBUF_DECODE_FUNC(proto_name, func, proto_de_func)                   \
    int32_t proto_de_func(hal_frame_info_t *p_frame_info)                                  \
    {                                                                                      \
        proto_name proto;                                                                  \
        uint8_t ret;                                                                       \
        DECODE_PROTO(proto_name, p_frame_info->pdata, p_frame_info->data_len, proto, ret); \
        if (ret == 0) {                                                                    \
            p_frame_info->data_len = 0;                                                    \
        }                                                                                  \
        else {                                                                             \
            p_frame_info->data_len = sizeof(proto_name);                                   \
        }                                                                                  \
        p_frame_info->pdata = (uint8_t *)&proto;                                           \
        return func(p_frame_info);                                                         \
    }

/**
 * @brief 注册 playload 为 proto 格式的链路回调函数，不区分指令还是应答
 *
 * @param [in] proto_name           proto 的结构体名称
 * @param [in] link_id              链路id
 * @param [in] cmd_set              指令集
 * @param [in] cmd_id               指令id
 * @param [in] func                 回调函数
 *
 * @details 当 playload 为空时，调用 REG_LINK_ACTION 注册即可
 *
 * @par eg: 注册 LINK_UART1 链路，指令集0x02, 指令0x01 的 回调函数 action_handle
 * @code
 *      int32_t action_handle(hal_frame_info_t *p_frame_info)
 *      {
 *			if(p_frame_info->data_len == 0){
 *			    return 0;		// 数据解析失败
 *			}
 *          proto_cmd_struct *p_proto_cmd = (proto_cmd_struct *)p_frame_info->pdata;
 *
 *          //....
 *
 *          return 0;
 *      }
 *      REG_LINK_PROTOBUF_ACTION(proto_cmd_struct, LINK_UART1, 0x02, 0x01, action_handle);
 * @endcode
 */
#define REG_LINK_PROTOBUF_ACTION(proto_name, link_id, cmd_set, cmd_id, func) \
    __GENERATE_PROTOBUF_DECODE_FUNC(proto_name, func, proto_name##func)      \
    REG_LINK_ACTION(link_id, cmd_set, cmd_id, proto_name##func)

/**
 * @brief 注册 playload 为 proto 格式的链路 指令 回调函数，is_ack 字段为 false
 *
 * @param [in] proto_name           proto 的结构体名称
 * @param [in] link_id              链路id
 * @param [in] cmd_set              指令集
 * @param [in] cmd_id               指令id
 * @param [in] func                 回调函数
 *
 * @details
 *      当 B设备 发送指令给 A设备 时，A接收指令的回调可以通过这个宏进行注册，当程序里面同时存在
 *      REG_LINK_PROTOBUF_ACTION 注册的回调时，REG_LINK_PROTOBUF_ACTION 的回调不会被调起
 *
 * @par eg: 注册 LINK_UART1 链路，指令集0x02, 指令0x01 的 回调函数 action_handle
 * @code
 *      int32_t action_handle(hal_frame_info_t *p_frame_info)
 *      {
 *			if(p_frame_info->data_len == 0){
 *			    return 0;		// 数据解析失败
 *			}
 *          proto_cmd_struct *p_proto_cmd = (proto_cmd_struct *)p_frame_info->pdata;
 *
 *          //....
 *
 *          return 0;
 *      }
 *      REG_LINK_PROTOBUF_ACTION_CMD(proto_cmd_struct, LINK_UART1, 0x02, 0x01, action_handle);
 * @endcode
 */
#define REG_LINK_PROTOBUF_ACTION_CMD(proto_name, link_id, cmd_set, cmd_id, func) \
    __GENERATE_PROTOBUF_DECODE_FUNC(proto_name, func, proto_name##func)          \
    REG_LINK_ACTION_CMD(link_id, cmd_set, cmd_id, proto_name##func)

/**
 * @brief 注册 playload 为 proto 格式的链路 应答 回调函数，is_ack 字段为 true
 *
 * @param [in] proto_name           proto 的结构体名称
 * @param [in] link_id              链路id
 * @param [in] cmd_set              指令集
 * @param [in] cmd_id               指令id
 * @param [in] func                 回调函数
 *
 * @details
 *      当 B设备 发送应答给 A设备 时，A接收应答的回调 可以通过这个宏进行注册，当程序里面同时存在
 *      REG_LINK_ACTION注册的回调时，REG_LINK_ACTION的回调不会被调起
 *
 * @par eg: 注册 LINK_UART1 链路，指令集0x02, 指令0x01 的 回调函数 action_handle
 * @code
 *      int32_t action_handle(hal_frame_info_t *p_frame_info)
 *      {
 *			if(p_frame_info->data_len == 0){
 *			    return 0;		// 数据解析失败
 *			}
 *          proto_cmd_struct *p_proto_cmd = (proto_cmd_struct *)p_frame_info->pdata;
 *
 *          //....
 *
 *          return 0;
 *      }
 *      REG_LINK_PROTOBUF_ACTION_ACK(proto_cmd_struct, LINK_UART1, 0x02, 0x01, action_handle);
 * @endcode
 */
#define REG_LINK_PROTOBUF_ACTION_ACK(proto_name, link_id, cmd_set, cmd_id, func) \
    __GENERATE_PROTOBUF_DECODE_FUNC(proto_name, func, proto_name##func)          \
    REG_LINK_ACTION_ACK(link_id, cmd_set, cmd_id, proto_name##func)

/**
 * @brief 注册指令集对应的id的回调函数
 *
 * @param [in] proto_name         proto 的结构体名称
 * @param [in] cmd_set            指令集
 * @param [in] cmd_id             指令id
 * @param [in] func               回调函数
 *
 * @details
 *      与 REG_LINK_PROTOBUF_ACTION 区别在于 REG_LINK_PROTOBUF_ACTION 与链路绑定，REG_CMDSET_PROTOBUF_ACTION
 *      与指令集绑定，指令集通过调起 REG_CMDSET_ACTION_LINK 与链路绑定，注册多个链路相同的指令集时，只需
 *      要调起 REG_CMDSET_ACTION_LINK 与链路绑定即可
 *
 * @par eg:
 * @code
 *      int32_t action_handle(hal_frame_info_t *p_frame_info)
 *      {
 *			if(p_frame_info->data_len == 0){
 *			    return 0;		// 数据解析失败
 *			}
 *          proto_cmd_struct *p_proto_cmd = (proto_cmd_struct *)p_frame_info->pdata;
 *
 *          //....
 *
 *          return 0;
 *      }
 *      REG_CMDSET_PROTOBUF_ACTION(proto_cmd_struct, 0x02, 0x01, action_handle);
 * @endcode
 */
#define REG_CMDSET_PROTOBUF_ACTION(proto_name, cmd_set, cmd_id, func)   \
    __GENERATE_PROTOBUF_DECODE_FUNC(proto_name, func, proto_name##func) \
    REG_CMDSET_ACTION(cmd_set, cmd_id, proto_name##func)

/**
 * @brief 注册指令集对应的id的 指令回调函数
 *
 * @param [in] proto_name         proto 的结构体名称
 * @param [in] cmd_set            指令集
 * @param [in] cmd_id             指令id
 * @param [in] func               回调函数
 *
 * @details
 *      1. 当 B设备 发送指令给 A设备 时，A接收指令的回调可以通过这个宏进行注册，当程序里面同时存在
 *      REG_CMDSET_PROTOBUF_ACTION 注册的回调时, REG_CMDSET_PROTOBUF_ACTION 的回调不会被调起
 *
 *      2. 与 REG_LINK_PROTOBUF_ACTION 区别在于 REG_LINK_PROTOBUF_ACTION 与链路绑定，REG_CMDSET_PROTOBUF_ACTION
 *      与指令集绑定，指令集通过调起 REG_CMDSET_ACTION_LINK 与链路绑定，注册多个链路相同的指令集时，只需
 *      要调起 REG_CMDSET_ACTION_LINK 与链路绑定即可
 *
 * @par eg:
 * @code
 *      int32_t action_handle(hal_frame_info_t *p_frame_info)
 *      {
 *			if(p_frame_info->data_len == 0){
 *			    return 0;		// 数据解析失败
 *			}
 *          proto_cmd_struct *p_proto_cmd = (proto_cmd_struct *)p_frame_info->pdata;
 *
 *          //....
 *
 *          return 0;
 *      }
 *      REG_CMDSET_PROTOBUF_ACTION_CMD(proto_cmd_struct, 0x02, 0x01, action_handle);
 * @endcode
 */
#define REG_CMDSET_PROTOBUF_ACTION_CMD(proto_name, cmd_set, cmd_id, func) \
    __GENERATE_PROTOBUF_DECODE_FUNC(proto_name, func, proto_name##func)   \
    REG_CMDSET_ACTION_CMD(cmd_set, cmd_id, LINK_ACTION_CMD_ACK, ACTION_GROUP_CMDSET, proto_name##func)

/**
 * @brief 注册指令集对应的id的 应答回调函数
 *
 * @param [in] proto_name         proto 的结构体名称
 * @param [in] cmd_set            指令集
 * @param [in] cmd_id             指令id
 * @param [in] func               回调函数
 *
 * @details
 *      1. 当 B设备 发送应答给 A设备 时，A接收应答的回调可以通过这个宏进行注册，当程序里面同时存在
 *      REG_CMDSET_PROTOBUF_ACTION 注册的回调时, REG_CMDSET_PROTOBUF_ACTION 的回调不会被调起
 *
 *      2. 与 REG_LINK_PROTOBUF_ACTION 区别在于 REG_LINK_PROTOBUF_ACTION 与链路绑定，REG_CMDSET_PROTOBUF_ACTION
 *      与指令集绑定，指令集通过调起 REG_CMDSET_ACTION_LINK 与链路绑定，注册多个链路相同的指令集时，只需
 *      要调起 REG_CMDSET_ACTION_LINK 与链路绑定即可
 *
 * @par eg:
 * @code
 *      int32_t action_handle(hal_frame_info_t *p_frame_info)
 *      {
 *			if(p_frame_info->data_len == 0){
 *			    return 0;		// 数据解析失败
 *			}
 *          proto_cmd_struct *p_proto_cmd = (proto_cmd_struct *)p_frame_info->pdata;
 *
 *          //....
 *
 *          return 0;
 *      }
 *      REG_CMDSET_PROTOBUF_ACTION_CMD(proto_cmd_struct, 0x02, 0x01, action_handle);
 * @endcode
 */
#define REG_CMDSET_PROTOBUF_ACTION_ACK(proto_name, cmd_set, cmd_id, func) \
    __GENERATE_PROTOBUF_DECODE_FUNC(proto_name, func, proto_name##func)   \
    REG_CMDSET_ACTION_ACK(cmd_set, cmd_id, LINK_ACTION_CMD_ACK, ACTION_GROUP_CMDSET, proto_name##func)

/**
 * @brief protobuf 发送指令的模板
 *
 * @param [in] frame_info  协议包参数指针
 * @param [in] proto_name    proto 的结构体名称
 * @param [in]  proto_struct  需要发送的 pprotobuf 结构体参数
 * @param [out] send_len     实际发送的数据长度
 * @param [in] send_func     发送函数指针
 *
 * @details 内部使用
 *
 */
#define __PROTOBUF_SEND_CMD_TEMPLATE(p_frame_info, proto_name, proto_struct, send_len, send_func)                                                      \
    {                                                                                                                                                \
                                                                                                                                                     \
        (proto_name##_size == 0) ? ({                                                                                                                \
            (p_frame_info)->playload_ver = PLAYLOAD_PROTOBUF;                                                                                             \
            (p_frame_info)->data_len = 0;                                                                                                                 \
            send_len = send_func(p_frame_info);                                                                                                       \
        })                                                                                                                                           \
                                 : ({                                                                                                                \
                                       uint8_t encode_flag = 0;                                                                                      \
                                       uint8_t proto_buf[(proto_name##_size > PROTOBUF_BUF_MAX_LEN) ? (PROTOBUF_BUF_MAX_LEN) : (proto_name##_size)]; \
                                       ENCODE_PROTOBUF_PTR(proto_name, proto_struct, proto_buf, sizeof(proto_buf), (p_frame_info)->data_len, encode_flag) \
                                       if (encode_flag != 0) {                                                                                       \
                                           (p_frame_info)->playload_ver = PLAYLOAD_PROTOBUF;                                                              \
                                           (p_frame_info)->pdata = proto_buf;                                                                             \
                                           send_len = send_func(p_frame_info);                                                                        \
                                       }                                                                                                             \
                                       else {                                                                                                        \
                                           send_len = 0;                                                                                             \
                                       }                                                                                                             \
                                   });                                                                                                               \
    }

/**
 * @brief 链路发送 protobuf 格式的数据，
 *
 * @param [in]  p_frame_info 协议包参数指针
 * @param [in]  proto_name   proto 的结构体名称
 * @param [in]  p_proto_struct  需要发送的 pprotobuf 结构体参数指针
 * @param [out] send_len     实际发送的数据长度
 *
 * @details
 *      接口默认填充本地机地址，产品信息等相关内容, 数据填充仅针对公司V2,V3协议，其它自
 *      定义协议不填充
 *
 * @par eg:往串口链路发送指令为0x02，0x01，目标地址为0x01， 数据为 proto_cmd_struct 的指令
 * @code
 *    void send_data_test(void)
 *    {
 *          uint16_t send_len;
 *          proto_name proto_cmd_struct = {0};
 *          proto_cmd_struct.count = 1;
 *          proto_cmd_struct.num = 2;
 *
 *	        hal_frame_info_t frame_info = {0};      // 必须初始化为 0
 *          frame_info.cmd_func         = 0x02;     // 0x01、0xFE、0xFF 不能使用
 *          frame_info.cmd_id           = 0x01;
 *          frame_info.dest             = 0x01;
 *          frame_info.d_dest           = 0x01;     // V3 协议使用，V2 协议不需要
 *          frame_info.link_id          = LINK_UART1; // 链路id
 *          PROTOBUF_SEND_BY_ID(&frame_info, proto_name, &proto_cmd_struct, send_len);
 *    }
 * @endcode
 */
#define PROTOBUF_SEND_BY_ID(p_frame_info, proto_name, p_proto_struct, send_len) \
    __PROTOBUF_SEND_CMD_TEMPLATE(p_frame_info, proto_name, p_proto_struct, send_len, protocol_send_by_id)

/**
 * @brief 发送 protobuf 格式的数据，
 *
 * @param [in] p_frame_info  协议包参数指针
 * @param [in] proto_name    protobuf的结构体名称
 * @param [in] p_proto_struct  需要发送的 pprotobuf 结构体参数指针
 * @param [out] send_len     实际发送的数据长度
 *
 * @details
 *      接口内部不填充任何相关数据
 *
 * @par eg:往串口链路发送指令为0x02，0x01，目标地址为0x01， 数据为 proto_cmd_struct 的指令
 * @code
 *    void send_data_test(void)
 *    {
 *          uint16_t send_len;
 *          proto_name proto_cmd_struct = {0};
 *          proto_cmd_struct.count = 1;
 *          proto_cmd_struct.num = 2;
 *
 *	        hal_frame_info_t frame_info = {0};      // 必须初始化为 0
 *          frame_info.cmd_func         = 0x02;     // 0x01、0xFE、0xFF 不能使用
 *          frame_info.cmd_id           = 0x01;
 *          p_frame_info->src           = HOST_ADDR;
 *          p_frame_info->d_src         = 0x01;
 *          p_frame_info->is_ack        = 0;
 *          p_frame_info->product_id    = CUR_PRODUCT_ID;
 *          frame_info.dest             = 0x01;
 *          frame_info.d_dest           = 0x01;     // V3 协议使用，V2 协议不需要
 *          frame_info.link_id          = LINK_UART1; // 链路id
 *          PROTOBUF_SEND_FRAME(&frame_info, proto_name, &proto_cmd_struct, send_len);
 *    }
 * @endcode
 */
#define PROTOBUF_SEND_FRAME(p_frame_info, proto_name, p_proto_struct, send_len) \
    __PROTOBUF_SEND_CMD_TEMPLATE(p_frame_info, proto_name, p_proto_struct, send_len, protocol_send_frame)

/**
 * @brief 发送 protobuf 格式的数据，
 *
 * @param [in] p_frame_info  协议包参数指针
 * @param [in] proto_name    protobuf的结构体名称
 * @param [in] p_proto_struct  需要发送的 pprotobuf 结构体参数指针
 * @param [out] send_len     实际发送的数据长度
 *
 * @details
 *      1.  接口默认填充本地机地址，产品信息等相关内容, 数据填充仅针对公司V2,V3协议，其它自
 *          定义协议不填充
 *      2.  会调起路由表，如果路由表存在相应的路由规则,则会根据路由表的信息修改发送的 link_id
 *
 * @par eg:往串口链路发送指令为0x02，0x01，目标地址为0x01， 数据为 proto_cmd_struct 的指令
 * @code
 *    void send_data_test(void)
 *    {
 *          uint16_t send_len;
 *          proto_name proto_cmd_struct = {0};
 *          proto_cmd_struct.count = 1;
 *          proto_cmd_struct.num = 2;
 *
 *	        hal_frame_info_t frame_info = {0};      // 必须初始化为 0
 *          frame_info.cmd_func         = 0x02;     // 0x01、0xFE、0xFF 不能使用
 *          frame_info.cmd_id           = 0x01;
 *          frame_info.dest             = 0x01;
 *          frame_info.d_dest           = 0x01;     // V3 协议使用，V2 协议不需要
 *          frame_info.link_id          = LINK_UART1; // 链路id, 可能被修改
 *          PROTOBUF_SEND(&frame_info, proto_name, &proto_cmd_struct, send_len);
 *    }
 * @endcode
 */
#define PROTOBUF_SEND(p_frame_info, proto_name, p_proto_struct, send_len) \
    __PROTOBUF_SEND_CMD_TEMPLATE(p_frame_info, proto_name, p_proto_struct, send_len, protocol_send)

/**
 * @brief 响应 protobuf 格式的数据，
 *
 * @param [in] p_frame_info  协议包参数指针
 * @param [in] proto_name    protobuf 的结构体名称
 * @param [in] p_proto_struct  需要应答的 pprotobuf 结构体参数指针
 * @param [out] send_len     实际发送的数据长度
 *
 * @details
 *      协议发送应答，会修改源地址为本机地址和修改目的地址为数据包的源地址，以及动态地址、
 *      响应标志位、产品信息等，保证数据响应时的基本配置的准确性
 *
 * @par eg:
 * @code
 *      int32_t action_handle(hal_frame_info_t *p_frame_info)
 *      {
 *			if(p_frame_info->data_len == 0){
 *			    return 0;		// 数据解析失败
 *			}
 *          proto_cmd_struct *p_proto_cmd = (proto_cmd_struct *)p_frame_info->pdata;
 *
 *          ...
 *
 *          uint16_t ack_len;
 *          proto_ack_struct proto_ack = {0};
 *          proto_ack.sta = 1;
 *
 *          PROTOBUF_SEND_ACK(p_frame_info, proto_ack_struct, &proto_ack, ack_len)
 *          return ack_len;
 *      }
 * @endcode
 */

#define PROTOBUF_SEND_ACK(p_frame_info, proto_name, p_proto_struct, send_len)                                                                             \
    {                                                                                                                                                     \
        (proto_name##_size == 0) ? ({ /* 防止playload为空时，不能发送数据 */                                                                  \
                                      p_frame_info->playload_ver = PLAYLOAD_PROTOBUF;                                                                     \
                                      send_len = protocol_send_ack(p_frame_info, NULL, 0);                                                                \
        })                                                                                                                                                \
                                 : ({                                                                                                                     \
                                       uint8_t encode_flag = 0;                                                                                           \
                                       uint8_t proto_buf[(proto_name##_size > PROTOBUF_BUF_MAX_LEN) ? (PROTOBUF_BUF_MAX_LEN) : (proto_name##_size)];      \
                                       ENCODE_PROTOBUF_PTR(proto_name, p_proto_struct, proto_buf, sizeof(proto_buf), p_frame_info->data_len, encode_flag) \
                                       if (encode_flag != 0) {                                                                                            \
                                           p_frame_info->playload_ver = PLAYLOAD_PROTOBUF;                                                                \
                                           send_len = protocol_send_ack(p_frame_info, proto_buf, p_frame_info->data_len);                                 \
                                       }                                                                                                                  \
                                       else {                                                                                                             \
                                           send_len = 0;                                                                                                  \
                                       }                                                                                                                  \
                                   });                                                                                                                    \
    }

#ifdef __cplusplus
}
#endif

#endif // XF_PROTOBUF_H
