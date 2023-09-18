/**
 * @brief protocol_hal.c.xtemp header file
 * @file protocol_hal.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 16:24:40
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 16:24:40
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __PROTOCOL_HAL_H__
#define __PROTOCOL_HAL_H__

#define ERR_UNKNOW (-1)      //未知错误
#define ERR_NOT_INIT (-2)    //协议未初始化
#define ERR_LOCK (-3)        //重入
#define ERR_NO_LINK (-4)     //未找到链路
#define ERR_NO_PROTOCOL (-5) //未找到协议

#define MAX_PROTCOL_PRIO 4

#define MAX_RECV_PACK_LEN 512 // 接收包的最大长度
#define MAX_SEND_PACK_LEN 512 // 发送包的最大长度

#define PROTOCOL_EF2_MAX_LEN (MAX_RECV_PACK_LEN - 18) /* 协议2最大解包长度 */
#define PROTOCOL_EF3_MAX_LEN (MAX_RECV_PACK_LEN - 20) /* 协议3最大解包长度 */

#define IS_HOST_PACK (0)      // 本机包
#define IS_ROUTE_PACK (1)     // 路由包
#define IS_HOST_ROUT_PACK (2) // 本机路由包
#define UN_KNOW_PACK (3)      // 未知包
#define DIRECT_PACK (4)       // 直发包
#define ERR_ADDR_PACK (5)     // 错误地址包

#define ROUTE_ADDR 0x01      // 路由地址
#define ROUTE_CMD 0x02       // 路由指令
#define ROUTE_LINK 0x03      // 路由链路
#define ROUTE_BROADCAST 0x04 //路由广播
#define ROUTE_LINK_ADDR 0x05 // 路由链路地址

#define EN_ROUTE 0x01 // 开启路由
#define EN_HACK 0x02

#define UN_USE_CMD 0xff  // 不使用指令
#define UN_USE_ADDR 0xff // 不使用地址

#define LINK_ACTION_CMD 1     // 链路回调指令
#define LINK_ACTION_ACK 2     // 链路回调应答
#define LINK_ACTION_CMD_ACK 3 // 链路回调指令应答

#define ACTION_GROUP_LINK 1   // 回调组链路
#define ACTION_GROUP_CMDSET 2 // 回调组指令设置

/**
 * @brief 链路注册宏
 *
 * @param [in] link_id                 // 链路id
 * @param [in] prio                    // 优先级
 * @param [in] dev                     // 设备名称
 * @param [in] protocol_ver            // 协议版本
 * @param [in] buf_size                // 链路数据流缓存大小
 * @param [in] en_route                // 是否路由
 * @param [in] en_debug                // 是否调试
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *    REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)
 * @endcode
 */
#define REG_LINK(link_id, prio, dev, protocol_ver, buf_size, en_route, en_debug)                                                                                \
    DEF_PROTOCOL_MATRIX(link_id, protocol_ver, buf_size, false, 500, get_sys_ms)                                                                                \
    link_hal_item reg_link_##link_id = {#link_id, link_id, prio, PTR_TYPE_XH_DEV(dev), PTR_TYPE_XH_DEV(dev), PROTOCOL_MATRIX_PTR(link_id), en_route, en_debug}; \
    AUTO_REG_ITEM(LINK_##link_id, AUTO_REG_TYPE_LINK, (void *)&reg_link_##link_id);

/**
 * @brief 协议回调组注册宏
 *
 * @param [in] link_id            // 链路id
 * @param [in] cmd_set            // 指令
 * @param [in] cmd_id             // 指令id
 * @param [in] cmd_type           // 指令类型
 * @param [in] group_type         // 组别
 * @param [in] func               // 回调函数
 *
 * @details
 * @par eg:
 * @code
 *      //  先注册链路：
 *      REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路
 *      void action_func(void)
 *      {
 *            // 回调执行内容
 *      }
 *      REG_PROTOCOL_ACTION_FULL( LINK_UART1, 0xf0, 0x03, LINK_ACTION_CMD_ACK, ACTION_GROUP_LINK, action_func);
 * @endcode
 */
#define REG_PROTOCOL_ACTION_FULL(link_id, cmd_set, cmd_id, cmd_type, group_type, func)                                                                \
    static const uint8_t __id_##link_id##_##cmd_set##_##cmd_id##_##cmd_type##_##group_type = link_id;                                                   \
    static const action_cfg_t __act_cfg_##link_id##_##cmd_set##_##cmd_id##_##cmd_type##_##group_type = {cmd_set, cmd_id, cmd_type, group_type, func, #func, &__id_##link_id##_##cmd_set##_##cmd_id##_##cmd_type##_##group_type, 1};\
    static action_tab_t action_##link_id##_##cmd_set##_##cmd_id##_##cmd_type##_##group_type = {&__act_cfg_##link_id##_##cmd_set##_##cmd_id##_##cmd_type##_##group_type}; \
    AUTO_REG_ITEM(action_##link_id##_##cmd_set##_##cmd_id##_##cmd_type##_##group_type, AUTU_REG_TYPE_LINK_ACTION, (void *)&action_##link_id##_##cmd_set##_##cmd_id##_##cmd_type##_##group_type);

/**
 * @brief 协议回调组注册不定长链路id宏，内部使用，不对外使用
 *
 * @param [in] cmd_set            // 指令
 * @param [in] cmd_id             // 指令id
 * @param [in] cmd_type           // 指令类型
 * @param [in] group_type         // 组别
 * @param [in] func               // 回调函数
 * @param [in] link_id...         // 不定长链路id
 *
 * @details
 * @par eg:
 * @code
 *      //  先注册链路：
 *      REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路1
 *      REG_LINK(LINK_UART2, 	VH_TASK, DRV_USART1, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路2
 *      void action_func(void)
 *      {
 *            // 回调执行内容
 *      }
 *      ///> 一次注册两条链路都支持相同指令
 *      REG_PROTOCOL_MULTI_ACTION_FULL( action_func, 0xf0, 0x03, LINK_ACTION_CMD_ACK, ACTION_GROUP_LINK, LINK_UART1, LINK_UART2);
 * @endcode
 */
#define REG_PROTOCOL_MULTI_ACTION_FULL(cmd_set, cmd_id, cmd_type, group_type, func, link_id...)                                                                 \
        static const uint8_t __id_##cmd_set##_##cmd_id##_##cmd_type##_##group_type[] = {link_id};                                                               \
        static const action_cfg_t __act_cfg_##cmd_set##_##cmd_id##_##cmd_type##_##group_type = {cmd_set, cmd_id, cmd_type, group_type, func, #func, __id_##cmd_set##_##cmd_id##_##cmd_type##_##group_type, sizeof(__id_##cmd_set##_##cmd_id##_##cmd_type##_##group_type)};\
        static action_tab_t action_##cmd_set##_##cmd_id##_##cmd_type##_##group_type[sizeof(__id_##cmd_set##_##cmd_id##_##cmd_type##_##group_type)] = {[0 ... (sizeof(__id_##cmd_set##_##cmd_id##_##cmd_type##_##group_type) - 1)] = {&__act_cfg_##cmd_set##_##cmd_id##_##cmd_type##_##group_type}}; \
        AUTO_REG_ITEM(action_##cmd_set##_##cmd_id##_##cmd_type##_##group_type, AUTU_REG_TYPE_LINK_ACTION, (void *)&action_##cmd_set##_##cmd_id##_##cmd_type##_##group_type);

/**
 * @brief 链路回调指令应答注册宏，不论是其它设备主动发送给本机的指令或者是其它设备响应本机的控制指令，发送响应指令到本机时，都会触发该回调
 *
 * @param [in] link_id            // 链路id
 * @param [in] cmd_set            // 指令
 * @param [in] cmd_id             // 指令id
 * @param [in] func               // 回调函数
 *
 * @details
 * @par eg:   //注册的函数需提前写好函数定义
 * @code
 *      //  先注册链路：
 *      REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路
 *      void action_func(void)
 *      {
 *            // 回调执行内容
 *      }
 *      REG_LINK_ACTION( LINK_UART1, 0xf0, 0x03, action_func);
 * @endcode
 */
#define REG_LINK_ACTION(link_id, cmd_set, cmd_id, func) REG_PROTOCOL_ACTION_FULL(link_id, cmd_set, cmd_id, LINK_ACTION_CMD_ACK, ACTION_GROUP_LINK, func)
/**
 * @brief 链路回调指令注册宏，只有其它设备主动发送给本机的指令才能触发该回调
 *
 * @param [in] link_id            // 链路id
 * @param [in] cmd_set            // 指令
 * @param [in] cmd_id             // 指令id
 * @param [in] func               // 回调函数
 *
 * @details
 * @par eg:   //注册的函数需提前写好函数定义
 * @code
 *      //  先注册链路：
 *      REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路
 *      void action_func(void)
 *      {
 *            // 回调执行内容
 *      }
 *      REG_LINK_ACTION_CMD( LINK_UART1, 0xf0, 0x03, action_func);
 * @endcode
 */
#define REG_LINK_ACTION_CMD(link_id, cmd_set, cmd_id, func) REG_PROTOCOL_ACTION_FULL(link_id, cmd_set, cmd_id, LINK_ACTION_CMD, ACTION_GROUP_LINK, func)
/**
 * @brief 链路回调应答注册宏，只有其它设备响应本机的控制指令，发送响应指令到本机时才能触发该回调
 *
 * @param [in] link_id            // 链路id
 * @param [in] cmd_set            // 指令
 * @param [in] cmd_id             // 指令id
 * @param [in] func               // 回调函数
 *
 * @details
 * @par eg:   //注册的函数需提前写好函数定义
 * @code
 *      //  先注册链路：
 *      REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路
 *      void action_func(void)
 *      {
 *            // 回调执行内容
 *      }
 *      REG_LINK_ACTION_ACK( LINK_UART1, 0xf0, 0x03, action_func);
 * @endcode
 */
#define REG_LINK_ACTION_ACK(link_id, cmd_set, cmd_id, func) REG_PROTOCOL_ACTION_FULL(link_id, cmd_set, cmd_id, LINK_ACTION_ACK, ACTION_GROUP_LINK, func)

/**
 * @brief 链路回调指令应答不定长链路注册宏，不论是其它设备主动发送给本机的指令或者是其它设备响应本机的控制指令，发送响应指令到本机时，都会触发该回调
 *
 * @param [in] cmd_set            // 指令
 * @param [in] cmd_id             // 指令id
 * @param [in] func               // 回调函数
 * @param [in] link_id...         // 不定长链路id
 *
 * @details
 * @par eg:   //注册的函数需提前写好函数定义
 * @code
 *      //  先注册链路：
 *      REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路1
 *      REG_LINK(LINK_UART2, 	VH_TASK, DRV_USART1, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路2
 *      void action_func(void)
 *      {
 *            // 回调执行内容
 *      }
 *      ///> 一次注册两条链路都支持相同指令
 *      REG_MULTI_LINK_ACTION( 0xf0, 0x03, action_func, LINK_UART1, LINK_UART2);
 * @endcode
 */
#define REG_MULTI_LINK_ACTION(cmd_set, cmd_id, func, link_id...) REG_PROTOCOL_MULTI_ACTION_FULL(cmd_set, cmd_id, LINK_ACTION_CMD_ACK, ACTION_GROUP_LINK, func, link_id)

/**
 * @brief 链路回调指令不定长链路注册宏，只有其它设备主动发送给本机的指令才能触发该回调
 *
 * @param [in] cmd_set            // 指令
 * @param [in] cmd_id             // 指令id
 * @param [in] func               // 回调函数
 * @param [in] link_id...         // 不定长链路id
 *
 * @details
 * @par eg:   //注册的函数需提前写好函数定义
 * @code
 *      //  先注册链路：
 *      REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路1
 *      REG_LINK(LINK_UART2, 	VH_TASK, DRV_USART1, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路2
 *      void action_func(void)
 *      {
 *            // 回调执行内容
 *      }
 *      ///> 一次注册两条链路都支持相同指令
 *      REG_MULTI_LINK_ACTION_CMD( 0xf0, 0x03, action_func, LINK_UART1, LINK_UART2);
 * @endcode
 */
#define REG_MULTI_LINK_ACTION_CMD(cmd_set, cmd_id, func, link_id...) REG_PROTOCOL_MULTI_ACTION_FULL(cmd_set, cmd_id, LINK_ACTION_CMD, ACTION_GROUP_LINK, func, link_id)

/**
 * @brief 链路回调指令不定长链路注册宏，只有其它设备主动发送给本机的指令才能触发该回调
 *
 * @param [in] cmd_set            // 指令
 * @param [in] cmd_id             // 指令id
 * @param [in] func               // 回调函数
 * @param [in] link_id...         // 不定长链路id
 *
 * @details
 * @par eg:   //注册的函数需提前写好函数定义
 * @code
 *      //  先注册链路：
 *      REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路1
 *      REG_LINK(LINK_UART2, 	VH_TASK, DRV_USART1, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路2
 *      void action_func(void)
 *      {
 *            // 回调执行内容
 *      }
 *      ///> 一次注册两条链路都支持相同指令
 *      REG_MULTI_LINK_ACTION_ACK( 0xf0, 0x03, action_func, LINK_UART1, LINK_UART2);
 * @endcode
 */
#define REG_MULTI_LINK_ACTION_ACK(cmd_set, cmd_id, func, link_id...) REG_PROTOCOL_MULTI_ACTION_FULL(cmd_set, cmd_id, LINK_ACTION_ACK, ACTION_GROUP_LINK, func, link_id)

/**
 * @brief 链路回调应答注册宏，批量注册多个链路支持一个cmd_set，这个注册是给REG_CMDSET_ACTION用的
 *
 * @param [in] cmdset             // 指令集
 * @param [in] link...            // 不定长链路id参数
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *      REG_CMDSET_ACTION_LINK( 0xf0, LINK_UART0, LINK_UART1 );
 * @endcode
 */
#define REG_CMDSET_ACTION_LINK(cmdset, link...)                                                                                            \
    uint32_t cmdset_action_link_##cmdset[] = {link};                                                                                       \
    cmdset_link_tab_t cmdset_action_link_tab##cmdset = {cmdset, cmdset_action_link_##cmdset, ARRY_ITEMS_NUM(cmdset_action_link_##cmdset)}; \
    AUTO_REG_ITEM(reg_cmdset_action_link_tab##cmdset, AUTU_REG_TYPE_CMDSET_LINK_TAB, (void *)&cmdset_action_link_tab##cmdset);

/**
 * @brief 指令集链路回调指令应答注册宏，如果注册的cmd_set没有用REG_CMDSET_ACTION_LINK注册过，那这个回调注册是不生效的
 *        不论是其它设备主动发送给本机的指令或者是其它设备响应本机的控制指令，发送响应指令到本机时，都会触发该回调
 *
 * @param [in] cmd_set            // 指令
 * @param [in] cmd_id             // 指令id
 * @param [in] func               // 回调函数
 *
 * @details
 * @par eg:   //注册的函数需提前写好函数定义
 * @code
 *      //  先注册链路：
 *      REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路
 *      void action_func(void)
 *      {
 *            // 回调执行内容
 *      }
 *      REG_CMDSET_ACTION_LINK(0xf0,LINK_UART1);        ///< 先注册一下链路，绑定cmd_set和link之间的关系
 *      REG_CMDSET_ACTION( 0xf0, 0x03, action_func);
 *      REG_CMDSET_ACTION( 0xf0, 0x04, action_func);
 * @endcode
 */
#define REG_CMDSET_ACTION(cmd_set, cmd_id, func) REG_PROTOCOL_ACTION_FULL(NULL, cmd_set, cmd_id, LINK_ACTION_CMD_ACK, ACTION_GROUP_CMDSET, func)

/**
 * @brief 指令集链路回调指令注册宏，如果注册的cmd_set没有用REG_CMDSET_ACTION_LINK注册过，那这个回调注册是不生效的
 *        只有其它设备主动发送给本机的指令才能触发该回调
 *
 * @param [in] cmd_set            // 指令
 * @param [in] cmd_id             // 指令id
 * @param [in] func               // 回调函数
 *
 * @details 特殊说明:    实际上是调用REG_PROTOCOL_ACTION_FULL宏接口将回调注册到组别为ACTION_GROUP_CMDSET、指令类型为LINK_ACTION_CMD的链路中。
 * @par eg:   //注册的函数需提前写好函数定义
 * @code
 *      void action_func(void)
 *      {
 *            // 回调执行内容
 *      }
 *      REG_CMDSET_ACTION_CMD( 0xf0, 0x03, action_func);
 * @endcode
 */
#define REG_CMDSET_ACTION_CMD(cmd_set, cmd_id, func) REG_PROTOCOL_ACTION_FULL(NULL, cmd_set, cmd_id, LINK_ACTION_CMD, ACTION_GROUP_CMDSET, func)

/**
 * @brief 指令集链路回调应答注册宏，如果注册的cmd_set没有用REG_CMDSET_ACTION_LINK注册过，那这个回调注册是不生效的
 *        只有其它设备响应本机的控制指令，发送响应指令到本机时才能触发该回调
 *
 * @param [in] cmd_set            // 指令
 * @param [in] cmd_id             // 指令id
 * @param [in] func               // 回调函数
 *
 * @details
 * @par eg:   //注册的函数需提前写好函数定义
 * @code
 *      void action_func(void)
 *      {
 *            // 回调执行内容
 *      }
 *      REG_CMDSET_ACTION_ACK( 0xf0, 0x03, action_func);
 * @endcode
 */
#define REG_CMDSET_ACTION_ACK(cmd_set, cmd_id, func) REG_PROTOCOL_ACTION_FULL(NULL, cmd_set, cmd_id, LINK_ACTION_ACK, ACTION_GROUP_CMDSET, func)

/**
 * @brief 指令集数据静默注册宏，实际场景是根据组别进行静默，最多支持8个组
 *
 * @param [in] cmd_set            // 指令
 * @param [in] cmd_id             // 指令id
 * @param [in] group_id           // 组id，范围0-7，不同的指令集可以注册到相同组id，静默时按照组进行开关
 *
 * @details
 * @par eg:
 * @code
 *      REG_SILENCE_CMD( 0xf0, 0x03, 0);
 * @endcode
 */
#define REG_SILENCE_CMD(cmd_set, cmd_id, group_id)                                 \
    silence_item_t silence_##cmd_set##_##cmd_id = {cmd_set, cmd_id, group_id % 8}; \
    AUTO_REG_ITEM(silence_##cmd_set##_##cmd_id, AUTO_REG_SILENCE, (void *)&silence_##cmd_set##_##cmd_id);
/**
 * @brief 注册链路消息条件监听，只要在该链路的数据中匹配到相关的src、dest、cmd_set、cmd_id都会执行注册的回调
 *
 * @param [in] link_id            // 链路id
 * @param [in] src                // 源地址
 * @param [in] dest               // 目标地址
 * @param [in] cmd_set            // 指令
 * @param [in] cmd_id             // 指令id
 * @param [in] func               // 执行回调，类型为：void (*pf_monitor_func)(hal_frame_info_t *const p_frame)
 *
 * @details
 * @par eg:
 * @code
 *      void monitor_func(hal_frame_info_t *const p_frame)
 *      {
 *          ...
 *      }
 *      REG_LINK_MONITOR_COND( 0x01, 0xf0, 0x03, 0xf0, 0x03, monitor_func);
 * @endcode
 */
#define REG_LINK_MONITOR_COND(link_id, src, dest, cmd_set, cmd_id, func)                                                                                 \
    static const link_monitor_item_t item_##link_id##_##src##_##dest##_##_##cmd_set##_##cmd_id = {link_id, func, src, dest, cmd_set, cmd_id};            \
    static link_monitor_data_t data_##link_id##_##src##_##dest##_##_##cmd_set##_##cmd_id = {&item_##link_id##_##src##_##dest##_##_##cmd_set##_##cmd_id}; \
    AUTO_REG_ITEM(data_##link_id##_##src##_##dest##_##_##cmd_set##_##cmd_id, AUTO_REG_LINK_MONITOR, (void *)&data_##link_id##_##src##_##dest##_##_##cmd_set##_##cmd_id);
/**
 * @brief 注册链路消息监听，只要该链路接收到数据都会执行注册的回调
 *
 * @param [in] link_id            // 链路id
 * @param [in] func               // 执行回调，类型为：void (*pf_monitor_func)(hal_frame_info_t *const p_frame)
 *
 * @details
 * @par eg:
 * @code
 *      void monitor_func(hal_frame_info_t *const p_frame)
 *      {
 *          ...
 *      }
 *      REG_LINK_MONITOR_COND( 0x01, monitor_func);
 * @endcode
 */
#define REG_LINK_MONITOR(link_id, func)                                            \
    static const link_monitor_item_t monitor_item_##link_id = {link_id, func};     \
    static link_monitor_data_t monitor_data_##link_id = {&monitor_item_##link_id}; \
    AUTO_REG_ITEM(monitor_data_##link_id, AUTO_REG_LINK_MONITOR, (void *)&monitor_data_##link_id);

/**
 * @brief 协议发送数据，接口内部会强制修改本机地址，本机动态地址(V3)，product id，数据校验类型以及可能修改payload加密方式，保证数据发送时的基本配置的一致性
 *
 * @param [in] p_frame_info  协议包参数指针
 *
 * @return int32_t           成功：写入驱动的字节数；失败：< 0
 *  @retval
 * @details 特殊说明:     //与protocol_send接口的区别：protocol_send会进行路由包判断，若为路由包会在发送的协议包中的link_id写入目标link_id, 本接口则不会；
 * @par eg:
 * @code
 *    //  先注册链路：
 *    REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路
 *    //  使用：
 *    void send_data_test(void)
 *    {
 *        ...
 *        //
 *        hal_frame_info_t frame_info_t = {....(结构体数据填充)};
 *        protocol_send_by_id(&frame_info_t);
 *        //
 *        ...
 *    }
 * @endcode
 */
#define protocol_send_by_id(p_frame_info) real_protocol_send_by_id(p_frame_info)

/**
 * @brief 协议发送应答，会修改源地址为本机地址和修改目的地址为数据包的源地址，以及动态地址、响应标志位、product id，保证数据响应时的基本配置的准确性
 *
 * @param [in] p_frame_info  协议应答包参数指针
 * @param [in] pbuf          应答包发送的数据源地址
 * @param [in] len           源数据长度
 *
 * @return int32_t
 *  @retval
 * @details 特殊说明:
 * @par eg:
 * @code
 *    void protocol_send_ack_test(hal_frame_info_t *p_frame_info)
 *    {
 *        ...
 *        //
 *        uint8_t data[] = { 01, 02, 03 };
 *        uint32_t len = (sizeof(data)/sizeof(uint8_t));
 *        if(p_frame_info->need_ack)
 *        {
 *            real_protocol_send_ack(p_frame_info,data,len);
 *        }
 *        //
 *        ...
 *    }
 * @endcode
 */
#define protocol_send_ack(p_frame_info, pbuf, len) real_protocol_send_ack(p_frame_info, pbuf, len)

/**
 * @brief 协议发送数据，这是比较早的接口，应该是很少项目在使用了，该接口会再进行路由规则判断，主动发送时使用protocol_send_by_id即可
 *
 * @param [in] p_frame_info  协议包参数指针
 *
 * @return int32_t           成功：写入驱动的字节数；失败：< 0
 *  @retval
 * @details 特殊说明:
 * @par eg:
 * @code
 *    //  先注册链路：
 *    REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路
 *    //  使用：
 *    void send_data_test(void)
 *    {
 *        ...
 *        //
 *        hal_frame_info_t p_frame_info_t = {....(结构体数据填充)};
 *        real_protocol_send(&p_frame_info_t);
 *        //
 *        ...
 *    }
 * @endcode
 */
#define protocol_send(p_frame_info) real_protocol_send(p_frame_info)

/**
 * @brief 协议发送数据，xframe内部使用接口，应用开发者等效使用protocol_send_frame接口
 *
 * @param [in] p_frame_info  协议包参数指针
 *
 * @return int32_t           成功：写入驱动的字节数；失败：< 0
 *  @retval
 * @details 特殊说明:
 * @par eg:
 * @code
 *    //  先注册链路：
 *    REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路
 *    //  使用：
 *    void send_data_test(void)
 *    {
 *        ...
 *        //
 *        hal_frame_info_t p_frame_info_t = {....(结构体数据填充)};
 *        protocol_route_send_by_id(&p_frame_info_t);
 *        //
 *        ...
 *    }
 * @endcode
 */
#define protocol_route_send_by_id(p_frame_info) route_protocol_send_by_id(p_frame_info)

/**
 * @brief 协议发送数据，这个接口内部不会对数据帧进行多余的修改，会直接打包为协议数据进行发送，与protocol_route_send_by_id相同，仅修改了接口命名方便应用开发
 *
 * @param [in] p_frame_info  协议包参数指针
 *
 * @return int32_t           成功：写入驱动的字节数；失败：< 0
 *  @retval
 * @details 特殊说明:
 * @par eg:
 * @code
 *    //  先注册链路：
 *    REG_LINK(LINK_UART1, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)         //注册链路
 *    //  使用：
 *    void send_data_test(void)
 *    {
 *        ...
 *        //
 *        hal_frame_info_t p_frame_info_t = {....(结构体数据填充)};
 *        protocol_send_frame(&p_frame_info_t);
 *        //
 *        ...
 *    }
 * @endcode
 */
#define protocol_send_frame(p_frame_info) route_protocol_send_by_id(p_frame_info)

/**
 * @brief 判断设备是否被注册成协议链路
 *
 * @param [in] debug_dev   所需判断的设备名
 *
 * @return uint32_t        已注册返回：1；未注册返回：0；
 *  @retval
 * @details 特殊说明:
 * @par eg:    判断 xh_DRV_USART0 是否被注册成协议链路，若注册，通过该设备发送数据
 * @code
 *    uint8_t str[] = {"a","b","c"};
 *    uint32_t len = (sizeof(str)/sizeof(uint8_t));
 *    if(judge_link_write_dev(xh_DRV_USART0) == 0)
 *    {
 *       xh_stream_write(xh_DRV_USART0, str, len)
 *    }
 * @endcode
 */
#define judge_link_write_dev(debug_dev) real_judge_link_write_dev(debug_dev)

/**
 * @brief 使能静默设置
 *
 * @param [in] enable 0：退出使能，bit0:1使能静默组号为0的所有指令集，
 *								   bit1:1使能静默组号为1的所有指令集，依次类推
 *
 * @details:
 * @par eg:每隔1秒使能静默一次
 * @code
 * 		void silence_test_task(void){
 *			protocol_send_silence_set(0x01);
 *		}
 *
 *		REG_TASK(VH_TASK, 1000, silence_test_task);
 * @endcode
 */
#define protocol_send_silence_set(enable) real_protocol_send_silence_set(enable)

#define V3_BROAD_CAST_ADDR 0

/**
 * @brief 获取动态地址
 *
 * @param [in] action...         //获取动态地址的代码块
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_UART0, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)
 *      // 使用：
 *      DEF_EF3_GET_D_DEST( if( link_id == LINK_UART0 ){ uint8_t d_dest; d_dest= 0x00; return d_dest;} )
 * @endcode
 */
#define DEF_EF3_GET_D_DEST(action...)        \
    uint8_t ef3_get_d_dest(uint32_t link_id) \
    {                                        \
        action                               \
    }

/**
 * @brief   定义路由表
 *
 * @param [in] item...                  //路由信息
 *
 * @details 特殊说明:
 * @par eg:        ● 将04地址的数据往 LINK_A_B 路由
 *                 ● 将01地址的数据往 LINK_PC_A 路由
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF03_PLUS, 512, true, false)
 *      // 使用:
 *      DEF_ROUT_INFO_V3(
 *          _ADDR_ROUTE_ITEM_V3(true, 04, LINK_A_B)
 *          _ADDR_ROUTE_ITEM_V3(true, 01, LINK_PC_A)
 *          )
 * @endcode
 */
#define DEF_ROUT_INFO_V3(item...)                                                                                           \
    static link_route_t _v3_sg_route_tab[] = {item};                                                                        \
    static link_route_data_t _v3_sg_route_data = {_v3_sg_route_tab, ARRY_ITEMS_NUM(_v3_sg_route_tab), VERSION_EF3, true};   \
    AUTO_REG_ITEM(_v3_sg_route_data, AUTO_REG_LINK_ROUTE_TAB, (void *)&_v3_sg_route_data);

/**
 * @brief V3协议链路路由地址信息
 *
 * @param [in]     en                   // 路由
 * @param [in]     src_link             // 源链路
 * @param [in]     dest                 // 目标地址
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:      从LINK_A_B接收数据后转到LINK_PC_A链路发送到目标地址0x01
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF03_PLUS, 512, true, false)
 *      // 使用：
 *      LINK_ADDR_ROUTE_ITEM_V3( true, LINK_A_B, 0x01, LINK_PC_A )
 * @endcode
 */
#define LINK_ADDR_ROUTE_ITEM_V3(en, src_link, dest, tag_link)                               \
    {                                                                                       \
        ROUTE_LINK_ADDR, en, src_link, dest, 0xff, UN_USE_CMD, UN_USE_CMD, tag_link, NULL   \
    }
/**
 * @brief V3协议路由地址信息
 *
 * @param [in]     en                   // 路由
 * @param [in]     dest                 // 目标地址
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)
 *      // 使用：
 *      ADDR_ROUTE_ITEM_V3(true, 0x01, LINK_A_B)
 * @endcode
 */
#define ADDR_ROUTE_ITEM_V3(en, dest, tag_link)                                      \
    {                                                                               \
        ROUTE_ADDR, en, 0xff, dest, 0xff, UN_USE_CMD, UN_USE_CMD, tag_link, NULL    \
    }

/**
 * @brief V3协议指令路由信息
 *
 * @param [in]     en                   // 路由
 * @param [in]     src_link             // 源链路
 * @param [in]     cmd_set              // 指令
 * @param [in]     cmd_id               // 指令id
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF03_PLUS, 512, true, false)
 *      // 使用：
 *      CMD_ROUTE_ITEM_V3( true, LINK_A_B, 0xf0, 0x01, LINK_PC_A )
 * @endcode
 */
#define CMD_ROUTE_ITEM_V3(en, src_link, cmd_set, cmd_id, tag_link)              \
    {                                                                           \
        ROUTE_CMD, en, src_link, 0xff, 0xff, cmd_set, cmd_id, tag_link, NULL    \
    }
/**
 * @brief V3协议链路路由信息
 *
 * @param [in]     en                   // 路由
 * @param [in]     src_link             // 源链路
 * @param [in]     tag_link             // 目标链路
 * @param [in]     func_opt             // 回调
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF03_PLUS, 512, true, false)
 *      ...
 *      // 使用:
 *      void  route_func_test(hal_frame_info_t *p_frame)
 *      {
 *           // 回调内容
 *      }
 *      LINK_ROUTE_ITEM_V3( true, LINK_A_B, LINK_PC_A, route_func_test )
 * @endcode
 */
#define LINK_ROUTE_ITEM_V3(en, src_link, tag_link, func_opt)                                \
    {                                                                                       \
        ROUTE_LINK, en, src_link, 0xff, 0xff, UN_USE_CMD, UN_USE_CMD, tag_link, func_opt    \
    }
/**
 * @brief 广播路由信息
 *
 * @param [in]     en                   // 路由
 * @param [in]     src_link             // 源链路
 * @param [in]     dest                 // 目标地址
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, 	VH_TASK, DRV_USART1, PROTOCOL_EF03_PLUS, 512, true, false)
 *      // 使用：
 *      BROADCAST_ROUTE_ITEM_V3( true, LINK_A_B, 0x01, LINK_PC_A )
 * @endcode
 */
#define BROADCAST_ROUTE_ITEM_V3(en, src_link, dest, tag_link)                               \
    {                                                                                       \
        ROUTE_BROADCAST, en, src_link, dest, 0xff, UN_USE_CMD, UN_USE_CMD, tag_link, NULL   \
    }

/**
 * @brief V3协议链路路由项目标路由链路修改
 *
 * @param [in]     src_link             // 源链路
 * @param [in]     dest                 // 目标地址
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:      从LINK_A_B接收数据后转到LINK_PC_A链路发送到目标地址0x01
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册的目标链路为LINK_PC_A：
 *      LINK_ADDR_ROUTE_ITEM_V3( true, LINK_A_B, 0x01, LINK_PC_A )
 *      // 后面应用场景变化，修改目标链路为LINK_PC_B：
 *      LINK_ADDR_ROUTE_ITEM_CHANGE_TAG_LINK_V3(LINK_A_B, 0x01, LINK_PC_B)
 * @endcode
 */
#define LINK_ADDR_ROUTE_ITEM_CHANGE_TAG_LINK_V3(src_link, dest, new_tag_link)                   \
    {                                                                                           \
        link_route_t route_new = LINK_ADDR_ROUTE_ITEM_V3(true,src_link, dest, new_tag_link);    \
        hal_route_info_tag_link_change(&route_new, VERSION_EF3);                                \
    }

/**
 * @brief V3协议链路路由项目标路由链路修改
 *
 * @param [in]     dest                 // 目标地址
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:      从LINK_A_B接收数据后转到LINK_PC_A链路发送到目标地址0x01
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册的目标链路为LINK_PC_A：
 *      ADDR_ROUTE_ITEM_V3( true, 0x01, LINK_PC_A )
 *      // 后面应用场景变化，修改目标链路为LINK_PC_B：
 *      ADDR_ROUTE_ITEM_CHANGE_TAG_LINK_V3(0x01, LINK_PC_B)
 * @endcode
 */
#define ADDR_ROUTE_ITEM_CHANGE_TAG_LINK_V3(dest, new_tag_link)                  \
    {                                                                           \
        link_route_t route_new = ADDR_ROUTE_ITEM_V3(true, dest, new_tag_link);  \
        hal_route_info_tag_link_change(&route_new, VERSION_EF3);                \
    }

/**
 * @brief V3协议链路路由项目标路由链路修改
 *
 * @param [in]     src_link             // 源链路
 * @param [in]     cmd_set              // 指令
 * @param [in]     cmd_id               // 指令id
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:      
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册的目标链路为LINK_PC_A：
 *      CMD_ROUTE_ITEM_V3( true, LINK_A_B, 0x01, 0x01, LINK_PC_A )
 *      // 后面应用场景变化，修改目标链路为LINK_PC_B：
 *      CMD_ROUTE_ITEM_CHANGE_TAG_LINK_V3(LINK_A_B, 0x01, 0x01, LINK_PC_B)
 * @endcode
 */
#define CMD_ROUTE_ITEM_CHANGE_TAG_LINK_V3(src_link, cmd_set, cmd_id, new_tag_link)                  \
    {                                                                                               \
        link_route_t route_new = CMD_ROUTE_ITEM_V3(true, src_link, cmd_set, cmd_id, new_tag_link);  \
        hal_route_info_tag_link_change(&route_new, VERSION_EF3);                                    \
    }

/**
 * @brief V3协议链路路由项目标路由链路修改
 *
 * @param [in]     src_link             // 源链路
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:      
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册的目标链路为LINK_PC_A：
 *      LINK_ROUTE_ITEM_V3( true, LINK_A_B, LINK_PC_A )
 *      // 后面应用场景变化，修改目标链路为LINK_PC_B：
 *      LINK_ROUTE_ITEM_CHANGE_TAG_LINK_V3(LINK_A_B, LINK_PC_B)
 * @endcode
 */
#define LINK_ROUTE_ITEM_CHANGE_TAG_LINK_V3(src_link, new_tag_link)                          \
    {                                                                                       \
        link_route_t route_new = LINK_ROUTE_ITEM_V3(true, src_link, new_tag_link, NULL);    \
        hal_route_info_tag_link_change(&route_new, VERSION_EF3);                            \
    }

/**
 * @brief V3协议路由功能开关
 *
 * @param [in]     enable                   // 是否使能，true：使能 
 *                                                       false：不使能
 *
 * @details 特殊说明:路由表注册后，默认是使能状态
 * @par eg:      
 * @code
 *      TURN_ROUTE_RULE_V3(false);       // 不使能V2协议路由功能
 * @endcode
 */
#define TURN_ROUTE_RULE_V3(enable)                                  turn_hal_route_rule(VERSION_EF3,enable);

/**
 * @brief V3协议路由项功能开关
 *
 * @param [in]     enable               // 是否使能
 * @param [in]     src_link             // 源链路
 * @param [in]     dest                 // 目标地址
 *
 * @details 特殊说明:
 * @par eg:      
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册时默认使能：
 *      LINK_ADDR_ROUTE_ITEM_V3( true, LINK_A_B, 0x01, LINK_PC_A )
 *      // 后面应用场景变化，关闭使能：
 *      TURN_LINK_ADDR_ROUTE_ITEM_V3(false, LINK_A_B, 0x01)
 * @endcode
 */
#define TURN_LINK_ADDR_ROUTE_ITEM_V3(enable, src_link, dest)                            \
    {                                                                                   \
        link_route_t route_new = LINK_ADDR_ROUTE_ITEM_V3(true,src_link, dest, 0xff);    \
        turn_hal_route_item_rule(&route_new, VERSION_EF3, enable);                      \
    }

/**
 * @brief V3协议路由项功能开关
 *
 * @param [in]     enable               // 是否使能
 * @param [in]     dest                 // 目标地址
 *
 * @details 特殊说明:
 * @par eg:      
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册时默认使能：
 *      ADDR_ROUTE_ITEM_V3( true, 0x01, LINK_PC_A )
 *      // 后面应用场景变化，关闭使能：
 *      TURN_ADDR_ROUTE_ITEM_V3(false, 0x01)
 * @endcode
 */
#define TURN_ADDR_ROUTE_ITEM_V3(enable, dest)                               \
    {                                                                       \
        link_route_t route_new = ADDR_ROUTE_ITEM_V3(true, dest, 0xff);      \
        turn_hal_route_item_rule(&route_new, VERSION_EF3, enable);          \
    }

/**
 * @brief V3协议路由项功能开关
 *
 * @param [in]     enable               // 是否使能
 * @param [in]     dest                 // 目标地址
 * @param [in]     d_dest               // 目标动态地址
 *
 * @details 特殊说明:
 * @par eg:      
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册时默认使能：
 *      DYNC_ADDR_ROUTE_ITEM_V3( true, 0x01, 0x01, LINK_PC_A )
 *      // 后面应用场景变化，关闭使能：
 *      TURN_DYNC_ADDR_ROUTE_ITEM_V3(false, 0x01, 0x01)
 * @endcode
 */
#define TURN_DYNC_ADDR_ROUTE_ITEM_V3(enable, dest, d_dest)                              \
    {                                                                                   \
        link_route_t route_new = DYNC_ADDR_ROUTE_ITEM_V3(true, dest, d_dest, 0xff);     \
        turn_hal_route_item_rule(&route_new, VERSION_EF3, enable);                      \
    }

/**
 * @brief V3协议路由项功能开关
 *
 * @param [in]     enable               // 是否使能
 * @param [in]     src_link             // 源链路
 * @param [in]     cmd_set              // 指令
 * @param [in]     cmd_id               // 指令id
 *
 * @details 特殊说明:
 * @par eg:      
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册时默认使能：
 *      CMD_ROUTE_ITEM_V3( true, LINK_A_B, 0x01, 0x01, LINK_PC_A )
 *      // 后面应用场景变化，关闭使能：
 *      TURN_CMD_ROUTE_ITEM_V3(false, LINK_A_B, 0x01, 0x01)
 * @endcode
 */
#define TURN_CMD_ROUTE_ITEM_V3(enable, src_link, cmd_set, cmd_id)                           \
    {                                                                                       \
        link_route_t route_new = CMD_ROUTE_ITEM_V3(true, src_link, cmd_set, cmd_id, 0xff);  \
        turn_hal_route_item_rule(&route_new, VERSION_EF3, enable);                          \
    }

/**
 * @brief V3协议路由项功能开关
 *
 * @param [in]     enable               // 是否使能
 * @param [in]     src_link             // 源链路
 *
 * @details 特殊说明:
 * @par eg:      
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册时默认使能：
 *      LINK_ROUTE_ITEM_V3( true, LINK_A_B, LINK_PC_A )
 *      // 后面应用场景变化，关闭使能：
 *      TURN_LINK_ROUTE_ITEM_V3(false, LINK_A_B)
 * @endcode
 */
#define TURN_LINK_ROUTE_ITEM_V3(enable, src_link)                                   \
    {                                                                               \
        link_route_t route_new = LINK_ROUTE_ITEM_V3(true, src_link, 0xff, NULL);    \
        turn_hal_route_item_rule(&route_new, VERSION_EF3, enable);                  \
    }

/**
 * @brief V2协议定义路由信
 *
 * @param [in] item...          //路由信息
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 * @par eg:        ● 将04地址的数据往 LINK_A_B 路由
 *                 ● 将01地址的数据往 LINK_PC_A 路由
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, 	VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 使用:
 *      DEF_ROUT_INFO_V2(
 *          _ADDR_ROUTE_ITEM_V2(true, 04, LINK_A_B)
 *          _ADDR_ROUTE_ITEM_V2(true, 01, LINK_PC_A)
 *          )
 * @endcode
 */
#define DEF_ROUT_INFO_V2(item...)                                                                                           \
    static link_route_t _v2_sg_route_tab[] = {item};                                                                        \
    static link_route_data_t _v2_sg_route_data = {_v2_sg_route_tab, ARRY_ITEMS_NUM(_v2_sg_route_tab), VERSION_EF2, true};   \
    AUTO_REG_ITEM(_v2_sg_route_data, AUTO_REG_LINK_ROUTE_TAB, (void *)&_v2_sg_route_data);
/**
 * @brief V2协议链路路由地址信息
 *
 * @param [in]     en                   // 路由
 * @param [in]     src_link             // 源链路
 * @param [in]     dest                 // 目标地址
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:      从LINK_A_B接收数据后转到LINK_PC_A链路发送到目标地址0x01
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 使用：
 *      LINK_ADDR_ROUTE_ITEM_V2( true, LINK_A_B, 0x01, LINK_PC_A )
 * @endcode
 */
#define LINK_ADDR_ROUTE_ITEM_V2(en, src_link, dest, tag_link)                               \
    {                                                                                       \
        ROUTE_LINK_ADDR, en, src_link, dest, 0xff, UN_USE_CMD, UN_USE_CMD, tag_link, NULL   \
    }
/**
 * @brief V2协议路由地址信息
 *
 * @param [in]     en                   // 路由
 * @param [in]     dest                 // 目标地址
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, 	VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 使用：
 *      ADDR_ROUTE_ITEM_V2(true, 0x01, LINK_A_B)
 * @endcode
 */
#define ADDR_ROUTE_ITEM_V2(en, dest, tag_link)                                      \
    {                                                                               \
        ROUTE_ADDR, en, 0xff, dest, 0xff, UN_USE_CMD, UN_USE_CMD, tag_link, NULL   \
    }
/**
 * @brief V2协议指令路由信息
 *
 * @param [in]     en                   // 路由
 * @param [in]     src_link             // 源链路
 * @param [in]     cmd_set              // 指令
 * @param [in]     cmd_id               // 指令id
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 使用：
 *      CMD_ROUTE_ITEM_V3( true, LINK_A_B, 0xf0, 0x01, LINK_PC_A )
 * @endcode
 */
#define CMD_ROUTE_ITEM_V2(en, src_link, cmd_set, cmd_id, tag_link)              \
    {                                                                           \
        ROUTE_CMD, en, src_link, 0xff, 0xff, cmd_set, cmd_id, tag_link, NULL   \
    }
/**
 * @brief V2协议链路路由信息
 *
 * @param [in]     en                   // 路由
 * @param [in]     src_link             // 源链路
 * @param [in]     tag_link             // 目标链路
 * @param [in]     func_opt             // 回调
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      ...
 *      // 使用:
 *      void  route_func_test(hal_frame_info_t *p_frame)
 *      {
 *           // 回调内容
 *      }
 *      LINK_ROUTE_ITEM_V2( true, LINK_A_B, LINK_PC_A, route_func_test )
 * @endcode
 */
#define LINK_ROUTE_ITEM_V2(en, src_link, tag_link, func_opt)                    \
    {                                                                           \
        ROUTE_LINK, en, src_link, 0xff, 0xff, NULL, NULL, tag_link, func_opt   \
    }

/**
 * @brief V2协议链路路由项目标路由链路修改
 *
 * @param [in]     src_link             // 源链路
 * @param [in]     dest                 // 目标地址
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:      从LINK_A_B接收数据后转到LINK_PC_A链路发送到目标地址0x01
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册的目标链路为LINK_PC_A：
 *      LINK_ADDR_ROUTE_ITEM_V2( true, LINK_A_B, 0x01, LINK_PC_A )
 *      // 后面应用场景变化，修改目标链路为LINK_PC_B：
 *      LINK_ADDR_ROUTE_ITEM_CHANGE_TAG_LINK_V2(LINK_A_B, 0x01, LINK_PC_B)
 * @endcode
 */
#define LINK_ADDR_ROUTE_ITEM_CHANGE_TAG_LINK_V2(src_link, dest, new_tag_link)                   \
    {                                                                                           \
        link_route_t route_new = LINK_ADDR_ROUTE_ITEM_V2(true,src_link, dest, new_tag_link);    \
        hal_route_info_tag_link_change(&route_new, VERSION_EF2);                                \
    }

/**
 * @brief V2协议链路路由项目标路由链路修改
 *
 * @param [in]     dest                 // 目标地址
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:      从LINK_A_B接收数据后转到LINK_PC_A链路发送到目标地址0x01
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册的目标链路为LINK_PC_A：
 *      ADDR_ROUTE_ITEM_V2( true, 0x01, LINK_PC_A )
 *      // 后面应用场景变化，修改目标链路为LINK_PC_B：
 *      ADDR_ROUTE_ITEM_CHANGE_TAG_LINK_V2(0x01, LINK_PC_B)
 * @endcode
 */
#define ADDR_ROUTE_ITEM_CHANGE_TAG_LINK_V2(dest, new_tag_link)                  \
    {                                                                           \
        link_route_t route_new = ADDR_ROUTE_ITEM_V2(true, dest, new_tag_link);  \
        hal_route_info_tag_link_change(&route_new, VERSION_EF2);                \
    }

/**
 * @brief V2协议链路路由项目标路由链路修改
 *
 * @param [in]     src_link             // 源链路
 * @param [in]     cmd_set              // 指令
 * @param [in]     cmd_id               // 指令id
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:      
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册的目标链路为LINK_PC_A：
 *      CMD_ROUTE_ITEM_V2( true, LINK_A_B, 0x01, 0x01, LINK_PC_A )
 *      // 后面应用场景变化，修改目标链路为LINK_PC_B：
 *      CMD_ROUTE_ITEM_CHANGE_TAG_LINK_V2(LINK_A_B, 0x01, 0x01, LINK_PC_B)
 * @endcode
 */
#define CMD_ROUTE_ITEM_CHANGE_TAG_LINK_V2(src_link, cmd_set, cmd_id, new_tag_link)                  \
    {                                                                                               \
        link_route_t route_new = CMD_ROUTE_ITEM_V2(true, src_link, cmd_set, cmd_id, new_tag_link);  \
        hal_route_info_tag_link_change(&route_new, VERSION_EF2);                                    \
    }

/**
 * @brief V2协议链路路由项目标路由链路修改
 *
 * @param [in]     src_link             // 源链路
 * @param [in]     tag_link             // 目标链路
 *
 * @details 特殊说明:
 * @par eg:      
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册的目标链路为LINK_PC_A：
 *      LINK_ROUTE_ITEM_V2( true, LINK_A_B, LINK_PC_A )
 *      // 后面应用场景变化，修改目标链路为LINK_PC_B：
 *      LINK_ROUTE_ITEM_CHANGE_TAG_LINK_V2(LINK_A_B, LINK_PC_B)
 * @endcode
 */
#define LINK_ROUTE_ITEM_CHANGE_TAG_LINK_V2(src_link, new_tag_link)                          \
    {                                                                                       \
        link_route_t route_new = LINK_ROUTE_ITEM_V2(true, src_link, new_tag_link, NULL);    \
        hal_route_info_tag_link_change(&route_new, VERSION_EF2);                            \
    }

/**
 * @brief V2协议路由功能开关
 *
 * @param [in]     enable                   // 是否使能，true：使能 
 *                                                       false：不使能
 *
 * @details 特殊说明:路由表注册后，默认是使能状态
 * @par eg:      
 * @code
 *      TURN_ROUTE_RULE_V2(false);       // 不使能V2协议路由功能
 * @endcode
 */
#define TURN_ROUTE_RULE_V2(enable)                                  turn_hal_route_rule(VERSION_EF2,enable);

/**
 * @brief V2协议路由项功能开关
 *
 * @param [in]     enable               // 是否使能
 * @param [in]     src_link             // 源链路
 * @param [in]     dest                 // 目标地址
 *
 * @details 特殊说明:
 * @par eg:      
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册时默认使能：
 *      LINK_ADDR_ROUTE_ITEM_V2( true, LINK_A_B, 0x01, LINK_PC_A )
 *      // 后面应用场景变化，关闭使能：
 *      TURN_LINK_ADDR_ROUTE_ITEM_V2(false, LINK_A_B, 0x01)
 * @endcode
 */
#define TURN_LINK_ADDR_ROUTE_ITEM_V2(enable, src_link, dest)                            \
    {                                                                                   \
        link_route_t route_new = LINK_ADDR_ROUTE_ITEM_V2(true,src_link, dest, 0xff);    \
        turn_hal_route_item_rule(&route_new, VERSION_EF2, enable);                      \
    }

/**
 * @brief V2协议路由项功能开关
 *
 * @param [in]     enable               // 是否使能
 * @param [in]     dest                 // 目标地址
 *
 * @details 特殊说明:
 * @par eg:      
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册时默认使能：
 *      ADDR_ROUTE_ITEM_V2( true, 0x01, LINK_PC_A )
 *      // 后面应用场景变化，关闭使能：
 *      TURN_ADDR_ROUTE_ITEM_V2(false, 0x01)
 * @endcode
 */
#define TURN_ADDR_ROUTE_ITEM_V2(enable, dest)                               \
    {                                                                       \
        link_route_t route_new = ADDR_ROUTE_ITEM_V2(true, dest, 0xff);      \
        turn_hal_route_item_rule(&route_new, VERSION_EF2, enable);          \
    }

/**
 * @brief V2协议路由项功能开关
 *
 * @param [in]     enable               // 是否使能
 * @param [in]     src_link             // 源链路
 * @param [in]     cmd_set              // 指令
 * @param [in]     cmd_id               // 指令id
 *
 * @details 特殊说明:
 * @par eg:      
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册时默认使能：
 *      CMD_ROUTE_ITEM_V2( true, LINK_A_B, 0x01, 0x01, LINK_PC_A )
 *      // 后面应用场景变化，关闭使能：
 *      TURN_CMD_ROUTE_ITEM_V2(false, LINK_A_B, 0x01, 0x01)
 * @endcode
 */
#define TURN_CMD_ROUTE_ITEM_V2(enable, src_link, cmd_set, cmd_id)                           \
    {                                                                                       \
        link_route_t route_new = CMD_ROUTE_ITEM_V2(true, src_link, cmd_set, cmd_id, 0xff);  \
        turn_hal_route_item_rule(&route_new, VERSION_EF2, enable);                          \
    }

/**
 * @brief V2协议路由项功能开关
 *
 * @param [in]     enable               // 是否使能
 * @param [in]     src_link             // 源链路
 *
 * @details 特殊说明:
 * @par eg:      
 * @code
 *      // 链路注册：
 *      REG_LINK(LINK_A_B, VH_TASK, DRV_USART0, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_A, VH_TASK, DRV_USART1, PROTOCOL_EF02_PLUS, 512, true, false)
 *      REG_LINK(LINK_PC_B, VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
 *      // 一开始注册时默认使能：
 *      LINK_ROUTE_ITEM_V2( true, LINK_A_B, LINK_PC_A )
 *      // 后面应用场景变化，关闭使能：
 *      TURN_LINK_ROUTE_ITEM_V2(false, LINK_A_B)
 * @endcode
 */
#define TURN_LINK_ROUTE_ITEM_V2(enable, src_link)                                   \
    {                                                                               \
        link_route_t route_new = LINK_ROUTE_ITEM_V2(true, src_link, 0xff, NULL);    \
        turn_hal_route_item_rule(&route_new, VERSION_EF2, enable);                  \
    }

typedef struct {
    uint32_t link_num;
    list_t link_head;
} link_ctrl_t;

typedef void (*pf_opt_route)(hal_frame_info_t *p_frame);
typedef struct {
    uint8_t route_type;
    uint8_t opt;
    uint8_t src_link_id;
    uint8_t dest;
    uint8_t d_dest;
    uint8_t cmd_set;
    uint8_t cmd_id;
    uint8_t tag_link_id;
    pf_opt_route func_opt;
} link_route_t;

typedef struct {
    link_route_t *arr_tab;
    uint8_t count;
    uint8_t version;
    uint8_t enable;
} link_route_data_t;

typedef struct {
    link_ctrl_t link_ctrl[MAX_PROTCOL_PRIO];
    link_route_data_t *v2_route;
    link_route_data_t *v3_route;
    uint16_t real_reg_link_num;
    uint8_t silence_enable;
    uint32_t silence_cur_time;
    list_t link_all_head;
    list_t silence_head;
} link_obj_t;

typedef int32_t (*pf_action_func)(hal_frame_info_t *p_frame);
typedef struct {
    uint8_t cmd_func;
    uint8_t cmd_id;
    uint8_t cmd_type;
    uint8_t group_type;
    pf_action_func action_func;
    const char *func_name;    
    const uint8_t *p_link_id;
    uint8_t link_cnt;
} action_cfg_t;

typedef struct {
    const action_cfg_t *cfg;
    list_t action_list;
} action_tab_t;

typedef struct {
    uint32_t cmdset;
    uint32_t *p_link_tab;
    uint32_t link_num;
} cmdset_link_tab_t;

typedef void (*pf_monitor_func)(hal_frame_info_t *const p_frame);
typedef struct {
    uint32_t link_id;
    pf_monitor_func func;
    uint8_t src;
    uint8_t dest;
    uint8_t cmd_func;
    uint8_t cmd_id;
} link_monitor_item_t;

typedef struct {
    const link_monitor_item_t *item;
    list_t node;
} link_monitor_data_t;

typedef pf_action_func (*pf_user_action_func)(hal_frame_info_t *p_frame);
typedef int32_t (*pf_user_action)(hal_frame_info_t *p_frame, action_tab_t *action_tab, uint32_t max_item);

//---REG_LINK
typedef struct {
    const char *name;
    uint32_t link_id;
    uint32_t prio;
    void*    const          read_dev;
    void*    const          write_dev;
    protocol_matrix_t *matrix;
    uint8_t en_route; // 是否路由 true:开启路由，false:关闭路由；
    uint8_t en_debug; // 是否开启调试  true:开启调试，false:关闭调试；

    uint8_t flag_init;
    uint32_t send_seq;        // 发送包数
    uint32_t recv_seq;        // 接收包数
    list_t link_list;         // 链路
    list_t link_all_list;     // 所有链路
    list_t action_list_head;  // 回调链路头
    list_t monitor_list_head; // 消息监听链路头
} link_hal_item;

/**
 * @brief  注册的协议结构体
 * @details
 */
typedef struct _reg_protocol_t {
    const protocol_obj_t *protocol_ptr; // 协议结构体指针
    list_t protocol_node;               // 协议链表节点
} reg_protocol_t;

typedef struct {
    uint8_t cmd_func;
    uint8_t cmd_id;
    uint8_t group_id;
    uint8_t silence;
    list_t node;
} silence_item_t;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif // __cplusplus

API_WEAK void protocol_hal_unpack_notify(hal_frame_info_t *p_frame_info);

int32_t real_protocol_send_by_id(hal_frame_info_t *p_frame_info);
int32_t real_protocol_send_ack(hal_frame_info_t *p_frame_info, uint8_t *pbuf, uint16_t len);
int32_t real_protocol_send(hal_frame_info_t *p_frame_info);
int32_t route_protocol_send_by_id(hal_frame_info_t *p_frame_info);

void real_protocol_send_silence_set(uint8_t enable);
void hal_route_info_tag_link_change(link_route_t *p_new, uint8_t version);
void turn_hal_route_rule(uint8_t version, bool enable);
void turn_hal_route_item_rule(link_route_t *p_new, uint8_t version, bool enable);


uint32_t real_judge_link_write_dev(void * const debug_dev);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif // __cplusplus

#endif // __PROTOCOL_HAL_H__
