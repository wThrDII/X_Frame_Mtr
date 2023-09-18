/**
 * @brief protobuf 增量上传私有协议部分
 * @file xf_protobuf_opt_pri.h
 * @version 1.0
 * @author juntong.chen
 * @date 2022-12-09 11:56:29
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @details 文件详细描述，包含使用场景，依赖项等
 * @par 修改日志:
 * <table>
 * <tr><th>Date         <th>Author         <th>Description
 * <tr><td>2022-12-09 11:56:29      <td>juntong.chen      <td>创建文件
 */

#ifndef XF_PROTOBUF_OPT_PRI_H
#define XF_PROTOBUF_OPT_PRI_H

#ifdef __cplusplus
extern "C" {
#endif

#define XF_PROTOBUF_OPT_TIMEOUT 3000  ///< 发送数据后等待应答的超时时间
#define XF_PROTOBUF_OPT_TASK_TIME 100 ///< 发送数据的间隔
#define XF_PROTOBUF_OPT_SINGLE_MAX 30 ///< 单次上传的最大个数

#define XF_PROTOBUF_OPT_TYPE_CMD_SEND 1   ///< 指令发送
#define XF_PROTOBUF_OPT_TYPE_ACK_HANDLE 2 ///< 应答处理

///< 选择时间的变量类型
#if (XF_PROTOBUF_OPT_TIMEOUT / XF_PROTOBUF_OPT_TASK_TIME) < 256
#define OPT_STIME_TYPE uint8_t
#elif (XF_PROTOBUF_OPT_TIMEOUT / XF_PROTOBUF_OPT_TASK_TIME) < 65535
#define OPT_STIME_TYPE uint16_t
#else
#define OPT_STIME_TYPE uint32_t
#endif

/**
 * @brief 发送item的应答处理
 *
 * @details 内部接口
 *          1. 判断item是否下发
 *          2. 拷贝数据
 */
#define __PROTOBUF_OPT_SEND_ITEM_ACK_HANDLE(proto_var, has_proto_var, user_var) \
    if (p_handle_buf->has_proto_var == true) {                                  \
        user_var = p_handle_buf->proto_var;                                     \
        p_cache_buf->proto_var = p_handle_buf->proto_var;                       \
    }

/**
 * @brief 发送 group 的应答处理
 *
 * @details 内部接口
 *          1. 判断item是否下发
 *          2. 拷贝数据
 */
#define __PROTOBUF_OPT_SEND_GROUP_ITEM_ACK_HANDLE(proto_var, has_proto_var, user_var, user_len) \
    if (p_handle_buf->has_proto_var == true) {                                                  \
        memcpy(&user_var, &p_handle_buf->proto_var, user_len);                                  \
        memcpy(&p_cache_buf->proto_var, &p_handle_buf->proto_var, user_len);                    \
    }

/**
 * @brief item 步长判断条件
 *
 * @details 内部接口
 *          1. 判断时间条件
 *          2. 判断步长条件
 */
#define __PROTOBUF_OPT_SEND_ITEM_STEP_CONDITION(proto_var, user_var, step) time_cnt_index[dest_index] == 0 && ((((int32_t)(p_cache_buf->proto_var - user_var)) > 0) ? (p_cache_buf->proto_var - user_var) : (user_var - p_cache_buf->proto_var)) >= step

/**
 * @brief item 判断条件
 *
 * @details 内部接口
 *          1. 判断时间条件
 *          2. 判断变量是否不相等
 */
#define __PROTOBUF_OPT_SEND_ITEM_CONDITION(proto_var, user_var) time_cnt_index[dest_index] == 0 && p_cache_buf->proto_var != user_var

/**
 * @brief 判断是否需要发送 item
 *
 * @details 内部接口
 *          1. 判断条件是否满足
 *              |-- 设置发送相关参数
 *          2. 时间计数递减
 *          3. 判断发送数据个数是否满足最大个数
 */
#define __PROTOBUF_OPT_SEND_ITEM_JUDGE(proto_var, has_proto_var, user_var, time_interval, send_condition) \
    if (send_condition) {                                                                                 \
        p_handle_buf->has_proto_var = true;                                                               \
        p_handle_buf->proto_var = user_var;                                                               \
        time_cnt_index[dest_index] = time_interval;                                                       \
        single_num++;                                                                                     \
    }                                                                                                     \
    else if (time_cnt_index[dest_index] >= dest_num) {                                                    \
        time_cnt_index[dest_index] -= dest_num;                                                           \
    }                                                                                                     \
    else {                                                                                                \
        time_cnt_index[dest_index] = 0;                                                                   \
    }                                                                                                     \
    if (single_num >= XF_PROTOBUF_OPT_SINGLE_MAX) {                                                       \
        goto end;                                                                                         \
    }

/**
 * @brief 判断是否需要发送 group
 *
 * @details 内部接口
 *          1. 判断条件是否满足
 *              |-- 设置发送相关参数
 *          2. 时间计数递减
 *          3. 判断发送数据个数是否满足最大个数
 */
#define __PROTOBUF_OPT_SEND_GROUP_JUDGE(proto_var, has_proto_var, user_var, user_len, time_interval)    \
    if (time_cnt_index[dest_index] == 0 && memcmp(&p_cache_buf->proto_var, &user_var, user_len) != 0) { \
        p_handle_buf->has_proto_var = true;                                                             \
        memcpy(&p_handle_buf->proto_var, &user_var, user_len);                                          \
        single_num = single_num + user_len / 4;                                                         \
        time_cnt_index[dest_index] = time_interval;                                                     \
    }                                                                                                   \
    else if (time_cnt_index[dest_index] >= dest_num) {                                                  \
        time_cnt_index[dest_index] -= dest_num;                                                         \
    }                                                                                                   \
    else {                                                                                              \
        time_cnt_index[dest_index] = 0;                                                                 \
    }                                                                                                   \
    if (single_num >= XF_PROTOBUF_OPT_SINGLE_MAX) {                                                     \
        goto end;                                                                                       \
    }

/**
 * @brief 判断是否需要发送 group
 *
 * @details 内部接口
 *          1. 判断条件是否满足
 *              |-- 设置发送相关参数
 *          2. 时间计数递减
 *          3. 判断发送数据个数是否满足最大个数
 */
#define __PROTOBUF_OPT_TIME_SEND_GROUP_JUDGE(proto_var, has_proto_var, user_var, user_len, time_interval) \
    if (time_cnt_index[dest_index] == 0) {                                                                \
        p_handle_buf->has_proto_var = true;                                                               \
        memcpy(&p_handle_buf->proto_var, &user_var, user_len);                                            \
        single_num = single_num + user_len / 4;                                                           \
        time_cnt_index[dest_index] = time_interval;                                                       \
    }                                                                                                     \
    else if (time_cnt_index[dest_index] >= dest_num) {                                                    \
        time_cnt_index[dest_index] -= dest_num;                                                           \
    }                                                                                                     \
    else {                                                                                                \
        time_cnt_index[dest_index] = 0;                                                                   \
    }                                                                                                     \
    if (single_num >= XF_PROTOBUF_OPT_SINGLE_MAX) {                                                       \
        goto end;                                                                                         \
    }

#ifdef __cplusplus
}
#endif

#endif // XF_PROTOBUF_OPT_PRI_H
