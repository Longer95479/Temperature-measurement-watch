#ifndef _TIM_H_
#define _TIM_H_

#include "stm32f10x.h"
#include "GLOBAL.h"

/**********************************注意*****************************************
* TIM1和TIM8为高级定时器，在外设总线APB2上
* TIM2、3、4、5为通用定时器，在外设总线APB1上

* 当使用不同的定时器时，对应的GPIO不一样，需要修改
* 这里使用的TIM4的通道4
******************************************************************************/

//low
#define DO_l 3817
#define RE_l 3401
#define MI_l 3030
#define FA_l 2865
#define SO_l 2551
#define LA_l 2273
#define SI_l 2024
//midumn
#define DO_m 1912
#define RE_m 1704
#define MI_m 1517
#define FA_m 1433
#define SO_m 1276
#define LA_m 1136
#define SI_m 1012
//high
#define DO_h 956
#define RE_h 851
#define MI_h 759
#define FA_h 716
#define SO_h 638
#define LA_h 568
#define SI_h 508

/**
 * @brief	蜂鸣器响
 */
void beep_beat(uint16_t tone);

/**
 * @brief	蜂鸣器停
 */
void stop_beat(void);



/*****************通用定时器TIM参数宏定义，只限TIM2、3、4、5********************/
#ifdef	VET6

#define		GENERAL_TIM									TIM4
#define 	GENERAL_TIM_APBxClock_FUN		RCC_APB1PeriphClockCmd
#define 	GENERAL_TIM_CLK							RCC_APB1Periph_TIM4
#define   GENERAL_TIM_Period          2000-1
#define   GENERAL_TIM_Prescaler       71

//TIM通道4
#define     GENERAL_TIM_CH4_GPIO_CLK      	RCC_APB2Periph_GPIOB
#define     GENERAL_TIM_CH4_PORT          	GPIOB
#define     GENERAL_TIM_CH4_PIN           	GPIO_Pin_9

extern void TIM_PWM_Init(void);

#endif
#ifdef	C8T6
#define		GENERAL_TIM									TIM2
#define 	GENERAL_TIM_APBxClock_FUN		RCC_APB1PeriphClockCmd
#define 	GENERAL_TIM_CLK							RCC_APB1Periph_TIM2
#define   GENERAL_TIM_Period          2000-1
#define   GENERAL_TIM_Prescaler       71

//TIM通道2
#define     GENERAL_TIM_CH4_GPIO_CLK      	RCC_APB2Periph_GPIOB
#define     GENERAL_TIM_CH4_PORT          	GPIOB
#define     GENERAL_TIM_CH4_PIN           	GPIO_Pin_10
#endif

void motor_init(void);
void motor_shake(void);
void motor_stop(void);

#endif
