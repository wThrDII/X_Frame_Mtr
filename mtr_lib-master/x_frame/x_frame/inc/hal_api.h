/********************** Copyright(c)***************************
** ECOFLOW 
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** Created by:              Xiao ShuLong
** Created date:            2022-04-12
** Version:                 V1.0.0
** Descriptions:            全部api接口
**
**------File Info----------------------------------------------
** File name:               hal_api.h
** Latest modified date:    2022-04-12
** Latest version:          V1.0.0
** Description:             全部api接口
**
*************************************************************/

#ifndef hal_api_h
#define hal_api_h
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
/*
** Descriptions:        驱动 bsp 初始化
** Remarks:             使用库前必须先调用
*/
extern void hal_bsp_init(void);

/*************************************************************
    SYS
*************************************************************/

/*
** Descriptions:        系统复位
*/
extern void sys_reset(void);

/*
** Descriptions:        跳转到指定程序地址
** Input parameters:    addr：需要跳转的地址
*/
extern void sys_jump_addr(uint32_t addr);

/*
** Descriptions:        获取cpu id
** Input parameters:    p_data: 存放 cpuid 的指针
**                      len：p_data指针最大长度，与cpuid实际长度相比较，取其中最小值，返回实际获取长度
** Returned value:      实际获取长度
*/
extern uint32_t sys_get_cpuid(uint8_t *p_data, uint32_t len);

/*
** Descriptions:        获取BSP库版本号
** Returned value:      BSP库版本号
*/
extern uint32_t sys_get_bsp_ver(void);
/*************************************************************
    WATCHDOG
*************************************************************/

/*
** Descriptions:        喂狗
*/
extern void watchdog_feed(uint32_t watchdog_id);

/*
** Descriptions:        获取看门狗复位状态
** Returned value:      false: 未复位，true 复位
*/
extern bool watchdog_get_reset_sta(uint32_t watchdog_id);

/*************************************************************
    IRQ
*************************************************************/

/*
** Descriptions:        注册中断回调
** Returned value:      irq_id:需要注册的中断
                        cb：回调函数
*/
extern void irq_reg_cb_init(uint32_t irq_id,void (*cb)(void));

/*
** Descriptions:        更改中断频率
** Input parameters:    irq_id: FREQ_IRQ
                        freq：频率
*/
extern void irq_set_freq(uint32_t irq_id,uint32_t freq);
/*************************************************************
    IO
*************************************************************/

/* W API */
/*
** Descriptions:        设置io状态
** Input parameters:    sta：状态，true or false
*/
extern void io_set_sta(uint32_t io_id,bool sta);

/*
** Descriptions:        开始模式
** Input parameters:    repeat_num：动作循环次数
**						fir_sta：第一个动作状态，true or false
**						p_action：执行的动作，示例: "100,100"
*/
extern void io_action_cfg(uint32_t io_id,uint16_t repeat_num,bool fir_sta,char *p_action);

/* R API */
/*
** Descriptions:        获取io状态
** Returned value:      状态，true or false
*/
extern bool io_get_sta(uint32_t io_id);

/*************************************************************
    CHA 
*************************************************************/

/*
** Descriptions:        数据写入
** Input parameters:    p_data：数据指针
**						len：写入数据长度
** Returned value:		实际写入长度						
*/
extern uint32_t stream_write(uint32_t cha_id,uint8_t *p_data,uint32_t len);

/*
** Descriptions:        数据读取
** Input parameters:    p_data：数据指针
**						len：读取数据的最大个数
** Returned value:		实际读取长度				
*/
extern uint32_t stream_read(uint32_t cha_id,uint8_t *p_data,uint32_t len);

/*************************************************************
    FLASH
*************************************************************/

/*
** Descriptions:        擦除
** Input parameters:    addr：地址
**						len：擦除长度
** Returned value:		true 成功 false 失败
** Remarks:             擦除是按照扇区大小进行的，以addr所在的起始扇区开始，addr + len的结束扇区中止
*/
extern bool flash_erase(uint32_t flash_id,uint32_t addr,uint32_t len);

/*
** Descriptions:        写入
** Input parameters:    addr：地址
**						p_data：数据指针
**						len：长度
** Returned value:		true 成功 false 失败	
*/
extern bool flash_write(uint32_t flash_id,uint32_t addr,uint8_t *p_data,uint32_t len);

/*
** Descriptions:        读取
** Input parameters:    addr：地址
**						p_data：数据指针
**						len：长度
** Returned value:		true 成功 false 失败
*/
extern bool flash_read(uint32_t flash_id,uint32_t addr,uint8_t *p_data,uint32_t len);

/*************************************************************
    AD
*************************************************************/


/*
** Descriptions:        读取原始值
** Returned value:		原始值
*/
extern uint16_t ad_read_raw_val(uint32_t dev_id);

/*
** Descriptions:        读取多个AD原始值
** Input parameters:    p_buf：缓存地址
                        start：起始的AD_ID (AD_SNS_IL1 和 AD_SNS_IL2  为注入通道不能通过这个读取)
                        num：个数
*/
extern void ad_read_multi_val(uint16_t *p_buf , uint32_t start , uint8_t num);

/*
** Descriptions:        更改规则通道采样数据缓存地址
** Input parameters:    buf：缓存地址
** Remarks:             调用一次
                        缓存大小不能小于(规则通道数量*2)
*/
extern void ad_set_regular_dma_init(uint16_t *buf);
/*************************************************************
    TEMP
*************************************************************/

/*
** Descriptions:        获取温度值
** Returned value:		温度值，单位：度
*/
extern int32_t temp_get_val(uint32_t temp_id);

/*************************************************************
    AC
*************************************************************/

/*
** Descriptions:        获取电压有效值
** Returned value:		电压值，单位: mV
*/
extern float ac_get_vol_val(uint32_t vol_id);

/*
** Descriptions:        获取电流有效值
** Returned value:		电流值，单位: mA
*/
extern float ac_get_cur_val(uint32_t cur_id);

/*
** Descriptions:        获取视在功率
** Returned value:		功率，单位：mwatt
*/
extern float ac_get_s_watt_val(uint32_t watt_id);

/*
** Descriptions:        获取有功功率
** Returned value:		功率，单位：mwatt
*/
extern float ac_get_p_watt_val(uint32_t watt_id);

/*
** Descriptions:        获取无功功率
** Returned value:		功率，单位：mwatt
*/
extern float ac_get_q_watt_val(uint32_t watt_id);



/*************************************************************
    PWM
*************************************************************/

/*
** Descriptions:        设置PWM的占空比
** Input parameters:    pwm_id：FAN_CONTROL_PWM
                        duty_cycle：占空比（0~10000）
*/
extern void pwm_set_duty_cycle(uint32_t pwm_id,uint16_t duty_cycle);

/*
** Descriptions:        开启或者关闭定时器
** Input parameters:    en：true 开启
                            false 关闭
*/
extern void pwm_timer_en(uint32_t pwm_id,uint8_t en);

/*************************************************************
    BUCK BOOST PWM
*************************************************************/

/*
** Descriptions:        设置BUCK BOOST占空比
** Input parameters:    buck_boost_id：UP_BUCK_PWM、UP_BOOST_PWM
                                        DN_BUCK_PWM、DN_BOOST_PWM
                        duty_cycle：占空比（0~10000）
*/
extern void buck_boost_set_dutycycle(uint32_t buck_boost_id,uint16_t duty_cycle);

/*
** Descriptions:        设置死区时间
** Input parameters:    deadtime：时间，单位us
** Remarks:             上路和下路一起设置
*/
extern void buck_boost_set_deadtime(float deadtime);

/*
** Descriptions:        开启或者关闭定时器
** Input parameters:    en：true 开启
                            false 关闭
** Remarks:             为主从定时器会同时开始或者关闭
*/
extern void buck_boost_timer_en(uint8_t en);

/*
** Descriptions:        设置互补通道开关
** Input parameters:    state：OPEN_COMPLEMENTARY、CLOSE_COMPLEMENTARY、CLOSE_ALL_CH
*/
extern void buck_boost_set_ch_sta(uint32_t buck_boost_id,uint32_t state);


#ifdef __cplusplus
}
#endif
#endif /* hal_api_h */



