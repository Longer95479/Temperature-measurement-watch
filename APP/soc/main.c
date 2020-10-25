#include "stm32f10x.h"
#include "stdio.h"

#include "GLOBAL.h"
#include "led.h"
#include "task_scheduler.h"
#include "oled.h"
#include "clock.h"
#include "wheel.h"
#include "delay.h"
#include "buzzer.h"
#include "LMT70.h"
#include "bsp_exti.h"
#include "bsp_rtc.h"
#include "bsp_data.h"
#include "usart.h"


int main(void)
{
	delay_init();
	#ifdef	C8T6
	RTC_CheckAndConfig(&systmtime);
	#endif
	Led_Init(LED1);
	OLED_init();
	uart1_init(115200);
	LMT70_ad_init();
	adc_config_Init();//ADC³õÊ¼»¯
	EXTI_Key_Config();
	//clock_init();
	#ifdef VET6
	TIM_PWM_Init();	//·äÃùÆ÷
	#endif
	#ifdef	C8T6
	motor_init();
	#endif
	
	rest_screen_time_init(10);
	temp_thrshold_init(38);
	task_rhythm_init(5);
	
	//draw_coordinate(15, 30);
	
	while(1) {
		//oled_draw_clock();
		//clock_menu();
		oled_print_temp();
		//draw_real_temp_line();
	}
	
}
