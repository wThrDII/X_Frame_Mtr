/**
 * @brief param_cfg.c.xtemp header file
 * @file param_cfg.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 16:29:14
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 16:29:14
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __PARAM_CFG_H__
#define __PARAM_CFG_H__

enum
{
    eu_int8 	= 0x10,
    eu_uint8 	= 0x01,
    eu_int16 	= 0x12,
    eu_uint16 	= 0x03,
    eu_int32 	= 0x14,
    eu_uint32 	= 0x05,
    eu_fp32 	= 0x06,
};


#define   PC_SHOW_LIST                  0
#define   PC_SHOW_SWITCH                1
#define   PC_SHOW_CHECK_BOX             2
#define   PC_SHOW_PROGRASS_BAR          3

#define PARM_RW    (attr_r|attr_w)
#define PARM_R     (attr_r)
#define PARM_W     (attr_w)

/**
 * @brief 注册参数配置项
 *
 * @param [in] var_name 调试参数名
 * @param [in] var 调试参数
 * @param [in] type 调试参数数据类型枚举
 *              e.g : eu_int8 	= 0x10,
 *                    eu_uint8 	= 0x01,
 *                    eu_int16 	= 0x12,
 *                    eu_uint16 = 0x03,
 *                    eu_int32 	= 0x14,
 *                    eu_uint32 = 0x05,
 *                    eu_fp32 	= 0x06,
 * @param [in] attr 参数读写属性
 *              e.g : #define PARM_RW    (attr_r|attr_w)
 *                    #define PARM_R     (attr_r)
 *                    #define PARM_W     (attr_w)
 * @param [in] def 参数默认值
 * @param [in] min 参数可调范围的最小值
 * @param [in] max 参数可调范围的最大值
 *
 * @details:
 * @par e.g:
 * @code
 *      staric uint8_t user_val;
 *
 *      _REG_CFG_ITEM(user_val, user_val, eu_uint8, PARM_RW, 0.0f, 0.0f, 100.0f);
 * @endcode
 */
#define REG_CFG_ITEM(var_name,var,type,attr,def,min,max) \
        const param_item_cfg_t  param_cfg_##var_name= {#var_name,type,attr,sizeof(var),def,min,max,&(var)};\
        param_item_t            param_##var_name = {&param_cfg_##var_name};\
        AUTO_REG_ITEM(param_item_##var_name,AUTO_REG_TYPE_PARAM_CFG,(void*)&param_##var_name)

/**
 * @brief 注册参数配置项，有回调函数
 *
 * @param [in] var_name 调试参数名
 * @param [in] var 调试参数
 * @param [in] type 调试参数数据类型枚举
 *              e.g : eu_int8 	= 0x10,
 *                    eu_uint8 	= 0x01,
 *                    eu_int16 	= 0x12,
 *                    eu_uint16 = 0x03,
 *                    eu_int32 	= 0x14,
 *                    eu_uint32 = 0x05,
 *                    eu_fp32 	= 0x06,
 * @param [in] attr 参数读写属性
 *              e.g : #define PARM_RW    (attr_r|attr_w)
 *                    #define PARM_R     (attr_r)
 *                    #define PARM_W     (attr_w)
 * @param [in] def 参数默认值
 * @param [in] min 参数可调范围的最小值
 * @param [in] max 参数可调范围的最大值
 * @param [in] cb_func 用户自定义回调函数
 *
 * @details:
 * @par e.g:
 * @code
 *      staric uint8_t user_val;
 *      void user_func(void);    //函数声明
 *
 *      _REG_CFG_ITEM_CB(user_val, user_val, eu_uint8, PARM_RW, 0.0f, 0.0f, 100.0f, user_func);
 * @endcode
 */
#define REG_CFG_ITEM_CB(var_name,var,type,attr,def,min,max,cb_func) \
        const param_item_cfg_t  param_cfg_##var_name= {#var_name,type,attr,sizeof(var),def,min,max,&(var),cb_func};\
        param_item_t            param_##var_name = {&param_cfg_##var_name};\
        AUTO_REG_ITEM(param_item_##var_name,AUTO_REG_TYPE_PARAM_CFG,(void*)&param_##var_name)

/**
 * @brief 注册参数配置项，有回调函数和显示类型
 * 
 * @param [in] var_name 调试参数名
 * @param [in] var 调试参数数组
 * @param [in] type 调试参数数据类型枚举
 *              e.g : eu_int8 	= 0x10,
 *                    eu_uint8 	= 0x01,
 *                    eu_int16 	= 0x12,
 *                    eu_uint16 = 0x03,
 *                    eu_int32 	= 0x14,
 *                    eu_uint32 = 0x05,
 *                    eu_fp32 	= 0x06,
 * @param [in] attr 参数读写属性
 *              e.g : #define PARM_RW    (attr_r|attr_w)
 *                    #define PARM_R     (attr_r)
 *                    #define PARM_W     (attr_w)   
 * @param [in] def 参数默认值
 * @param [in] min 参数可调范围的最小值
 * @param [in] max 参数可调范围的最大值
 * @param [in] cb_func 用户自定义回调函数  
 * @param [in] gui_type 显示类型
 * 
 * @details: 
 * @par e.g:
 * @code
 *      staric uint8_t user_val;
 *      void user_func(void);    //函数声明
 *
 *      _REG_GUI_CFG_ITEM(user_val, user_val, eu_uint8, PARM_RW, 0.0f, 0.0f, 100.0f, user_func, 0); 
 * @endcode
 */           
#define REG_GUI_CFG_ITEM(var_name,var,type,attr,def,min,max,cb_func,gui_type) \
        const param_item_cfg_t  param_cfg_##var_name= {#var_name,type,attr,sizeof(var),def,min,max,&(var),cb_func,gui_type};\
        param_item_t            param_##var_name = {&param_cfg_##var_name};\
        AUTO_REG_ITEM(param_item_##var_name,AUTO_REG_TYPE_PARAM_CFG,(void*)&param_##var_name)        

/**
 * @brief 对参数功能实现函数的接口进行链路注册
 *
 * @param [in] param_cfg_link 链路id
 *
 * @details:
 * @par e.g:
 * @code
 *      #define UART_1 0
 * 
 *      _REG_PARAM_CFG_LINK(UART_1);
 * @endcode
 */
#define REG_PARAM_CFG_LINK(param_cfg_link)                                                                   	\
        REG_LINK_ACTION(param_cfg_link, COMM_CMD_SET, CMD_ID_GET_PARAM_CNT, param_act_param_cnt);               \
        REG_LINK_ACTION(param_cfg_link, COMM_CMD_SET, CMD_ID_GET_PARAM_ITEM, param_act_param_item);             \
        REG_LINK_ACTION(param_cfg_link, COMM_CMD_SET, CMD_ID_WR_PARAM_ITEM, param_act_param_wr);                \
        REG_LINK_ACTION(param_cfg_link, COMM_CMD_SET, CMD_ID_RD_PARAM_ITEM, param_act_param_rd);                \
        REG_LINK_ACTION(param_cfg_link, COMM_CMD_SET, CMD_ID_RS_PARAM_ITEM, param_act_param_rs);                \
        REG_LINK_ACTION(param_cfg_link, COMM_CMD_SET, CMD_ID_RS_ALL_PARAM_ITEM, param_act_param_rs_all);        \
        REG_LINK_ACTION_CMD(param_cfg_link, COMM_CMD_SET, CMD_ID_WR_PARAM_NAME_ITEM, param_act_param_name_wr);  \
        REG_LINK_ACTION_CMD(param_cfg_link, COMM_CMD_SET, CMD_ID_RD_PARAM_NAME_ITEM, param_act_param_name_rd);  \
        REG_LINK_ACTION_CMD(param_cfg_link, COMM_CMD_SET, CMD_ID_RS_PARAM_NAME_ITEM, param_act_param_name_rs);

enum
{
    attr_r		= 1,
    attr_w		= 2,
};

//1-内存运行数据结构

typedef struct
{
    list_t          param_head;
    uint32_t        param_count;
} param_core_t;

typedef union
{
    uint8_t         u8_val;
    int8_t         i8_val;
    uint16_t        u16_val;
    int16_t        i16_val;
    uint32_t        u32_val;
    int32_t        i32_val;
    float           fp32_val;
} value_contain_t;


typedef void (*pf_save_func)(uint32_t addr, uint32_t len);
typedef void (*pf_user_cb_func)(void);
typedef struct
{
    const char	*name;
    uint8_t			type;
    uint8_t 		attr;
    uint8_t  		size;
    float			def_val;
    float			min_val;
    float			max_val;
    void		    *p_param_val_addr;
	pf_user_cb_func user_cb_func;  
    uint8_t         show_type;    
} param_item_cfg_t;

//之所以加一层是因为param_item_cfg_t 基本是不变得内存，节省内存从36bytes->16
typedef struct
{
    const param_item_cfg_t *p_item_cfg;
    //pf_save_func    save_func;
    list_t          param_node;
    uint32_t        hash_value;
} param_item_t;

#pragma  pack(1)
//2-调参软件通信数据结构
typedef struct
{
    uint8_t         status;
    uint32_t        item_num;
    uint8_t			type;
    uint8_t 		attr;
    uint8_t		    size;
    float			cur_val;
    float			def_val;
    float			min_val;
    float			max_val;
    uint8_t         name[1];        //字符长度以 伪0 结束
    uint8_t         show_type;
} protocol_param_item_t;

typedef struct
{
    uint8_t     status;
    uint32_t    total_item_num;
} ack_param_cnt_t;

typedef struct
{
    int8_t     status;
    uint32_t    wr_index;
    float       ret_val;
} ack_param_wr_t;

typedef struct
{
    int8_t      status;
    float       ret_val;
    char        name[1];        /* 注意要以伪0结束 */
} ack_param_wr_name_t;

typedef struct
{
    uint32_t  wr_index;
    float     val;
} param_wr_t;

typedef struct
{
    float     val;
    char        name[1];        /* 注意要以伪0结束 */
} param_wr_name_t;

#pragma pack()

//3-存储参数数据结构
typedef struct
{
    uint32_t        hash_value;//crc16+sum16
    uint8_t			type;
    uint8_t 		attr;
    uint16_t		size;
    float			cur_val;
    float			def_val;
    float			min_val;
    float			max_val;
} save_param_item_t;


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

/**
 * @brief 向上位机返回注册的调试参数的总数
 * 
 * @param [in] p_frame_info 链路结构体
 * 
 * @return int32_t 处理结果
 *  @retval 
 * @details: 
 * @par e.g:
 * @code
 *      hal_frame_info_t UART_link;
 *
 *      param_act_param_cnt(UART_link);
 * @endcode
 */
int32_t param_act_param_cnt(hal_frame_info_t *p_frame_info);

/**
 * @brief 向上位机返回所有注册的调试参数的所有信息
 * 
 * @param [in] p_frame_info 链路结构体
 * 
 * @return int32_t 处理结果
 *  @retval 
 * @details: 
 * @par e.g:
 * @code
 *      hal_frame_info_t UART_link;
 *
 *      param_act_param_item(UART_link);
 * @endcode
 */
int32_t param_act_param_item(hal_frame_info_t *p_frame_info);

/**
 * @brief 上位机向设备中写入调试参数的数值
 *
 * @param [in] p_frame_info 链路结构体
 *
 * @return int32_t 处理结果
 *  @retval
 * @details:
 * @par e.g:
 * @code
 *      hal_frame_info_t UART_link;
 *
 *      param_act_param_wr(UART_link);
 * @endcode
 */
int32_t param_act_param_wr(hal_frame_info_t *p_frame_info);

/**
 * @brief 设备向上位机返回调试参数的数值
 * 
 * @param [in] p_frame_info 链路结构体
 * 
 * @return int32_t 处理结果
 *  @retval 
 * @details: 
 * @par e.g:
 * @code
 *      hal_frame_info_t UART_link;
 *
 *      param_act_param_rd(UART_link);   
 * @endcode
 */
int32_t param_act_param_rd(hal_frame_info_t *p_frame_info);

/**
 * @brief 重置单个调试参数
 * 
 * @param [in] p_frame_info 链路结构体
 * 
 * @return int32_t 处理结果 
 *  @retval 
 * @details: 
 * @par e.g:
 * @code
 *      hal_frame_info_t UART_link;
 *
 *      param_act_param_rs(UART_link);    
 * @endcode
 */
int32_t param_act_param_rs(hal_frame_info_t *p_frame_info);

/**
 * @brief 重置所有调试参数的数值
 * 
 * @param [in] p_frame_info 链路结构体
 * 
 * @return int32_t 处理结果
 *  @retval 
 * @details: 
 * @par e.g:
 * @code
 *      hal_frame_info_t UART_link;
 *
 *      param_act_param_rs_all(UART_link);   
 * @endcode
 */
int32_t param_act_param_rs_all(hal_frame_info_t *p_frame_info);

/**
 * @brief 上位机向设备中写入调试参数的数值,通过name获取信息，然后修改
 * 
 * @param [in] p_frame_info 链路结构体
 * 
 * @return int32_t 处理结果
 *  @retval 
 * @details: 
 * @par e.g:
 * @code
 *      hal_frame_info_t UART_link;
 *
 *      param_act_param_name_wr(UART_link);    
 * @endcode
 */
int32_t param_act_param_name_wr(hal_frame_info_t *p_frame_info);

/**
 * @brief 设备向上位机返回调试参数的数值，通过name获取信息，然后读取
 * 
 * @param [in] p_frame_info 链路结构体
 * 
 * @return int32_t 处理结果
 *  @retval 
 * @details: 
 * @par e.g:
 * @code
  *      hal_frame_info_t UART_link;
 *
 *      param_act_param_name_rd(UART_link);    
 * @endcode
 */
int32_t param_act_param_name_rd(hal_frame_info_t *p_frame_info);

/**
 * @brief 重置单个调试参数，通过name获取信息，然后重置
 * 
 * @param [in] p_frame_info 链路结构体
 * 
 * @return int32_t 处理结果
 *  @retval 
 * @details: 
 * @par e.g:
 * @code
 *      hal_frame_info_t UART_link;
 *
 *      param_act_param_name_rs(UART_link);   
 * @endcode
 */
int32_t param_act_param_name_rs(hal_frame_info_t *p_frame_info);

extern param_item_t  *rd_param_item_name(char *p_item_name, float *val);
extern param_item_t  *wr_param_item_name(char *p_item_name, float val);
extern param_item_t  *rs_param_item_name(char *p_item_name);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __PARAM_CFG_H__
