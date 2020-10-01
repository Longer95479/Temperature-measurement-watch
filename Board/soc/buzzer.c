/**********************PWM�ź� ���ں�ռ�ձȵļ���*********************
* ARR���Զ���װ�ؼĴ�����ֵ
* CLK_CNT����������ʱ�ӣ����� clk_int / (psc+1) = 72M/(psc+1)
* PWM �źŵ����� T = (ARR + 1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
* ռ�ձ�P = (ARR+1-CCR)/(ARR+1)     
******************************************************************/

#include "buzzer.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"


/**
  * @brief	ͨ�ö�ʱ��GPIO����
  *	@param	��
  *	@retval ��
  */
static void GENERAL_TIM_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO��ʼ���ṹ��
	//����Ƚ�ͨ��4 GPIO��ʼ��
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK, ENABLE);	 //ʹ����������ʱ��
	GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH4_PIN;		//����GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�������Ƶ��
	GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);	 //��ʼ��GPIO
}

/**
  * @brief	ͨ�ö�ʱ��PWMģʽ����
  *	@param	��
  *	@retval ��
  */
static void GENERAL_TIM_Mode_Config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//��ʱ��ʱ���ṹ��
	TIM_OCInitTypeDef  TIM_OCInitStructure;	 //��ʱ������ȽϽṹ��
	
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);	 //ʹ�ܶ�ʱ��ʱ��
	
	/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	// �������ڣ���������Ϊ0.002s
	TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM_Period;	//�Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;	 //����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		//ʱ�ӷ�Ƶ���� ���������������������ʱ����Ҫ�õ�
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	//�����ظ���������ֵΪ0���������ظ�����
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);  //��ʼ����ʱ��

	/*--------------------����ȽϽṹ���ʼ��-------------------*/	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //ʹ������Ƚ�
	TIM_OCInitStructure.TIM_Pulse = 0;  //���ó�ʼռ�ձ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //�������:TIM����Ƚϼ��Ը�(�ߵ�ƽ��Ч)
	TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);  //��ʼ������Ƚ�ͨ��4
	TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);  //ʹ��ͨ��4��CCR4�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(GENERAL_TIM, ENABLE);  //ʹ��TIM
}

/**
  * @brief	��ʱ��PWMģʽ��ʼ��
  *	@param	��
  *	@retval ��
  */
void TIM_PWM_Init()
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_Mode_Config();
}

/**
 * @brief	��������
 */
void beep_beat(uint16_t tone)
{
	TIM_SetAutoreload(GENERAL_TIM, tone);
	TIM_SetCompare4(GENERAL_TIM, tone/2);
}

/**
 * @brief	������ͣ
 */
void stop_beat(void)
{
	TIM_SetAutoreload(GENERAL_TIM, 0);
	TIM_SetCompare4(GENERAL_TIM, 0);
}



#ifdef	C8T6

void motor_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO��ʼ���ṹ��
	//����Ƚ�ͨ��4 GPIO��ʼ��
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK, ENABLE);	 //ʹ����������ʱ��
	GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH4_PIN;		//����GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�������Ƶ��
	GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);	 //��ʼ��GPIO
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}

void motor_shake(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
}

void motor_stop(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}

#endif

