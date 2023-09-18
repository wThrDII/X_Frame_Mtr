/********************** Copyright(c)***************************
** ECOFLOW
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** Created by:              Chen JunTong
** Created date:            2022-06-24
** Version:                 V1.0.0
** Descriptions:            驱动抽象层
**
**------File Info----------------------------------------------
** File name:               xh_core.h
** Latest modified date:    2022-06-24
** Latest version:          V1.0.0
** Description:             驱动抽象层
**
*************************************************************/
#ifndef xh_core_h
#define xh_core_h
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

#define MAX_DRV_NUM 32

typedef void (*xh_init_obj)(void* var); 			/* 驱动初始化函数 */
typedef void (*xh_deinit_obj)(void* var); 		/* 驱动反初始化函数 */

typedef struct{
  void *var;                        /* 驱动参数指针 */
  uint16_t prio;                    /* 初始化优先级 */
  bool auto_init;
  xh_init_obj init; 			/* 驱动初始化函数 */
  xh_deinit_obj deinit; 		/* 驱动反初始化函数 */
}xh_comm_t;

#define DEV_NOT_INIT   	(0x00)
#define DEV_AUTO_INIT   (0x01)

#define PTR_TYPE_XH_DEV(id)                     &xh_##id
#define EXT_XH_DEV(var_type, id)                extern var_type xh_##id; extern void * const id;
#define REG_XH_DEV(var_type, id, var, prio, auto_init, init, deinit, custom...)                         \
        var_type xh_##id = {var,prio,auto_init,(xh_init_obj)init,(xh_deinit_obj)deinit, custom };       \
        void * const id = &xh_##id;                                                                     \
        AUTO_REG_ITEM(auto_reg_##id,AUTO_REG_XH_TYPE,&xh_##id);

/**
 * @brief 驱动抽象层初始化
 *
 * @details:
 * @par e.g:
 * @code
 *      xh_core_init();
 * @endcode
 */
void xh_core_init(void);

/**
 * @brief 驱动抽象层反初始化
 *
 * @details:
 * @par e.g:
 * @code
 *      xh_core_deinit();
 * @endcode
 */
void xh_core_deinit(void);

/**
 * @brief 驱动初始化
 *
 * @param [in] id 需要初始化的驱动名
 *
 * @details:
 * @par e.g:
 * @code
 *      xh_init(PTR_TYPE_XH_DEV(DRV_ADC_I));
 * @endcode
 */
static inline void xh_init(void * const id)
{
    xh_comm_t *xh = (xh_comm_t *)id;
    if(xh->init != NULL){
        xh->init(xh->var);
    }
}

/**
 * @brief 驱动反初始化
 *
 * @param [in] id 需要反初始化的驱动名
 *
 * @details:
 * @par e.g:
 * @code
 *      xh_deinit(PTR_TYPE_XH_DEV(DRV_ADC_I));
 * @endcode
 */
static inline void xh_deinit(void * const id)
{
    xh_comm_t *xh = (xh_comm_t *)id;
    if(xh->deinit != NULL){
        xh->deinit(xh->var);
    }
}

#ifdef __cplusplus
}
#endif
#endif /* xh_core_h */
