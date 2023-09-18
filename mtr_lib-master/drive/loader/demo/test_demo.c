#define DEF_DBG_I
#define DEF_DBG_W
#define DEF_DBG_E
#include "test_demo.h"
#include "lks32mc45x.h"
#include "lks32mc45x_lib.h"
#define GPIO_TEST 			0
#define LED_TEST 			0
#define CAN_TEST 			0
#define CAN_SRC_TEST        0
#define CAN_S_TEST          0
#define FLASH_TEST 			0
#define SPI_TEST        	0
#define ADC_TEST        	0
#define DAC_TEST        	0
#define IIC_TEST        	0
#define PWM_TEST        	0
#define GD25Q_TEST			0
#define UART_TEST           0
#define EXTI_TEST			0
//void test_init(void)
//{
////		ADC_Trigger(ADC2);
//}
//REG_INIT_MOD(test_init)

//float tmp_voltage;
//void test(void)
//{
////	ADC_Trigger(ADC2);
////	 tmp_voltage = ADC_GetVoltage(ADC2,ADC_DAT_0);
//}
//REG_TASK(1, 500, test)

#if UART_TEST
uint8_t uart0_rbuffer[20] = {0};
uint8_t uart1_rbuffer[20] = {0};
uint8_t uart2_rbuffer[20] = {0};

uint8_t uart0_tbuffer[16] = "UART0_DRIVE_TEST";
uint8_t uart1_tbuffer[16] = "UART1_DRIVE_TEST";
uint8_t uart2_tbuffer[16] = "UART2_DRIVE_TEST";

uint8_t uartx_sta = 1;

void uart_baud_test()
{
    xh_stream_set_baudrate(DRV_UART0,9600);
    xh_stream_set_baudrate(DRV_UART1,115200);
    xh_stream_set_baudrate(DRV_UART2,9600);
}
REG_INIT_MOD(uart_baud_test)
void uart_test()
{
    uartx_sta = !uartx_sta;
    if(uartx_sta == 1)
    {
        xh_stream_read(DRV_UART0,uart0_rbuffer,15);
        xh_stream_read(DRV_UART1,uart1_rbuffer,15);
        xh_stream_read(DRV_UART2,uart2_rbuffer,15);
    }
    else
    {
        xh_stream_write(DRV_UART0,uart0_tbuffer,16);
        xh_stream_write(DRV_UART1,uart1_tbuffer,16);
        xh_stream_write(DRV_UART2,uart2_tbuffer,16);
    }
}
REG_TASK(1,200,uart_test)
#endif

#if FLASH_TEST

void demo(void)
{
	uint8_t wdata[20] = {0};
	uint8_t rdata0[20] = {0};
	uint32_t addr = 0x08025800;
	for(uint32_t i = 0; i<20; i++)
	{
		wdata[i] = 5;
	}
	for(uint32_t i = 0; i<20; i++)
	{
		rdata0[i] = 1;
	}
	xh_flash_erase(DRV_FLASH, addr, 1025);
	xh_flash_read(DRV_FLASH, addr+0x3F0, rdata0, 20);
	xh_flash_write(DRV_FLASH, addr+0x3F0, wdata, 20);
	xh_flash_read(DRV_FLASH, addr+0x3F0, rdata0, 20);
	xh_flash_erase(DRV_FLASH, addr+0x3F0, 10);
	xh_flash_read(DRV_FLASH, addr+0x3F0, rdata0, 20);
	
	xh_flash_erase(DRV_FLASH, addr+0x3F0, 20);
	xh_flash_read(DRV_FLASH, addr+0x3F0, rdata0, 20);
	
	xh_flash_erase(DRV_FLASH, addr, 1025);
	xh_flash_read(DRV_FLASH, addr+0xFF0, rdata0, 20);
	
	xh_flash_write(DRV_FLASH, addr+0x3F0, wdata, 20);
	xh_flash_erase(DRV_FLASH, addr+0x400, 10);
	xh_flash_read(DRV_FLASH, addr+0x3F0, rdata0, 20);
}
REG_TASK(1, 500, demo)
#endif

#if GPIO_TEST
uint16_t sta_io = 1, input_sta_io = 0;
void change_io_mode()
{
    xh_io_mode(DRV_GPIO,IO_NAME,GPIO_Mode_OUT);
    block_ms(2000);
}
//REG_INIT_MOD(change_io_mode)
void test_task(void)
{
    sta_io = !sta_io;
//    input_sta_io = xh_io_read(DRV_GPIO, IO_NAME);
    xh_io_write(DRV_GPIO, IO_NAME, sta_io);
    xh_io_write(DRV_GPIO, IO_NAME1, sta_io);
    xh_io_write(DRV_GPIO, IO_NAME2, sta_io);
    xh_io_write(DRV_GPIO, IO_NAME3, sta_io);
    xh_io_write(DRV_GPIO, IO_NAME4, sta_io);
    xh_io_write(DRV_GPIO, IO_NAME5, sta_io);
//    input_sta_io = xh_io_read(DRV_GPIO, LED1);
}
REG_TASK(1, 300, test_task)
#endif

#if LED_TEST
static uint8_t led_flag;
void led_task(void)
{
    led_flag = !led_flag;
    xh_io_write(DRV_GPIO, SYS_LED, led_flag);
}
REG_TASK(1, 300, led_task)
#endif

#if CAN_SRC_TEST == 1
void can_src_test_task(void)
{
    uint8_t tx_data[10] = {0xAA, 1, 2, 3, 4, 5, 6, 7};
    xh_stream_write(DRV_CAN_COMP, tx_data, 4);
}
REG_TASK(ID_TASK, 200, can_src_test_task)
#endif

#if CAN_S_TEST == 1
void can_s_test_task(void)
{
    uint8_t tx_data[10] = {0xAA, 1, 2, 3, 4, 5, 6, 7};
    xh_stream_write(DRV_CAN_S, tx_data, 4);
}
REG_TASK(ID_TASK, 200, can_s_test_task)
#endif

#if CAN_TEST == 1
void can_test_cb_id(uint32_t pack_type, uint32_t id, uint8_t *p_pack, uint32_t pack_len)
{
    uint8_t rx_data[8];
    uint32_t rx_len;
    uint32_t pack_id;
    uint32_t type;

    type = pack_type;
    pack_id = id;
    rx_len = pack_len;
    uint32_t i;
    for (i = 0; i < rx_len; i++)
    {
        rx_data[i] = p_pack[i];
    }
    xh_can_write(DRV_CAN0, 0, 2, rx_data, rx_len);
}
void can_test_init(void)
{
    xh_can_reg_rx_irq_cb(DRV_CAN0, can_test_cb_id);
    xh_can_set_baudrate(DRV_CAN0, 500000);
}
REG_INIT_MOD(can_test_init);
void can_test_task(void)
{
    uint8_t tx_data[10] = {0xAA, 1, 2, 3, 4, 5, 6, 7};
    xh_can_write(DRV_CAN0, 0, 2, tx_data, 4);
}
REG_TASK(ID_TASK, 200, can_test_task)
#endif

#if IIC_TEST == 1
uint8_t tx_data[10] = {0xAA,1,2,3,4,5,6,7};
uint8_t rx_data[10] = {0};
void IIC_test_init(void)
{
//    xh_i2c_set_slave_addr(DRV_I2C_SIM,0xA0);
}
REG_INIT_MOD(IIC_test_init)

uint8_t s = 0;
void test_task(void)
{
	if(s)
	{
		xh_i2c_write(DRV_I2C_SIM,tx_data,0x27,8); 
	}else
	{
		xh_i2c_read(DRV_I2C_SIM,rx_data,0x27,8); 
	}
	s =!s;
}
REG_TASK(1,50,test_task)
#endif

#if SPI_TEST == 1

uint8_t spi_w_data[10] = {0xA2,1,2,3,4,5,6,7,8,9};
uint8_t spi_r_data[10] = {0};
void test_spi(void)
{
	xh_spi_set_cs_level(DRV_SPI,CS_PIN_NAME,0);
    xh_spi_rw(DRV_SPI,spi_w_data,spi_r_data,10);
	xh_spi_set_cs_level(DRV_SPI,CS_PIN_NAME,1);
}
REG_TASK(1,500,test_spi)
#endif
#if GD25Q_TEST == 1
uint8_t tx_data[20] = {0xAA,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0x19};
uint8_t rx_data[20] = {0};
uint32_t spi_flash_addr = 0x00000000;
void test_task(void)
{
    xh_flash_erase(DRV_SPI_FLASH,spi_flash_addr,20);        // 擦除外部flash 首地址为 spi_flash_addr
    xh_flash_read(DRV_SPI_FLASH,spi_flash_addr,rx_data,20); // 读外部flash 首地址为 spi_flash_addr
    xh_flash_erase(DRV_SPI_FLASH,spi_flash_addr,20);        // 擦除外部flash 首地址为 spi_flash_addr
    xh_flash_write(DRV_SPI_FLASH,spi_flash_addr,tx_data,20);// 写外部flash 首地址为 spi_flash_addr
    xh_flash_read(DRV_SPI_FLASH,spi_flash_addr,rx_data,20); // 读外部flash 首地址为 spi_flash_addr
}
REG_TASK(1,300,test_task)
#endif
#if ADC_TEST == 1
int16_t adc1_value[ADC1_MAX] = {0};
int16_t adc2_value[ADC2_MAX] = {0};
// uint16_t adc_value_insertend_0_1[2];
int16_t adc_value_insertend_0_2[ADC2_MAX];
// uint16_t adc1_value_ind[ADC_REGULAR_MAX] = {0};
 int8_t adc_irq_state_0 = 0;
int8_t i= 0;
//uint16_t ad_test = 0;
void irq_test_0(void)
{
 	adc_irq_state_0 = !adc_irq_state_0;
// 	adc_value_insertend_0_1[0]= xh_adc_read_value(DRV_ADC_INSERTED, ADC_CH7);
// 	adc_value_insertend_0_1[1]= xh_adc_read_value(DRV_ADC_INSERTED, ADC_CH9);
 	xh_adc_read_all_value(DRV_ADC, adc_value_insertend_0_2,ADC2_MAX);
}
void adc_test_init(void)
{
		xh_adc_reg_irq(DRV_ADC, irq_test_0);
//		xh_adc_set_buf(DRV_ADC,adc0_value,ADC2_MAX); 
		xh_adc_set_buf(DRV_ADC1,adc1_value,ADC1_MAX); 
		xh_adc_start(DRV_ADC);
		xh_adc_disable(DRV_ADC);
}
REG_INIT_MOD(adc_test_init)

void adc_task(void)
{
//	xh_adc_set_buf(DRV_ADC_DMA,adc0_value,ADC_REGULAR_MAX); 
	adc2_value[0] =xh_adc_read_value(DRV_ADC, ADC_CH_NAME);
	adc2_value[1] =xh_adc_read_value(DRV_ADC, ADC_CH_NAME1);
	adc2_value[2] =xh_adc_read_value(DRV_ADC, ADC_CH_NAME2);
//	if( i < 5 || i > 10){
//	xh_adc_start(DRV_ADC_INSERTED);
//	}
//		xh_adc_start(DRV_ADC);

//	i++;
//	if(5 == i){
		/* ADC失能 */
//		xh_adc_disable(DRV_ADC);
//	}
//	if(10 == i){
//		xh_adc_enable(DRV_ADC);
////		xh_adc_start(DRV_ADC_DMA);
//	}
//	if(i >15){
//    i=0;
//  }
}
REG_TASK(1,500,adc_task)
#endif

#if DAC_TEST == 1
#include "bsp_gpio.h"
void test_task(void)
{
	GPIO_BOP(GPIOC) = (uint32_t)GPIO_PIN_6;
	GPIO_BOP(GPIOC) = (uint32_t)GPIO_PIN_7;
	xh_dac_set(DRV_DAC0, 1024);

	xh_dac_set(DRV_DAC1, 2048);
}
REG_TASK(1, 300, test_task)
#endif


#if PWM_TEST == 1
uint32_t count[8] = {0}; 
void PWM_IRQ7(void)
{
//	xh_io_write(DRV_GPIO, IO_NAME, 1);
//	count[7] ++;
//	xh_io_write(DRV_GPIO, IO_NAME, 0);
}
void PWM_IRQ6(void)
{
	count[6] ++;
}
void PWM_IRQ5(void)
{
	count[5] ++;
}
void PWM_IRQ4(void)
{
	count[4] ++;
}
void PWM_IRQ3(void)
{
	count[3] ++;
}
void PWM_IRQ2(void)
{
	count[2] ++;
}
void test_task(void)
{
//		xh_pwm_reg_irq(DRV_PWM0,XH_PWM_UPDATA,PWM_IRQ7);
//		xh_pwm_reg_irq(DRV_PWM2,XH_PWM_UPDATA,PWM_IRQ6);
////		xh_pwm_reg_irq(DRV_PWM3,XH_PWM_CMT,PWM_IRQ5);
////		xh_pwm_reg_irq(DRV_PWM1,XH_PWM_CH_0,PWM_IRQ4);
////		xh_pwm_set_ch(DRV_PWM2,PWM_FAN,XH_PWM_CHO_ON);
////		xh_pwm_set_ch(DRV_PWM3,PD_LLC_LH_PWM,XH_PWM_CHO_ON);
////		xh_pwm_set_ch(DRV_PWM3,PD_LLC_RH_PWM,XH_PWM_CHO_ON);
////		xh_pwm_set_ch(DRV_PWM1,O_PWM_PV2,XH_PWM_CHO_ON);
////		xh_pwm_set_ch(DRV_PWM8,PWM_FAN_12V,XH_PWM_CHO_ON);
//	
//		xh_pwm_set_ch(DRV_PWM13,PWM13_CH0,XH_PWM_CHO_ON);
////		xh_pwm_set_ch(DRV_PWM0,LV_LLC_RH_PWM,XH_PWM_CHO_ON);
//		xh_pwm_set_ch(DRV_PWM0,PWM0_CH0,XH_PWM_CHO_ON);
//		xh_pwm_set_ch(DRV_PWM0,PWM0_CH0,XH_PWM_CHON_ON);
//	
//		xh_pwm_set_ch(DRV_MCPWM0,0,0);
//		xh_pwm_set_ch(DRV_MCPWM0,0,0);
//	
//		xh_pwm_disable(DRV_MCPWM0);
//	xh_mcpwm_reg_irq(DRV_MCPWM0, XH_CNT0_MCPWM_T0_IRQ, PWM_IRQ2);
//	xh_mcpwm_reg_irq(DRV_MCPWM0, XH_Cnt1_MCPWM_TH30_IRQ, PWM_IRQ3);
	block_ms(2000);
	xh_mcpwm_enable(DRV_MCPWM0);
	xh_mcpwm_set_ch(DRV_MCPWM0, MCPWM_CH_NAME, XH_MCPWM_CH_P_LOW_N_PWM);
	xh_mcpwm_set_ch_mode(DRV_MCPWM0,XH_MCPWM_OUT_CH_012,XH_MCPWM_OUT_MODE_PWM);
	block_ms(500);
	xh_mcpwm_set_ch(DRV_MCPWM0, MCPWM_CH_NAME, XH_MCPWM_CH_P_LOW_N_HIGHT);
	block_ms(500);
	xh_mcpwm_set_ch(DRV_MCPWM0, MCPWM_CH_NAME, XH_MCPWM_CH_P_PWM_N_LOW);
	block_ms(500);
	xh_mcpwm_set_ch(DRV_MCPWM0, MCPWM_CH_NAME, XH_MCPWM_CH_P_PWM_N_HIGHT);
	block_ms(500);
	xh_mcpwm_set_ch(DRV_MCPWM0, MCPWM_CH_NAME, XH_MCPWM_CH_P_HIGHT_N_PWM);
	block_ms(500);
	xh_mcpwm_set_ch(DRV_MCPWM0, MCPWM_CH_NAME, XH_MCPWM_CH_P_HIGHT_N_LOW);
	block_ms(500);
	xh_mcpwm_set_ch(DRV_MCPWM0, MCPWM_CH_NAME, XH_MCPWM_CH_P_LOW_N_LOW);
	block_ms(500);
	xh_mcpwm_set_ch(DRV_MCPWM0, MCPWM_CH_NAME, XH_MCPWM_CH_P_PWM_N_PWM);
//		xh_pwm_enable(DRV_PWM13);

//		xh_timer_reg_irq(DRV_TIMER0,PWM_IRQ7);
}
REG_INIT_MOD(test_task)

uint32_t flag = 0;
void pwm_test_task(void)
{
	if(flag == 0)
	{
//		xh_mcpwm_set_duty_cycle(DRV_MCPWM0,MCPWM_CH_NAME,7000);
//		xh_mcpwm_disable(DRV_MCPWM0);
//		xh_mcpwm_set_ch(DRV_MCPWM0, MCPWM_CH_NAME1, XH_MCPWM_CH_P_LOW_N_LOW);
	}
	else
	{
//		xh_mcpwm_set_duty_cycle(DRV_MCPWM0,MCPWM_CH_NAME,8000);
//		xh_mcpwm_enable(DRV_MCPWM0);
//		xh_mcpwm_set_ch(DRV_MCPWM0, MCPWM_CH_NAME1, XH_MCPWM_CH_P_PWM_N_PWM);
	}
//	MCPWM0->TH00 = 60;
//	MCPWM0->TH01 = 60;
	flag = !flag;
}
REG_TASK(1, 300, pwm_test_task)
#endif

#if EXTI_TEST == 1
uint32_t count[16] = {0}; 
void EXTI_IRQ1(void)
{
//	xh_io_write(DRV_GPIO, IO_NAME, 1);
	count[1] ++;
//	xh_io_write(DRV_GPIO, IO_NAME, 0);
}
void EXTI_IRQ2(void)
{
	count[2] ++;
}
void EXTI_IRQ8(void)
{
	count[8] ++;
}
void EXTI_test_init(void)
{
	xh_exti_reg_irq(DEV_EXTI1,EXTI_IRQ1);
	xh_exti_reg_irq(DEV_EXTI2,EXTI_IRQ2);
	xh_exti_reg_irq(DEV_EXTI5,EXTI_IRQ8);
}
REG_INIT_MOD(EXTI_test_init)
uint8_t flag = 0;
void test_task(void)
{

}
REG_TASK(1, 300, test_task)
#endif

