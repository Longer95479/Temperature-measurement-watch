#ifndef _TIM_H_
#define _TIM_H_

#include "stm32f10x.h"
#include "GLOBAL.h"

/**********************************ע��*****************************************
* TIM1��TIM8Ϊ�߼���ʱ��������������APB2��
* TIM2��3��4��5Ϊͨ�ö�ʱ��������������APB1��

* ��ʹ�ò�ͬ�Ķ�ʱ��ʱ����Ӧ��GPIO��һ������Ҫ�޸�
* ����ʹ�õ�TIM4��ͨ��4
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
 * @brief	��������
 */
void beep_beat(uint16_t tone);

/**
 * @brief	������ͣ
 */
void stop_beat(void);



/*****************ͨ�ö�ʱ��TIM�����궨�壬ֻ��TIM2��3��4��5********************/
#ifdef	VET6

#define		GENERAL_TIM									TIM4
#define 	GENERAL_TIM_APBxClock_FUN		RCC_APB1PeriphClockCmd
#define 	GENERAL_TIM_CLK							RCC_APB1Periph_TIM4
#define   GENERAL_TIM_Period          2000-1
#define   GENERAL_TIM_Prescaler       71

//TIMͨ��4
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

//TIMͨ��2
#define     GENERAL_TIM_CH4_GPIO_CLK      	RCC_APB2Periph_GPIOB
#define     GENERAL_TIM_CH4_PORT          	GPIOB
#define     GENERAL_TIM_CH4_PIN           	GPIO_Pin_10
#endif

void motor_init(void);
void motor_shake(void);
void motor_stop(void);

#endif
