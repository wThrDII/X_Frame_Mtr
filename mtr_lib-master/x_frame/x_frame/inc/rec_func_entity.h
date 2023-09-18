/**
 * @brief rec_func_entity.c.xtemp header file
 * @file rec_func_entity.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 15:29:53
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 15:29:53
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __REC_FUNC_ENTITY_H__
#define __REC_FUNC_ENTITY_H__

//--------------recorder
enum
{
    REC_TYPE_ERR = 0,
    REC_TYPE_UINT8 = 1,
    REC_TYPE_INT8 = 2,
    REC_TYPE_UINT16 = 3,
    REC_TYPE_INT16 = 4,
    REC_TYPE_UINT32 = 5,
    REC_TYPE_INT32 = 6,
    REC_TYPE_FLOAT = 8,
    REC_TYPE_DOUBLE = 9,
    // REC_TYPE_STRING	= 10,
};

#define MAX_RECORDER_PACK_LEN 400
#define MAX_REC_ITEM_NUM 50 // max_item_num per pack

/**
 * @brief    注册REC链路
 *
 * @details 特殊说明:
 * @par eg:
 * @code
 *      REG_REC_LINK（ LINK_UART0，LINK_UART1 ）
 * @endcode
 */
#define REG_REC_LINK(link...)                 \
    uint8_t g_x_rec_link_arry[] = {link};      \
    X_REC_LINK_TAB g_x_rec_link_tab =          \
        {                                      \
            g_x_rec_link_arry,                 \
            ARRY_ITEMS_NUM(g_x_rec_link_arry), \
    };\
    REG_CMDSET_ACTION_LINK(REC_CMDSET_REC,link)

/**
 * @brief   声明REC上报使用的外部数据（_RUN_REC接口使用）
 *
 * @param [in] name     上报数据名
 * @details 特殊说明:
 * @par eg:
 * @code
 *      // 外部定义的数据填充上报函数:
 *      hal_frame_info_t p_frame_info_t = {....(结构体数据填充)}；
 *      rec_dat_rec_func(hal_frame_info_t *p_frame_info, void (*rec_protocol_send)(hal_frame_info_t *))
 *      {
 *          //p_frame_info_t结构体数据处理
 *      }
 *      // 使用：
 *      EXT_REC(rec_func)
 * @endcode
 */
#define EXT_REC(name) extern int32_t rec_dat_##name(hal_frame_info_t *p_frame_info, int32_t (*rec_protocol_send)(hal_frame_info_t *));

/**
 * @brief   REC数据上报
 *
 * @param [in] name     数据上报名
 * @details 特殊说明: 调用_EXT_REC接口声明REC上报的外部数据
 * @par eg:
 * @code
 *      // 先注册链路
 *      REG_LINK(LINK_UART0, 	VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)
 *      _REG_REC_LINK（ LINK_UART0 ）
 *      参考_EXT_REC定义好外部数据填充上报函数
 *      ...
 *      void rec_test(void)
 *      {
 *          ...
 *          //
 *          _RUN_REC(rec_func)
 *          //
 *          ...
 *      }
 *      REG_TASK(0,10,rec_test)
 * @endcode
 */
#define RUN_REC(name)                                   \
    EXT_REC(name)                                       \
    while (1)                                            \
    {                                                    \
        if (enable_rec_dat == 0)                         \
        {                                                \
            break;                                       \
        }                                                \
        hal_frame_info_t frame_info = {0};               \
        rec_dat_##name(&frame_info, x_rec_default_send); \
        break;                                           \
    }

/**
 * @brief   REC数据上报
 *
 * @param [in] name      数据上报名
 * @param [in] _link     链路id
 * @param [in] _dest     数据上报目标地址
 * @details 特殊说明:     往指定的链路及目标地址上报
 * @par eg:
 * @code
 *      // 先注册链路
 *      REG_LINK(LINK_UART0, VH_TASK, DRV_USART0, PROTOCOL_EF03_PLUS, 512, true, false)
 *      _REG_REC_LINK（ LINK_UART0 ）
 *      参考_EXT_REC定义好外部数据填充上报函数
 *      ...
 *      void rec_test(void)
 *      {
 *          ...
 *          //
 *          _RUN_REC_CUSTOM(rec_func, LINK_UART0, 0x01 )
 *          //
 *          ...
 *      }
 *      REG_TASK(0, 1, rec_test)
 * @endcode
 */
#define RUN_REC_CUSTOM(name, _link, _dest)             \
    EXT_REC(name)                                      \
    while (1)                                           \
    {                                                   \
        hal_frame_info_t frame_info = {0};              \
        frame_info.link_id = _link;                     \
        frame_info.dest = _dest;                        \
        rec_dat_##name(&frame_info, x_rec_custom_send); \
        break;                                          \
    }

/**
 * @brief   REC上报链路及目标地址配置
 *
 * @param [in] name      数据上报名
 * @param [in] _link     链路id
 * @param [in] _dest     数据上报目标地址
 * @details 特殊说明:     配置REC上报的链路及目标地址
 * @par eg:
 * @code
 *      // 需提前在外部声明定义REC配置函数：rec_cfg_##name
 *      CFG_REC_CUSTOM(rec_func, LINK_UART0, 0x01)
 * @endcode
 */
#define CFG_REC_CUSTOM(name, _link, _dest)                                                                    \
    extern bool rec_cfg_##name(hal_frame_info_t *p_frame_info, int32_t (*rec_protocol_send)(hal_frame_info_t *)); \
    while (1)                                                                                                  \
    {                                                                                                          \
        hal_frame_info_t frame_info = {0};                                                                     \
        frame_info.link_id = _link;                                                                            \
        frame_info.dest = _dest;                                                                               \
        rec_cfg_##name(&frame_info, x_rec_custom_send);                                                        \
        break;                                                                                                 \
    }

/**
 * @brief   声明使用缓存的REC上报使用的外部数据（_RUN_REC_CACHE接口使用）
 *
 * @param [in] name      数据上报名
 * @details 特殊说明:
 * @par eg:
 * @code
 *      //参考_EXT_REC接口的使用例子
 * @endcode
 */
#define EXT_REC_CACHE(name) extern void rec_cache_dat_##name(hal_frame_info_t *p_frame_info, int32_t (*rec_protocol_send)(hal_frame_info_t *));

/**
 * @brief   声明使用缓存的REC上报使用的外部数据（_RUN_REC_CACHE接口使用）
 *
 * @param [in] name      数据上报名
 * @details 特殊说明:
 * @par eg:
 * @code
 *      //参考_RUN_REC接口的使用例子
 * @endcode
 */
#define RUN_REC_CACHE(name)                                   \
    EXT_REC_CACHE(name)                                       \
    while (1)                                                  \
    {                                                          \
        if (enable_rec_dat == 0)                               \
        {                                                      \
            break;                                             \
        }                                                      \
        hal_frame_info_t frame_info = {0};                     \
        rec_cache_dat_##name(&frame_info, x_rec_default_send); \
        break;                                                 \
    }

/**
 * @brief   使用缓存的REC上报
 *
 * @param [in] name      数据上报名
 * @param [in] _link     链路id
 * @param [in] _dest     数据上报目标地址
 * @details 特殊说明:
 * @par eg:
 * @code
 *      // 参考_RUN_REC_CUSTOM接口的使用例子
 * @endcode
 */
#define RUN_REC_CACHE_CUSTOM(name, _link, _dest)             \
    EXT_REC_CACHE(name)                                      \
    while (1)                                                 \
    {                                                         \
        hal_frame_info_t frame_info = {0};                    \
        frame_info.link_id = _link;                           \
        frame_info.dest = _dest;                              \
        rec_cache_dat_##name(&frame_info, x_rec_custom_send); \
        break;                                                \
    }

/**
 * @brief   使用缓存的REC上报链路及目标地址配置
 *
 * @param [in] name      数据上报名
 * @param [in] _link     链路id
 * @param [in] _dest     数据上报目标地址
 * @details 特殊说明:     实际是调用_CFG_REC_CUSTOM接口
 * @par eg:
 * @code
 *      CFG_REC_CACHE_CUSTOM(rec_func, LINK_UART0, 0x01)
 * @endcode
 */
#define CFG_REC_CACHE_CUSTOM(name, _link, _dest) CFG_REC_CUSTOM(name, _link, _dest)

#define EXT_REC_CAP(name)                          extern void rec_cap_dat_##name(void);
#define RUN_REC_CAP(name)                           \
        EXT_REC_CAP(name)                           \
        do                                          \
        {                                           \
            if (enable_rec_dat == 0)                \
            {                                       \
                break;                              \
            }                                       \
            rec_cap_dat_##name();                   \
            break;                                  \
        }while(0);

#pragma pack(1)
typedef struct
{
    uint8_t  rec_var_index;
    uint8_t  rec_var_type;
    uint8_t  rec_var_len; //怕string这周依旧不确定长度
    uint8_t  rec_var_name_len;
    uint8_t  rec_var_name[0];
}rec_var_item_t;

typedef struct
{
    uint8_t         pack_cmd_set;
    uint8_t         pack_cmd_id;
    uint8_t         pack_item_num;
    uint8_t         pack_name_len;
    uint8_t         pack_name[0];
}rec_cfg_item_t;

typedef struct 
{
		uint8_t   cap_pack_cmd_set;
		uint8_t   cap_pack_cmd_id;
		uint32_t   max_trans_bytes_per_block;
		uint32_t  total_blocks;
}cap_rec_cfg_dat_t;

typedef struct 
{
		uint8_t   is_ready;
		uint32_t  total_size;
		uint32_t  current_size;
}cap_rec_priv_dat_t;
#pragma pack()

typedef struct
{
    list_t rec_init_node;
    bool (*rec_cfg_func)(hal_frame_info_t *, int32_t (*rec_protocol_send)(hal_frame_info_t *));
} rec_node_t;

typedef struct
{
    list_t             rec_init_node;
    bool (*rec_cfg_func)(hal_frame_info_t*, int32_t (*rec_protocol_send)(hal_frame_info_t *));
	cap_rec_priv_dat_t cap_rec_priv_dat;
}rec_cap_node_t;

typedef struct _X_REC_LINK_TAB
{
    uint8_t *p_link_arry;
    uint8_t link_num;
} X_REC_LINK_TAB, *P_X_REC_LINK_TAB;

#define REC_CAP_TRANS_DAT_LEN          	   128u

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif // __cplusplus

const char *get_var_name(const char *var_name);

void add_rec_node(list_t *p_add_node);

extern uint8_t enable_rec_dat;

void real_rec_init(void);

/**
 * @brief  rec数据发送
 *
 * @param [in] p_frame_info      协议包参数指针
 *
 * @details 特殊说明:     实际是调用protocol_send_by_id进行发送，与x_rec_custom_send接口区别在于本接口会对使用REG_REC_LINK接口注册的链路都进行发送
 * @par eg:
 * @code
 *    //  先注册链路：
 *    REG_REC_LINK（LINK_UART0，LINK_UART1）         //注册链路
 *    //  使用：
 *    void send_data_test(void)
 *    {
 *        ...
 *        //
 *        hal_frame_info_t p_frame_info_t = {....(结构体数据填充)}；
 *        hal_frame_info_t *p_frame_info_test = &p_frame_info_t；
 *        x_rec_default_send(p_frame_info_test)；                                //将在LINK_UART0和LINK_UART1链路上都进行发送
 *        //
 *        ...
 *    }
 * @endcode
 */
int32_t x_rec_default_send(hal_frame_info_t *p_frame_info);

/**
 * @brief   rec数据发送
 *
 * @param [in] p_frame_info      协议包参数指针
 *
 * @details 特殊说明:      实际是调用protocol_send_by_id进行发送
 * @par eg:
 * @code
 *    //  先注册链路：
 *    REG_REC_LINK（LINK_UART0，LINK_UART1）         //注册链路
 *    //  使用：
 *    void send_data_test(void)
 *    {
 *        ...
 *        //
 *        hal_frame_info_t p_frame_info_t = {....(结构体数据填充)}；
 *        hal_frame_info_t *p_frame_info_test = &p_frame_info_t；
 *        x_rec_custom_send(p_frame_info_test)；
 *        //
 *        ...
 *    }
 * @endcode
 */
int32_t x_rec_custom_send(hal_frame_info_t *p_frame_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif // __cplusplus

#endif // __REC_FUNC_ENTITY_H__
