/**
 * @brief xf_debug.c.xtemp header file
 * @file xf_debug.h
 * @version 1.0
 * @author hzf
 * @date 2022-8-8 15:17:38
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-8-8 15:17:38
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __XF_DEBUG_H__
#define __XF_DEBUG_H__

#define DEBUG_BUF_SIZE 512

#if DISABLE_DBG_I == 1
#undef 			DEF_DBG_I
#endif 
#if DISABLE_DBG_W == 1
#undef 			DEF_DBG_W
#endif 
#if DISABLE_DBG_E == 1
#undef 			DEF_DBG_E
#endif
			
#if ENABLE_DBG != 1

#undef	DEF_DBG_I
#undef	DEF_DBG_W
#undef	DEF_DBG_E

#endif

#define USER_KEIL 0

#if USER_KEIL == 1
    #define __FILE_NAME__	__MODULE__
#else
    #ifndef __FILE_NAME__
    #define __FILE_NAME__	filename(__FILE__)
	#endif
#endif

#define COMPILER_VERSION_5	0
#define COMPILER_VERSION_6	1
#define COMPILER_VERSION	COMPILER_VERSION_5

#if (COMPILER_VERSION == COMPILER_VERSION_6)
	#define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#elif (COMPILER_VERSION == COMPILER_VERSION_5)
	#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#endif

#define _PREFIX_I	"[INFO]: "
#define _PREFIX_W	"[WARN]: "
#define _PREFIX_E	"[ERRO]: "
#define _APPEND_FIX "\r\n"
#define _FILE_INFO	"[%-5d][%8s][%8s][%-3d]"
#define _FILE_INFO_ARG	SYS_MS_FUNC,__FILE_NAME__,__FUNCTION__,__LINE__
#define FIX_PARAM           1

#define COLOR_RED  "\033[1;31;40m"
#define COLOR_GRE  "\033[1;32;40m"
#define COLOR_BLU  "\033[1;34;40m"
#define COLOR_WHI  "\033[1;37;40m"
#define COLOR_YEL  "\033[1;33;40m"
#define COLOR_END  "\033[0m"

#define NOP_FUNC do{}while(0)

#if defined ENABLE_DBG && ENABLE_DBG == 0
#define PRINTF_FUNC(args...)
#else
#define PRINTF_FUNC(args...) dbg(args)
#endif

/**
 * @brief 打印信息
 *
 *
 * @details 特殊说明: 打印裸数据信息，无协议包头，打印信息在DataViewer日志显示为蓝色
 * @par eg:在DataViewer中打印：DBG_RAW
 * @code
 * DBG_RAW("DBG_RAW");
 * @endcode
 */
#define DBG_RAW(fmt,args...) 		PRINTF_FUNC(1,log_info_id,DEF_WIND,fmt,##args)

/**
 * @brief 打印信息
 *
 *
 * @details 特殊说明: 打印裸数据信息，无协议包头，打印信息在DataViewer日志显示为蓝色
 * @par eg:在DataViewer中打印：DBG_RAW_SER
 * @code
 *  DBG_RAW_SER("DBG_RAW_SER");
 * @endcode
 */
#define DBG_RAW_SER(fmt,args...) 	PRINTF_FUNC(0,log_info_id,DEF_WIND,fmt,##args)

/**
 * @brief 打印信息
 *
 *
 * @details 特殊说明: 打印裸数据信息，无协议包头，打印完成会强制换行，打印信息在DataViewer日志显示为蓝色
 * @par eg:在DataViewer中打印：PRF
 * @code
 * PRF("PRF");
 * @endcode
 */
#define PRF(fmt,args...) 			PRINTF_FUNC(1, log_info_id,DEF_WIND,fmt _APPEND_FIX,##args)
			
#ifdef DEF_DBG_I

/**
 * @brief 打印INFO信息
 *
 *
 * @details 特殊说明: 调用后立即打印，打印信息在DataViewer日志显示为蓝色
 * @par eg:在DataViewer中打印：DBG_I!————I
 * @code
 * DBG_I("DBG_I!————I");
 * @endcode
 */
#define DBG_I(fmt,args...) 								  	do{if(g_enable_dbg_i == 0){break;} if(flag_enable_cpf==0) {PRINTF_FUNC(FIX_PARAM,	log_info_id,DEF_WIND,_FILE_INFO 	_PREFIX_I	fmt _APPEND_FIX,	_FILE_INFO_ARG,##args);}\
                                                                    else                   {PRINTF_FUNC(FIX_PARAM,log_info_id,DEF_WIND,	"%s" _FILE_INFO 	_PREFIX_I	fmt COLOR_END _APPEND_FIX,	COLOR_GRE,_FILE_INFO_ARG,##args);}}while(0)
/**
  * @brief 打印INFO信息
  *
  *
  * @details 特殊说明: 调用时，只有判断这一次和上一次打印的时间间隔大于指定时间才打印，打印信息在DataViewer显示为蓝色
  * @par eg:限制两次打印的时间间隔大于200ms，在DataViewer中打印：DBG_I_LIMIT!————I
  * @code
  *DBG_I_LIMIT(200,"DBG_I_LIMIT!————I");
  * @endcode
  */
#define DBG_I_LIMIT(ms,fmt,args...) 		RUN_BY_LIMIT_BLOCK(ms,	DBG_I(fmt, ##args))

/**
  * @brief 打印INFO信息
  *
  *
  * @details 特殊说明: 调用后，阻塞指定时间后打印，打印信息在DataViewer显示为蓝色
  * @par eg:调用后,阻塞200ms后打印：DBG_I_BLOCK!————I
  * @code
  *DBG_I_BLOCK(200,"DBG_I_BLOCK!————I");
  * @endcode
  */
#define DBG_I_BLOCK(ms,fmt,args...)   		block_ms(ms); DBG_I(fmt, ##args)
#else
#define DBG_I(fmt,args...) 			NOP_FUNC
#define DBG_I_LIMIT(ms,fmt,args...)		NOP_FUNC
#define DBG_I_BLOCK(ms,fmt,args...)		NOP_FUNC
#endif


#ifdef DEF_DBG_W

/**
 * @brief 打印WARN信息
 *
 *
 * @details 特殊说明: 调用后立即打印，打印信息在DataViewer显示为黄色
 * @par eg:在DataViewer中打印：DBG_W!————W
 * @code
 * DBG_W("DBG_W!————W");
 * @endcode
 */
#define DBG_W(fmt,args...) 									do{if(g_enable_dbg_w == 0){break;} if(flag_enable_cpf==0) {PRINTF_FUNC(FIX_PARAM,log_warn_id,DEF_WIND,	_FILE_INFO 	_PREFIX_W	fmt _APPEND_FIX,	_FILE_INFO_ARG,##args);}\
                                                                    else                   {PRINTF_FUNC(FIX_PARAM,log_warn_id,DEF_WIND,	"%s" _FILE_INFO 	_PREFIX_W	fmt COLOR_END _APPEND_FIX,	COLOR_YEL,_FILE_INFO_ARG,##args);}}while(0)

/**
  * @brief 打印WARN信息
  *
  *
  * @details 特殊说明: 调用时，只有判断这一次和上一次打印的时间间隔大于指定时间才打印，打印信息在DataViewer显示为黄色
  * @par eg:限制两次打印的时间间隔大于200ms，在DataViewer中打印：DBG_W_LIMIT!————W
  * @code
  *DBG_W_LIMIT(200,"DBG_W_LIMIT!————W");
  * @endcode
  */
#define DBG_W_LIMIT(ms,fmt,args...) 		RUN_BY_LIMIT_BLOCK(ms,	DBG_W(fmt, ##args))

/**
  * @brief 打印WARN信息
  *
  *
  * @details 特殊说明: 调用后，阻塞指定时间后打印，打印信息在DataViewer显示为黄色
  * @par eg:调用后,阻塞200ms后打印：DBG_W_BLOCK!————W
  * @code
  *DBG_W_BLOCK(200,"DBG_W_BLOCK!————W");
  * @endcode
  */
#define DBG_W_BLOCK(ms,fmt,args...)		block_ms(ms); DBG_W(fmt, ##args)
#else
#define DBG_W(fmt,args...) 				NOP_FUNC
#define DBG_W_LIMIT(ms,fmt,args...)		NOP_FUNC
#define DBG_W_BLOCK(ms,fmt,args...)		NOP_FUNC
#endif

#ifdef DEF_DBG_E

/**
 * @brief 打印ERRO信息
 *
 *
 * @details 特殊说明: 调用后立即打印，打印信息在DataViewer显示为红色
 * @par eg:在DataViewer中打印：DBG_E!————E
 * @code
 * DBG_E("DBG_E!————E");
 * @endcode
 */
#define DBG_E(fmt,args...) 									do{if(g_enable_dbg_e == 0){break;} if(flag_enable_cpf==0) {PRINTF_FUNC(FIX_PARAM,log_err_id,DEF_WIND,	_FILE_INFO 	_PREFIX_E	fmt _APPEND_FIX,	_FILE_INFO_ARG,##args);}\
                                                                    else                   {PRINTF_FUNC(FIX_PARAM,log_err_id,DEF_WIND,	"%s" _FILE_INFO 	_PREFIX_E	fmt COLOR_END _APPEND_FIX,	COLOR_RED,_FILE_INFO_ARG,##args);}}while(0)
/**
  * @brief 打印ERRO信息
  *
  *
  * @details 特殊说明: 调用时，只有判断这一次和上一次打印的时间间隔大于指定时间才打印，打印信息在DataViewer显示为红色
  * @par eg:限制两次打印的时间间隔大于200ms，在DataViewer中打印：DBG_E_LIMIT!————E
  * @code
  *DBG_E_LIMIT(200,"DBG_E_LIMIT!————E");
  * @endcode
  */
#define DBG_E_LIMIT(ms,fmt,args...) 		RUN_BY_LIMIT_BLOCK(ms,	DBG_E(fmt, ##args))

/**
  * @brief 打印ERRO信息
  *
  *
  * @details 特殊说明: 调用后，阻塞指定时间后打印，打印信息在DataViewer显示为红色
  * @par eg:调用后,阻塞200ms后打印：DBG_E_BLOCK!————E
  * @code
  *DBG_E_BLOCK(200,"DBG_E_BLOCK!————E");
  * @endcode
  */
#define DBG_E_BLOCK(ms,fmt,args...)		block_ms(ms); DBG_E(fmt, ##args)
#else
#define DBG_E(fmt,args...) 				NOP_FUNC
#define DBG_E_LIMIT(ms,fmt,args...)		NOP_FUNC
#define DBG_E_BLOCK(ms,fmt,args...)		NOP_FUNC
#endif


#define REG_DEBUG(dev_id)                               \
         static uint8_t      debug_lb_buf_##dev_id[DEBUG_BUF_SIZE]={0 };\
         static linebuf_t    debug_lb_##dev_id = {NULL,debug_lb_buf_##dev_id,DEBUG_BUF_SIZE,0,0,0};\
         static debug_obj_t   debug_ch_##dev_id = {PTR_TYPE_XH_DEV(id),&debug_lb_##dev_id,};\
         AUTO_REG_ITEM(DBG_DEV_##dev_id  ,AUTO_REG_TYPE_DBG_DEV ,(void*)&debug_ch_##dev_id)>

/**
 * @brief   注册打印数据链路
 *
 *
 * @details 特殊说明:可以注册多个数据链路
 * @par eg:注册两个打印数据链路
 * @code
 * REG_DEBUG_LINK(LINK_UART1,LINK_UART2)
 * @endcode
 */            
#define REG_DEBUG_LINK(link...)                             \
                        uint8_t g_x_dbg_link_arry[] = {link};           \
                        X_DBG_LINK_TAB g_x_dbg_link_tab =               \
                        {                                               \
                            g_x_dbg_link_arry,                          \
                            ARRY_ITEMS_NUM(g_x_dbg_link_arry),          \
                        };

typedef struct _X_DBG_LINK_TAB{
    uint8_t *p_link_arry;
    uint8_t link_num;
}X_DBG_LINK_TAB, *P_X_DBG_LINK_TAB;

typedef struct
{
    void * const debug_dev;
    linebuf_t	*p_line_buf;
	
	list_t   	debug_node;			//dbg			--dbg.c
	list_t   	shell_debug_node;	//shell_node	--shell.c
} debug_obj_t;



enum
{
	log_info_id = 0x0,
	log_warn_id ,
	log_err_id ,
	log_debug_id ,
};

#define DEF_WIND  0

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus


extern uint8_t g_enable_dbg_i;
extern uint8_t g_enable_dbg_w;
extern uint8_t g_enable_dbg_e;
extern uint32_t flag_enable_cpf;

void dbg(uint8_t enable_protocol, uint8_t log_type,uint8_t log_window,char *fmt, ...);
void protocol_dbg(uint8_t enable_protocol, char *fmt, ...);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __XF_DEBUG_H__
