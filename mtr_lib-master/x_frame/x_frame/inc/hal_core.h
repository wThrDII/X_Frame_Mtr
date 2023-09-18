/**
 * @brief hal_core.c.xtemp header file
 * @file hal_core.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 15:38:29
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 15:38:29
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __HAL_CORE_H__
#define __HAL_CORE_H__


#define DEV_FLAG_REG    (0x1<<0)
#define DEV_FLAG_INIT   (0x1<<1)
#define DEV_READY       (DEV_FLAG_REG|DEV_FLAG_INIT)

typedef int32_t (*pf_dev_init)		 (void);
typedef int32_t (*pf_dev_write)	     (uint8_t *pSrc     , uint32_t len  , uint32_t offset, uint32_t *real_w_bytes);
typedef int32_t (*pf_dev_read)		 (uint8_t *pDest    , uint32_t len  , uint32_t offset, uint32_t *real_r_bytes);
typedef int32_t (*pf_dev_ctrl)		 (uint32_t ctrl_cmd , uint8_t *pDest, uint32_t len   , uint32_t *real_c_bytes);
typedef int32_t (*pf_dev_close)	     (void);
typedef int32_t (*pf_dev_push)       (uint32_t code, uint8_t *pbuf, uint32_t len);

struct  _dev_push_t
{
    const char     *cb_func_name;
    uint32_t        dev_id;
    pf_dev_push     cb_func;
};

struct _dev_obj_
{
    const char 	*dev_name;
    uint32_t		dev_id;           

    uint32_t        is_auto_init;

    pf_dev_init		dev_init;
    pf_dev_write	dev_write;
    pf_dev_read		dev_read;
    pf_dev_ctrl		dev_ctrl;
    pf_dev_close	dev_close;
    struct _dev_push_t     *p_dev_push_obj;
    uint32_t		dev_status;
    uint32_t        dev_flag;        
};



typedef struct _dev_obj_ dev_obj_t;
typedef struct _dev_push_t dev_push_t;

#define DEV_NOT_INIT   	(0x00)
#define DEV_AUTO_INIT   (0x01)

#define REG_DEV(id,is_auto_init,init,write,read,ctrl,close) \
			static dev_obj_t device_##id = {#id,id,is_auto_init,init,write,read,ctrl,close,NULL,0,0};\
			AUTO_REG_ITEM(DRIVER_##id      ,AUTO_REG_TYPE_DRIVER           ,(void*)&device_##id);
					
#define REG_DEV_CB(dev_id,func)     \
			dev_push_t    dev_cb_##func  = {#func,dev_id,func};\
			AUTO_REG_ITEM(CB_##dev_id##func,AUTO_REG_TYPE_DRIVER_PUSH,(void*)& dev_cb_##func);


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

void hal_core_init(void);
uint32_t hal_init_driver(uint32_t dirver_id);
int32_t hal_app_write(uint32_t dev_handle, uint8_t *pbuf, uint32_t len, uint32_t offset, uint32_t *p_real_bytes);		/* 驱动写入函数 */
int32_t hal_app_read(uint32_t dev_handle, uint8_t *pbuf, uint32_t len, uint32_t offset, uint32_t *p_real_bytes);		/* 驱动读取函数 */
int32_t hal_app_ctrl(uint32_t dev_handle, uint32_t ctrl_cmd, uint8_t *pDest, uint32_t len, uint32_t *p_real_bytes);		/* 驱动控制函数 */
void hal_dev_notify(uint32_t dev_id, uint32_t code, uint8_t *pbuf, uint32_t len);										/* 驱动注册回调 */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __HAL_CORE_H__
