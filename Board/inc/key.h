#ifndef __KEY_H
#define __KEY_H	 

#include "GLOBAL.h"

#ifdef	VET6
#define KEY1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14) //��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15) //��ȡ����2  
#endif

#ifdef	C8T6
#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) //��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) //��ȡ����2  
#endif

void key_init(void);//IO��ʼ��
				    
#endif

