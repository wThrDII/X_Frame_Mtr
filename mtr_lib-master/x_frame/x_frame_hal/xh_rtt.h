/**
 * @brief RTT打印接口
 * @file xh_rtt.h
 * @version 1.0
 * @author Xiao Shulong
 * @date 2022-07-21 11:43:52
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-21 11:43:52 <td>1.0       <td>Xiao Shulong      <td>Created function
 */
#ifndef xh_rtt_h
#define xh_rtt_h
#ifdef __cplusplus
extern "C" {
#endif




#include "xh_core.h"
#include <stdarg.h>

typedef int32_t (*xh_rtt_printf_obj)(void *var,const char * sFormat,va_list * pParamList);
typedef uint32_t (*xh_rtt_read_obj)(void *var,uint8_t *p_buf,uint32_t size);

typedef struct _xh_rtt_t{
    xh_comm_t                   core;
    xh_rtt_printf_obj           rtt_printf;
    xh_rtt_read_obj             rtt_read;
}xh_rtt_t, *p_xh_rtt_t;


/* 驱动宏定义实现 */
#define REG_XH_RTT_DRV(id,var,prio,auto_init,init,deinit,rtt_printf,rtt_read)   \
            REG_XH_DEV(xh_rtt_t,id,var,prio,auto_init,init,deinit,                       \
            (xh_rtt_printf_obj)rtt_printf,                                                       \
            (xh_rtt_read_obj)rtt_read,                                                     \
        )

#define EXT_XH_RTT_DRV(id)        EXT_XH_DEV(xh_rtt_t,id)


/**
 * @brief RTT打印
 *
 * @param [in] id            驱动ID
 * @param [in] sFormat       格式化字符指针
 * @param [in] ...           转换的参数
 *
 * @return int32_t
 *  @retval  >= 0:打印的字节数
 *          < 0:错误
 * @details 使用方式类似于printf()
 * @par eg:
 * @code
 * int a=3,b=4;
 * void printf_func(void)
 * {
 *      xh_rtt_printf(DRV_RTT,"a=%d,b=%d,a+b=%d /n",a,b,a+b);
 * }
 * ///< 现象
 *  > a=3,b=4,a+b=7
 * @endcode
 */
static inline int32_t xh_rtt_printf(void * const id, const char * sFormat,...)
{
    int r;
    xh_rtt_t *p = (xh_rtt_t *)id;
    va_list ParamList;
    va_start(ParamList, sFormat);
    r = p->rtt_printf(p->core.var,sFormat, &ParamList);
    va_end(ParamList);
    return r;
}

/**
 * @brief RTT读取
 *
 * @param [in] id            驱动ID
 * @param [in] p_buf         读取数据的缓存
 * @param [in] size          缓存大小
 *
 * @return uint32_t
 *  @retval 读取的字节数
 * @details
 * @par eg:读取RTT接收到的字符
 * @code
 * uint32_t buf[20];
 * void rtt_read(void)
 * {
 *      xh_rtt_read(DRV_RTT,buf,10);
 * }
 * @endcode
 */
static inline uint32_t xh_rtt_read(void * const id, uint8_t *p_buf,uint32_t size)
{
    xh_rtt_t *p = (xh_rtt_t *)id;
    return p->rtt_read(p->core.var,p_buf,size);
}


#ifdef __cplusplus
}
#endif
#endif /* xh_rtt_h */
