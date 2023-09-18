/********************** Copyright(c)***************************
** ECOFLOW
**
** https://ecoflow.com/
**
**------Author Info--------------------------------------------
** File name:               run_demo.c
** Created by:              Chen JunTong
** Created date:            2022-01-11
** Version:                 V1.0.0
** Descriptions:            运行示例
**
**------File Info----------------------------------------------
** Last modified by:        Chen JunTong
** Last modified date:      2022-01-11
** Last version:            V1.0.0
** Description:             运行示例
**
*************************************************************/
#define DEF_DBG_I
#define DEF_DBG_W
#define DEF_DBG_E
#include "x_frame.h"
#include "run_demo.h"
//#include "app_base_cfg.h"
#include "hal.h"

#include "lks32mc45x.h"
#include "lks32mc45x_lib.h"
//#include "dcdc_driver.h"
//#include "hardware_cfg.h"
typedef enum {
    LINK_UART0 = 0,
	LINK_UART2,
	LINK_UART3,
	LINK_CAN0,
	LINK_CAN0_OLD,
} LINK_ID_LIST;

REG_LINK(LINK_UART0, 	VH_TASK, DRV_UART0, PROTOCOL_EF03_PLUS, 512, true, false)
////REG_LINK(LINK_UART2, 	VH_TASK, DRV_USART2, PROTOCOL_EF02_PLUS, 512, true, false)
//REG_LINK(LINK_CAN0, 	VH_TASK, DRV_CAN_S, PROTOCOL_EF03_PLUS, 512, true, false)


REG_REC_LINK(LINK_UART0)

REG_DEBUG_LINK(LINK_UART0)

// REG_X_SHELL_LINK(LINK_UART1)
// REG_X_SHELL_LINK(LINK_CAN0)

 REG_PARAM_CFG_LINK(LINK_UART0)
// REG_PARAM_CFG_LINK(LINK_CAN0)

REG_LINK_APP_BASE_ACTION(LINK_UART0)
// REG_LINK_APP_BASE_ACTION(LINK_UART2)
// REG_LINK_APP_BASE_ACTION(LINK_CAN0)

REG_LINK_UPDATA_ACTION(LINK_UART0)
//REG_LINK_UPDATA_ACTION(LINK_UART2)
//REG_LINK_UPDATA_ACTION(LINK_CAN0)

//static void demo_init(void)
//{
//	uint32_t len = (uint32_t)&len;
//}


//REG_INIT_MOD(demo_init)


//uint16_t test_add(void)
//{
//    static uint16_t i = 0;
//    xh_io_write(DRV_GPIO, GPIO_BLUE_LED, !GPIO_ReadOutputDataBit(GPIO0, GPIO_Pin_8));
//    return i++;
//}

//int16_t set_flag = 0;
//uint8_t user_test = 10;
//	static uint8_t cnt = 0;
//    cnt++;
//	if (cnt < user_test)
//		return ; 
//	cnt = 0;
//	if (set_flag){
//		set_flag = 0;
//		xh_io_write(DRV_GPIO, GPIO_RED_LED, 0);
//        GPIO_SetPinBits(GPIO0, GPIO_PinSource_8);
//	} else {
//		set_flag = 1;
//		xh_io_write(DRV_GPIO, GPIO_RED_LED, 1);
//		GPIO_ResetPinBits(GPIO0, GPIO_PinSource_8);
//	}
    
//	xh_io_write(DRV_GPIO, GPIO_BLUE_LED, xh_io_read(DRV_GPIO, GPIO_BLUE_LED) == 0 ? 1 : 0);
//    DBG_W("test!\n");
//	DBG_I("DBG_I\n");
	// test_rec++;
	// RUN_REC(test_rec);
/************************Flash Demo************************** */
//uint8_t test_flash[10] = {1, 2, 3, 4, 5};
//EXT_FLASH_PARAM_NAME(NVM_PARAM)
//EXT_FLASH_PARAM_NAME(NVM_PARAM_CFG)
// REG_FLASH_NORMAL_PARAM(SYS_APP_CFG_PART, NVM_PARAM, test_flash, NULL, sizeof(test_flash))
//REG_FLASH_NORMAL_PARAM(SYS_APP_CFG_PART, NVM_PARAM_CFG, test_flash, NULL, sizeof(test_flash))
uint8_t test_end = 0xff, test_end_cfg = 0xff, read_flag = 0xff;
uint8_t Read_Flash[10] = {0};

//void TestFlash(void)
//{
//    static uint8_t init = 0;
////    static uint8_t save_buf[12] = { 0x4d, 0x5f, 0x50, 0x41, 0x52, 0x41, 0x4d, 0x5f, 0x43, 0x46, 0x47, 0x00};
////    static uint8_t save_1c_bus[4] = {0x79, 0xb9, 0x66, 0x77};
////    static uint8_t save_20_bus[3] = {0x88, 0x99, 0xAA};
//    static uint8_t save_buf_CFG[10] = {0x11,0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
//    if(init == 0) {
//       test_end_cfg = flash_param_save(NVM_PARAM_CFG, save_buf_CFG, sizeof(save_buf_CFG));
//       init = 1;
//    } else if (init == 1) {
////        xh_flash_read(DRV_FLASH, 0x1d810, Read_Flash, sizeof(Read_Flash));
//        read_flag= flash_param_read(NVM_PARAM_CFG,Read_Flash, sizeof(Read_Flash) );
//        init = 2;
//    }
//}

//REG_TASK(ID_TASK, 100, TestFlash)
/* *******************TASK Demo*************************** */
uint16_t test_read_io = 0, test_read_io_2 = 0;
uint16_t test_rec_var;
bool can_send_flag = false;

uint32_t can_read_cnt = 0;
void can_test_cb_id(uint32_t pack_type, uint32_t id, uint8_t *p_pack, uint32_t pack_len)
{
//    uint8_t  rx_data[8];
//    uint32_t rx_len;
//    uint32_t pack_id;
//    uint32_t type;
//    
    can_read_cnt++;
//    type = pack_type;
//    pack_id = id;
//    rx_len = pack_len;
//    
//    uint32_t i = 0;
//    for (i = 0; i<rx_len; i++)
//    {
//        rx_data[i] = p_pack[i];
//    }
//    xh_can_write (DRV_CAN, 0, 2, rx_data, rx_len);
}

void can_test_init(void)
{
    xh_can_reg_rx_irq_cb(DRV_CAN, can_test_cb_id);
    xh_can_set_baudrate(DRV_CAN, 500000);
}                                        
//REG_INIT_MOD(can_test_init);


uint8_t can_write[1] = {0};
int32_t write_cnt = 100;
void can_test_func(void)
{
    if(write_cnt != 0)
    {
        write_cnt--;
        can_write[0]++;
        xh_can_write (DRV_CAN, 0, 2, can_write, sizeof(can_write));
    }
}

//REG_TASK(ID_TASK, 1, can_test_func)

uint8_t set_trig_flag = 0, set_scan_flag = 0, set_trig_err = 0;
float read_Il = 0, read_Vout = 0;
int16_t read_Il_Value = 0, read_Vout_Value = 0, read_Il_Value_2 = 0;
int16_t cail_Il_value = 0;
int16_t read_Il_Bias = 0;
float rs = 0.00175;

//   cmp 

///  dac 
int16_t cmp_data = 0;
uint8_t set_cmp_flag = 0;
float dac0_data = 1.5f, dac1_data = 2.0f;

//MCPWM
uint8_t set_crl_fail_flag = 0;

//uint16_t set_duty = 0, read_duty = 0;
//void test(void)
//{    
//    static bool init = false;
//	if (init == false) {
//        static uint8_t i = 0;
//        static int32_t sum = 0;
//        sum += read_Il_Value;
//        if (++i == 64) {
//            read_Il_Bias = sum >> 6;
//            init = true;
//        }
//    } else {
//        read_Vout = read_Vout_Value * 2.2 / 32768 * 21;
//        read_Il   = (cail_Il_value - read_Il_Bias) *2.2 / 32768 / 16 / rs;
//    }
//		
//        //-------------DAC测试----------------
//		if (set_cmp_flag) {
//            set_cmp_flag = 0;
//            AFE_DacSetVoltage(AFE_MODULE_DAC0, dac0_data);
//            AFE_DacSetVoltage(AFE_MODULE_DAC1, dac1_data);
//        }
//        //-------------MCPWM的fail恢复测试--------
//        if (set_crl_fail_flag) {
//            set_crl_fail_flag = 0;
//            MCPWM0->PRT = 0xDEAD;           
//            MCPWM0_EIF     = 0xFF;
//            MCPWM0_FAIL012 |= 1<<6;           //恢复Fail
//            MCPWM0->PRT = 0;        
//        }
//		//------------扫描ADC采集-----------------
//		if (set_scan_flag == 0) return;
//		static uint8_t scan_cnt = 0;
//		if (scan_cnt++ > 100) {
//			scan_cnt = 0;
//			if (set_duty + 10 > BUCK_PWM_PERIOD) {
//				set_duty = 0;
//			} else {
//				set_duty += 10;
//			}
//		}
//		
//		
//    
//    cmp_data = CMP_DATA;
//}
//REG_TASK(ID_TASK, 1, test)

// void MCPWM00_IRQHandler(void)
//void ADC0_IRQHandler(void)
//{
//	ADC0_IF = 0xFFFF;
//	static uint8_t i = 0;
//	i = i == 0 ? 1 : 0;
//	read_duty = BUCK_PWM_PERIOD;
////	xh_io_write(DRV_GPIO, GPIO_BLUE_LED, i);
//	dcdc_drvier.dcdc_set_pwm_duty(BUCK,set_duty);
//    read_Il_Value = ADC0_DAT0;
//    read_Vout_Value = ADC0_DAT1;
//	read_Il_Value_2 = ADC0_DAT2;
//	cail_Il_value = ((int32_t)read_Il_Value_2 * set_duty + (int32_t)read_Il_Value * (BUCK_PWM_PERIOD - set_duty)) / BUCK_PWM_PERIOD;
//	
//	if (set_trig_flag == 1) {
//			set_trig_flag = 0;
//			int16_t temp = BUCK_PWM_PERIOD - set_trig_err;
//			MCPWM0->PRT = 0x0000DEAD;
//			MCPWM0->TMR0 = temp;
//			temp = 0 - set_trig_err;
//			MCPWM0->TMR1 = temp;
//			MCPWM0->PRT = 0x0000CAFE;
//		}
//    
//}


/* *******************PARM_CFG Demo*************************** */
//REG_CFG_ITEM(user_test, user_test, eu_uint8, PARM_RW, 10.0f, 0.0f, 100.0f);


/* *******************REC Demo******************************** */
#define REG_REC_TASK(group,perio,task_name,def_rec_sta)                                     		\
        uint8_t enable_##task_name = def_rec_sta;                                                  	\
        REG_CFG_ITEM(task_name, enable_##task_name, eu_uint8, PARM_RW, def_rec_sta, 0.0f, 1.0f);  	\
        EXT_REC(task_name);                                                                 		\
        static void task_name(void)                                                         		\
        {                                                                                   		\
            if(enable_##task_name == false){                                                		\
                return;                                                                     		\
            }                                                                               		\
            RUN_REC(task_name);                                                             		\
        }                                                                                   		\
        REG_TASK(group,perio,task_name);

//REG_REC_TASK(VH_TASK, 100, TEST_PUSH, true)

