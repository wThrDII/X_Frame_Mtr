/**
 * @brief LCD驱动芯片接口
 * @file xh_lcd.h
 * @version 1.0
 * @author Xiao Shulong
 * @date 2022-07-12 14:58:49
 * @copyright Copyright (c) ECOFLOW 2017 - 2022. All rights reserved.
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date                <th>Version   <th>Author         <th>Description
 * <tr><td>2022-07-12 14:58:49 <td>1.0       <td>Xiao Shulong      <td>Created function
 */

#ifndef xh_lcd_h
#define xh_lcd_h
#ifdef __cplusplus
extern "C" {
#endif


#include "xh_core.h"

typedef void (*xh_lcd_refresh_obj)(void *var);
typedef void (*xh_lcd_set_bit_obj)(void *var, uint32_t state,uint32_t off);
typedef void (*xh_lcd_set_bl_obj)(void *var,uint32_t sta);
typedef void (*xh_lcd_set_all_obj)(void *var,uint32_t sta);

typedef struct _xh_lcd_t{
    xh_comm_t core;
    xh_lcd_refresh_obj          refresh;
    xh_lcd_set_bit_obj          set_bit;
    xh_lcd_set_bl_obj           set_bl;
    xh_lcd_set_all_obj          set_all;
}xh_lcd_t, *p_xh_lcd_t;

/* 驱动宏定义实现 */
#define REG_XH_LCD_DRV(id,var,prio,auto_init,init,deinit,refresh,set_bit,set_bl,set_all)    \
            REG_XH_DEV(xh_lcd_t,id,var,prio,auto_init,init,deinit,                   \
            (xh_lcd_refresh_obj)refresh,                                             \
            (xh_lcd_set_bit_obj)set_bit,                                             \
            (xh_lcd_set_bl_obj)set_bl,                                               \
            (xh_lcd_set_all_obj)set_all,                                             \
        )

#define EXT_XH_LCD_DRV(id)        EXT_XH_DEV(xh_lcd_t,id)

/**
 * @brief 刷新屏幕
 * @param [in] id            驱动ID
 * @details 将缓存的内容刷新到屏幕上
 * @par eg: 刷新屏幕
 * @code
 * #include "hal.h"
 * void lcd_refresh(void)
 * {
 *    xh_lcd_refresh(DRV_HT1621B);
 * }
 * @endcode
 */
static inline void xh_lcd_refresh(void * const id)
{
    xh_lcd_t *p = (xh_lcd_t *)id;
    p->refresh(p->core.var);
}

/**
 * @brief 设置一个点的状态
 *
 * @param [in] id            驱动ID
 * @param [in] state         状态
 * @param [in] off           偏移
 *
 * @details 写入的是缓存
 *          state：0灭 1亮
 *          off：在LCD缓存的偏移量
 * @par eg:点亮HT1621B的某一行列的点
 * @code
 * ///<                     row：行      col：列    sta：状态
 * void set_point_obj(uint16_t row,uint16_t col,uint8_t sta)
 * {
 *     if(row > 4 || col < 5){
 *         return;
 *     }
 *     //ht1621b中SEG27和SEG28 没有按顺序排列
 *     if(col == 33){
 *         col=32;
 *     }
 *     else if(col == 32){
 *         col=33;
 *     }
 *     uint32_t off = (col - 5) * 4 + row - 1;
 *     xh_lcd_set_bit(DRV_HT1621B,sta,off);
}
 * @endcode
 */
static inline void xh_lcd_set_bit(void *const id, uint32_t state,uint32_t off)
{
    xh_lcd_t *p = (xh_lcd_t *)id;
    p->set_bit(p->core.var,state,off);
}

/**
 * @brief 设置背光状态
 *
 * @param [in] id            驱动ID
 * @param [in] sta           状态
 *
 * @details sta：0灭 1亮
 * @par eg:开启LCD屏背光
 * @code
 * #include "hal.h"
 * #define LCD_BL_ON 1
 * #define LCD_BL_OFF 0
 * void lcd_on(void)
 * {
 *    xh_lcd_set_bl(DRV_HT1621B,LCD_BL_ON);
 * }
 * @endcode
 */
static inline void xh_lcd_set_bl(void *const id, uint32_t sta)
{
    xh_lcd_t *p = (xh_lcd_t *)id;
    p->set_bl(p->core.var,sta);
}

/**
 * @brief 设置全部点的状态
 *
 * @param [in] id            驱动ID
 * @param [in] sta           状态
 *
 * @details 不经过缓存，直接显示在屏幕上
 * @par eg:点亮屏幕上的全部点
 * @code
 * #include "hal.h"
 * #define LCD_ALL_ON 1
 * #define LCD_ALL_OFF 0
 * void lcd_on(void)
 * {
 *    xh_lcd_set_all(DRV_HT1621B,LCD_ALL_ON);
 * }
 * @endcode
 */
static inline void xh_lcd_set_all(void *const id, uint32_t sta)
{
    xh_lcd_t *p = (xh_lcd_t *)id;
    p->set_all(p->core.var,sta);
}


#ifdef __cplusplus
}
#endif
#endif /* xh_lcd_h */


