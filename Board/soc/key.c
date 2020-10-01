#define KEY_GLOBALS

#include "stm32f10x.h"
#include "GLOBAL.h"
#include "key.h"



/**
 * @brief	������ʼ��
 */
void key_init(void)
{
	GPIO_InitTypeDef GPIO_init_str;
	
	#ifdef	VET6
	//������ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_init_str.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_init_str.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOD, &GPIO_init_str);
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_14|GPIO_Pin_15);
	//������ʼ�����
	#endif
	
	#ifdef	C8T6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_init_str.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_init_str.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_init_str);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);
	//������ʼ�����
	#endif
	
}

