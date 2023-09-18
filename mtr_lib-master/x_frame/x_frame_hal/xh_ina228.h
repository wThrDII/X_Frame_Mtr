/**
 * @brief 
 * @file xh_ina228.h
 * @version 1.0
 * @author zain.zhou 
 * @date 2022-09-08 10:28:01
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-09-08 10:28:01 <td>1.0       <td>zain.zhou      <td>Created function
 */
#ifndef __xh_ina228_H__
#define __xh_ina228_H__
#include "stdint.h"
#include "xh_core.h"



typedef void    (*xh_INA228_setCURRENT_LSB_obj)(void *var,float CURRENT_LSB);
typedef float   (*xh_INA228_getVSHUNT_mV_obj)(void *var);
typedef float   (*xh_INA228_getVBUS_V_obj)(void *var);
typedef float   (*xh_INA228_getDIETEMP_C_obj)(void *var);
typedef float   (*xh_INA228_getDIETEMP_F_obj)(void *var);
typedef float   (*xh_INA228_getCURRENT_A_obj)(void *var);
typedef float   (*xh_INA228_getPOWER_W_obj)(void *var);
typedef float  (*xh_INA228_getENERGY_J_obj)(void *var);
typedef float  (*xh_INA228_getCHARGE_C_obj)(void *var);

typedef struct _xh_ina228_t{
    xh_comm_t core;
    xh_INA228_setCURRENT_LSB_obj           setCURRENT_LSB;
    xh_INA228_getVSHUNT_mV_obj             getVSHUNT_mV;
    xh_INA228_getVBUS_V_obj                getVBUS_V;
    xh_INA228_getDIETEMP_C_obj             getDIETEMP_C;
    xh_INA228_getDIETEMP_F_obj             getDIETEMP_F;
    xh_INA228_getCURRENT_A_obj             getCURRENT_A;
    xh_INA228_getPOWER_W_obj               getPOWER_W;
    xh_INA228_getENERGY_J_obj              getENERGY_J;
    xh_INA228_getCHARGE_C_obj              getCHARGE_C;
}xh_ina228_t, *p_xh_ina228_t;

/* 驱动宏定义实现 */
#define REG_XH_INA228_DRV(id,var,prio,auto_init,init,deinit,setCURRENT_LSB,getVSHUNT_mV,getVBUS_V,getDIETEMP_C,getDIETEMP_F,getCURRENT_A,getPOWER_W,getENERGY_J,getCHARGE_C) \
        REG_XH_DEV(xh_ina228_t,id,var,prio,auto_init,init,deinit,              \
            (xh_INA228_setCURRENT_LSB_obj)setCURRENT_LSB,                      \
            (xh_INA228_getVSHUNT_mV_obj)getVSHUNT_mV,                          \
            (xh_INA228_getVBUS_V_obj)getVBUS_V,                                \
            (xh_INA228_getDIETEMP_C_obj)getDIETEMP_C,                          \
            (xh_INA228_getDIETEMP_F_obj)getDIETEMP_F,                          \
            (xh_INA228_getCURRENT_A_obj)getCURRENT_A,                          \
            (xh_INA228_getPOWER_W_obj)getPOWER_W,                              \
            (xh_INA228_getENERGY_J_obj)getENERGY_J,                            \
            (xh_INA228_getCHARGE_C_obj)getCHARGE_C                            \
        )

#define EXT_XH_INA228_DRV(id)        EXT_XH_DEV(xh_ina228_t,id)



/**
 * @brief xh_ina228_setCURRENT_LSB 设置设置CURRENT_LSB的值用于计算的值用于计算
 * 
 * @param [in] id            INA228的驱动ID
 * @param [in] current_lsb   CURRENT_LSB的值
 * 
 * @details 特殊说明: 
 *          该值为 current_lsb = 预计最大电流值 / pow( 2, 19)  2的19次方
 * @par eg: 获取INA228:DRV_INA228 设置CURRENT_LSB的值用于计算
 * @code
 *    float current_lsb = 100;
 *    xh_ina228_setCURRENT_LSB(DRV_INA228,current_lsb);
 * @endcode
 */
static inline void xh_ina228_setCURRENT_LSB(void *const id ,float current_lsb)
{
    xh_ina228_t *p = (xh_ina228_t *)id;
    p->setCURRENT_LSB(p->core.var,current_lsb);
}


/**
 * @brief xh_ina228_getVSHUNT_mV 获取VSHUNT(并联电压)的值 单位(mv)
 * 
 * @param [in] id            INA228的驱动ID
 * 
 * @return float  返回获取的并联电压值(mv)
 *  @retval 
 * @details 特殊说明: 
 * @par eg: 获取INA228:DRV_INA228 模块的并联电压值 (mv)
 * @code
 *    float  vshunt_mv = 0;
 *    vshunt_mv = xh_ina228_getVSHUNT_mV(DRV_INA228);
 * @endcode
 */
static inline float xh_ina228_getVSHUNT_mV(void *const id)
{
    xh_ina228_t *p = (xh_ina228_t *)id;
    return p->getVSHUNT_mV(p->core.var);
}


/**
 * @brief xh_ina228_getVBUS_V 获取总线电压值 VBUS 单位（V）
 * 
 * @param [in] id            INA228的驱动ID
 * 
 * @return float 返回获取的总线电压值(V)
 *  @retval 
 * @details 特殊说明: 
 * @par eg: 获取INA228:DRV_INA228 模块的总线电压值 (V)
 * @code
 *    float vbus_v = 0;
 *    vbus_v = xh_ina228_getVBUS_V(DRV_INA228);
 * @endcode
 */
static inline float xh_ina228_getVBUS_V(void *const id)
{
    xh_ina228_t *p = (xh_ina228_t *)id;
    return p->getVBUS_V(p->core.var);
}


/**
 * @brief xh_ina228_getDIETEMP_C 获取INA228模块内部温度 单位(摄氏度 C)
 * 
 * @param [in] id            INA228的驱动ID
 * 
 * @return float 返回内部温度值 单位(摄氏度)
 *  @retval 
 * @details 特殊说明: 
 * @par eg:获取INA228:DRV_INA228 模块的 (V)
 * @code
 *    float temp_c = 0;
 *    temp_c = xh_ina228_getDIETEMP_C(DRV_INA228)
 * @endcode
 */
static inline float xh_ina228_getDIETEMP_C(void *const id)
{
    xh_ina228_t *p = (xh_ina228_t *)id;
    return p->getDIETEMP_C(p->core.var);
}


/**
 * @brief xh_ina228_getDIETEMP_F 获取INA228模块内部温度 单位(华摄氏度 F)
 * 
 * @param [in] id            INA228的驱动ID
 * 
 * @return float 返回获取的内部温度 单位(华摄氏度)
 *  @retval 
 * @details 特殊说明: 
 * @par eg:获取INA228:DRV_INA228 模块的温度 单位(华摄氏度 F)
 * @code
 *    float temp_f = 0;
 *    temp_f = xh_ina228_getDIETEMP_F(DRV_INA228);
 * @endcode
 */
static inline float xh_ina228_getDIETEMP_F(void *const id)
{
    xh_ina228_t *p = (xh_ina228_t *)id;
    return p->getDIETEMP_F(p->core.var);
}


/**
 * @brief xh_ina228_getCURRENT_A 获取计算后的 电流值（电流值） 单位 A
 * 
 * @param [in] id            INA228的驱动ID
 * 
 * @return float 返回获取的电流值 单位(A)
 *  @retval 
 * @details 特殊说明: 
 * @par eg:获取模块计算后输出的电流值 单位(A)
 * @code
 *    float current_a = 0;
 *    current_a = xh_ina228_getCURRENT_A(DRV_INA228);
 * @endcode
 */
static inline float xh_ina228_getCURRENT_A(void *const id)
{
    xh_ina228_t *p = (xh_ina228_t *)id;
    return p->getCURRENT_A(p->core.var);
}

/* Get POWER value (W) */

/**
 * @brief xh_ina228_getPOWER_W 获取计算后的功率值 单位(W)
 * 
 * @param [in] id            INA228的驱动ID
 * 
 * @return double 返回值 功率值 单位（W）
 *  @retval 
 * @details 特殊说明: 
 * @par eg:获取模块计算输出功率值 单位(W)
 * @code
 *    double power_w = 0;
 *    power_w = xh_ina228_getPOWER_W(DRV_INA228);
 * @endcode
 */
static inline float xh_ina228_getPOWER_W(void *const id)
{
    xh_ina228_t *p = (xh_ina228_t *)id;
    return p->getPOWER_W(p->core.var);
}

/* Get ENERGY value (J) */

/**
 * @brief xh_ina228_getENERGY_J 获取模块的能量输出值 单位(J)
 * 
 * @param [in] id            INA228的驱动ID
 * 
 * @return double 返回能量输出的值 单位(J)
 *  @retval 
 * @details 特殊说明: 
 * @par eg:获取模块的能量输出值 单位(J)
 * @code
 *    double energy_w = 0;
 *    energy_w = xh_ina228_getENERGY_J(DRV_INA228);   
 * @endcode
 */
static inline float xh_ina228_getENERGY_J(void *const id)
{
    xh_ina228_t *p = (xh_ina228_t *)id;
    return (float)p->getENERGY_J(p->core.var);
}

/* Get CHARGE value (C) */

/**
 * @brief  xh_ina228_getCHARGE_C 获取计算后的电荷输出值 单位库伦(C)
 * 
 * @param [in] id            INA228的驱动ID
 * 
 * @return double 返回获取计算后的的电荷 单位库伦(C)
 *  @retval 
 * @details 特殊说明: 
 * @par eg:获取模块计算后的的电荷 单位库伦(C)
 * @code
 *    double charge_c = 0;
 *    charge_c = xh_ina228_getCHARGE_C(DRV_INA228);   
 * @endcode
 */
static inline float xh_ina228_getCHARGE_C(void *const id)
{
    xh_ina228_t *p = (xh_ina228_t *)id;
    return (float)p->getCHARGE_C(p->core.var);
}

#endif //__xh_ina228_H__
