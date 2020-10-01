/**
 * @brief       ȫ�ֱ��������ļ�
 * @note        ��Ӧ�ļ��ĺ궨��һ��Ҫ���ڴ�ͷ�ļ��İ���(#include)֮ǰ
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
 * @brief	ʱ���֡���
 */
CLOCK_EXT uint8_t hour, minute, second;

/**
 * @brief	����ʱ���ʱ���֡���
 */
CLOCK_EXT uint8_t alarm_hour, alarm_minute, alarm_second;

/**
 * @brief	�������־λ
 */
CLOCK_EXT uint8_t alarm_ringing;

#ifdef	KEY_GLOBALS
#define KEY_EXT
#else
#define KEY_EXT	extern
#endif	//KEY_GLOBALS
/**
 * @brief	�����Ƿ���
 */
KEY_EXT int8_t key_1, key_2;


#ifdef	LMT70_GLOBALS
#define	LMT70_EXT
#else
#define LMT70_EXT	extern
#endif	//LMT70_GLOBALS
/**
 * @brief	�¶�ֵ
 */
LMT70_EXT float temp;

/**
 * @brief	�¶���ֵ
 */
LMT70_EXT float temp_threshold;

/**
 * @brief	һ�����ڵ��¶�ֵ��5��һ������
 */
LMT70_EXT	float temp_que[14];

/**
 * @brief	ʮ������ÿ���ӵ�ƽ���¶�
 */
LMT70_EXT	float temp_aver[10];


#ifdef	OLED_GLOBALS
#define OLED_EXT	
#else
#define	OLED_EXT	extern
#endif	//OLED_GLOBALS
/**
 * @brief	Ϣ��ʱ��
 */
OLED_EXT int rest_screen_time;

/**
 * @brief	����ͼ����
 */
OLED_EXT uint8_t oled_gram[128][8];



#ifdef	RTC_GLOBALS
#define RTC_EXT	
#else
#define	RTC_EXT	extern
#endif	//RTC_GLOBALS


/**
 * @RTCʱ��ṹ��
 */
RTC_EXT struct rtc_time systmtime;



