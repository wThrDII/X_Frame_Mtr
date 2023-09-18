/**
 * @brief protocol_push.c.xtemp header file
 * @file protocol_push.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 16:58:27
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 16:58:27
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __PROTOCOL_PUSH_H__
#define __PROTOCOL_PUSH_H__

/**
 * @brief 定义推送组策略
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 * @param [in]  rate           推流速度，单位为byte/sec
 * 
 * @details 
 * @par 示例:定义一个组策略，链路ID为LINK_UART1，推送的目标地址为PC_ADDR(宏定义为0x01)，推流速度为6000 byte/sec
 * @code
 *   REG_PROTOCOL_PUSH_GROUP(LINK_UART1,PC_ADDR,6000)
 * @endcode
 */
#define REG_PROTOCOL_PUSH_GROUP(link_id,dest,rate)                              \
        PROTOCOL_PUSH_GROUP g_protocol_push_group_##link_id##_##dest =          \
        {                                                                       \
            link_id,                                                            \
            dest,                                                               \
            rate,                                                               \
        };                                                                      \
        AUTO_REG_ITEM(  g_reg_protocol_push_group_##link_id##_##dest,           \
                        AUTO_REG_PROTOCOL_PUSH_GROUP,                           \
                        (void*)&g_protocol_push_group_##link_id##_##dest)

/**
 * @brief 定义成员策略，非强制推送
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 * @param [in]  def_interval   最小推送时间间隔，单位为ms 
 * @param [in]  priority       推送优先级，数值越小，优先级越高 
 * @param [in]  push_cb        推送处理回调函数名
 * 
 * @details 非强制推送，调用停止推送接口可以停止推送
 * @par 示例:定义两个成员策略，链路ID为LINK_UART1，目标地址为PC_ADDR(宏定义为0x01)，最小推送间隔为700ms，优先级为0
 *           一个成员的回调函数名为push_sn_info，另一个成员回调函数名为push_product_info
 * @code
 *  uint16_t push_sn_info(uint8_t is_first,uint32_t link_id,uint8_t dest)
 *  {
 * 	    DBG_I("push_sn_info");
 *      return 0;
 *  }
 *  uint16_t push_product_info(uint8_t is_first,uint32_t link_id,uint8_t dest)
 *  {
 * 	    DBG_I("push_product_info");
 * 	    return 0;
 *  }
 * 	 REG_PROTOCOL_PUSH_ITEM(LINK_UART1,PC_ADDR,700,0,push_sn_info) 
 * 	 REG_PROTOCOL_PUSH_ITEM(LINK_UART1,PC_ADDR,700,0,push_product_info)
 * @endcode
 */
#define REG_PROTOCOL_PUSH_ITEM(link_id,dest,def_interval,priority,push_cb)      \
        PROTOCOL_PUSH_ITEM g_protocol_push_item_##link_id##_##dest##push_cb = { link_id, dest, def_interval, priority, push_cb, false, }; \
		AUTO_REG_ITEM(  g_reg_protocol_push_item_##link_id##_##dest##push_cb,   \
                        AUTO_REG_PROTOCOL_PUSH_ITEM,                            \
                        (void*)&g_protocol_push_item_##link_id##_##dest##push_cb)

/**
 * @brief 定义成员策略，强制推送
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 * @param [in]  def_interval   最小推送间隔，单位为ms 
 * @param [in]  priority       推送优先级，数值越小，优先级越高 
 * @param [in]  push_cb        推送处理回调函数名
 * 
 * @details 强制推送，不受停止推送接口影响
 * @par 示例:定义两个强制推送的成员策略，链路ID为LINK_UART1，目标地址为PC_ADDR(宏定义为0x01)，最小推送间隔为700ms，优先级为0
 *           一个成员的回调函数名为push_sn_info，另一个成员回调函数名为push_product_info
 * @code
 *  uint16_t push_sn_info(uint8_t is_first,uint32_t link_id,uint8_t dest)
 *  {
 * 	    DBG_I("push_sn_info");
 *      return 0;
 *  }
 *  uint16_t push_product_info(uint8_t is_first,uint32_t link_id,uint8_t dest)
 *  {
 * 	    DBG_I("push_product_info");
 * 	    return 0;
 *  }
 * 	REG_PROTOCOL_FORCE_PUSH_ITEM(LINK_UART1,PC_ADDR,700,0,push_sn_info) 
 * 	REG_PROTOCOL_FORCE_PUSH_ITEM(LINK_UART1,PC_ADDR,700,0,push_product_info)
 * @endcode
 */
#define REG_PROTOCOL_FORCE_PUSH_ITEM(link_id,dest,def_interval,priority,push_cb)        \
        PROTOCOL_PUSH_ITEM g_protocol_push_item_##link_id##_##dest##push_cb = { link_id, dest, def_interval, priority, push_cb, true, };   \
		AUTO_REG_ITEM(  g_reg_protocol_push_item_##link_id##_##dest##push_cb,           \
                        AUTO_REG_PROTOCOL_PUSH_ITEM,                                    \
                        (void*)&g_protocol_push_item_##link_id##_##dest##push_cb)
            
#define protocol_group_rate(link_id,dest)                                               real_protocol_group_rate(link_id,dest)   

/**
 * @brief 获取协议组速率占用百分比
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 *
 * @return uint8_t
 * @retval 返回协议组速率占用百分比
 * @details 
 * @par 示例:获取协议组速率占用百分比，链路ID为LINK_UART1，目标地址为PC_ADDR(宏定义为0x01)
 * @code
 *   uint8_t ratio;
 *   ratio = protocol_group_use_ratio(LINK_UART1,PC_ADDR);
 * @endcode
 */
#define protocol_group_use_ratio(link_id,dest)                                          real_protocol_group_use_ratio(link_id,dest)   

/**
 * @brief 组相关API，重置组推送
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 *
 * @details 
 * @par 示例:重置组推送，链路ID为LINK_UART1，目标地址为PC_ADDR(宏定义为0x01)
 * @code
 *   protocol_reset_group_push(LINK_UART1,PC_ADDR);  
 * @endcode
 */
#define protocol_reset_group_push(link_id,dest)                                         real_protocol_reset_group_push(link_id,dest)  

/**
 * @brief 组相关API，停止组推送，除强制推送项目外
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 *
 * @details 
 * @par 示例:停止组推送，链路ID为LINK_UART1，目标地址为PC_ADDR(宏定义为0x01)
 * @code
 *   protocol_stop_group_push(LINK_UART1,PC_ADDR);
 * @endcode
 */
#define protocol_stop_group_push(link_id,dest)                                          real_protocol_stop_group_push(link_id,dest)   

/**
 * @brief 组相关API，开始组推送，除强制推送项目外
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 *
 * @details 
 * @par 示例:开始组推送，链路ID为LINK_UART1，目标地址为PC_ADDR(宏定义为0x01)
 * @code
 *   protocol_start_group_push(LINK_UART1,PC_ADDR);
 * @endcode
 */
#define protocol_start_group_push(link_id,dest)                                         real_protocol_start_group_push(link_id,dest)  
  
/**
 * @brief 项目相关API，停止所有项目推送，除强制推送项目外
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 *
 * @details 
 * @par 示例:停止除强制推送项目外的所有项目推送，链路ID为LINK_UART1，目标地址为PC_ADDR(宏定义为0x01)
 * @code
 *   protocol_stop_all_item_push(LINK_UART1,PC_ADDR);
 * @endcode
 */    
#define protocol_stop_all_item_push(link_id,dest)                                       real_protocol_stop_all_item_push(link_id,dest)  

/**
 * @brief 项目相关API，开始所有项目推送，除强制推送项目外
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 *
 * @details 
 * @par 示例:开始除强制推送项目外的所有项目推送，链路ID为LINK_UART1，目标地址为PC_ADDR(宏定义为0x01)
 * @code
 *   protocol_start_all_item_push(LINK_UART1,PC_ADDR);
 * @endcode
 */
#define protocol_start_all_item_push(link_id,dest)                                      real_protocol_start_all_item_push(link_id,dest) 

/**
 * @brief 项目相关API，重置所有项目推送
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 *
 * @details 重置后停止的推送项目将开启
 * @par 示例:复位所有项目推送，链路ID为LINK_UART1，目标地址为PC_ADDR(宏定义为0x01)
 * @code
 *   protocol_reset_all_item_push(LINK_UART1,PC_ADDR); 
 * @endcode
 */
#define protocol_reset_all_item_push(link_id,dest)                                      real_protocol_reset_all_item_push(link_id,dest)  

/**
 * @brief 项目相关API，重置指定的项目推送
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 * @param [in]  p_push_ch      项目的回调函数名
 *
 * @details 重置后停止的推送项目将开启
 * @par 示例:重置一个项目推送，链路ID为LINK_UART1，目标地址为PC_ADDR(宏定义为0x01)，项目的回调函数名为push_product_info
 * @code
 *   protocol_reset_item_push(LINK_UART1,PC_ADDR,push_product_info);
 * @endcode
 */
#define protocol_reset_item_push(link_id,dest,p_push_ch)                                real_protocol_reset_item_push(link_id,dest,p_push_ch) 

/**
 * @brief 项目相关API，停止指定的项目推送，除强制推送项目外
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 * @param [in]  p_push_ch      项目的回调函数名 
 *
 * @details 
 * @par 示例:停止一个项目推送，链路ID为LINK_UART1，目标地址为PC_ADDR(宏定义为0x01)，回调函数名为push_product_info
 * @code
 *   protocol_stop_item_push(LINK_UART1,PC_ADDR,push_product_info);
 * @endcode
 */
#define protocol_stop_item_push(link_id,dest,p_push_ch)                                 real_protocol_stop_item_push(link_id,dest,p_push_ch)  

/**
 * @brief 项目相关API，开始指定的项目推送
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 * @param [in]  p_push_ch      项目的回调函数名 
 *
 * @details 
 * @par 示例:开始一个项目推送，链路ID为LINK_UART1，目标地址为PC_ADDR(宏定义为0x01)，回调函数名为push_product_info
 * @code
 *   protocol_start_item_push(LINK_UART1,PC_ADDR,push_product_info);
 * @endcode
 */
#define protocol_start_item_push(link_id,dest,p_push_ch)                                real_protocol_start_item_push(link_id,dest,p_push_ch)     

/**
 * @brief 项目相关API，设置项目的推送的最小时间间隔
 *
 * @param [in]  link_id        链路ID 
 * @param [in]  dest           目标地址
 * @param [in]  p_push_ch      项目的回调函数名  
 * @param [in]  push_interval  推送的最小时间间隔，单位ms
 * 
 * @details 
 * @par 示例:设置一个项目推送最小时间间隔为1000ms，链路ID为LINK_UART1，目标地址为PC_ADDR(宏定义为0x01)，回调函数名为push_product_info
 * @code
 *   protocol_set_item_push_interval(LINK_UART1,PC_ADDR,push_product_info,1000);
 * @endcode
 */
#define protocol_set_item_push_interval(link_id,dest,p_push_ch,push_interval)           real_protocol_set_item_push_interval(link_id,dest,p_push_ch,push_interval)

typedef enum {
    PROTOCOL_PUSH_TIME,
    PROTOCOL_PUSH_ONE,
}PROTOCOL_PUSH_MODE;

typedef struct _PROTOCOL_PUSH_ITEM{
    uint32_t link_id;
    uint8_t dest;
    uint32_t def_interval;      /* 默认间隔 */
    uint16_t priority;          /* 推送优先级,数字越低，优先级越高 */
/*************************************************************
** Function name:       push_cb
** Descriptions:        数据推送回调
** Input parameters:    None
** Returned value:      返回发送数据的长度
** Remarks:             None
*************************************************************/
    uint16_t (*push_cb)(uint8_t is_first,uint32_t link_id,uint8_t dest);
    bool force_mark;            /* 强制推送 */
    struct{
        uint8_t push_mark:1;
        uint8_t is_first:1;
        uint32_t start_time;
        uint32_t push_interval;      /* 运行推送间隔 */
    }exec;
    list_t list;                    		/* 链表参数 */
}PROTOCOL_PUSH_ITEM, *P_PROTOCOL_PUSH_ITEM;

typedef struct _PROTOCOL_PUSH_GROUP{
    uint32_t link_id;
    uint8_t dest;
    uint32_t rate;                          /* 速率,单位 b/s */
    struct{
        uint8_t push_mark:1;
        uint32_t start_time;
        uint32_t next_time;

        uint32_t run_time;                  /* 单位s */
        uint32_t real_rate;
        uint32_t send_byte;                 /* 发送数据字节 */
        uint8_t use_ratio;                  /* 占用率 */
        uint32_t remain_byte;
    }exec;
    list_t item_list_head;
    list_t list;                    		/* 链表参数 */
}PROTOCOL_PUSH_GROUP, *P_PROTOCOL_PUSH_GROUP;


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

/* 组相关API */
uint32_t real_protocol_group_real_rate(uint8_t link_id,uint8_t dest);
uint8_t real_protocol_group_use_ratio(uint8_t link_id,uint8_t dest);
void real_protocol_reset_group_push(uint8_t link_id,uint8_t dest);
void real_protocol_stop_group_push(uint8_t link_id,uint8_t dest);
void real_protocol_start_group_push(uint8_t link_id,uint8_t dest);
                        
/* 项目相关API - 所有项 */                   
void real_protocol_stop_all_item_push(uint8_t link_id,uint8_t dest);
void real_protocol_start_all_item_push(uint8_t link_id,uint8_t dest);
void real_protocol_reset_all_item_push(uint8_t link_id,uint8_t dest);

/* 项目相关API - 单项 */
void real_protocol_reset_item_push(uint8_t link_id,uint8_t dest,void *p_push_ch);
void real_protocol_stop_item_push(uint8_t link_id,uint8_t dest,void *p_push_ch);
void real_protocol_start_item_push(uint8_t link_id,uint8_t dest,void *p_push_ch);
void real_protocol_set_item_push_interval(uint8_t link_id,uint8_t dest,void *p_push_ch,uint32_t push_interval);
      


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __PROTOCOL_PUSH_H__
