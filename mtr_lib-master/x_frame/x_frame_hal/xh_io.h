/**
 * @brief IO驱动接口
 * @file xh_io.h
 * @version 1.0
 * @author Xiao Shulong
 * @date 2022-07-08 19:14:54
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-08 19:14:54 <td>1.0       <td>Xiao Shulong      <td>Created function
 */

#ifndef xh_gpio_h
#define xh_gpio_h
#ifdef __cplusplus
extern "C" {
#endif

#include "xh_core.h"

typedef uint32_t (*xh_io_read_obj)(void *var,uint32_t ch);
typedef void (*xh_io_write_obj)(void *var, uint32_t ch, uint32_t len);
typedef void (*xh_io_mode_obj)(void *var, uint32_t ch,uint32_t mode);

typedef struct _xh_gpio_t{
    xh_comm_t core;
    xh_io_read_obj          read;
    xh_io_write_obj         write;
    xh_io_mode_obj          set_mode;
}xh_gpio_t, *p_xh_gpio_t;

/* 驱动宏定义实现 */
#define REG_XH_GPIO_DRV(id,var,prio,auto_init,init,deinit,read,write,set_mode)    \
            REG_XH_DEV(xh_gpio_t,id,var,prio,auto_init,init,deinit,                   \
            (xh_io_read_obj)read,                                                 \
            (xh_io_write_obj)write,                                               \
            (xh_io_mode_obj)set_mode,                                             \
        )

#define EXT_XH_GPIO_DRV(id)        EXT_XH_DEV(xh_gpio_t,id)

typedef enum {
    XH_GPIO_MODE_AIN,
    XH_GPIO_MODE_IN_FLOATING,
    XH_GPIO_MODE_IPD,
    XH_GPIO_MODE_IPU,
    XH_GPIO_MODE_OUT_OD,
    XH_GPIO_MODE_OUT_PP,
    XH_GPIO_MODE_AF_OD,
    XH_GPIO_MODE_AF_PP,
}XH_GPIO_CTRL_CMD;

/**
 * @brief xh_io_read
 * 
 * @param [in] id            
 * @param [in] ch            
 * 
 * @return uint32_t 
 *  @retval 
 * @details 特殊说明: 无
 * @par eg: 获取pin引脚电平
 * @code
 *    uint8_t sta = 0;
 *    sta = xh_io_read(DRC_GPIO, IO_key)
 * @endcode
 */
static inline uint32_t xh_io_read(void * const id, uint32_t ch)
{
    xh_gpio_t *p = (xh_gpio_t *)id;
    return p->read(p->core.var,ch);
}

/**
 * @brief xh_io_write
 * 
 * @param [in] id            驱动ID
 * @param [in] ch            IO序号
 * @param [in] sta           状态
 * 
 * @details 特殊说明: 
 * @par eg: pin引脚输出电平
 * @code
 *    uint8_t sta = 0;
 *    xh_io_write(DRV_GPIO, RUN_LED, sta);
 * @endcode
 */
static inline void xh_io_write(void *const id, uint32_t ch, uint32_t sta)
{
    xh_gpio_t *p = (xh_gpio_t *)id;
    p->write(p->core.var,ch,sta);
}

/**
 * @brief xh_io_mode
 * 
 * @param [in] id            驱动ID 
 * @param [in] ch            IO序号
 * @param [in] mode          模式
 * 
 * @details 特殊说明: 
 * @par eg: 改变引脚的输入输出模式
 * @code
 *    xh_io_mode(DRV_GPIO,RUN_LED,GPIO_OUTPUT_PP);
 * @endcode
 */
static inline void xh_io_mode(void *const id, uint32_t ch ,uint32_t mode)
{
    xh_gpio_t *p = (xh_gpio_t *)id;
    p->set_mode(p->core.var,ch, mode);
}


#ifdef __cplusplus
}
#endif
#endif /* xh_gpio_h */


