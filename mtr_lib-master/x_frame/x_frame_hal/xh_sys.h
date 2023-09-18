/**
 * @brief 系统时钟驱动接口
 * @file xh_sys.h
 * @version 1.0
 * @author Xiao Shulong
 * @date 2022-07-08 19:30:34
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-08 19:30:34 <td>1.0       <td>Xiao Shulong      <td>Created function
 */
#ifndef xh_sys_h
#define xh_sys_h
#ifdef __cplusplus
extern "C" {
#endif

#include "xh_core.h"


typedef uint32_t (*xh_sys_get_cpuid_obj)(void *var,uint8_t *pbuf, uint32_t length);
typedef void (*xh_sys_reset_obj)(void *var);
typedef void (*xh_sys_jump_to_obj)(uint32_t address);

typedef struct _xh_sys_t{
    xh_comm_t               core;
    xh_sys_get_cpuid_obj    sys_get_cupid;
    xh_sys_reset_obj        sys_reset;
    xh_sys_jump_to_obj  	sys_jump_to;
}xh_sys_t, *p_xh_sys_t;

/* 驱动宏定义实现 */
#define REG_XH_SYS_DRV(id,var,prio,auto_init,init,deinit,sys_get_cupid,sys_reset,sys_jump_to)   \
            REG_XH_DEV(xh_sys_t,id,var,prio,auto_init,init,deinit,                              \
            (xh_sys_get_cpuid_obj)sys_get_cupid,                                                \
            (xh_sys_reset_obj)sys_reset,                                                        \
            (xh_sys_jump_to_obj)sys_jump_to,										            \
        )

#define EXT_XH_SYS_DRV(id)        EXT_XH_DEV(xh_sys_t,id)

/**
 * @brief 获取CPUID
 * @param [in] id            驱动ID
 * @param [in] pbuf          存放CPUID的缓存
 * @param [in] length        缓存长度
 * @return uint32_t
 *  @retval 实际存入pbuf的长度
 * @details 一般由框架调用，CPUID为12字节，length大于或者等于12字节才能获取到完整的CPUID
 * @par eg:获取芯片的CPUID
 * @code
 * uint8_t cpuid[12];///< CPUID为12字节 缓存大小要大于等于12字节才能获取到完整的CPUID
 * void get_cpuid(void)
 * {
 *    xh_sys_get_cpuid(DRV_SYS,cpuid,sizeof(cpuid));
 * }
 * @endcode
 */
static inline uint32_t xh_sys_get_cpuid(void *const id, uint8_t *pbuf, uint32_t length)
{
    xh_sys_t *p = (xh_sys_t *)id;
    return p->sys_get_cupid(p->core.var,pbuf,length);
}

/**
 * @brief 系统复位
 * @param [in] id            驱动ID
 * @details 软件复位
 * @par eg:软件复位
 * @code
 *    xh_sys_reset(DRV_SYS);
 * @endcode
 */
static inline void xh_sys_reset(void * const id)
{
    xh_sys_t *p = (xh_sys_t *)id;
    p->sys_reset(p->core.var);
}

/**
 * @brief 跳转系统地址
 * @param [in] id               驱动ID
 * @param [in] address          跳转地址
 * @details 软件复位
 * @par eg:软件复位
 * @code
 *    xh_sys_jump_to_addr(DRV_SYS，0x08000000);
 * @endcode
 */
static inline void xh_sys_jump_to(void * const id, uint32_t address)
{
    xh_sys_t *p = (xh_sys_t *)id;
    p->sys_jump_to(address);
}

#ifdef __cplusplus
}
#endif
#endif /* xh_sys_h */
