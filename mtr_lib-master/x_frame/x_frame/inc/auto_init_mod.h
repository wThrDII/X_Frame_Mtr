/**
 * @brief auto_init_mod.c header file
 * @file auto_init_mod.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-28 15:27:59
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-28 15:27:59
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __AUTO_INIT_MOD_H__
#define __AUTO_INIT_MOD_H__

enum{
    INIT_PRIO_RESOURCE = 0,
    INIT_PRIO_SETUP,
    INIT_PRIO_BASIC,
    INIT_PRIO_PRE_APP = 10,
    INIT_PRIO_APP,
    INIT_PRIO_APP_CUSTOM = 20,
};

typedef struct{
    void (*pf_init_func)(void);
    uint8_t init_priority;  ///< 0最高，255最低
}XF_AUTO_INIT_REG_T;

#define AUTO_INIT_REG(priority, func)                               static const XF_AUTO_INIT_REG_T _##func##_init = {func , priority};\
                                                                    AUTO_REG_ITEM(_##func##_init,AUTO_REG_INIT_MOD,(void*)&_##func##_init);
///> 以下三个是X_FRAME框架内部使用，请勿使用
#define REG_INIT_MOD_RESOURCE(func)                                 AUTO_INIT_REG(INIT_PRIO_RESOURCE, func)
#define REG_INIT_MOD_SETUP(func)                                    AUTO_INIT_REG(INIT_PRIO_SETUP, func)
#define REG_INIT_MOD_BASIC(func)                                    AUTO_INIT_REG(INIT_PRIO_BASIC, func)

///> 以下三个是应用开发使用的初始化接口
/**
 * @brief               高优先级自动初始化
 * @param [in] func     需要进行初始化的函数
 *
 * @details:            1.是应用开发使用中优先级最高的
 *                      2.注意！同一优先级里的函数初始化顺序不固定，系统默认有两层优先级，
 *                        需要自定义优先级，请使用REG_INIT_MOD_APP_PRIO自行定义优先级。
 * @par e.g:
 * @code
 *      static void function(void)
 *      {
 *          ;
 *      }
 *      REG_INIT_MOD_RESOURCE(function);
 *
 * @endcode
 */
#define REG_INIT_MOD_PRE_APP(func)                                  AUTO_INIT_REG(INIT_PRIO_PRE_APP, func)

/**
 * @brief               低优先级自动初始化
 * @param [in] func     需要进行初始化的函数
 *
 * @details:            1.优先级比 REG_INIT_MOD_PRE_APP 低。
 *                      2.注意！同一优先级里的函数初始化顺序不固定，系统默认有两层优先级，
 *                        需要自定义优先级，请使用REG_INIT_MOD_APP_PRIO自行定义优先级。
 * @par e.g:
 * @code
 *      static void function(void)
 *      {
 *          ;
 *      }
 *      REG_INIT_MOD_APP(function);
 *
 * @endcode
 */
#define REG_INIT_MOD_APP(func)                                      AUTO_INIT_REG(INIT_PRIO_APP, func)

/**
 * @brief               用户自定义函数初始化顺序
 * @param [in] prio     初始化的优先级顺序，prio的范围是0~235，数值越小优先级越高
 * @param [in] func     需要进行初始化的函数
 *
 * @details:            1.优先级比 REG_INIT_MOD_APP 低
 *                      2.系统默认有两层优先级，如需要两层以上优先级顺序的，使用
 *                        REG_INIT_MOD_APP_PRIO 对函数进行自定义优先级排序。
 *                      3.REG_INIT_MOD_APP_PRIO 支持0~235层优先级定义，数值越小优先级越高
 * @par e.g:
 * @code
 *      static void function(void)
 *      {
 *          ;
 *      }
 *      REG_INIT_MOD_APP_PRIO(0,function);
 *
 *      static void function(void)
 *      {
 *          ;
 *      }
 *      REG_INIT_MOD_APP_PRIO(1,function);
 *
 * @endcode
 */
#define REG_INIT_MOD_APP_PRIO(prio, func)                           AUTO_INIT_REG(INIT_PRIO_APP_CUSTOM + prio, func)


///> 旧的初始化宏
#define REG_INIT_MOD(func)                                          AUTO_INIT_REG(INIT_PRIO_APP + 1, func)
#define REG_VH_INIT_MOD(func)                                       AUTO_INIT_REG(INIT_PRIO_APP + 2, func)
#define REG_HI_INIT_MOD(func)                                       AUTO_INIT_REG(INIT_PRIO_APP + 3, func)
#define REG_LW_INIT_MOD(func)                                       AUTO_INIT_REG(INIT_PRIO_APP + 4, func)
#define REG_ID_INIT_MOD(func)                                       AUTO_INIT_REG(INIT_PRIO_APP + 5, func)

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

extern void hal_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __AUTO_INIT_MOD_H__
