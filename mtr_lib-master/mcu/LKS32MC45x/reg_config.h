#ifndef reg_config_h
#define reg_config_h
#ifdef __cplusplus
extern "C" {
#endif
//GD32F30x 定时器寄存器地址操作接口
#define BITBAND(addr,bitnum) ((addr & 0xF0000000)+0x02000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile uint32_t  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
/**************************************************************************************************/
#define M_TIMER0                           (0x40012C00U)
#define M_TIMER1                           (0x40000000U)
#define M_TIMER2                           (0x40000400U)
#define M_TIMER3                           (0x40000800U)
#define M_TIMER4                           (0x40000C00U)
#define M_TIMER5                           (0x40001000U)
#define M_TIMER6                           (0x40001400U)
#define M_TIMER7                           (0x40013400U)
#define M_TIMER8                           (0x40014C00U)
#define M_TIMER9                           (0x40015000U)
#define M_TIMER10                          (0x40015400U)
#define M_TIMER11                          (0x40001800U)
#define M_TIMER12                          (0x40001C00U)
#define M_TIMER13                          (0x40002000U)
#define M_TIMER_CCHP(timerx)               (*(volatile uint32_t *)(uint32_t)(((timerx) + 0x44U)))
/**************************************************************************************************/
/*计数最大值*/
#define M_TIMER_CAR(timerx)                (*(volatile uint32_t *)(uint32_t)(((timerx) + 0x2CU)))
/*通道比较值*/
#define M_TIMER_CH0CV(timerx)              (*(volatile uint32_t *)(uint32_t)(((timerx) + 0x34U)))
#define M_TIMER_CH1CV(timerx)              (*(volatile uint32_t *)(uint32_t)(((timerx) + 0x38U)))
#define M_TIMER_CH2CV(timerx)              (*(volatile uint32_t *)(uint32_t)(((timerx) + 0x3CU)))
#define M_TIMER_CH3CV(timerx)              (*(volatile uint32_t *)(uint32_t)(((timerx) + 0x40U)))
/*主通道状态*/
#define M_TIMER_CH0_STA(timerx)              BIT_ADDR(((timerx) + 0x20U),0)
#define M_TIMER_CH1_STA(timerx)              BIT_ADDR(((timerx) + 0x20U),4)
#define M_TIMER_CH2_STA(timerx)              BIT_ADDR(((timerx) + 0x20U),8)
#define M_TIMER_CH3_STA(timerx)              BIT_ADDR(((timerx) + 0x20U),12)
/*互补通道状态*/
#define M_TIMER_CHN0_STA(timerx)              BIT_ADDR(((timerx) + 0x20U),2)
#define M_TIMER_CHN1_STA(timerx)              BIT_ADDR(((timerx) + 0x20U),6)
#define M_TIMER_CHN2_STA(timerx)              BIT_ADDR(((timerx) + 0x20U),10)
/*所有通道状态*/
#define M_TIMER_POEN(timerx)                  BIT_ADDR(((timerx) + 0x44U),15)
/*死区时间*/
#define M_TIMER_DEAD(timerx,val)             M_TIMER_CCHP(timerx) &= ~(0xFF);M_TIMER_CCHP(timerx) |= val;
/******************IO**********************************/
#define M_GPIOA                           ((uint32_t)0x40010800U)
#define M_GPIOB                           ((uint32_t)0x40010C00U)
#define M_GPIOC                           ((uint32_t)0x40011000U)
#define M_GPIOD                           ((uint32_t)0x40011400U)
#define M_GPIOE                           ((uint32_t)0x40011800U)
#define M_GPIOF                           ((uint32_t)0x40011C00U)
#define M_GPIOG                           ((uint32_t)0x40012000U)
#define M_GPIO_OUT(gpiox)                 ((uint32_t)(((gpiox) + 0x0CU)))
#define M_GPIO_STA(gpiox)                 ((uint32_t)(((gpiox) + 0x08U)))
/**************************************************************************************************/
/*IO设置电平*/
#define M_IO_SET(gpiox,pin)               BIT_ADDR((M_GPIO_OUT(gpiox)),pin)
/*IO读取电平*/
#define M_IO_GET(gpiox,pin)               BIT_ADDR((M_GPIO_STA(gpiox)),pin)
/*******************************************************************************************************
reg_config.h使用说明
一、更改频率
M_TIMER_CAR(timerx)
例子：
1.修改TIMER0 频率为50000
    M_TIMER_CAR(M_TIMER0) = 120000000 / 50000;
*如果定时器为中央对齐方式，要获得50000频率的PWM：
    M_TIMER_CAR(M_TIMER0) = 120000000 / 100000;

2.修改TIMER1 频率为50000
    M_TIMER_CAR(M_TIMER1) = 120000000 / 50000;
*如果定时器为中央对齐方式，要获得50000频率的PWM：
    M_TIMER_CAR(M_TIMER0) = 120000000 / 100000;

二、修改占空比
M_TIMER_CH0CV(timerx)、M_TIMER_CH1CV(timerx)、M_TIMER_CH2CV(timerx)、M_TIMER_CH3CV(timerx)
例子：
1.需要修改 TIMER0的 通道1 为 40% 占空比
    M_TIMER_CH1CV(M_TIMER0) = M_TIMER_CAR(M_TIMER0) * 4000 / 10000;
2.需要修改 TIMER1的 通道0 为 26% 占空比
    M_TIMER_CH0CV(M_TIMER1) = M_TIMER_CAR(M_TIMER1) * 2600 / 10000;

三、设置通道开关
M_TIMER_CH0_STA(timerx)、M_TIMER_CH1_STA(timerx)、M_TIMER_CH2_STA(timerx)、M_TIMER_CH3_STA(timerx)
M_TIMER_CHN0_STA(timerx)、M_TIMER_CHN1_STA(timerx)、M_TIMER_CHN2_STA(timerx)
例子：
1.需要关闭TIMER0 的通道1
    M_TIMER_CH1_STA(M_TIMER0) = 0;
2.需要打开TIMER0 的通道1
    M_TIMER_CH1_STA(M_TIMER0) = 1;
3.需要打开TIMER0 的互补通道0
    M_TIMER_CHN0_STA(M_TIMER0) = 1;

四、修改死区时间
M_TIMER_DEAD(timerx,val)
例子：
1.修改TIMER0死区时间为0x34
    M_TIMER_DEAD(M_TIMER0,0x34);

五、IO操作
M_IO_SET(gpiox,pin)
M_IO_GET(gpiox,pin)
例子：
1、设置PC13为高电平
    M_IO_SET(M_GPIOC,13) = 1;
2、获取PB2电平
    sta = M_IO_GET(M_GPIOB,2);
****************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* reg_config_h */

