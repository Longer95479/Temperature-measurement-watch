#ifndef __OLED_H__
#define __OLED_H__
#include "stm32f10x.h"
#include "GLOBAL.h"
/*
用户若需要修改引脚，
修改本处宏定义的引脚和
oled_init()函数中的初始化引脚即可
*/
#ifdef	VET6
#define OLED_SCL_H GPIO_SetBits(GPIOB,GPIO_Pin_11)		//SCL引脚输出高电平
#define OLED_SCL_L GPIO_ResetBits(GPIOB,GPIO_Pin_11)	//SCL引脚输出低电平

#define OLED_SDA_H GPIO_SetBits(GPIOB,GPIO_Pin_10)		//SDA引脚输出高电平
#define OLED_SDA_L GPIO_ResetBits(GPIOB,GPIO_Pin_10)	//SDA引脚输出低电平
#endif

#ifdef	C8T6
#define OLED_SCL_H GPIO_SetBits(GPIOB,GPIO_Pin_6)		//SCL引脚输出高电平
#define OLED_SCL_L GPIO_ResetBits(GPIOB,GPIO_Pin_6)	//SCL引脚输出低电平

#define OLED_SDA_H GPIO_SetBits(GPIOB,GPIO_Pin_7)		//SDA引脚输出高电平
#define OLED_SDA_L GPIO_ResetBits(GPIOB,GPIO_Pin_7)	//SDA引脚输出低电平
#endif

/*
说明：初始化oled模块
*/
extern void OLED_init(void);

/*
说明：清除oled显示
*/
extern void OLED_clear(void);

/*
说明：以6*8(8*16)的大小在oled上显示一个字符
参数：x 取值范围0~127
			y 取值范围0~7(8*16时范围为0-6)，与x一起决定显示位置
			character 你要显示的字符
*/
extern void OLED_show6x8char(uint8_t x,uint8_t y,char character);
extern void OLED_show8x16char(uint8_t x,uint8_t y,char character);
extern void OLED_show6x8char_inverse(uint8_t x,uint8_t y,char character);
extern void OLED_show8x16char_inverse(uint8_t x,uint8_t y,char character);
/*
说明：以6*8(8*16)的大小在oled上显示一个以'\0'字符结尾的字符串
参数：x 取值范围0~127
			y 取值范围0~7(8*16时范围为0-6)，与x一起决定显示位置
			string 你要显示的字符串
			
			该函数遇到'\0'字符才会结束显示。
*/
extern void OLED_show6x8string(uint8_t x,uint8_t y,char* string);
extern void OLED_show8x16string(uint8_t x,uint8_t y,char* string);
extern void OLED_show6x8string_inverse(uint8_t x,uint8_t y,char* string);
extern void OLED_show8x16string_inverse(uint8_t x,uint8_t y,char* string);
/*
说明：以6*8(8*16)的大小在oled上显示一个数字
参数：x 取值范围0~127
			y 取值范围0~7(8*16时为0-6)，与x一起决定显示位置
			number 用户要显示的数字，正负均可
*/
extern void OLED_show6x8number(uint8_t x,uint8_t y,int number);
extern void OLED_show8x16number(uint8_t x,uint8_t y,int number);


/*
说明：设置显示位置
参数：x 取值范围0~127
			y 取值范围0~7
*/
extern void OLED_setpos(uint8_t x,uint8_t y);
/*
说明：发送一字节的信息
参数：data 要发送的一字节数据
*/
extern void OLED_writedata(uint8_t data);


void oled_turn_on(void);

void oled_turn_off(void);

/**
 * @breif	初始化息屏时间
 * @note	单位 s
 */
void rest_screen_time_init(int rst);

/**
 * @brief	设置息屏时间
 */
void set_rest_screen_time(void);


void draw_dot(uint8_t x, uint8_t y, uint8_t status);

void draw_oled_gram(void);

void oled_gram_line_update(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void draw_coordinate(int xgrids, int ygrids);

///时间显示传入三个整数分别为时，分秒
void OLED_Time(uint8_t x,uint8_t y,uint8_t z);//时间显示

//温度显示
//传入一个浮点数中右上显示温度
void OLED_Temperature(float x);


#define OLED_COMMAND 	0
#define OLED_DATA 		1


#endif

