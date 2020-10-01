/**
 * @brief       全局变量管理文件
 * @note        对应文件的宏定义一定要放在此头文件的包含(#include)之前
 *
 */
#include "stm32f10x.h"
 
#define PI 3.1415926f

#define C8T6
//#define VET6


#ifdef  CLOCK_GLOBALS
#define CLOCK_EXT 
#else
#define CLOCK_EXT extern
#endif  //CLOCK_GLOBALS
/**
 * @brief	时、分、秒
 */
CLOCK_EXT uint8_t hour, minute, second;

/**
 * @brief	闹钟时间的时、分、秒
 */
CLOCK_EXT uint8_t alarm_hour, alarm_minute, alarm_second;

/**
 * @brief	闹钟响标志位
 */
CLOCK_EXT uint8_t alarm_ringing;

#ifdef	KEY_GLOBALS
#define KEY_EXT
#else
#define KEY_EXT	extern
#endif	//KEY_GLOBALS
/**
 * @brief	按键是否按下
 */
KEY_EXT int8_t key_1, key_2;


#ifdef	LMT70_GLOBALS
#define	LMT70_EXT
#else
#define LMT70_EXT	extern
#endif	//LMT70_GLOBALS
/**
 * @brief	温度值
 */
LMT70_EXT float temp;

/**
 * @brief	温度阈值
 */
LMT70_EXT float temp_threshold;

/**
 * @brief	一分钟内的温度值，5秒一个数据
 */
LMT70_EXT	float temp_que[14];

/**
 * @brief	十分钟内每分钟的平均温度
 */
LMT70_EXT	float temp_aver[10];


#ifdef	OLED_GLOBALS
#define OLED_EXT	
#else
#define	OLED_EXT	extern
#endif	//OLED_GLOBALS
/**
 * @brief	息屏时间
 */
OLED_EXT int rest_screen_time;

/**
 * @brief	折线图数组
 */
OLED_EXT uint8_t oled_gram[128][8];



#ifdef	RTC_GLOBALS
#define RTC_EXT	
#else
#define	RTC_EXT	extern
#endif	//RTC_GLOBALS


/**
 * @RTC时间结构体
 */
RTC_EXT struct rtc_time systmtime;



