/**
 * @brief 字符流驱动接口
 * @file xh_stream.h
 * @version 1.0
 * @author Xiao Shulong
 * @date 2022-07-08 19:28:15
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-08 19:28:15 <td>1.0       <td>Xiao Shulong      <td>Created function
 */
#ifndef xh_stream_h
#define xh_stream_h
#ifdef __cplusplus
extern "C" {
#endif

#include "xh_core.h"

typedef uint32_t (*xh_stream_read_obj)(void *var,uint8_t* data, uint32_t len);
typedef uint32_t (*xh_stream_write_obj)(void *var, uint8_t* data, uint32_t len);
typedef uint32_t (*xh_stream_set_baudrate_obj)(void *var, uint32_t baudrate);

typedef struct _xh_stream_t{
    xh_comm_t                   core;
    xh_stream_read_obj          read;
    xh_stream_write_obj         write;
    xh_stream_set_baudrate_obj  set_baudrate;
}xh_stream_t, *p_xh_stream_t;


/* 驱动宏定义实现 */
#define REG_XH_STREAM_DRV(id,var,prio,auto_init,init,deinit,read,write,set_baudrate)   \
            REG_XH_DEV(xh_stream_t,id,var,prio,auto_init,init,deinit,                       \
            (xh_stream_read_obj)read,                                                       \
            (xh_stream_write_obj)write,                                                     \
            (xh_stream_set_baudrate_obj)set_baudrate,                                       \
        )

#define EXT_XH_STREAM_DRV(id)        EXT_XH_DEV(xh_stream_t,id)

/**
 * @brief xh_stream_read
 * 
 * @param [in] id            驱动ID
 * @param [in] pSrc          读缓存区
 * @param [in] len           读的长度
 * 
 * @return int32_t 
 *  @retval 
 * @details 特殊说明: len 一般不超过255
 * @par eg: 读取指定长度的数据到指定的buffer
 * @code
 *    uint8_t rx_buffer[20] = {};
 *    xh_stream_read(DRV_USART1, rx_buffer, 10);
 * @endcode
 */
static inline int32_t xh_stream_read(void * const id, uint8_t* pSrc, uint32_t len)
{
    xh_stream_t *p = (xh_stream_t *)id;
    return p->read(p->core.var,pSrc,len);
}

/**
 * @brief xh_stream_write
 * 
 * @param [in] id            驱动ID
 * @param [in] pSrc          写入的数据
 * @param [in] len           长度
 * 
 * @return int32_t 
 *  @retval 
 * @details 特殊说明: len 一般不超过255
 * @par eg:将buffer通过对应驱动id设备输出
 * @code
 *    uint8_t tx_buffer[10] = {1,2,3,4,5,6,7,8,9,10};
 *    xh_stream_write(DRV_USART1, tx_buffer, 10);
 * @endcode
 */
static inline int32_t xh_stream_write(void * const id, uint8_t* pSrc, uint32_t len)
{
    xh_stream_t *p = (xh_stream_t *)id;
    return p->write(p->core.var,pSrc,len);
}

/**
 * @brief xh_stream_set_baudrate
 * 
 * @param [in] id            
 * @param [in] baudrate      
 * 
 * @details 特殊说明: 无
 * @par eg: 设置波特率
 * @code
 *    xh_stream_set_baudrate(DRV_USART1, 115200);
 * @endcode
 */
static inline void xh_stream_set_baudrate(void * const id, uint32_t baudrate)
{
    xh_stream_t *p = (xh_stream_t *)id;
    p->set_baudrate(p->core.var, baudrate);
}
#ifdef __cplusplus
}
#endif
#endif /* xh_stream_h */
