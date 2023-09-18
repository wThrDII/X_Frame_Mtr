/********************** File Info ***************************
** File name:               can_s.h
** Latest modified date:    2021-11-21
** Latest version:          V1.0.0
** Description:             can总线 - 字符流
**
**------------------------------------------------------------
** Created by:              Chen JunTong
** Created date:            2021-11-21
** Version:                 V1.0.0
** Descriptions:            can总线 - 字符流
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#ifndef can_s_h
#define can_s_h

#include "x_frame.h"
#include "xh_can_s.h"
#include "xh_can.h"
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "string.h"

#define CAN_S_VER                   1           /* 程序版本 */
#define CAN_S_CAST_D_ADDR           0           /* 广播动态地址 */

#define CAN_S_PACK_DATA_LEN         8           /* 帧数据长度 */

#define CAN_S_CAST_INTE_TIME        100         /* 广播间隔时间 */
#define CAN_S_CAST_FINI_TIME        2000        /* 广播结束时间 */
#define CAN_S_HEAR_TIME             100         /* 心跳时间 */
#define CAN_S_MAST_TIME             1000        /* 主机竞争时间 */
#define CAN_S_DEV_RM_TIME           60000		/* 设备移除时间 */

typedef struct _CAN_S_CACHE_OBJ CAN_S_CACHE_OBJ ,*P_CAN_S_CACHE_OBJ;

typedef enum {
    CAN_S_NORM_PACK = 0,        /* 正常帧 */
    CAN_S_HEAR_PACK,            /* 心跳帧 */
    CAN_S_CAST_PACK,            /* 广播帧 */
    CAN_S_ADDR_PACK,            /* 地址帧 */
}CAN_S_PACK_TYPE;

typedef enum {
    CAN_S_IDLE_STA = 0,         /* 空闲态 */
    CAN_S_CAST_STA,             /* 广播态 */
    CAN_S_CONN_STA,             /* 连接态 */
}CAN_S_DEV_STA;

typedef enum {
    CAN_S_PACK_HEAD = 0,        /* 帧头 */
    CAN_S_PACK_BODY,            /* 帧身 */
    CAN_S_PACK_FOOT,            /* 帧尾 */
    CAN_S_PACK_COMP,            /* 完整帧 */
}CAN_S_PACK_PROC;


typedef struct _CAN_S_ID_OBJ {
    uint32_t d_addr:12;         /* 动态地址 */
    uint32_t s_addr:8;          /* 静态地址 */
    uint32_t proc:2;            /* 0：帧头，1：帧身，2，帧尾，3：完整帧 */
    uint32_t type:2;            /* 0 数据帧，1 心跳帧，2 广播帧，3 地址帧 */
    uint32_t reserved:4;        /* 预留字节 */
    uint32_t ver:1;
}CAN_S_ID_OBJ ,*P_CAN_S_ID_OBJ;


typedef struct _CAN_S_PACK_OBJ {
    CAN_S_ID_OBJ id;
    uint8_t len;
    uint8_t data[CAN_S_PACK_DATA_LEN];
}CAN_S_PACK_OBJ ,*P_CAN_S_PACK_OBJ;

struct _CAN_S_CACHE_OBJ {
    CAN_S_PACK_OBJ pack;
    uint32_t rece_time;
    P_CAN_S_CACHE_OBJ p_head_list;
    P_CAN_S_CACHE_OBJ p_son_list;
};

typedef struct _CAN_S_TIME_OBJ {
    uint32_t start_time;
    uint32_t send_time;
}CAN_S_TIME_OBJ ,*P_CAN_S_TIME_OBJ;

typedef struct _CAN_S_OBJ {
    void *const sys_drv_id;
    void *const can_drv_id;
    bool single_module;                 /* 单模块 */
    uint8_t dev_s_addr;                 /* 设备静态地址 */
    P_CAN_S_CACHE_OBJ p_r_cache;
    uint16_t r_cache_num;
    uint32_t r_timeout;
    struct{
        CAN_S_DEV_STA dev_sta;                             /* 上线标记 */
        uint8_t dev_d_addr;                                /* 设备动态地址 */
        uint8_t uniquely_id[CAN_S_PACK_DATA_LEN];          /* 唯一id */
        bool uniquely_id_mark;
        uint32_t free_cache;
        struct{
            uint32_t start_time;                            /* 广播起始时间 */
            uint32_t send_time;                             /* 发送广播时间 */
            uint8_t set_addr_mark;                          /* 地址配置标记 */
        }cast;

        struct{
            uint32_t hear_send_time;                        /* 心跳时间 */
            uint32_t mast_wait_time;                        /* 主机等待时间 */
            uint8_t  wait_mark;                             /* 等待标记 */
            uint32_t mast_conn_time;                        /* 主机连接时间 */
            uint8_t  host_mark;                             /* 主机标记 */
            uint8_t  conflict_mark;                         /* 冲突标记 */
        }conn;

        CAN_S_CACHE_OBJ norm_list;
        CAN_S_CACHE_OBJ hear_list;
        CAN_S_CACHE_OBJ cast_list;
        CAN_S_CACHE_OBJ addr_list;
        CAN_S_CACHE_OBJ empt_list;
        CAN_S_CACHE_OBJ dev_list;                           /* 当前设备，同类型放一个son_list */

        /* 临时解决缓存丢失问题 */
        uint32_t cache_reset_time;
        uint8_t reset_mark;
        uint32_t conflict_timestamp;                        //冲突时间戳
    }exec;
}CAN_S_OBJ ,*P_CAN_S_OBJ;

#define REG_CAN_S_DRV(id,prio,sys_drv_id_,can_drv_id_,single_module,dev_s_addr,r_timeout,r_cache_num)               \
        CAN_S_CACHE_OBJ g_can_s_r_cache_##id[r_cache_num];                                                          \
        CAN_S_OBJ g_can_s_##id = {                                                                                  \
            PTR_TYPE_XH_DEV(sys_drv_id_),                                                                           \
            PTR_TYPE_XH_DEV(can_drv_id_),                                                                           \
            single_module,                                                                                          \
            dev_s_addr,                                                                                             \
            g_can_s_r_cache_##id,                                                                                   \
            r_cache_num,                                                                                            \
            r_timeout,                                                                                              \
        };                                                                                                          \
        void can_s_rece_pack_##id(uint32_t pack_type, uint32_t pack_id, uint8_t *p_pack, uint32_t pack_len)         \
        {                                                                                                           \
            can_s_rece_pack(&g_can_s_##id,pack_type,pack_id,p_pack,pack_len);                                       \
        }                                                                                                           \
        void can_s_init_##id(P_CAN_S_OBJ p_obj)                                                                     \
        {                                                                                                           \
            can_s_init(p_obj);                                                                                      \
            xh_can_reg_rx_irq_cb(p_obj->can_drv_id,can_s_rece_pack_##id);                                           \
        }                                                                                                           \
        void can_s_task_##id(void){can_s_task(&g_can_s_##id);} REG_TASK(VH_TASK, 1, can_s_task_##id);               \
        REG_XH_CAN_S_DRV(id,&g_can_s_##id,prio,DEV_AUTO_INIT,can_s_init_##id,NULL,                                  \
                            can_s_read_data,                                                                        \
                            can_s_send_data,                                                                        \
                            can_s_set_baudrate,                                                                     \
                            can_s_get_d_addr,                                                                       \
                            can_s_get_host_mark,                                                                    \
                            can_s_get_dev_num,                                                                      \
                            can_s_get_dev_online,                                                                   \
                            can_s_get_free_cache,                                                                   \
                            can_s_get_dev_total,                                                                    \
                            can_s_set_dev_s_addr                                                                    \
        )

extern void can_s_init(P_CAN_S_OBJ p_obj);
extern void can_s_task(P_CAN_S_OBJ p_obj);

/* 驱动层数据包发送和接收can帧接口 */
extern void can_s_rece_pack(P_CAN_S_OBJ p_obj, uint32_t pack_type, uint32_t pack_id, uint8_t *p_pack, uint32_t pack_len);

/* 应用层发送和读取字符流数据接口 */
extern uint32_t can_s_send_data(P_CAN_S_OBJ p_obj, uint8_t *p_data,uint32_t len);
extern uint32_t can_s_read_data(P_CAN_S_OBJ p_obj, uint8_t *p_data,uint32_t len);
extern uint32_t can_s_set_baudrate(P_CAN_S_OBJ p_obj, uint32_t baudrate);
/* 获取状态接口 */
extern uint32_t can_s_get_d_addr(P_CAN_S_OBJ p_obj);
extern bool can_s_get_host_mark(P_CAN_S_OBJ p_obj);
extern uint32_t can_s_get_dev_num(P_CAN_S_OBJ p_obj,uint32_t dev_s_addr);
extern bool can_s_get_dev_online(P_CAN_S_OBJ p_obj,uint32_t dev_s_addr,uint32_t dev_d_addr);
extern uint32_t can_s_get_free_cache(P_CAN_S_OBJ p_obj);
extern uint32_t can_s_get_dev_total(P_CAN_S_OBJ p_obj);
extern void can_s_set_dev_s_addr(P_CAN_S_OBJ p_obj,uint32_t s_addr);
#endif /* can_s_h */



