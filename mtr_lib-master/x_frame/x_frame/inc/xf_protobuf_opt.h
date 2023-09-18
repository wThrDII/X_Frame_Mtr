/**
 * @brief
 * @file xf_protobuf_opt.h
 * @version 1.0
 * @author juntong.chen
 * @date 2022-11-09 20:46:24
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @details
 * 实现数据的增量上报和接收
 * 实现数据的定时上报
 * 实现数据的分包上报
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2022-11-09 20:46:24      <td>juntong.chen      <td>创建文件
 */

#ifndef XF_PROTOBUF_OPT_H
#define XF_PROTOBUF_OPT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "xf_protobuf_opt_pri.h"

/**
 * @brief 注册 protobuf 可选字段接收功能
 *
 * @param [in]  proto_name          proto 参数结构体名称
 * @param [in]  cmd_func            指令集
 * @param [in]  cmd_id              指令id
 * @param [in]  link_id             链路id
 *
 * @details
 *          调用该接口注册前需要调用 DEF_PROTOBUF_OPT_RECV_XXX 相关接口定义 item 项
 *
 * @par 在 USART_LINK 链路注册 接收内容名称为 proto_name，指令集 0x02， 指令id 0x01，的接收功能
 * @code
 *          REG_PROTOBUF_OPT_RECV_SRC(proto_name, 0x02, 0x01, USART_LINK)
 * @endcode
 */
#define REG_PROTOBUF_OPT_RECV_SRC(proto_name, cmd_func, cmd_id, link_id)                         \
    extern void opt_rece_##proto_name(proto_name *p_proto);                                      \
    int32_t proto_name##cmd_func##cmd_id##link_id##action_handle(hal_frame_info_t *p_frame_info) \
    {                                                                                            \
        if (p_frame_info->data_len == 0) {                                                       \
            return 0;                                                                            \
        }                                                                                        \
        proto_name *p_proto_cmd = (proto_name *)p_frame_info->pdata;                             \
        opt_rece_##proto_name(p_proto_cmd);                                                      \
        uint16_t ack_len;                                                                        \
        PROTOBUF_SEND_ACK(p_frame_info, proto_name, p_proto_cmd, ack_len);                       \
        return ack_len;                                                                          \
    }                                                                                            \
    REG_LINK_PROTOBUF_ACTION(proto_name, link_id, cmd_func, cmd_id, proto_name##cmd_func##cmd_id##link_id##action_handle);

/**
 * @brief 头部 - 定义可选字段接收信息
 *
 * @param [in]  proto_name          proto 参数结构体名称
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_RECV_XXX_ITEM 和 DEF_PROTOBUF_OPT_RECV_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_RECV_SRC 注册接收的功能
 *
 * @par 定义消息为proto_name的接收信息
 * @code
 *          DEF_PROTOBUF_OPT_RECV_START(proto_name)
 *              DEF_PROTOBUF_OPT_RECV_ITEM(proto_var, user_var)
 *              ///< ...
 *              DEF_PROTOBUF_OPT_RECV_ITEM(proto_var, user_var)
 *          DEF_PROTOBUF_OPT_RECV_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_RECV_START(proto_name)     \
    void opt_rece_##proto_name(proto_name *p_proto) \
    {

/**
 * @brief 选项 - 定义可选字段接收信息
 *
 * @param [in]  proto_var           proto 参数结构体内部的成员名称
 * @param [in]  has_proto_var       proto 参数结构体里面标识成员是否上传的字段
 * @param [in]  user_var            用户变量
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_RECV_START 和 DEF_PROTOBUF_OPT_RECV_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_RECV_SRC 注册接收的功能
 *
 * @par
 *          定义消息为 proto_name 的接收信息,
 *              proto_name 里面的成员有 info
 *              用于用于接收信息的全局变量为 user_info
 * @code
 *          DEF_PROTOBUF_OPT_RECV_START(proto_name)
 *              DEF_PROTOBUF_OPT_RECV_CB_ITEM(info, has_info, user_info)
 *          DEF_PROTOBUF_OPT_RECV_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_RECV_ITEM(proto_var, has_proto_var, user_var) \
    if (p_proto->has_proto_var == true) {                              \
        user_var = p_proto->proto_var;                                 \
    }

/**
 * @brief 选项 - 定义可选字段接收信息 - 带回调
 *
 * @param [in]  proto_var           proto 参数结构体内部的成员名称
 * @param [in]  user_var            用户变量
 * @param [in]  callback            类型为 void (*callback)(void *p_rece,void *p_user)
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_RECV_START 和 DEF_PROTOBUF_OPT_RECV_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_RECV_SRC 注册接收的功能
 *
 * @par
 *          定义消息为proto_name的接收信息,
 *              proto_name 里面的成员有 info
 *              用于用于接收信息的全局变量为 user_info
 *              callback 为对应的回调函数
 * @code
 *          DEF_PROTOBUF_OPT_RECV_START(proto_name)
 *              DEF_PROTOBUF_OPT_RECV_CB_ITEM(info, has_info, user_info, callback)
 *          DEF_PROTOBUF_OPT_RECV_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_RECV_CB_ITEM(proto_var, has_proto_var, user_var, callback) \
    if (p_proto->has_proto_var == true) {                                           \
        callback(&p_proto->proto_var, &user_var);                                   \
        user_var = p_proto->proto_var;                                              \
    }

/**
 * @brief 选项 - 定义可选字段接收信息 - 字符流, 结构体，数组等
 *
 * @param [in]  proto_var           proto 参数结构体内部的成员名称
 * @param [in]  user_var            用户变量
 * @param [in]  user_len            用户变量长度
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_RECV_START 和 DEF_PROTOBUF_OPT_RECV_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_RECV_SRC 注册接收的功能
 *
 * @par
 *          定义消息为 proto_name 的接收信息,
 *              proto_name 里面的成员有 student.name
 *              用于用于接收信息的全局变量为 user_student.name，长度为 10
 * @code
 *          DEF_PROTOBUF_OPT_RECV_START(proto_name)
 *              DEF_PROTOBUF_OPT_RECV_GROUP_ITEM(student.name, student.has_name, user_student.name, 10)
 *          DEF_PROTOBUF_OPT_RECV_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_RECV_GROUP_ITEM(proto_var, has_proto_var, user_var, user_len) \
    if (p_proto->has_proto_var == true) {                                              \
        memcpy(&user_var, &p_proto->proto_var, user_len);                              \
    }

/**
 * @brief 选项 - 定义可选字段接收信息 - 字符流, 结构体，数组等 - 带回调
 *
 * @param [in]  proto_var           proto 参数结构体内部的成员名称
 * @param [in]  user_var            用户变量
 * @param [in]  user_len            用户变量长度
 * @param [in]  callback            类型为 void (*callback)(void *p_rece,void *p_user)
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_RECV_START 和 DEF_PROTOBUF_OPT_RECV_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_RECV_SRC 注册接收的功能
 *
 * @par
 *          定义消息为proto_name的接收信息,
 *              proto_name 里面的成员有student.name
 *              用于用于接收信息的全局变量为user_student.name 长度为10
 *              callback 为对应的回调函数
 * @code
 *          DEF_PROTOBUF_OPT_RECV_START(proto_name)
 *              DEF_PROTOBUF_OPT_RECV_GROUP_ITEM(student.name, student.has_name, user_student.name, 10, callback)
 *          DEF_PROTOBUF_OPT_RECV_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_RECV_CB_GROUP_ITEM(proto_var, has_proto_var, user_var, user_len, callback) \
    if (p_proto->has_proto_var == true) {                                                           \
        callback(&p_proto->proto_var, &user_var);                                                   \
        memcpy(&user_var, &p_proto->proto_var, user_len);                                           \
    }

/**
 * @brief 尾部 - 定义可选字段接收信息
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_RECV_START 和 DEF_PROTOBUF_OPT_RECV_XXX_ITEM 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_RECV_SRC 注册接收的功能
 *
 * @par 定义指令集 0x20 指令id 0x01 消息为proto_name的接收信息
 * @code
 *          DEF_PROTOBUF_OPT_RECV_START(proto_name)
 *              DEF_PROTOBUF_OPT_RECV_ITEM(proto_var, user_var)
 *              ///< ...
 *              DEF_PROTOBUF_OPT_RECV_ITEM(proto_var, user_var)
 *          DEF_PROTOBUF_OPT_RECV_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_RECV_END() }

/**
 * @brief 清空可选增量上报的缓存数据
 *
 * @param [in]  proto_name          proto 参数结构体名称
 * @param [in]  dest                目标地址
 *
 * @details 特殊说明
 *
 * @par eg:
 * @code
 *      CLEAR_PROTOBUF_OPT_INCRE_CACHE(proto_name, 0x20)
 * @endcode
 */
#define CLEAR_PROTOBUF_OPT_INCRE_CACHE(proto_name, dest)                                                     \
    {                                                                                                        \
        extern uint8_t proto_opt_send_dest_##proto_name[];                                                   \
        extern proto_name proto_cache_struct_##proto_name[ARRY_ITEMS_NUM(proto_opt_send_dest_##proto_name)]; \
        for (uint16_t cir = 0; cir < ARRY_ITEMS_NUM(proto_opt_send_dest_##proto_name); cir++) {              \
            if (proto_opt_send_dest_##proto_name[cir] == dest) {                                             \
                memset(proto_cache_struct_##proto_name + cir, 0, sizeof(proto_name));                        \
            }                                                                                                \
        }                                                                                                    \
    }

/**
 * @brief 注册 protobuf 可选字段发送功能
 *
 * @param [in]  proto_name          proto 参数结构体名称
 * @param [in]  cmd_func            指令集
 * @param [in]  cmd_id              指令id
 * @param [in]  link_id             链路id
 * @param [in]  dest...             目标地址，不定长
 *                |--- @param [in]  dest    目标地址
 *
 * @details
 *          1. 调用该接口注册前需要调用 DEF_PROTOBUF_OPT_SEND_XXX 相关接口定义 item 项
 *          2. 实现数据的增量上报
 *          3. 实现数据的分包上报
 *          4. 实现设备断连时的全量上报
 *
 * @par 在 USART_LINK 链路注册 发送信息为 proto_name，指令集 0x02， 指令id 0x01，目标地址 0x20， 0x21 的发送功能
 * @code
 *          REG_PROTOBUF_OPT_SEND_DEST(proto_name, 0x02, 0x01, USART_LINK, 0x20, 0x21)
 * @endcode
 */
#define REG_PROTOBUF_OPT_SEND_DEST(proto_name, _cmd_func, _cmd_id, _link_id, _dest...)                                                                           \
    extern uint32_t protobuf_opt_handle_##proto_name(uint8_t type, proto_name *p_handle_buf, proto_name *p_cache_buf, uint32_t dest_index);                      \
    uint8_t proto_opt_send_dest_##proto_name[] = {_dest};                                                                                                        \
    proto_name proto_cache_struct_##proto_name[ARRY_ITEMS_NUM(proto_opt_send_dest_##proto_name)];                                                                \
    static void protobuf_opt_send_dest_##proto_name(void)                                                                                                        \
    {                                                                                                                                                            \
        static uint32_t dest_index = 0;                                                                                                                          \
        proto_name proto_cmd_struct;                                                                                                                             \
        if (protobuf_opt_handle_##proto_name(XF_PROTOBUF_OPT_TYPE_CMD_SEND, &proto_cmd_struct, &proto_cache_struct_##proto_name[dest_index], dest_index) != 0) { \
            hal_frame_info_t frame_info = {0};                                                                                                                   \
            frame_info.cmd_func = _cmd_func;                                                                                                                     \
            frame_info.cmd_id = _cmd_id;                                                                                                                         \
            frame_info.dest = proto_opt_send_dest_##proto_name[dest_index];                                                                                      \
            frame_info.d_dest = 0x01;                                                                                                                            \
            frame_info.link_id = _link_id;                                                                                                                       \
            uint32_t send_len = 0;                                                                                                                               \
            (void)send_len;                                                                                                                                      \
            PROTOBUF_SEND_BY_ID(&frame_info, proto_name, &proto_cmd_struct, send_len);                                                                           \
        }                                                                                                                                                        \
        dest_index = (dest_index + 1) % ARRY_ITEMS_NUM(proto_opt_send_dest_##proto_name);                                                                        \
    }                                                                                                                                                            \
    REG_TASK(VH_TASK, XF_PROTOBUF_OPT_TASK_TIME, protobuf_opt_send_dest_##proto_name)                                                                            \
    int32_t protobuf_opt_send_ack_handle_##proto_name(hal_frame_info_t *p_frame_info)                                                                            \
    {                                                                                                                                                            \
        if (p_frame_info->data_len == 0) {                                                                                                                       \
            return 0;                                                                                                                                            \
        }                                                                                                                                                        \
        proto_name *p_proto_cmd = (proto_name *)p_frame_info->pdata;                                                                                             \
        uint16_t dest_index = 0;                                                                                                                                 \
        for (; dest_index < ARRY_ITEMS_NUM(proto_opt_send_dest_##proto_name); dest_index++) {                                                                    \
            if (p_frame_info->src == proto_opt_send_dest_##proto_name[dest_index]) {                                                                             \
                protobuf_opt_handle_##proto_name(XF_PROTOBUF_OPT_TYPE_ACK_HANDLE, p_proto_cmd, &proto_cache_struct_##proto_name[dest_index], dest_index);        \
                return 0;                                                                                                                                        \
            }                                                                                                                                                    \
        }                                                                                                                                                        \
        return 0;                                                                                                                                                \
    }                                                                                                                                                            \
    REG_LINK_PROTOBUF_ACTION(proto_name, _link_id, _cmd_func, _cmd_id, protobuf_opt_send_ack_handle_##proto_name);

/**
 * @brief 头部 - 获取可选字段需要发送信息
 *
 * @param [in]  proto_name          proto 参数结构体名称
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_SEND_XXX_ITEM 和 DEF_PROTOBUF_OPT_SEND_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_SEND_DEST 注册发送的功能
 *
 * @par     定义消息为proto_name的发送信息
 * @code
 *          DEF_PROTOBUF_OPT_SEND_START(proto_name)
 *              DEF_PROTOBUF_OPT_SEND_ITEM(proto_var, user_var)
 *              ///< ...
 *              DEF_PROTOBUF_OPT_SEND_ITEM(proto_var, user_var)
 *          DEF_PROTOBUF_OPT_SEND_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_SEND_START(proto_name)                                                                                     \
    uint32_t protobuf_opt_handle_##proto_name(uint8_t type, proto_name *p_handle_buf, proto_name *p_cache_buf, uint32_t dest_index) \
    {                                                                                                                               \
        static const uint8_t dest_num = ARRY_ITEMS_NUM(proto_opt_send_dest_##proto_name);                                           \
        static const OPT_STIME_TYPE def_time_out_index = XF_PROTOBUF_OPT_TIMEOUT / XF_PROTOBUF_OPT_TASK_TIME;                       \
        uint32_t single_num = 0;                                                                                                    \
        if (type == XF_PROTOBUF_OPT_TYPE_CMD_SEND) {                                                                                \
            memset(p_handle_buf, 0, sizeof(proto_name));                                                                            \
        }

/**
 * @brief 选项 - 获取可选字段需要发送信息 - 单个变量
 *
 * @param [in]  proto_var          proto 成员变量名称
 * @param [in]  user_var           用户变量
 *
 * @details
 *          1. 当用户变量值与 proto 的缓存变量值不一致时，触发发送
 *          2. 当发送后，需要等待应答，收到应答后可立即触发下一次发送
 *          3. 当超过 XF_PROTOBUF_OPT_TIMEOUT 未收到应答时，也会触发下一次发送
 *          4. 该接口需要与 DEF_PROTOBUF_OPT_SEND_START 和 DEF_PROTOBUF_OPT_SEND_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_SEND_DEST 注册发送的功能
 *
 * @par     定义消息为proto_name的发送信息
 * @code
 *          DEF_PROTOBUF_OPT_SEND_START(proto_name)
 *              DEF_PROTOBUF_OPT_SEND_ITEM(proto_var, user_var)
 *              ///< ...
 *              DEF_PROTOBUF_OPT_SEND_ITEM(proto_var, user_var)
 *          DEF_PROTOBUF_OPT_SEND_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_SEND_ITEM(proto_var, has_proto_var, user_var)                                                                                      \
    {                                                                                                                                                       \
        static OPT_STIME_TYPE time_cnt_index[dest_num];                                                                                                     \
        if (type == XF_PROTOBUF_OPT_TYPE_CMD_SEND) {                                                                                                        \
            __PROTOBUF_OPT_SEND_ITEM_JUDGE(proto_var, has_proto_var, user_var, def_time_out_index, __PROTOBUF_OPT_SEND_ITEM_CONDITION(proto_var, user_var)) \
        }                                                                                                                                                   \
        else {                                                                                                                                              \
            __PROTOBUF_OPT_SEND_ITEM_ACK_HANDLE(proto_var, has_proto_var, user_var)                                                                         \
            time_cnt_index[dest_index] = 0;                                                                                                                 \
        }                                                                                                                                                   \
    }

/**
 * @brief 选项 - 获取可选字段需要发送信息 - 单个变量，满足步长发送
 *
 * @param [in]  proto_var           proto 成员变量名称
 * @param [in]  user_var            用户变量
 * @param [in]  step                步长
 *
 * @details
 *          1. 当用户变量值与 proto 的缓存值相差正负超过 step，触发发送
 *          2. 当发送后，需要等待应答，收到应答后可立即触发下一次发送
 *          3. 当超过 XF_PROTOBUF_OPT_TIMEOUT 未收到应答时，也会触发下一次发送
 *          4. 该接口需要与 DEF_PROTOBUF_OPT_SEND_START 和 DEF_PROTOBUF_OPT_SEND_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_SEND_DEST 注册发送的功能
 *
 * @par     定义消息为proto_name的发送信息
 * @code
 *          DEF_PROTOBUF_OPT_SEND_START(proto_name)
 *              DEF_PROTOBUF_OPT_SEND_STEP_ITEM(proto_var, user_var, 5)
 *              ///< ...
 *              DEF_PROTOBUF_OPT_SEND_STEP_ITEM(proto_var, user_var, 5)
 *          DEF_PROTOBUF_OPT_SEND_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_SEND_STEP_ITEM(proto_var, has_proto_var, user_var, step)                                                                                      \
    {                                                                                                                                                                  \
        static OPT_STIME_TYPE time_cnt_index[dest_num];                                                                                                                \
        if (type == XF_PROTOBUF_OPT_TYPE_CMD_SEND) {                                                                                                                   \
            __PROTOBUF_OPT_SEND_ITEM_JUDGE(proto_var, has_proto_var, user_var, def_time_out_index, __PROTOBUF_OPT_SEND_ITEM_STEP_CONDITION(proto_var, user_var, step)) \
        }                                                                                                                                                              \
        else {                                                                                                                                                         \
            __PROTOBUF_OPT_SEND_ITEM_ACK_HANDLE(proto_var, has_proto_var, user_var)                                                                                    \
            time_cnt_index[dest_index] = 0;                                                                                                                            \
        }                                                                                                                                                              \
    }

/**
 * @brief 选项 - 获取可选字段需要发送信息 - 单个变量
 *
 * @param [in]  proto_var           proto 成员变量名称
 * @param [in]  user_var            用户变量
 * @param [in]  timeout             超时时间, 最大值不能超过 65535 * XF_PROTOBUF_OPT_TASK_TIME
 *
 * @details
 *          1. 当用户变量值与 proto 的缓存变量值不一致时，触发发送，下一次发送必须间隔  timeout 时间
 *          4. 该接口需要与 DEF_PROTOBUF_OPT_SEND_START 和 DEF_PROTOBUF_OPT_SEND_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_SEND_DEST 注册发送的功能
 *
 * @par     定义消息为proto_name的发送信息
 * @code
 *          DEF_PROTOBUF_OPT_SEND_START(proto_name)
 *              DEF_PROTOBUF_OPT_SEND_TIME_ITEM(proto_var, user_var, 4000)
 *              ///< ...
 *              DEF_PROTOBUF_OPT_SEND_TIME_ITEM(proto_var, user_var, 4000)
 *          DEF_PROTOBUF_OPT_SEND_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_SEND_TIME_ITEM(proto_var, has_proto_var, user_var, timeout)                                                                                         \
    {                                                                                                                                                                        \
        static uint16_t time_cnt_index[dest_num];                                                                                                                            \
        if (type == XF_PROTOBUF_OPT_TYPE_CMD_SEND) {                                                                                                                         \
            __PROTOBUF_OPT_SEND_ITEM_JUDGE(proto_var, has_proto_var, user_var, timeout / XF_PROTOBUF_OPT_TASK_TIME, __PROTOBUF_OPT_SEND_ITEM_CONDITION(proto_var, user_var)) \
        }                                                                                                                                                                    \
        else {                                                                                                                                                               \
            __PROTOBUF_OPT_SEND_ITEM_ACK_HANDLE(proto_var, has_proto_var, user_var)                                                                                          \
        }                                                                                                                                                                    \
    }

/**
 * @brief 选项 - 获取可选字段需要发送信息 - 单个变量，满足步长和一定时间发送
 *
 * @param [in]  proto_var           proto 成员变量名称
 * @param [in]  user_var            用户变量
 * @param [in]  timeout             超时时间
 * @param [in]  step                步长
 *
 * @details
 *          1. 当用户变量值与 proto 的缓存值相差正负超过 step，触发发送
 *          2. 当发送后，需要等待应答，收到应答后可立即触发下一次发送
 *          3. 当超过 timeout 未收到应答时，也会触发下一次发送
 *          4. 该接口需要与 DEF_PROTOBUF_OPT_SEND_START 和 DEF_PROTOBUF_OPT_SEND_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_SEND_DEST 注册发送的功能
 *
 * @par     定义消息为proto_name的发送信息
 * @code
 *          DEF_PROTOBUF_OPT_SEND_START(proto_name)
 *              DEF_PROTOBUF_OPT_SEND_TIME_STEP_ITEM(proto_var, user_var, 4000, 5)
 *              ///< ...
 *              DEF_PROTOBUF_OPT_SEND_TIME_STEP_ITEM(proto_var, user_var, 4000, 5)
 *          DEF_PROTOBUF_OPT_SEND_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_SEND_TIME_STEP_ITEM(proto_var, has_proto_var, user_var, timeout, step)                                                                                         \
    {                                                                                                                                                                                   \
        static uint16_t time_cnt_index[dest_num];                                                                                                                                       \
        if (type == XF_PROTOBUF_OPT_TYPE_CMD_SEND) {                                                                                                                                    \
            __PROTOBUF_OPT_SEND_ITEM_JUDGE(proto_var, has_proto_var, user_var, timeout / XF_PROTOBUF_OPT_TASK_TIME, __PROTOBUF_OPT_SEND_ITEM_STEP_CONDITION(proto_var, user_var, step)) \
        }                                                                                                                                                                               \
        else {                                                                                                                                                                          \
            __PROTOBUF_OPT_SEND_ITEM_ACK_HANDLE(proto_var, has_proto_var, user_var)                                                                                                     \
        }                                                                                                                                                                               \
    }

/**
 * @brief 选项 - 获取可选字段需要发送信息 - 组选项，可以是数组，结构体等
 *
 * @param [in]  proto_var          proto 成员变量名称，首变量地址
 * @param [in]  user_var           用户变量，首变量地址
 * @param [in]  user_len           用户参数长度
 *
 * @details
 *          注意：首地址示例： 比如数组为 arry[0], 结构体为成员名，指针则为 point[0]
 *          1. 当用户组选项值与 proto 的缓存组选项值不一致时，触发发送
 *          2. 当发送后，需要等待应答，收到应答后可立即触发下一次发送
 *          3. 当超过 XF_PROTOBUF_OPT_TIMEOUT 未收到应答时，也会触发下一次发送
 *          4. 该接口需要与 DEF_PROTOBUF_OPT_SEND_START 和 DEF_PROTOBUF_OPT_SEND_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_SEND_DEST 注册发送的功能
 *
 * @par     定义消息为proto_name的发送信息
 * @code
 *          DEF_PROTOBUF_OPT_SEND_START(proto_name)
 *              DEF_PROTOBUF_OPT_SEND_GROUP_ITEM(proto_arry_var[0], user_arry_var[0], 100)
 *              DEF_PROTOBUF_OPT_SEND_GROUP_ITEM(proto_struct_var, user_struct_var, 30)
 *          DEF_PROTOBUF_OPT_SEND_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_SEND_GROUP_ITEM(proto_var, has_proto_var, user_var, user_len)                        \
    {                                                                                                         \
        static OPT_STIME_TYPE time_cnt_index[dest_num];                                                       \
        if (type == XF_PROTOBUF_OPT_TYPE_CMD_SEND) {                                                          \
            __PROTOBUF_OPT_SEND_GROUP_JUDGE(proto_var, has_proto_var, user_var, user_len, def_time_out_index) \
        }                                                                                                     \
        else {                                                                                                \
            __PROTOBUF_OPT_SEND_GROUP_ITEM_ACK_HANDLE(proto_var, has_proto_var, user_var, user_len)           \
            time_cnt_index[dest_index] = 0;                                                                   \
        }                                                                                                     \
    }

/**
 * @brief 选项 - 获取可选字段需要发送信息 - 组选项，可以是数组，结构体等
 *
 * @param [in]  proto_var          proto 成员变量名称，首变量地址
 * @param [in]  user_var           用户变量，首变量地址
 * @param [in]  user_len           用户参数长度
 * @param [in]  timeout            超时时间
 *
 * @details
 *          注意：首地址示例： 比如数组为 arry[0], 结构体为成员名，指针则为 point[0]
 *          1. 当用户组选项值与 proto 的缓存组选项值不一致时，触发发送
 *          2. 当发送后，需要等待应答，收到应答后可立即触发下一次发送
 *          3. 当超过 timeout 未收到应答时，也会触发下一次发送
 *          4. 该接口需要与 DEF_PROTOBUF_OPT_SEND_START 和 DEF_PROTOBUF_OPT_SEND_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_SEND_DEST 注册发送的功能
 *
 * @par     定义消息为proto_name的发送信息
 * @code
 *          DEF_PROTOBUF_OPT_SEND_START(proto_name)
 *              DEF_PROTOBUF_OPT_SEND_GROUP_ITEM(proto_arry_var[0], user_arry_var[0], 100, 4000)
 *              DEF_PROTOBUF_OPT_SEND_GROUP_ITEM(proto_struct_var, user_struct_var, 30, 4000)
 *          DEF_PROTOBUF_OPT_SEND_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_SEND_TIME_GROUP_ITEM(proto_var, has_proto_var, user_var, user_len, timeout)                           \
    {                                                                                                                          \
        static uint16_t time_cnt_index[dest_num];                                                                              \
        if (type == XF_PROTOBUF_OPT_TYPE_CMD_SEND) {                                                                           \
            __PROTOBUF_OPT_SEND_GROUP_JUDGE(proto_var, has_proto_var, user_var, user_len, timeout / XF_PROTOBUF_OPT_TASK_TIME) \
        }                                                                                                                      \
        else {                                                                                                                 \
            __PROTOBUF_OPT_SEND_GROUP_ITEM_ACK_HANDLE(proto_var, has_proto_var, user_var, user_len)                            \
        }                                                                                                                      \
    }

/**
 * @brief 尾部 - 获取可选字段需要发送信息
 *
 * @param [in]  proto_name          proto 参数结构体名称
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_SEND_XXX_ITEM 和 DEF_PROTOBUF_OPT_SEND_START 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_SEND_DEST 注册发送的功能
 *
 * @par     定义消息为proto_name的发送信息
 * @code
 *          DEF_PROTOBUF_OPT_SEND_START(proto_name)
 *              DEF_PROTOBUF_OPT_SEND_ITEM(proto_var, user_var)
 *              ///< ...
 *              DEF_PROTOBUF_OPT_SEND_ITEM(proto_var, user_var)
 *          DEF_PROTOBUF_OPT_SEND_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_SEND_END() \
    end:                            \
    return single_num;              \
    }

/**
 * @brief 注册 protobuf 定时字段接收功能
 *
 * @param [in]  proto_name          proto 参数结构体名称
 * @param [in]  cmd_func            指令集
 * @param [in]  cmd_id              指令id
 * @param [in]  link_id             链路id
 *
 * @details
 *          调用该接口注册前需要调用 DEF_PROTOBUF_OPT_TIME_RECV_XXX 相关接口定义 item 项
 *
 * @par 在 USART_LINK 链路注册 接收内容名称为 proto_name，指令集 0x02， 指令id 0x01，的接收功能
 * @code
 *          REG_PROTOBUF_OPT_TIME_RECV_SRC(proto_name, 0x02, 0x01, USART_LINK)
 * @endcode
 */
#define REG_PROTOBUF_OPT_TIME_RECV_SRC(proto_name, cmd_func, cmd_id, link_id)                    \
    extern void opt_rece_##proto_name(proto_name *p_proto);                                      \
    int32_t proto_name##cmd_func##cmd_id##link_id##action_handle(hal_frame_info_t *p_frame_info) \
    {                                                                                            \
        if (p_frame_info->data_len == 0) {                                                       \
            return 0;                                                                            \
        }                                                                                        \
        proto_name *p_proto_cmd = (proto_name *)p_frame_info->pdata;                             \
        opt_rece_##proto_name(p_proto_cmd);                                                      \
        return 0;                                                                                \
    }                                                                                            \
    REG_LINK_PROTOBUF_ACTION(proto_name, link_id, cmd_func, cmd_id, proto_name##cmd_func##cmd_id##link_id##action_handle);

/**
 * @brief 头部 - 定义可选字段定时接收信息
 *
 * @param [in]  proto_name          proto 参数结构体名称
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_TIME_RECV_XXX_ITEM 和 DEF_PROTOBUF_OPT_TIME_RECV_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_TIME_RECV_SRC 注册接收的功能
 *
 * @par 定义消息为proto_name的接收信息
 * @code
 *          DEF_PROTOBUF_OPT_TIME_RECV_START(proto_name)
 *              DEF_PROTOBUF_OPT_TIME_RECV_ITEM(proto_var, user_var)
 *              ///< ...
 *              DEF_PROTOBUF_OPT_TIME_RECV_ITEM(proto_var, user_var)
 *          DEF_PROTOBUF_OPT_TIME_RECV_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_TIME_RECV_START(proto_name) DEF_PROTOBUF_OPT_RECV_START(proto_name)

/**
 * @brief 选项 - 定义可选字段定时接收信息
 *
 * @param [in]  proto_var           proto 参数结构体内部的成员名称
 * @param [in]  has_proto_var       proto 参数结构体里面标识成员是否上传的字段
 * @param [in]  user_var            用户变量
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_TIME_RECV_START 和 DEF_PROTOBUF_OPT_TIME_RECV_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_TIME_RECV_SRC 注册接收的功能
 *
 * @par
 *          定义消息为 proto_name 的接收信息,
 *              proto_name 里面的成员有 info
 *              用于用于接收信息的全局变量为 user_info
 * @code
 *          DEF_PROTOBUF_OPT_TIME_RECV_START(proto_name)
 *              DEF_PROTOBUF_OPT_TIME_RECV_ITEM(info, has_info, user_info)
 *          DEF_PROTOBUF_OPT_TIME_RECV_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_TIME_RECV_ITEM(proto_var, has_proto_var, user_var) DEF_PROTOBUF_OPT_RECV_ITEM(proto_var, has_proto_var, user_var)

/**
 * @brief 选项 - 定义可选字段定时接收信息 - 字符流, 结构体，数组等
 *
 * @param [in]  proto_var           proto 参数结构体内部的成员名称
 * @param [in]  user_var            用户变量
 * @param [in]  user_len            用户变量长度
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_TIME_RECV_START 和 DEF_PROTOBUF_OPT_TIME_RECV_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_TIME_RECV_SRC 注册接收的功能
 *
 * @par
 *          定义消息为 proto_name 的接收信息,
 *              proto_name 里面的成员有 student.name
 *              用于用于接收信息的全局变量为 user_student.name，长度为 10
 * @code
 *          DEF_PROTOBUF_OPT_TIME_RECV_START(proto_name)
 *              DEF_PROTOBUF_OPT_TIME_RECV_GROUP_ITEM(student.name, student.has_name, user_student.name, 10)
 *          DEF_PROTOBUF_OPT_TIME_RECV_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_TIME_RECV_GROUP_ITEM(proto_var, has_proto_var, user_var, user_len) DEF_PROTOBUF_OPT_RECV_GROUP_ITEM(proto_var, has_proto_var, user_var, user_len)

/**
 * @brief 尾部 - 定义可选字段定时接收信息
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_TIME_RECV_START 和 DEF_PROTOBUF_OPT_TIME_RECV_XXX_ITEM 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_TIME_RECV_SRC 注册接收的功能
 *
 * @par 定义指令集 0x20 指令id 0x01 消息为proto_name的接收信息
 * @code
 *          DEF_PROTOBUF_OPT_TIME_RECV_START(proto_name)
 *              DEF_PROTOBUF_OPT_TIME_RECV_ITEM(proto_var, user_var)
 *              ///< ...
 *              DEF_PROTOBUF_OPT_TIME_RECV_ITEM(proto_var, user_var)
 *          DEF_PROTOBUF_OPT_TIME_RECV_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_TIME_RECV_END() DEF_PROTOBUF_OPT_RECV_END()

/**
 * @brief 注册 protobuf 可选字段定时发送功能，无 ack
 *
 * @param [in]  proto_name          proto 参数结构体名称
 * @param [in]  cmd_func            指令集
 * @param [in]  cmd_id              指令id
 * @param [in]  link_id             链路id
 * @param [in]  dest...             目标地址，不定长
 *                |--- @param [in]  dest    目标地址
 *
 * @details
 *          1. 调用该接口注册前需要调用 DEF_PROTOBUF_OPT_TIME_SEND_XXX 相关接口定义 item 项
 *          2. 实现数据的增量上报
 *          3. 实现数据的分包上报
 *          4. 实现设备断连时的全量上报
 *
 * @par 在 USART_LINK 链路注册 发送信息为 proto_name，指令集 0x02， 指令id 0x01，目标地址 0x20， 0x21 的发送功能
 * @code
 *          REG_PROTOBUF_OPT_TIME_SEND_DEST(proto_name, 0x02, 0x01, USART_LINK, 0x20, 0x21)
 * @endcode
 */
#define REG_PROTOBUF_OPT_TIME_SEND_DEST(proto_name, _cmd_func, _cmd_id, _link_id, _dest...)                        \
    extern uint32_t protobuf_opt_handle_##proto_name(uint8_t type, proto_name *p_cache_buf, uint32_t dest_index);  \
    uint8_t proto_opt_send_dest_##proto_name[] = {_dest};                                                          \
    static void protobuf_opt_send_dest_##proto_name(void)                                                          \
    {                                                                                                              \
        static uint32_t dest_index = 0;                                                                            \
        proto_name proto_cmd_struct;                                                                               \
        if (protobuf_opt_handle_##proto_name(XF_PROTOBUF_OPT_TYPE_CMD_SEND, &proto_cmd_struct, dest_index) != 0) { \
            hal_frame_info_t frame_info = {0};                                                                     \
            frame_info.cmd_func = _cmd_func;                                                                       \
            frame_info.cmd_id = _cmd_id;                                                                           \
            frame_info.dest = proto_opt_send_dest_##proto_name[dest_index];                                        \
            frame_info.d_dest = 0x01;                                                                              \
            frame_info.link_id = _link_id;                                                                         \
            uint32_t send_len = 0;                                                                                 \
            send_len = send_len;                                                                                   \
            PROTOBUF_SEND_BY_ID(&frame_info, proto_name, &proto_cmd_struct, send_len);                             \
        }                                                                                                          \
        dest_index = (dest_index + 1) % ARRY_ITEMS_NUM(proto_opt_send_dest_##proto_name);                          \
    }                                                                                                              \
    REG_TASK(VH_TASK, XF_PROTOBUF_OPT_TASK_TIME, protobuf_opt_send_dest_##proto_name)

/**
 * @brief 头部 - 获取可选字段需要发送信息，无 ack
 *
 * @param [in]  proto_name          proto 参数结构体名称
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_TIME_SEND_GROUP_ITEM 和 DEF_PROTOBUF_OPT_TIME_SEND_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_TIME_SEND_DEST 注册发送的功能
 *
 * @par     定义消息为proto_name的发送信息
 * @code
 *          DEF_PROTOBUF_OPT_TIME_SEND_START(proto_name)
 *              DEF_PROTOBUF_OPT_TIME_SEND_GROUP_ITEM(proto_arry_var[0], has_proto_arry_var, user_arry_var[0], sizeof(user_arry_var), 1000)
 *              DEF_PROTOBUF_OPT_TIME_SEND_GROUP_ITEM(proto_struct_var, has_proto_struct_var, user_struct_var, sizeof(user_struct_var), 2000)
 *          DEF_PROTOBUF_OPT_TIME_SEND_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_TIME_SEND_START(proto_name)                                                       \
    uint32_t protobuf_opt_handle_##proto_name(uint8_t type, proto_name *p_handle_buf, uint32_t dest_index) \
    {                                                                                                      \
        static const uint8_t dest_num = ARRY_ITEMS_NUM(proto_opt_send_dest_##proto_name);                  \
        uint32_t single_num = 0;                                                                           \
        if (type == XF_PROTOBUF_OPT_TYPE_CMD_SEND) {                                                       \
            memset(p_handle_buf, 0, sizeof(proto_name));                                                   \
        }

/**
 * @brief 选项 - 获取定时字段需要发送信息 - 单个变量，无 ack
 *
 * @param [in]  proto_var           proto 成员变量名称
 * @param [in]  has_proto_var       设置可选变量可选参数的名称
 * @param [in]  user_var            用户变量
 * @param [in]  timeout             超时时间, 最大值不能超过 65535 * XF_PROTOBUF_OPT_TASK_TIME
 *
 * @details
 *          1. 间隔  timeout 时间发送数据
 *          4. 该接口需要与 DEF_PROTOBUF_OPT_TIME_SEND_START 和 DEF_PROTOBUF_OPT_TIME_SEND_END 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_TIME_SEND_DEST 注册发送的功能
 *
 * @par     定义消息为proto_name的发送信息
 * @code
 *          DEF_PROTOBUF_OPT_TIME_SEND_START(proto_name)
 *              DEF_PROTOBUF_OPT_TIME_TIME_SEND_ITEM(proto_var, has_proto_var, user_var, 4000)
 *              ///< ...
 *          DEF_PROTOBUF_OPT_TIME_SEND_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_TIME_SEND_ITEM(proto_var, has_proto_var, user_var, timeout)                                                             \
    {                                                                                                                                            \
        static uint16_t time_cnt_index[dest_num];                                                                                                \
        __PROTOBUF_OPT_SEND_ITEM_JUDGE(proto_var, has_proto_var, user_var, timeout / XF_PROTOBUF_OPT_TASK_TIME, time_cnt_index[dest_index] == 0) \
    }

/**
 * @brief 选项 - 获取定时字段需要发送信息 - 组选项，可以是数组，结构体等，无 ack
 *
 * @param [in]  proto_var          proto 成员变量名称，变量首地址
 * @param [in]  has_proto_var      设置可选变量可选参数的名称
 * @param [in]  user_var           用户变量，变量首地址
 * @param [in]  user_len           用户参数长度
 * @param [in]  timeout            超时时间
 *
 * @details
 *          注意：首地址示例： 比如数组为 arry[0], 结构体为成员名，指针则为 point[0]
 *          1. 超过设定的时间后进行发送
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_TIME_SEND_DEST 注册发送的功能
 *
 * @par     定义消息为proto_name的发送信息
 * @code
 *          DEF_PROTOBUF_OPT_TIME_SEND_START(proto_name)
 *              DEF_PROTOBUF_OPT_TIME_SEND_GROUP_ITEM(proto_arry_var[0], has_proto_arry_var, user_arry_var[0], sizeof(user_arry_var), 1000)
 *              DEF_PROTOBUF_OPT_TIME_SEND_GROUP_ITEM(proto_struct_var, has_proto_struct_var, user_struct_var, sizeof(user_struct_var), 2000)
 *          DEF_PROTOBUF_OPT_TIME_SEND_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_TIME_SEND_GROUP_ITEM(proto_var, has_proto_var, user_var, user_len, timeout)                            \
    {                                                                                                                           \
        static OPT_STIME_TYPE time_cnt_index[dest_num];                                                                         \
        __PROTOBUF_OPT_TIME_SEND_GROUP_JUDGE(proto_var, has_proto_var, user_var, user_len, timeout / XF_PROTOBUF_OPT_TASK_TIME) \
    }

/**
 * @brief 尾部 - 获取可选字段需要发送信息，无 ack
 *
 * @param [in]  proto_name          proto 参数结构体名称
 *
 * @details
 *          该接口需要与 DEF_PROTOBUF_OPT_TIME_SEND_GROUP_ITEM 和 DEF_PROTOBUF_OPT_TIME_SEND_START 配合使用
 *          定义完成之后需要调用 REG_PROTOBUF_OPT_TIME_SEND_DEST 注册发送的功能
 *
 * @par     定义消息为proto_name的发送信息
 * @code
 *          DEF_PROTOBUF_OPT_TIME_SEND_START(proto_name)
 *              DEF_PROTOBUF_OPT_TIME_SEND_GROUP_ITEM(proto_arry_var[0], has_proto_arry_var, user_arry_var[0], sizeof(user_arry_var), 1000)
 *              DEF_PROTOBUF_OPT_TIME_SEND_GROUP_ITEM(proto_struct_var, has_proto_struct_var, user_struct_var, sizeof(user_struct_var), 2000)
 *          DEF_PROTOBUF_OPT_TIME_SEND_END()
 * @endcode
 */
#define DEF_PROTOBUF_OPT_TIME_SEND_END() \
    end:                                 \
    return single_num;                   \
    }

#ifdef __cplusplus
}
#endif

#endif // XF_PROTOBUF_OPT_H
