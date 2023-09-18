/**
 * @brief proto协议模块头文件
 * @file xf_com_proto.h
 * @version 1.0
 * @author xianze.yu
 * @date 2022-08-29 12:00:54
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-08-29 12:00:54 <td>1.0       <td>xianze.yu      <td>Created function
 */

#ifndef XF_COM_PROTOBUF_H
#define XF_COM_PROTOBUF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

/******************************Decode-解码部分-Start*******************************/
/**
 * @brief protobuf,解码函数
 * @param [in]  name        解码结构体名称
 * @param [out] out_str     解码结构体指针-存放解码后结构体
 * @param [in]  pd          待解码数据指针
 * @param [in]  pb_len      待解码数据长度
 * @param [out] ret         解码结果<type>:bool 0:解码失败，1:解码成功
 *
 * @details pb_len = 0，不论缓存数据正确与否直接会返回编码失败
 * @par 示例:
 * @code
 *      abc_t adc = {0};
 *      uint32_t ret    =   0;
 *      DECODE_PROTO(adc_t,rev_buff,rec_len,adc,ret);
 *      if(ret)
 *      {
 *          ///< 解码成功，开始处理数据
 *      }
 *      else
 *      {
 *          ///< 解码失败，发送警告
 *      }
 * @endcode
 */
#define DECODE_PROTO(name, out_str,pb, pb_len, ret)            \
    if (pb_len != 0)                                              \
    {                                                          \
        pb_istream_t stream = pb_istream_from_buffer(pb, pb_len); \
        ret = pb_decode(&stream, name##_fields, &out_str);     \
    }                                                          \
    else                                                       \
    {                                                          \
        ret = 0;                                               \
    }

/******************************Decode-解码部分-End*******************************/

/******************************Encode-编码部分-Start*******************************/

/**
 * @brief protobuf编码函数
 *
 * @param [in]  name        编码结构体名称
 * @param [in]  en_str      赋值完，待编码的结构体
 * @param [out] buff        编码缓存，编码完成后的数据存在的地方
 * @param [in]  buff_size   缓存长度
 * @param [out] en_len      编码后实际数据长度
 *
 * @return uint32_t 编码后数据长度
 *  @retval 0：编码失败，n:编码后数据长度
 * @details 缓存长度=0；函数会直接返回编码失败
 * @par 示例:
 * @code
 *      dac_t dac = {0};
 *      dac.1 = x;
 *      dac.2 = y;
 *      memcpy(&dac.3,&z,sizeof(dac.3));
 *      uint32_t en_len = 0;
 *      uint8_t send_buf[1200];
 *      uint32_t buf_len = 1200;
 *      ENCODE_PROTO(dac_t,dac,send_buf,buf_len,en_len);
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
#define ENCODE_PROTO(name, en_str, buff, buff_size, en_len)           \
    if (buff_size != 0)                                                \
    {                                                                  \
        pb_ostream_t stream = pb_ostream_from_buffer(buff, buff_size); \
        if (pb_encode(&stream, name##_fields, &en_str))                 \
        {                                                              \
            en_len = stream.bytes_written; /* Success */               \
        }                                                              \
        else                                                           \
        {                                                              \
            en_len = 0; /* Failure */                                  \
        }                                                              \
    }                                                                  \
    else                                                               \
    {                                                                  \
        en_len = 0;                                                    \
    }

/******************************Encode-编码部分-End*******************************/

#ifdef __cplusplus
}
#endif

#endif  // XF_COM_PROTOBUF_H
