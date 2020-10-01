#ifndef __KEY_H
#define __KEY_H	 

#include "GLOBAL.h"

#ifdef	VET6
#define KEY1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14) //读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15) //读取按键2  
#endif

#ifdef	C8T6
#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) //读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) //读取按键2  
#endif

void key_init(void);//IO初始化
				    
#endif

