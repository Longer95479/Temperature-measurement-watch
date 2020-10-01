/**
 * @brief	时钟，可设置闹钟
 */
#define CLOCK_GLOBALS

#include "stm32f10x.h"
#include "stdio.h"

#include "GLOBAL.h"
#include "clock.h"
#include "oled.h"
#include "key.h"
#include "wheel.h"
#include "menu.h"
#include "buzzer.h"
#include "LMT70.h"
#include "bsp_exti.h"
#include "bsp_rtc.h"
#include "bsp_data.h"

static const char *const clock_menu_text[] = {
	"====Menu====",
	"1. Set clock",
	"2. Set alarm ",
	"3. Set R.S.T.",
	"4. Set temp threshold ",
	"5. Temp line chart ",
	"6. Set LPR mode "
};

/**
 * @brief	时钟菜单
 */
void clock_menu(void)
{
	uint8_t selection = 0;
	
	while (selection != 0xff) {		//此处改成此种形式，通过menu_active返回值来判断是进入还是退出
		selection = menu_active((char **)clock_menu_text, 6);
		
		OLED_clear();
		switch (selection) {
			case 0: oled_draw_clock(); break;
			case 1: set_alarm_clock(); break;
			case 2: set_rest_screen_time(); break;
			case 3: set_temp_threshold(); break;
			case 4: draw_real_temp_line(); break;
			case 5: ;
			case 6: set_LPR_mode(); break;
			default: break;
		}
	}

	OLED_clear();
}



/**
 * @brief	设置时钟的初始时间
 */
void clock_init(void)
{
	get_time(RTC_GetCounter(),&systmtime);
	
	hour = systmtime.tm_hour;
	minute = systmtime.tm_min;
	second = systmtime.tm_sec;
}

/**
 * @brief	时钟更新
 * @note	放在任务中，每秒执行一次
 */
void update_clock(void)
{
	#if 0
	second++;
	if (second == 60) {
		second = 0;
		minute++;
		if (minute == 60) {
			minute = 0;
			hour++;
			if (hour == 24)
				hour = 0;
		}
	}
	#endif
	
	get_time(RTC_GetCounter(),&systmtime);
	
	hour = systmtime.tm_hour;
	minute = systmtime.tm_min;
	second = systmtime.tm_sec;
	
}


/**
 * @brief	oled显示时间
 */
void oled_draw_clock(void)
{
	char time[9];
	
	key_1 = 0;
	while (key_1 != 1) {
		time[0] = '0' + hour / 10;
		time[1] = '0' + hour % 10;
		
		time[2] = ':';
		
		time[3] = '0' + minute / 10;
		time[4] = '0' + minute % 10;
		
		time[5] = ':';
		
		time[6] = '0' + second / 10;
		time[7] = '0' + second % 10;
		
		time[8] = '\0';
		
		OLED_show8x16string(33, 3, time);
		
		if (key_2 == 1) {
			key_2 = 0;
			set_clock();
		}
	}
	key_1 = 0;
}



static void update_chars_time(char *time)
{
	time[0] = '0' + hour / 10;
	time[1] = '0' + hour % 10;
	time[2] = ':';
	time[3] = '0' + minute / 10;
	time[4] = '0' + minute % 10;
	time[5] = ':';
	time[6] = '0' + second / 10;
	time[7] = '0' + second % 10;	
	time[8] = '\0';
}

static void update_chars_alarm(char *time)
{
	time[0] = '0' + alarm_hour / 10;
	time[1] = '0' + alarm_hour % 10;
	time[2] = ':';
	time[3] = '0' + alarm_minute / 10;
	time[4] = '0' + alarm_minute % 10;
	time[5] = ':';
	time[6] = '0' + alarm_second / 10;
	time[7] = '0' + alarm_second % 10;	
	time[8] = '\0';
}


/**
 * @brief	设置时间
 */
void set_clock(void)
{
	int wheel_init_pos, first_equ_init_pos = 1;
	wheel_init_pos = get_wheel_position(24);
	
	
	int wheel_pos;
	int item = 0;	//h,m,s
	
	char time[9];
	
	update_chars_time(time);
	OLED_show8x16char_inverse(33, 3, time[0]);
	OLED_show8x16char_inverse(33 + 8, 3, time[1]);
	OLED_show8x16string(33 + 2*8, 3, time + 2);
	
	while (key_1 != 1) {
		switch (item) {
			case 0:
				update_chars_time(time);
			
				wheel_pos = get_wheel_position(24);	
				if (wheel_pos != wheel_init_pos) {
					time[0] = '0' + wheel_pos / 10;
					time[1] = '0' + wheel_pos % 10;
					
					first_equ_init_pos = 0;
				}
				else if (!first_equ_init_pos) {	//不是第一次等于初始值，意味着转了电位器又转了回来
					time[0] = '0' + wheel_pos / 10;
					time[1] = '0' + wheel_pos % 10;
				}
				
				//OLED_show8x16string(33, 3, time);
				OLED_show8x16char_inverse(33, 3, time[0]);
				OLED_show8x16char_inverse(33 + 8, 3, time[1]);
				OLED_show8x16char(33 + 2*8, 3, time[2]);
				OLED_show8x16char(33 + 3*8, 3, time[3]);
				OLED_show8x16char(33 + 4*8, 3, time[4]);
				OLED_show8x16char(33 + 5*8, 3, time[5]);
				OLED_show8x16char(33 + 6*8, 3, time[6]);
				OLED_show8x16char(33 + 7*8, 3, time[7]);
				
				if (key_2 == 1) {
					item++;
					wheel_init_pos = get_wheel_position(60);	//确定下个设置项的滚轮初始位置
					if (!first_equ_init_pos) {
						//hour = wheel_pos;
						systmtime.tm_hour = wheel_pos;
						Time_Adjust(&systmtime);
					}
					key_2 = 0;
					first_equ_init_pos = 1;
				}
				break;
				
			case 1:
				update_chars_time(time);	
			
				wheel_pos = get_wheel_position(60);	
				if (wheel_pos != wheel_init_pos) {
					time[3] = '0' + wheel_pos / 10;
					time[4] = '0' + wheel_pos % 10;
					
					first_equ_init_pos = 0;
				}
				else if (!first_equ_init_pos) {	//不是第一次等于初始值，意味着转了电位器又转了回来
					time[3] = '0' + wheel_pos / 10;
					time[4] = '0' + wheel_pos % 10;
				}
				
				//OLED_show8x16string(33, 3, time);
				OLED_show8x16char(33 + 0*8, 3, time[0]);
				OLED_show8x16char(33 + 1*8, 3, time[1]);
				OLED_show8x16char(33 + 2*8, 3, time[2]);
				OLED_show8x16char_inverse(33 + 3*8, 3, time[3]);
				OLED_show8x16char_inverse(33 + 4*8, 3, time[4]);
				OLED_show8x16char(33 + 5*8, 3, time[5]);
				OLED_show8x16char(33 + 6*8, 3, time[6]);
				OLED_show8x16char(33 + 7*8, 3, time[7]);
				
				if (key_2 == 1) {
					item++;
					wheel_init_pos = get_wheel_position(60);	//确定下个设置项的滚轮初始位置
					if (!first_equ_init_pos) {
						//minute = wheel_pos;
						systmtime.tm_min = wheel_pos;
						Time_Adjust(&systmtime);
					}
					key_2 = 0;
					first_equ_init_pos = 1;
				}
				break;
				
			case 2:
				update_chars_time(time);
			
				wheel_pos = get_wheel_position(60);	
				if (wheel_pos != wheel_init_pos) {
					time[6] = '0' + wheel_pos / 10;
					time[7] = '0' + wheel_pos % 10;
					
					first_equ_init_pos = 0;
				}
				else if (!first_equ_init_pos) {
					time[6] = '0' + wheel_pos / 10;
					time[7] = '0' + wheel_pos % 10;
				}
				
				//OLED_show8x16string(33, 3, time);
				OLED_show8x16char(33 + 0*8, 3, time[0]);
				OLED_show8x16char(33 + 1*8, 3, time[1]);
				OLED_show8x16char(33 + 2*8, 3, time[2]);
				OLED_show8x16char(33 + 3*8, 3, time[3]);
				OLED_show8x16char(33 + 4*8, 3, time[4]);
				OLED_show8x16char(33 + 5*8, 3, time[5]);
				OLED_show8x16char_inverse(33 + 6*8, 3, time[6]);
				OLED_show8x16char_inverse(33 + 7*8, 3, time[7]);
				
				if (key_2 == 1) {
					item = 0;
					wheel_init_pos = get_wheel_position(24);	//确定下个设置项的滚轮初始位置
					if (!first_equ_init_pos) {
						//second = wheel_pos;
						systmtime.tm_sec = wheel_pos;
						Time_Adjust(&systmtime);
					}
					key_2 = 0;
					first_equ_init_pos = 1;
				}
				break;
				
			
		}
		
	}
	
	key_1 = 0;	// 按键置位和复位要成对出现 
}


/**
 * @brief	设置闹铃时间
 */
void set_alarm_clock(void)
{
	int wheel_init_pos, first_equ_init_pos = 1;
	wheel_init_pos = get_wheel_position(24);
	
	
	int wheel_pos;
	int item = 0;	//h,m,s
	
	char time[9];
	
	update_chars_alarm(time);
	OLED_show8x16char_inverse(33, 3, time[0]);
	OLED_show8x16char_inverse(33 + 8, 3, time[1]);
	OLED_show8x16string(33 + 2*8, 3, time + 2);
	
	while (key_1 != 1) {
		switch (item) {
			case 0:
				update_chars_alarm(time);
			
				wheel_pos = get_wheel_position(24);	
				if (wheel_pos != wheel_init_pos) {
					time[0] = '0' + wheel_pos / 10;
					time[1] = '0' + wheel_pos % 10;
					
					first_equ_init_pos = 0;
				}
				else if (!first_equ_init_pos) {	//不是第一次等于初始值，意味着转了电位器又转了回来
					time[0] = '0' + wheel_pos / 10;
					time[1] = '0' + wheel_pos % 10;
				}
				
				OLED_show8x16char_inverse(33, 3, time[0]);
				OLED_show8x16char_inverse(33 + 8, 3, time[1]);
				OLED_show8x16char(33 + 2*8, 3, time[2]);
				OLED_show8x16char(33 + 3*8, 3, time[3]);
				OLED_show8x16char(33 + 4*8, 3, time[4]);
				OLED_show8x16char(33 + 5*8, 3, time[5]);
				OLED_show8x16char(33 + 6*8, 3, time[6]);
				OLED_show8x16char(33 + 7*8, 3, time[7]);
				
				if (key_2 == 1) {
					item++;
					wheel_init_pos = get_wheel_position(60);	//确定下个设置项的滚轮初始位置
					if (!first_equ_init_pos)
						alarm_hour = wheel_pos;
					key_2 = 0;
					first_equ_init_pos = 1;
				}
				break;
				
			case 1:
				update_chars_alarm(time);	
			
				wheel_pos = get_wheel_position(60);	
				if (wheel_pos != wheel_init_pos) {
					time[3] = '0' + wheel_pos / 10;
					time[4] = '0' + wheel_pos % 10;
					
					first_equ_init_pos = 0;
				}
				else if (!first_equ_init_pos) {	//不是第一次等于初始值，意味着转了电位器又转了回来
					time[3] = '0' + wheel_pos / 10;
					time[4] = '0' + wheel_pos % 10;
				}
				
				OLED_show8x16char(33 + 0*8, 3, time[0]);
				OLED_show8x16char(33 + 1*8, 3, time[1]);
				OLED_show8x16char(33 + 2*8, 3, time[2]);
				OLED_show8x16char_inverse(33 + 3*8, 3, time[3]);
				OLED_show8x16char_inverse(33 + 4*8, 3, time[4]);
				OLED_show8x16char(33 + 5*8, 3, time[5]);
				OLED_show8x16char(33 + 6*8, 3, time[6]);
				OLED_show8x16char(33 + 7*8, 3, time[7]);
				
				if (key_2 == 1) {
					item++;
					wheel_init_pos = get_wheel_position(60);	//确定下个设置项的滚轮初始位置
					if (!first_equ_init_pos)
						alarm_minute = wheel_pos;
					key_2 = 0;
					first_equ_init_pos = 1;
				}
				break;
				
			case 2:
				update_chars_alarm(time);
			
				wheel_pos = get_wheel_position(60);	
				if (wheel_pos != wheel_init_pos) {
					time[6] = '0' + wheel_pos / 10;
					time[7] = '0' + wheel_pos % 10;
					
					first_equ_init_pos = 0;
				}
				else if (!first_equ_init_pos) {
					time[6] = '0' + wheel_pos / 10;
					time[7] = '0' + wheel_pos % 10;
				}
				
				OLED_show8x16char(33 + 0*8, 3, time[0]);
				OLED_show8x16char(33 + 1*8, 3, time[1]);
				OLED_show8x16char(33 + 2*8, 3, time[2]);
				OLED_show8x16char(33 + 3*8, 3, time[3]);
				OLED_show8x16char(33 + 4*8, 3, time[4]);
				OLED_show8x16char(33 + 5*8, 3, time[5]);
				OLED_show8x16char_inverse(33 + 6*8, 3, time[6]);
				OLED_show8x16char_inverse(33 + 7*8, 3, time[7]);
				
				if (key_2 == 1) {
					item = 0;
					wheel_init_pos = get_wheel_position(24);	//确定下个设置项的滚轮初始位置
					if (!first_equ_init_pos)
						alarm_second = wheel_pos;
					key_2 = 0;
					first_equ_init_pos = 1;
				}
				break;
				
			
		}
		
	}
	
	key_1 = 0;	// 按键置位和复位要成对出现 
}


/**
 * @brief	检测是否闹铃
 */
void check_alarm(void)
{
	if (hour == alarm_hour)
		if (minute == alarm_minute)
			if (second == alarm_second) {
				#ifdef	VET6
				beep_beat(DO_m);
				#endif
				#ifdef	C8T6
				motor_shake();
				#endif
				alarm_ringing = 1;
			}
			
	if (hour == alarm_hour)
		if (minute == alarm_minute + 1)
			if (second == alarm_second) {
				#ifdef	VET6
				stop_beat();
				#endif
				#ifdef	C8T6
				motor_stop();
				#endif
				alarm_ringing = 0;
			}
}

