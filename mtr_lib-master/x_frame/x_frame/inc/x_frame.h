#ifndef __X_FRAME_H__
#define __X_FRAME_H__

#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "string.h"
#include "stdarg.h"

/**
 * @file list.h
 *
 * @note 功能注解
 * 双向链表模块
 * 应用层可以直接调用
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: LIST_HEAD(name)
 * :: list_entry(ptr, type, member)
 * :: list_for_each(pos, head)
 * :: list_for_each_prev(pos, head)
 * :: list_for_each_safe(pos, n, head)
 * :: list_for_each_entry(pos, head, member)
 * :: list_for_each_entry_safe(pos, n, head, member)
 * :: void list_add(struct list_head *new, struct list_head *head);
 * :: void list_add_tail(struct list_head *new, struct list_head *head);
 * :: void list_del(struct list_head *entry);
 * :: void list_del_init(struct list_head *entry);
 * :: void list_move(struct list_head *list, struct list_head *head);
 * :: void list_move_tail(struct list_head *list, struct list_head *head)
 * :: int list_empty(struct list_head *head);
 * :: void list_splice(struct list_head *list, struct list_head *head);
 *
 * @pre 依赖项列表
 * 无
 * @attention 注意事项(attention)
 */
#include "list.h"

/**
 * @file xf_common.h
 *
 * @note 功能注解
 * 通用宏定义
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 宏定义信息：(宏定义信息详细说明，请在文件中查看)
 * :: 数据位交换宏
 * :: 计算数组元素个数
 * :: 设置字节对齐
 * :: 程序编译时间
 * :: 弱定义
 * :: 置位和复位宏
 * :: UID的配置宏
 *
 * @pre 依赖项列表
 * 无
 * @attention 注意事项(attention)
 */
#include "xf_common.h"

/**
 * @file kfifo.h
 *
 * @note 功能注解
 * 通用环形队列缓存模块，用于缓存数据
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: DEF_KFIFO(name,len)
 * :: uint32_t kfifo_push_in(kfifo_t *p_kfifo, uint8_t *p_buf, uint32_t len);
 * :: uint32_t kfifo_pull_out(kfifo_t *p_kfifo, uint8_t *p_buf, uint32_t len);
 * :: void kfifo_reset(kfifo_t *pfifo);
 * :: uint32_t kfifo_get_data_len(kfifo_t *p_kfifo);
 * :: uint32_t kfifo_get_rest_len(kfifo_t *p_kfifo);
 * :: uint32_t kfifo_get_w_index(kfifo_t *p_kfifo);
 * :: uint32_t kfifo_get_r_index(kfifo_t *p_kfifo);
 *
 * @pre 依赖项列表
 * 无
 * @attention 注意事项(attention)
 */
#include "kfifo.h"

/**
 * @file line_buf.h
 *
 * @note 功能注解
 * 通用线性队列缓存模块
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: int32_t lb_init(const char *name, linebuf_t *p_lb,uint8_t *pbuf, uint16_t size);
 * :: int32_t lb_get_rest_len(linebuf_t	*p_lb);
 * :: int32_t lb_get_data_len(linebuf_t	*p_lb);
 * :: uint8_t lb_get_statues(linebuf_t *p_lb);
 * :: int32_t lb_push(linebuf_t	*p_lb, uint8_t *p_src, uint16_t len);
 * :: int32_t lb_pull(linebuf_t	*p_lb, uint8_t *p_dest, uint16_t len);
 * :: int32_t lb_move(linebuf_t	*p_lb, uint16_t len);
 * :: uint8_t *lb_get_r_ptr(linebuf_t *p_lb);
 * :: void lb_reset(linebuf_t *p_lb);
 *
 * @pre 依赖项列表
 * 无
 * @attention 注意事项(attention)
 */
#include "line_buf.h"

/**
 * @file bkfifo.h
 *
 * @note 功能注解
 * 通用块状环形队列模块
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: DEF_BKFIFO_OBJ(name,type,num)
 * :: EXT_BKFIFO_OBJ(name)
 * :: uint32_t bkfifo_cur_num(P_BKFIFO_OBJ p_obj);
 * :: uint32_t bkfifo_free_num(P_BKFIFO_OBJ p_obj);
 * :: bool bkfifo_is_empty(P_BKFIFO_OBJ p_obj);
 * :: bool bkfifo_is_full(P_BKFIFO_OBJ p_obj);
 * :: void bkfifo_clear(P_BKFIFO_OBJ p_obj);
 * :: uint32_t bkfifo_en(P_BKFIFO_OBJ p_obj, void *p_msg);
 * :: uint32_t bkfifo_de(P_BKFIFO_OBJ p_obj, void *p_msg);
 *
 * @pre 依赖项列表
 * 无
 * @attention 注意事项(attention)
 */
#include "bkfifo.h"

/**
 * @file md5.h
 *
 * @note 功能注解
 * 通用md5加密模块
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: void MD5Init(MD5_CTX *context);
 * :: void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);
 * :: void MD5Final(MD5_CTX *context,unsigned char digest[16]);
 * :: void MD5Transform(unsigned int state[4],unsigned char block[64]);
 * :: void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);
 * :: void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);
 *
 * @pre 依赖项列表
 * 无
 * @attention 注意事项(attention)
 */
#include "md5.h"

/**
 * @file check_algorithm.h
 *
 * @note 功能注解
 * 数据校验模块
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: uint32_t calc_chk_init_val(uint8_t chk_type,uint32_t init_val,uint8_t* pbuf,uint32_t len);
 * :: uint32_t calc_chk_val(uint8_t chk_type,uint8_t* pbuf,uint32_t len);
 *
 * @pre 依赖项列表
 * 依赖项
 * @attention 注意事项(attention)
 */
#include "check_algorithm.h"

/**
 * @file x_frame_cfg.h
 *
 * @note 功能注解
 * 框架配置模板
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 配置信息：(配置信息详细说明，请在文件中查看)
 * :: 硬件版本信息
 * :: 功能迭代信息
 * :: 系统配置信息
 *
 * @pre 依赖项列表
 * 无
 * @attention 注意事项(attention)
 */
#include "x_frame_cfg.h"

/**
 * @file x_frame_protocol.h
 *
 * @note 功能注解
 * x frame协议通用命令集宏
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 宏定义信息：(宏定义信息详细说明，请在文件中查看)
 * :: 心跳包
 * :: 查询是否在bootloader中
 * :: 升级传输的子命令ID
 * :: shell的子命令ID
 * :: 参数配置的子命令ID
 * :: sn_info
 * :: 产品名称
 * :: UNIX TIME
 * :: 数据记录指令集
 *
 * @pre 依赖项列表
 * 无
 * @attention 注意事项(attention)
 */
#include "x_frame_protocol.h"

/**
 * @file protocol_ef_plus.h
 *
 * @note 功能注解
 * 传输数据协议处理模块，定义了应用层统一的协议将数据结构以及协议处理功能注册接口
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_PROTOCOL(protocol_ver,get_head_len,get_pack_len,check_head,check_pack,pack_data,unpack_data,send_ack,check_route,set_send_info)
 *
 * @pre 依赖项列表
 * 无
 * @attention 注意事项(attention)
 */
#include "protocol_ef_plus.h"

/**
 * @file protocol_matrix.h
 *
 * @note 功能注解
 * 协议编解码模块
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: MATRIX_ID_DATA_LEN
 * :: MATRIX_ID_TOTAL_LEN
 * :: EXT_PROTOCOL_OBJ(protocol_ver)
 * :: PROTOCOL_MATRIX_PTR(name)
 * :: DEF_PROTOCOL_MATRIX(name,protocol_ver,buff_len,record_en,timeout_ms)
 * :: int32_t protocol_decode(protocol_matrix_t *matrix, uint8_t *data, uint32_t len, uint32_t id, hal_frame_info_t *frame);
 * :: uint16_t protocol_encode(protocol_matrix_t *matrix, hal_frame_info_t *frame, uint8_t *buff);
 *
 * @pre 依赖项列表
 * protocol_ef_plus.h 文件
 *
 * @attention 注意事项(attention)
 */
#include "protocol_matrix.h"

/**
 * @file protocol_hal.h
 *
 * @note 功能注解
 * 协议收发模块
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_LINK(link_id,prio,dev,protocol_ver,buf_size,en_route,en_debug)
 * :: REG_LINK_ACTION(link_id,cmd_set,cmd_id,func)
 * :: REG_LINK_ACTION_CMD(link_id,cmd_set,cmd_id,func)
 * :: REG_LINK_ACTION_ACK(link_id,cmd_set,cmd_id,func)
 * :: REG_MULTI_LINK_ACTION(cmd_set, cmd_id, func, link_id...)
 * :: REG_MULTI_LINK_ACTION_CMD(cmd_set, cmd_id, func, link_id...)
 * :: REG_MULTI_LINK_ACTION_ACK(cmd_set, cmd_id, func, link_id...)
 * :: REG_CMDSET_ACTION_LINK(cmdset,link...)
 * :: REG_CMDSET_ACTION(cmd_set,cmd_id,func)
 * :: REG_CMDSET_ACTION_CMD(cmd_set,cmd_id,func)
 * :: REG_CMDSET_ACTION_ACK(cmd_set,cmd_id,func)
 * :: protocol_send_by_id(p_frame_info)
 * :: protocol_send_ack(p_frame_info,pbuf,len)
 * :: protocol_send(p_frame_info)
 * :: void protocol_hal_unpack_notify(hal_frame_info_t * p_frame_info);
 *
 * :: DEF_ROUT_INFO_V2(item...)
 * :: LINK_ADDR_ROUTE_ITEM_V2(en,src_link,dest,tag_link)
 * :: ADDR_ROUTE_ITEM_V2(en,dest,tag_link)
 * :: CMD_ROUTE_ITEM_V2(en,src_link,cmd_set,cmd_id,tag_link)
 * :: LINK_ROUTE_ITEM_V2(en,src_link,tag_link,func_opt)
 * :: LINK_ADDR_ROUTE_ITEM_CHANGE_TAG_LINK_V2(src_link, dest, new_tag_link)
 * :: ADDR_ROUTE_ITEM_CHANGE_TAG_LINK_V2(dest, new_tag_link)
 * :: CMD_ROUTE_ITEM_CHANGE_TAG_LINK_V2(src_link, cmd_set, cmd_id, new_tag_link)
 * :: LINK_ROUTE_ITEM_CHANGE_TAG_LINK_V2(src_link, new_tag_link)
 * :: TURN_ROUTE_RULE_V2(enable)
 * :: TURN_LINK_ADDR_ROUTE_ITEM_V2(enable, src_link, dest)
 * :: TURN_ADDR_ROUTE_ITEM_V2(enable, dest)
 * :: TURN_CMD_ROUTE_ITEM_V2(enable, src_link, cmd_set, cmd_id)
 * :: TURN_LINK_ROUTE_ITEM_V2(enable, src_link)
 *
 * :: DEF_EF3_GET_D_DEST(action...)
 * :: DEF_ROUT_INFO_V3(item...)
 * :: LINK_ADDR_ROUTE_ITEM_V3(en,src_link,dest,tag_link)
 * :: ADDR_ROUTE_ITEM_V3(en,dest,tag_link)
 * :: CMD_ROUTE_ITEM_V3(en,src_link,cmd_set,cmd_id,tag_link)
 * :: LINK_ROUTE_ITEM_V3(en,src_link,tag_link,func_opt)
 * :: BROADCAST_ROUTE_ITEM_V3(en,src_link,dest,tag_link)
 * :: LINK_ADDR_ROUTE_ITEM_CHANGE_TAG_LINK_V3(src_link, dest, new_tag_link)
 * :: ADDR_ROUTE_ITEM_CHANGE_TAG_LINK_V3(dest, new_tag_link)
 * :: CMD_ROUTE_ITEM_CHANGE_TAG_LINK_V3(src_link, cmd_set, cmd_id, new_tag_link)
 * :: LINK_ROUTE_ITEM_CHANGE_TAG_LINK_V3(src_link, new_tag_link)
 * :: TURN_ROUTE_RULE_V3(enable)
 * :: TURN_LINK_ADDR_ROUTE_ITEM_V3(enable, src_link, dest)
 * :: TURN_ADDR_ROUTE_ITEM_V3(enable, dest)
 * :: TURN_DYNC_ADDR_ROUTE_ITEM_V3(enable, dest, d_dest)
 * :: TURN_CMD_ROUTE_ITEM_V3(enable, src_link, cmd_set, cmd_id)
 * :: TURN_LINK_ROUTE_ITEM_V3(enable, src_link)
 *
 * @pre 依赖项列表
 * auto_reg_module.h 文件
 * HAL_CFG
 *  ├─ xh_core.h    (HAL_VER==X_HAL_XH_VER)
 *  └─ hal_core.h   (HAL_VER==X_HAL_CORE_VER)
 * protocol_matrix.h
 * tasks.h
 * auto_init_mod.h
 *
 * @attention 注意事项(attention)
 */
#include "protocol_hal.h"

/**
 * @file auto_reg_module.h
 *
 * @note 功能注解
 * 自动注册模块
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: AUTO_REG_ITEM(name,type,p_user_data)
 * :: void *auto_reg_get_func_cont_by_index(uint8_t func_type, uint32_t index);
 * :: int32_t auto_reg_get_func_type_cont(uint8_t func_type, void *p_cont[], uint8_t max_read_item);
 * :: void auto_init_modules(uint32_t init_type);
 * :: uint32_t auto_reg_get_num_by_func_type(uint8_t func_type);
 *
 * @pre 依赖项列表
 * 无
 * @attention 注意事项(attention)
 */
#include "auto_reg_module.h"

/**
 * @file auto_init_mod.h
 *
 * @note 功能注解
 * 自动初始化模块
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_INIT_MOD_PRE_APP(func)
 * :: REG_INIT_MOD_APP(func)
 * :: REG_INIT_MOD_APP_PRIO(prio, func)
 * :: REG_INIT_MOD(func)
 * :: REG_VH_INIT_MOD(func)
 * :: REG_HI_INIT_MOD(func)
 * :: REG_LW_INIT_MOD(func)
 * :: REG_ID_INIT_MOD(func)
 *
 * @pre 依赖项列表
 * auto_reg_module.h
 *
 * @attention 注意事项(attention)
 */
#include "auto_init_mod.h"

/**
 * @file cfm.h
 *
 * @note 功能注解
 * 提供一个安全阈值的控制策略
 *
 * @details 使用场景说明
 * 任何需要用到安全阈值控制策略的参数。如充电电压的过压和欠压保护
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_CFM_TAB(name,item...)
 * :: RUN_CFM(name,var)
 * :: PTR_CFM_TAB(name)
 *
 * @pre 依赖项列表
 * 无
 * @attention 注意事项(attention)
 */
#include "cfm.h"

/**
 * @file comm_cmd.h
 *
 * @note 功能注解
 * 通用指令处理模块
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_LINK_UPDATA_ACTION(link_id)
 * :: REG_LINK_APP_BASE_ACTION(link_id)
 * :: DEF_SYS_INFO_ROUTE(item...)
 * :: SYS_INFO_ROUTE_ITEM(link_id,dest)
 * :: load_boot_cfg(p_data)
 * :: read_hw_version(p_data)
 * :: void firmware_updata_jump_cb(void);
 * :: void cmd_dev_reset_cb(void);
 * :: X_FRAME_WEAK int32_t firmware_download_event_notify(P_FW_DL_OBJ dl_obj, uint32_t ev, void *param);
 *
 * @pre 依赖项列表
 * flash_part.h
 * protocol_hal.h
 * auto_init_mod.h
 * HAL_CFG
 *  ├─ xh_core.h    (HAL_VER==X_HAL_XH_VER)
 *  └─ hal_core.h   (HAL_VER==X_HAL_CORE_VER)
 * sys_time.h
 * x_calendar.h
 *
 * @attention 注意事项(attention)
 */
#include "comm_cmd.h"

/**
 * @file flash_part.h
 *
 * @note 功能注解
 * flash数据通用操作模块，以便使用于各种flash驱动，如片内flash和片外flash
 *
 * @details 使用场景说明
 * 在x-frame应用层任意需要存取参数的地方都适用。
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_FLASH_PART(drv_id,start_addr,flash_size,block_size,part_item...)
 * :: part_item_t *get_flash_item_by_part_id(uint32_t part_id);
 * :: bool flash_part_is_exit(uint32_t part_id);
 * :: uint32_t flash_part_get_part_size(uint32_t part_id);
 * :: uint32_t flash_part_get_sector_size(uint32_t part_id);
 * :: bool flash_part_erase(uint32_t part_id);
 * :: bool flash_part_erase_len(uint32_t part_id, uint32_t offset, uint32_t len);
 * :: bool flash_part_write(uint32_t part_id, uint32_t offset, uint8_t *pbuf, uint32_t len);
 * :: bool flash_part_read(uint32_t part_id, uint32_t offset, uint8_t *pbuf, uint32_t len);
 *
 * @pre 依赖项列表
 * auto_init_mod.h
 * auto_reg_module.h
 * HAL_CFG
 *  ├─ xh_core.h    (HAL_VER==X_HAL_XH_VER)
 *  └─ hal_core.h   (HAL_VER==X_HAL_CORE_VER)
 *
 * @attention 注意事项(attention)
 */
#include "flash_part.h"

/**
 * @file flash_param.h
 *
 * @note 功能注解
 * 应用数据存储管理模块，提供了两种管理方案，一种是普通管理方案，另一种是队列管理方案
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: EXT_FLASH_PARAM_NAME(param_name)
 * :: REG_FLASH_NORMAL_PARAM(part_id, param_name, p_param, p_def_param, param_len)
 * :: REG_FALSH_QUEUE_PARAM(part_id, param_name, p_param, p_def_param, param_len, queue_size, block_size)
 * :: REG_LINK_FLASH_PARAM_ACTION(link_id)
 * :: flash_param_save_all()
 * :: flash_param_rd_all()
 * :: flash_param_read_all()
 * :: flash_param_id_save_all(part_id)
 * :: flash_param_id_rd_all(part_id)
 * :: flash_param_id_read_all(part_id)
 * :: flash_param_save(param_name,p_buf,len)
 * :: flash_param_rd(param_name)
 * :: flash_param_read(param_name,p_buf,len)
 * :: flash_queue_param_read_history(param_name,history_index,p_buf,len)
 *
 * @pre 依赖项列表
 * flash_part.h
 * protocol_hal.h
 * auto_init_mod.h
 * auto_reg_module.h
 *
 * @attention 注意事项(attention)
 */
#include "flash_param.h"

/**
 * @file fsm.h
 *
 * @note 功能注解
 * 提供一种统一状态进出规则处理的状态机功能。支持开发者定义多个状态机，每个状态机互相独立，
 * 状态机的状态节点可以灵活新增，每个状态节点的切换逻辑由开发者自行设计，x_frame只提供切换接口。
 * 根据需求可支持一级或多级状态机。
 *
 * @details 使用场景说明
 * 任意事件存在多个工作状态的情况下都可用fsm。
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_MULTI_FSM(fsm_id,fsm_run_time,max_param_len,init_state,in_func,out_func,parent_fsm_id,parent_state_id)
 * :: REG_FSM(fsm_id,fsm_run_time,max_param_len,init_state,in_func,out_func)
 * :: GET_FSM_STATE(fsm_id)
 * :: REG_FSM_STATE(fsm_id,state_id,in_func,exe_func,out_func,cond_func)
 * :: fsm_send_event(fsm_id,event,pbuf,len)
 * :: fsm_reset_state(fsm_id,new_state)
 * @pre 依赖项列表
 * tasks.h
 * auto_init_mod.h
 * auto_reg_module.h
 * sys_time.h
 *
 * @attention 注意事项(attention)
 * fsm使用的链表，fam_tab使用的是数组
 */
#include "fsm.h"

/**
 * @file fsm_tab.h
 *
 * @note 功能注解
 * 提供一种统一状态进出规则处理的状态机功能。支持开发者定义多个状态机，每个状态机互相独立，
 * 状态机的状态节点可以灵活新增，每个状态节点的切换逻辑由开发者自行设计，x_frame只提供切换接口。
 *
 * @details 使用场景说明
 * 任意事件存在多个工作状态的情况下都可用fsm_tab。
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_FSM_TAB(id,param_len,init_state_id,item...)
 * :: RUN_FSM(id)
 * :: EMIT_FSM(id,event,pbuf,len)
 * :: FSM_OBJ(id)
 * :: EXT_FSM_OBJ(id)
 * :: fsm_fun(p_fsm)
 * :: fsm_emit_event(p_fsm,event,pbuf,len)
 *
 * @pre 依赖项列表
 * 无
 * @attention 注意事项(attention)
 */
#include "fsm_tab.h"

/**
 * @file param_cfg.h
 *
 * @note 功能注解
 * 参数配置用于设置触发信标志，查看当前数据变化，param_CFG每次获取数据需要手动读取获取最新数据
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_CFG_ITEM(var_name,var,type,attr,def,min,max)
 * :: REG_CFG_ITEM_CB(var_name,var,type,attr,def,min,max,cb_func)
 * :: REG_GUI_CFG_ITEM(var_name,var,type,attr,def,min,max,cb_func,gui_type)
 * :: REG_PARAM_CFG_LINK(param_cfg_link)
 *
 * @pre 依赖项列表
 * auto_init_mod.h
 * auto_reg_module.h
 * protocol_hal.h
 *
 * @attention 注意事项(attention)\
 * 函数指针类型指定为以下类型：
 * ● param_item_t *rd_param_item_name(char *p_item_name, float *val);
 * ● param_item_t *wr_param_item_name(char *p_item_name, float val);
 * ● param_item_t *rs_param_item_name(char *p_item_name);
 */
#include "param_cfg.h"

/**
 * @file protocol_push.h
 *
 * @note 功能注解
 * 推流模块
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_PROTOCOL_PUSH_GROUP(link_id,dest,rate)
 * :: REG_PROTOCOL_PUSH_ITEM(link_id,dest,def_interval,priority,push_cb)
 * :: REG_PROTOCOL_FORCE_PUSH_ITEM(link_id,dest,def_interval,priority,push_cb)
 * :: protocol_group_rate(link_id,dest)
 * :: protocol_group_use_ratio(link_id,dest)
 * :: protocol_reset_group_push(link_id,dest)
 * :: protocol_stop_group_push(link_id,dest)
 * :: protocol_start_group_push(link_id,dest)
 * :: protocol_stop_all_item_push(link_id,dest)
 * :: protocol_start_all_item_push(link_id,dest)
 * :: protocol_reset_all_item_push(link_id,dest)
 * :: protocol_reset_item_push(link_id,dest,p_push_ch)
 * :: protocol_stop_item_push(link_id,dest,p_push_ch)
 * :: protocol_start_item_push(link_id,dest,p_push_ch)
 * :: protocol_set_item_push_interval(link_id,dest,p_push_ch,push_interval)
 *
 * @pre 依赖项列表
 * tasks.h
 * auto_init_mod.h
 * auto_reg_module.h
 * sys_time.h
 *
 * @attention 注意事项(attention)
 */
#include "protocol_push.h"

/**
 * @file rec_def.h
 *
 * @note 功能注解
 * x_frame内部rec宏定义,根据不同的定义，选择不同的宏函数
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REC_START(name,rec_cmd_set,rec_cmd_id)
 * :: REC_ITEM(type,var)
 * :: REC_END()
 * :: REC_CACHE_START(name,rec_cmd_set,rec_cmd_id,cache_size)
 * :: REC_CACHE_ITEM(name,type,var)
 * :: REC_CACHE_END(name)
 *
 * @pre 依赖项列表
 * 无
 * @attention 注意事项(attention)
 */
#include "rec_def.h"

/**
 * @file rec_func_entity.h
 *
 * @note 功能注解
 * 数据记录主要用于绘制数据曲线和查看数据的实时变化，通过数据记录功能添加相关需要查看的数据，
 * 在dataview上可以查看到相应数据的曲线和数据。
 *
 * @details 使用场景说明
 * x-frame应用层任何需要通过dataview显示数据变化趋势的场景。
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_REC_LINK(link...)
 * :: EXT_REC(name)
 * :: RUN_REC(name)
 * :: RUN_REC_CUSTOM(name, _link, _dest)
 * :: CFG_REC_CUSTOM(name, _link, _dest)
 * :: EXT_REC_CACHE(name)
 * :: RUN_REC_CACHE(name)
 * :: RUN_REC_CACHE_CUSTOM(name, _link, _dest)
 * :: CFG_REC_CACHE_CUSTOM(name, _link, _dest)
 *
 * @pre 依赖项列表
 * tasks.h
 * auto_int_mod.h
 * sys_time.h
 * protocol_hal.h
 * @attention 注意事项(attention)
 */
#include "rec_func_entity.h"

/**
 * @file rpc.h
 *
 * @note 功能注解
 * 远程调用功能，可以通过ID和CMD规则从当前模块调用其它模块的能力，达到降低耦合的作用。
 * 同步处理一般就只是为了降低耦合而采用的方法。
 * 非阻塞的异步处理机制，有普通异步和延时时间异步。
 * 普通异步：异步处理时机不确定，最久一般是延时1ms后处理，由于异步处理的task优先级是ID_TASK（最低），
 *           而应用的task优先级都会比较高，所以一般都是在当前周期内就会处理该异步事件。
 * 延时异步处理：根据设置的延时时间进行异步处理。
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_RPC_ITEM(rpc_id,func,param_max_len)
 * :: rpc_call_sync(rpc_id,cmd,pbuf,len)
 * :: rpc_call_async(rpc_id,cmd,pbuf,len)
 * :: rpc_call_delay(ms,rpc_id,cmd,pbuf,len)
 *
 * @pre 依赖项列表
 * tasks.h
 * auto_init_mod.h
 * auto_reg_module.h
 * protocol_hal.h
 *
 * @attention 注意事项(attention)
 */
#include "rpc.h"

/**
 * @file shell.h
 *
 * @note 功能注解
 * 通过shell命令行实现调试命令的控制，包括参数的修改、打印日志等功能。功能比CFG（调试参数配置）丰富些。
 *
 * @details 使用场景说明
 * 在x-frame框架的应用层的任意需要shell命令行的地方都可以使用。
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_SHELL_CMD(name,maxargs,cmd,usage,detail)
 * :: REG_SHELL_CMD_X(name,maxargs,cmd,usage,detail)
 * :: REG_X_SHELL_LINK(link_id)
 * :: show_buf(pbuf,len)
 *
 * @pre 依赖项列表
 * tasks.h
 * auto_init_mod.h
 * auto_reg_module.h
 * protocol_hal.h
 *
 * @attention 注意事项(attention)
 * REG_SHELL_CMD和REG_SHELL_CMD_X的实现和功能是一样的，区别是REG_SHELL_CMD_X主要用于平台组开发使用。
 */
#include "shell.h"

/**
 * @file sys_time.h
 *
 * @note 功能注解
 * 系统时钟
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: RUN_XF_SYS_TIME(cnt)
 * :: DEF_XF_XH_SYS_ID(id)
 * :: SYS_MS_FUNC
 * :: TIME_INTERVAL_FUNC(cur_ms,last_ms)
 * :: RUN_BY_LIMIT_FUNC(ms, func)
 * :: RUN_BY_LIMIT_AFTER_FUNC(ms, func)
 * :: RUN_BY_LIMIT_BLOCK(ms, block)
 * :: RUN_BY_LIMIT_AFTER_BLOCK(ms, block)
 * :: uint32_t get_sys_ms(void);
 * :: uint32_t get_sys_s(void);
 * :: uint32_t get_sys_min(void);
 * :: uint64_t get_sys_work_time(void);
 * :: uint32_t get_elapse_time(uint32_t start_time);
 * :: void block_ms(uint32_t ms);
 * :: bool check_time_out(uint32_t start_time, uint32_t time_out);
 *
 * @pre 依赖项列表
 * HAL_CFG
 *  ├─ xh_core.h    (HAL_VER==X_HAL_XH_VER)
 *  └─ hal_core.h   (HAL_VER==X_HAL_CORE_VER)
 * @attention 注意事项(attention)
 */
#include "sys_time.h"

/**
 * @file tasks.h
 *
 * @note 功能注解
 * 任务模块
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_TASK(group,perio,task_name)
 * :: void task_sch(uint8_t group);
 * :: void current_task_delete(void);
 *
 * @pre 依赖项列表
 * auto_init_mod.h
 * auto_reg_module.h
 * sys_time.h
 *
 * @attention 注意事项(attention)
 */
#include "tasks.h"

/**
 * @file x_calendar.h
 *
 * @note 功能注解
 * 日历模块
 *
 * @details 使用场景说明
 * 需要使用的到日历的所有场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_UNIX_CALENDAR(_drv_rtc)
 * :: calendar_date_to_sec(p_date)
 * :: calendar_sec_to_date(p_date, sec)
 * :: set_calendar_date(p_date)
 * :: set_calendar_sec(sec)
 * :: get_calendar_date(p_date)
 * :: get_calendar_sec()
 * :: set_time_zone(zone)
 * :: get_time_zone()
 * :: calendar_date_cmp(p_date_1,p_date_2)
 *
 * @pre 依赖项列表
 * HAL_CFG
 *  ├─ xh_core.h    (HAL_VER==X_HAL_XH_VER)
 *  └─ hal_core.h   (HAL_VER==X_HAL_CORE_VER)
 * auto_init_mod.h
 *
 * @attention 注意事项(attention)
 * 当时间小于 1980 年 1 月 1 日的时候，会直接返回 0 时间
 */
#include "x_calendar.h"

/**
 * @file watth_statistics.h
 *
 * @note 功能注解
 * ● 统计规则说明
 *   ○ 能量统计时间以日历模块的0时为准，按照小时为单位，统计一天24小时使用的能量
 * ● 保存规则
 *   ○ 能量统计的保存依赖flash param模块，当时间翻转一天时，先读取内存中一周的能量统计数据，根据前一天的
 *     周日期，比如前一天是星期三，则将能量统计数据保存在星期三的数据位置，再将能量统计数据写入flash中保存。
 * ● 能量统计推送规则
 *   ○ 能量统计推送依赖protocol push模块，一次推送一个能量统计项一天的能量统计，首先完成当天能量统计推送，
 *     当完成推送日期所有能量统计项的推送，则开始下一天的能量统计推送，推送间隔1秒一次，直至完成一周的能量
 *     统计推送，当完成所有能量统计推送之后，修改推送间隔为30秒，持续推送当天的能量统计项
 *
 * @details 使用场景说明
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_WATTH_STATISTICS(watth_type,part_id)
 * :: REG_WATTH_STATISTICS_PUSH_ITEM(link,dest,priority)
 * :: REG_WATTH_STATISTICS_PUSH_ACK(link)
 * :: get_watth_statistics_all_watth(watt_type)
 * :: watth_statistics_save_all_watth()
 *
 * @pre 依赖项列表
 * tasks.h
 * auto_init_mod.h
 * auto_reg_module.h
 * x_calendar.h
 * protocol_push.h
 * flash_param.h
 * protocol_hal.h
 *
 * @attention 注意事项(attention)
 */
#include "watth_statistics.h"

/**
 * @file x_log.h
 *
 * @note 功能注解
 * ● 日志信息：
 *  ○ 日志产生的时间 - 用户无须填充
 *  ○ 日志事件编号
 *  ○ 日志产生时的四个浮点型参数，可少填或者不填
 * ● 日志存储功能：
 *  ○ 存储依赖flash param模块，使用flash param的环形队列存储功能进行存储，需要指定存储的flash区域以及
 *    存储的日志条数，存储的区域需要通过 REG_FLASH_PART 进行注册
 * ● 日志推送规则如下：
 *  ○ 推送时，先从flash读取最旧的日志，每次推送五条，收到应答后，推送后续的日志
 *  ○ 当推送完所有日志后，停止日志推送
 *  ○ 产生新的日志时，主动触发推送
 *
 * @details 使用场景说明
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_X_LOG_MOD(part_id,part_block_size,log_max_num)
 * :: REG_LOG_PUSH_ITEM(link,dest,priority)
 * :: REG_LOG_PUSH_ACK(link)
 * :: x_log_add(p_item)
 * :: x_log_read(p_log_arry, index, num)
 *
 * @pre 依赖项列表
 * auto_init_mod.h
 * auto_reg_module.h
 * x_calendar.h
 * protocol_push.h
 * flash_param.h
 * protocol_hal.h
 * sys_time.h
 *
 * @attention 注意事项(attention)
 */
#include "x_log.h"


/**
 * @file xh_core.h
 *
 * @note 功能注解
 * 驱动抽象层和驱动初始化模块
 *
 * @details 使用场景说明
 * 使用场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: PTR_TYPE_XH_DEV(id)
 * :: EXT_XH_DEV(var_type, id)
 * :: REG_XH_DEV(var_type, id, var, prio, auto_init, init, deinit, custom...)
 * :: void xh_core_deinit(void);
 * :: static inline void xh_init(void *const id);
 * :: static inline void xh_deinit(void *const id);
 *
 * @pre 依赖项列表
 * 无
 *
 * @attention 注意事项(attention)
 */
#include "xh_core.h"


/**
 * @file xf_debug.h
 *
 * @note 功能注解
 * DBG用于打印调试信息，支持字符串打印，也支持带协议格式的信息打印，一般使用带协议格式的调试信息打印，
 * 带调试功能的协议打印信息可以通过dataview的log模块进行查看
 *
 * @details 使用场景说明
 * 用于打印调试信息，通过dataview的log模块进行查看
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: REG_DEBUG(dev_id)
 * :: REG_DEBUG_LINK(link...)
 * :: DBG_I(fmt,args...)
 * :: DBG_I_LIMIT(ms,fmt,args...)
 * :: DBG_I_BLOCK(ms,fmt,args...)
 * :: DBG_W(fmt,args...)
 * :: DBG_W_LIMIT(ms,fmt,args...)
 * :: DBG_W_BLOCK(ms,fmt,args...)
 * :: DBG_E(fmt,args...)
 * :: DBG_E_LIMIT(ms,fmt,args...)
 * :: DBG_E_BLOCK(ms,fmt,args...)
 *
 * @pre 依赖项列表
 *  protocol_hal.h
 *
 * @attention 注意事项(attention)
 */
#include "xf_debug.h"

/**
 * @file xf_bbox.h
 *
 * @note 功能注解
 * ● xf_bbox(黑匣子)是一个具有 flash 存储和读取功能的模块。
 * ● dataview回读黑匣子数据，采用通用指令集，支持三个命令交互：
 *   ○ BMS_BBOX_DATA_READ_ID：设置需要读取的数据类型、页大小、向前的页数、上报间隔
 *   ○ BMS_BBOX_DATA_RE_READ_ID：设置需要读取的数据类型、索引值
 *   ○ BMS_EXFLASH_CMD_ID：设置停止数据上报、停止数据上报及擦除常规数据
 *
 * @details 使用场景说明
 * 需要将数据存储到flash中，并能读取出来的所有场景
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 * :: int32_t real_bbox_upload_param_set(hal_frame_info_t* p_frame_info);
 * :: int32_t real_bbox_record_replenish_data_act(hal_frame_info_t* p_frame_info);
 * :: int32_t real_bbox_record_cmd_act(hal_frame_info_t* p_frame_info);
 * :: void bbox_state_set(uint16_t name, uint16_t type, XF_BBOX_STATE_E state);
 * :: void bbox_data_record(void);
 * :: void bbox_recorded_data_save(uint16_t name, uint16_t type);
 *
 * @pre 依赖项列表
 * auto_reg_module.h
 * REC 模块
 *   rec_def.h
 *   rec_func_entity.h
 * flash_part.h
 * tasks.h
 * sys_time.h
 * auto_int_mod.h
 * protocol_hal.h
 * x_celeendar.h
 *
 * @attention 注意事项(attention)
 */
#include "xf_bbox.h"

/**
 * @file xf_protobuf.h
 *
 * @note: protobuf 适配x_frame 框架的相关接口
 *
 * @details: 使用protobuf协议的相关功能时需使用
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 *
 * :: DECODE_PROTOBUF(proto_name, de_ptr, de_len, proto_struct, ret)
 *
 * :: ENCODE_PROTOBUF(proto_name, proto_struct, buff, buff_size, en_len)
 *
 * :: REG_LINK_PROTOBUF_ACTION(proto_name, link_id, cmd_set, cmd_id, func)
 *
 * :: REG_LINK_PROTOBUF_ACTION_CMD(proto_name, link_id, cmd_set, cmd_id, func)
 *
 * :: REG_LINK_PROTOBUF_ACTION_ACK(proto_name, link_id, cmd_set, cmd_id, func)
 *
 * :: REG_CMDSET_PROTOBUF_ACTION(proto_name, cmd_set, cmd_id, func)
 *
 * :: REG_CMDSET_PROTOBUF_ACTION_CMD(proto_name, cmd_set, cmd_id, func)
 *
 * :: REG_CMDSET_PROTOBUF_ACTION_ACK(proto_name, cmd_set, cmd_id, func)
 *
 * :: PROTOBUF_SEND_BY_ID(p_frame_info, proto_name, send_len)
 *
 * :: PROTOBUF_SEND_FRAME(p_frame_info, proto_name, send_len)
 *
 * :: PROTOBUF_SEND(p_frame_info, proto_name, send_len)
 *
 * :: PROTOBUF_SEND_ACK(p_frame_info, proto_name, proto_struct, send_len)
 *
 * @pre 依赖项列表
 * protocol_hal.h
 * pb.h
 * pb_common.h
 * pb_decode.h
 * pb_encode.h
 * @attention 注意事项(attention)
 */
#include "xf_protobuf.h"

/**
 * @file xf_protobuf_opt.h
 *
 * @note protobuf 增量上传功能
 *
 * @details 对指定的变量或者消息按照一定的时间间隔，or 步长等条件进行增量上报，
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 *
 * :: REG_PROTOBUF_OPT_RECE_SRC(proto_name, cmd_func, cmd_id, link_id)
 *
 * :: DEF_PROTOBUF_OPT_RECE_START(proto_name)
 *
 * :: DEF_PROTOBUF_OPT_RECE_ITEM(proto_var, has_proto_var, user_var)
 *
 * :: DEF_PROTOBUF_OPT_RECE_CB_ITEM(proto_var, has_proto_var, user_var, callback)
 *
 * :: DEF_PROTOBUF_OPT_RECE_GROUP_ITEM(proto_var, has_proto_var, user_var, user_len)
 *
 * :: DEF_PROTOBUF_OPT_RECE_CB_GROUP_ITEM(proto_var, has_proto_var, user_var, user_len, callback)
 *
 * :: DEF_PROTOBUF_OPT_RECE_END()
 *
 * :: REG_PROTOBUF_OPT_SEND_DEST(proto_name, _cmd_func, _cmd_id, _link_id, _dest...)
 *
 * :: DEF_PROTOBUF_OPT_SEND_START(proto_name)
 *
 * :: DEF_PROTOBUF_OPT_SEND_ITEM(proto_var, has_proto_var, user_var)
 *
 * :: DEF_PROTOBUF_OPT_SEND_STEP_ITEM(proto_var, has_proto_var, user_var, step)
 *
 * :: DEF_PROTOBUF_OPT_SEND_TIME_ITEM(proto_var, has_proto_var, user_var, timeout)
 *
 * :: DEF_PROTOBUF_OPT_SEND_TIME_STEP_ITEM(proto_var, has_proto_var, user_var, timeout, step)
 *
 * :: DEF_PROTOBUF_OPT_SEND_GROUP_ITEM(proto_var, has_proto_var, user_var, user_len)
 *
 * :: DEF_PROTOBUF_OPT_SEND_TIME_GROUP_ITEM(proto_var, has_proto_var, user_var, user_len, timeout)
 *
 * :: DEF_PROTOBUF_OPT_SEND_END()
 *
 * @pre
 * xf_protobuf.h
 *
 */
#include "xf_protobuf_opt.h"



/**
 * @file xf_unix_time_update.h
 *
 * @note 主从机制时间同步功能
 *
 * @details 1、从指定模块获取unix time并向所有从机模块同步 2、主机上电后同步一次 3、主机定时30s向动态从机发送同步数据
 *
 * @par 接口列表：(接口功能详细说明，请在文件中查看)
 *
 * :: REG_UNIX_TIME_FUNC_REPLICA_LINK(link...)
 *
 * :: REG_UNIX_TIME_FUNC_REPLICA_INFO(info_link,info_dest,item...)
 *
 * :: UNIX_TIME_REPLICA_ITEM(link_id,dest,is_dynamic)
 *
 *
 * @pre 依赖项列表
 * protocol_hal.h
 * x_calendar.h
 * xf_replica_solution.h
 * sys_time.h
 *
 */
#include "xf_unix_time_update.h"

/*
** 使用说明详见: https://ecoflow.feishu.cn/wiki/wikcnqCS5QIoYamX4MWAmpqQpKf 《x_frame使用手册》
** 源码仓库：http://rdgit.efsz.com:8070/projects/GES/repos/x_frame/browse
** lib仓库：http://rdgit.efsz.com:8070/projects/GES/repos/x_frame_lib/browse
*/

/*************************************************************
    BASE
*************************************************************/

#define X_FRAME_COMMIT_ID "7e7ce1177fae6a620a85d585eb1731c6f5fd8e91"
#define X_FRAME_SRF_VER "1.0.0.27"

#define X_FRAME_NAME "ECOFLOW X_FRAME"
#define X_FRAME_VER "3.7.9.2"
#define XFRAME_MAIN_VER 3 /* 软件架构重构版本 */
#define XFRAME_SUB_VER 7  /* 软件不兼容版本 */
#define XFRAME_DBG_VER 9  /* 软件新增功能，bug修复 */
#define XFRAME_TEST_VER 2 /* 驱动接口版本号 */

#define ERR_DRV_ID 0xff

/**
 * @brief 定义看门狗驱动ID
 *
 * @param [in] id 需要定义的看门狗驱动id
 *
 * @details:
 * @par e.g:
 * @code
 *      #define WATCH_DOG_ID 1    //看门狗驱动id
 *
 *      DEF_XF_XH_IWDG_ID(WATCH_DOG_ID);
 * @endcode
 */
#define DEF_XF_XH_IWDG_ID(id) void *const gx_xh_iwdg_id = PTR_TYPE_XH_DEV(id);

extern uint8_t x_frame_host_addr;
extern uint8_t x_frame_pc_addr;
extern uint32_t g_product_type;
extern uint32_t g_product_detail;
extern void *const gx_xh_iwdg_id;

extern const uint8_t x_frame_rec_dat_enable;

/*************************************************************
    END
*************************************************************/
#endif
