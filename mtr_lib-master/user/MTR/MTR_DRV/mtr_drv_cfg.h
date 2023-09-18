/*
*********************************************************************************************************
*                                                  MTR_CFG
*                                              MTR_CFG MOUDULE
*
*********************************************************************************************************
*/
 
/*
*********************************************************************************************************
*                                                  MTR_CFG
* Filename     :mtr_cfg.h
*
*********************************************************************************************************
*/ 
#ifndef MTR_CFG_H
#define MTR_CFG_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#ifdef  MTR_CFG_MODULE
#define MTR_CFG_EXT
#else
#define MTR_CFG_EXT                 extern
#endif


#define   MTRNUM             2

#define   MTR0              0
#define   MTR1              1


#define MTR_SPD_CTRL                1


#define MTR_SENSEOLESS              0
#define MTR_ANTI_AURGE              0
#define MTR_MAG_ALPHA               1
#define MTR_HALL_ABC                0

#define MTR_V_SLOP_UP               1024
#define MTR_V_SLOP_DN              -1024*100
#define PWM_BLANK                   300
#define ADC_RANGE                   3.3f



#define   RANGE_VOL                (ADC_RANGE * 16.0f)
//#define   RANGE_CUR                (ADC_RANGE / 0.0025f / 6.0f )
#define   RANGE_CUR                (ADC_RANGE / 0.002f / 32.0f )


//#define   POLES                     2
//#define   RS                       (0.15f)   
//#define   LS                       (0.000400f)     
//#define   FLUX                     (0.176f)    


#define   POLES                     2
#define   RS                       (0.42f)   
#define   LS                       (0.001200f)     
#define   FLUX                     (0.176f) 

#define   FLUX_RCPT                (1.5f / FLUX)  

#define   DT                       (int32_t)(PWM_FREQ >> PWM_DIV) 


#define MCU_MCLK            ((uint32_t)192000000uL) /* MCU������Ƶ */
/* ==============================PWM Ƶ�ʼ���������=========================== */
#define PWM_MCLK            ((uint32_t)192000000uL) /* PWMģ��������Ƶ */
#define PWM_PRSC            ((uint8_t)0) /* PWMģ������Ԥ��Ƶ�� */
#define PWM_FREQ            ((uint16_t)10000) /* PWMն��Ƶ�� */
/* PWM ���ڼ�����ֵ */
#define PWM_PERIOD          ((uint16_t) (PWM_MCLK / (uint32_t)(2 * PWM_FREQ *(PWM_PRSC+1))))            //1600
//#define PWM_FEQ 20000
//#define PERIOD 	192000000/PWM_FEQ/2			//((192M/FREQ)-1)/2
#define PWM_DIV              2
#define PWM_TIME_2S         (uint16_t)(2*PWM_FREQ)
#define PWM_TIME_500MS      (uint16_t)(500*PWM_FREQ/1000)
#define PWM_TIME_500US      (uint16_t)(PWM_FREQ/500)
#define PWM_TIME_500uS      (uint16_t)(PWM_FREQ/1000/2)
#define PWM_TIME_1MS        (uint16_t)(PWM_FREQ/1000)
#define PWM_TIME_2MS        (uint16_t)(2*PWM_FREQ/1000)
#define PWM_TIME_4MS        (uint16_t)(4*PWM_FREQ/1000)
#define PWM_TIME_5MS        (uint16_t)(5*PWM_FREQ/1000)
#define PWM_TIME_10MS       (uint16_t)(20*PWM_FREQ/1000)
#define PWM_TIME_20MS       (uint16_t)(40*PWM_FREQ/1000)

#define TIME_2S_BASE_5MS    ((uint16_t)400)
#define TIME_1S6_BASE_5MS   ((uint16_t)130)
#define TIME_0S5_BASE_5MS   ((uint16_t)20)

#define HALL_BLOCK_TIME_S   (uint8_t)(2)
#define HALL_TIMER_TH       (uint32_t)(PWM_MCLK * HALL_BLOCK_TIME_S)
#define SAMPLING_FREQ       ((uint16_t)PWM_FREQ)   // Resolution: 1Hz

#define MAX_MODULE_VALUE   19600 /* �����Ʊ� */

#define DEADTIME_NS         ((uint16_t)(1000/10))/* ����ʱ�� */
#define DEADTIME            (uint16_t)(((unsigned long long)PWM_MCLK * (unsigned long long)DEADTIME_NS)/1000000000uL)

#define DEADTIMECOMPVOLTAGE (uint16_t)(DEADTIME_NS/(1000000000.0/PWM_FREQ)*MAX_MODULE_VALUE)	//385(1us/((1/15k)*1000000)us)*18907



#define SYSTICK_MAXCOUNT    ((1<<24) -1)                                     /*!< SysTick MaxCount   */

//��������������
#define USE_IdealPID     			
//#define USE_ParallelPID     
#ifdef   USE_IdealPID
#define CURRBandWidth       PWM_FREQ/20
#define CURR_KP							CURRBandWidth*LS
#define CURR_KI							RS/LS/PWM_FREQ
#else 
#ifdef USE_ParallelPID
#define CURRBandWidth       PWM_FREQ/20
#define CURR_KP							CURRBandWidth*LS
#define CURR_KI							CURR_KP*RS/LS/PWM_FREQ
#endif
#endif

#endif
