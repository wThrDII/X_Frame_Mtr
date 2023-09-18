/**
 * @brief can 字符流驱动接口
 * @file xh_can_s.h
 * @version 1.0
 * @author juntong.chen
 * @date 2022-07-13 11:44:18
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-13 11:44:18 <td>1.0       <td>juntong.chen      <td>Created function
 */
#ifndef xh_can_s_h
#define xh_can_s_h
#ifdef __cplusplus
extern "C" {
#endif

#include "xh_stream.h"

typedef uint32_t    (*xh_can_s_get_d_addr_obj)(void *var);
typedef bool        (*xh_can_s_get_host_mark_obj)(void *var);
typedef uint32_t    (*xh_can_s_get_dev_num_obj)(void *var,uint32_t dev_s_addr);
typedef bool        (*xh_can_s_get_dev_online_obj)(void *var,uint32_t dev_s_addr,uint32_t dev_d_addr);
typedef uint32_t    (*xh_can_s_get_free_cache_obj)(void *var);
typedef uint32_t    (*xh_can_s_get_dev_total_obj)(void *var);
typedef void        (*xh_can_s_set_dev_s_addr_obj)(void *var,uint32_t s_addr);

typedef struct _xh_can_s_t{
    xh_stream_t     stream;
    xh_can_s_get_d_addr_obj get_d_addr;
    xh_can_s_get_host_mark_obj get_host_mark;
    xh_can_s_get_dev_num_obj get_dev_num;
    xh_can_s_get_dev_online_obj get_dev_online;
    xh_can_s_get_free_cache_obj get_free_cache;
    xh_can_s_get_dev_total_obj get_dev_total;
    xh_can_s_set_dev_s_addr_obj set_dev_s_addr;
}xh_can_s_t, *p_xh_can_s_t;


/**
 * @brief 注册 xh can_s 驱动
 */
#define REG_XH_CAN_S_DRV(id,var,prio,auto_init,init,deinit,read,write,set_baudrate,get_d_addr,get_host_mark,get_dev_num,get_dev_online,get_free_cache,get_dev_total,set_dev_s_addr)     \
            REG_XH_DEV(xh_can_s_t,id,var,prio,auto_init,init,deinit,                    \
            (xh_stream_read_obj)read,                                                   \
            (xh_stream_write_obj)write,                                                 \
            (xh_stream_set_baudrate_obj)set_baudrate,                                   \
            (xh_can_s_get_d_addr_obj)get_d_addr,                                        \
            (xh_can_s_get_host_mark_obj)get_host_mark,                                  \
            (xh_can_s_get_dev_num_obj)get_dev_num,                                      \
            (xh_can_s_get_dev_online_obj)get_dev_online,                                \
            (xh_can_s_get_free_cache_obj)get_free_cache,                                \
            (xh_can_s_get_dev_total_obj)get_dev_total,                                  \
            (xh_can_s_set_dev_s_addr_obj)set_dev_s_addr,                                        \
        )

/**
 * @brief 声明 xh can_s 驱动
 */
#define EXT_XH_CAN_S_DRV(id)        EXT_XH_DEV(xh_can_s_t,id)

/**
 * @brief 获取动态地址
 *
 * @param [in] id            驱动id
 *
 * @return uint32_t
 *  @retval 动态地址
 *
 * @details
 */
static inline uint32_t xh_can_s_get_d_addr(void * const id)
{
    xh_can_s_t *p = (xh_can_s_t *)id;
    return p->get_d_addr(p->stream.core.var);
}

/**
 * @brief 获取主机标记
 *
 * @param [in] id            驱动id
 *
 * @return true
 *  @retval 主机
 * @return false
 *  @retval 从机
 *
 * @details
 */
static inline bool xh_can_s_get_host_mark(void * const id)
{
    xh_can_s_t *p = (xh_can_s_t *)id;
    return p->get_host_mark(p->stream.core.var);
}

/**
 * @brief 获取对应对应静态地址设备个数
 *
 * @param [in] id            驱动id
 * @param [in] dev_s_addr    静态地址
 *
 * @return uint32_t
 *  @retval 设备个数
 *
 * @details
 */
static inline uint32_t xh_can_s_get_dev_num(void * const id, uint32_t dev_s_addr)
{
    xh_can_s_t *p = (xh_can_s_t *)id;
    return p->get_dev_num(p->stream.core.var,dev_s_addr);
}

/**
 * @brief 获取设备是否在线
 *
 * @param [in] id            驱动id
 * @param [in] dev_s_addr    静态地址
 * @param [in] dev_d_addr    动态地址
 *
 * @return true
 *  @retval 在线
 * @return false
 *  @retval 离线
 *
 * @details
 */
static inline bool xh_can_s_get_dev_online(void * const id, uint32_t dev_s_addr, uint32_t dev_d_addr)
{
    xh_can_s_t *p = (xh_can_s_t *)id;
    return p->get_dev_online(p->stream.core.var,dev_s_addr,dev_d_addr);
}

/**
 * @brief 获取剩余缓存个数
 *
 * @param [in] id            驱动id
 *
 * @return uint32_t
 *  @retval 剩余缓存个数
 *
 * @details
 */
static inline uint32_t xh_can_s_get_free_cache(void * const id)
{
    xh_can_s_t *p = (xh_can_s_t *)id;
    return p->get_free_cache(p->stream.core.var);
}

/**
 * @brief 获取设备总数
 *
 * @param [in] id            驱动id
 *
 * @return uint32_t
 *  @retval 设备总数
 *
 * @details
 */
static inline uint32_t xh_can_s_get_dev_total(void * const id)
{
    xh_can_s_t *p = (xh_can_s_t *)id;
    return p->get_dev_total(p->stream.core.var);
}

/**
 * @brief 设置设备静态地址
 *
 * @param [in] id            驱动id
 * @param [in] s_addr        静态地址
 *
 * @return uint32_t
 *  @retval
 *
 * @details
 */
static inline void xh_can_s_set_dev_s_addr(void * const id,uint32_t s_addr)
{
    xh_can_s_t *p = (xh_can_s_t *)id;
    p->set_dev_s_addr(p->stream.core.var,s_addr);
}

#ifdef __cplusplus
}
#endif
#endif /* xh_can_s_h */






