/*
*********************************************************************************************************
*                                                  HARDWARE_CFG
*                                              HARDWARE_CFG MOUDULE
*
*********************************************************************************************************
*/
 
/*
*********************************************************************************************************
*                                                  HARDWARE_CFG
* Filename     :hardware_cfg.h
*
*********************************************************************************************************
*/ 
#ifndef __HARDWARE_CFG_H__
#define __HARDWARE_CFG_H__


/*
*********************************************************************************************************
*                                                  EXTERNS
*********************************************************************************************************
*/
#ifdef HARDWARE_CFG_MODULE
#define HARDWARE_CFG_EXT
#else 
#define HARDWARE_CFG_EXT extern
#endif
#ifndef RAM_FUNC
#define RAM_FUNC      __attribute__((section("ramfunc")))  
#endif

/*
*********************************************************************************************************
*                                              DEFINES
*********************************************************************************************************
*/ 
#define MCU_MCLK          ((uint32_t) 192000000uL)      //MCU运行主频
//-----------------BUCK配置-----------------
#define BUCK_PWM_FREQ     ((uint32_t) 100000)          //设置频率
#define BUCK_PWM_PRSC     ((uint8_t) 0)                 //分频系数
#define BUCK_PWM_PERIOD   ((uint16_t) ((MCU_MCLK) / (uint32_t)(2 * BUCK_PWM_FREQ * (BUCK_PWM_PRSC + 1))))
#define BUCK_DT_NS        ((uint16_t) 500)             //设置死区
#define BUCK_DT           (uint16_t)(((unsigned long long)MCU_MCLK * (unsigned long long)BUCK_DT_NS) / 1000000000uL)

#define BUCK_UTIME_FREQ     ((uint32_t) 10000)          //模拟DAC的输出PWM的频率
#define BUCK_UTIME_TH       (MCU_MCLK / BUCK_UTIME_FREQ)

//-----------------MTR配置-----------------
#define MTR_PWM_MCLK            ((uint32_t)192000000uL) /* PWM模块运行主频 */
#define MTR_PWM_PRSC            ((uint8_t)0) /* PWM模块运行预分频器 */
#define MTR_PWM_FREQ            ((uint16_t)16383) /* PWM斩波频率 */
#define MTR_PWM_PERIOD          ((uint16_t) (MTR_PWM_MCLK / (uint32_t)(2 * MTR_PWM_FREQ *(MTR_PWM_PRSC+1))))            //1600
#define MTR_DT_NS				((uint16_t)(100))/* 死区时间 */
#define MTR_DT					(uint16_t)(((unsigned long long)MTR_PWM_MCLK * (unsigned long long)MTR_DT_NS)/1000000000uL)



/*
*********************************************************************************************************
*                                              EXTERN FUNCTIONS
*********************************************************************************************************
*/
//HARDWARE_CFG_EXT void adc_config_cb(void);
//HARDWARE_CFG_EXT void mcpwm_config_cb(void);

/*
*********************************************************************************************************
*                                              MOUDULE END
*********************************************************************************************************
*/
#endif
