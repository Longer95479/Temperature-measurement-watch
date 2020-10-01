#ifndef _ADC_H_
#define _ADC_H_
#include "stm32f10x.h"
#include "GLOBAL.h"
#define adc_select_mode 2


#if adc_select_mode==1
#define adc_dma_channel DMA1_Channel1
#define adc_size    40
#define adc_pin0   GPIO_Pin_0
#define adc_pin1   GPIO_Pin_1
#define adc_pin2   GPIO_Pin_2
#define adc_pin3   GPIO_Pin_3
#define adc_pin4   GPIO_Pin_4
#define adc_pin5   GPIO_Pin_5
#define adc_pin6   GPIO_Pin_6
#define adc_pin7   GPIO_Pin_7
 
#define adc_gpio   GPIOA
#define adc  ADC1
#define RCC_ADC1_APB2Periph              RCC_APB2Periph_ADC1
#define DMA_IRQn DMA1_Channel1_IRQn
void adc_dma_init(void);
void adc_config_Init(void);
void adc_gpio_Init(void);
static void NVIC_DMA_config(void);
#elif  adc_select_mode==2

#ifdef	VET6

#define adc_dma_channel DMA1_Channel1
#define adc_pin3   GPIO_Pin_5

#define adc_gpio   GPIOC
#define adc  ADC1
#define RCC_ADC1_APB2Periph              RCC_APB2Periph_ADC1
#define adc_IRQn ADC1_2_IRQn

#endif

#ifdef	C8T6
#define adc_dma_channel DMA1_Channel1
#define adc_pin3   GPIO_Pin_3

#define adc_gpio   GPIOA
#define adc  ADC1
#define RCC_ADC1_APB2Periph              RCC_APB2Periph_ADC1
#define adc_IRQn ADC1_3_IRQn
#endif

void adc_config_Init(void);
void adc_gpio_Init(void);

#endif

void Get_more_Adc_Average(u16 Times,u16 channel,u16 ad[Times][channel],float adtrue[channel]);
uint16_t Get_Adc(uint8_t Ch);
uint16_t Get_Adc_Average(uint8_t Ch,uint8_t Times);

/**
 * @brief	根据给定的组数，得到电位器的位置
 */
int get_wheel_position(int group);
#endif






