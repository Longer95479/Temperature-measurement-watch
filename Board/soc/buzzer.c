/**********************PWM信号 周期和占空比的计算*********************
* ARR：自动重装载寄存器的值
* CLK_CNT：计数器的时钟，等于 clk_int / (psc+1) = 72M/(psc+1)
* PWM 信号的周期 T = (ARR + 1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
* 占空比P = (ARR+1-CCR)/(ARR+1)     
******************************************************************/

#include "buzzer.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"


/**
  * @brief	通用定时器GPIO配置
  *	@param	无
  *	@retval 无
  */
static void GENERAL_TIM_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO初始化结构体
	//输出比较通道4 GPIO初始化
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK, ENABLE);	 //使能外设总线时钟
	GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH4_PIN;		//设置GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //设置输出频率
	GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);	 //初始化GPIO
}

/**
  * @brief	通用定时器PWM模式配置
  *	@param	无
  *	@retval 无
  */
static void GENERAL_TIM_Mode_Config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//定时器时基结构体
	TIM_OCInitTypeDef  TIM_OCInitStructure;	 //定时器输出比较结构体
	
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);	 //使能定时器时钟
	
	/*--------------------时基结构体初始化-------------------------*/
	// 配置周期，这里配置为0.002s
	TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM_Period;	//自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;	 //驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		//时钟分频因子 ，输出互补脉冲配置死区时间需要用到
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	//设置重复计数器的值为0，不设置重复计数
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);  //初始化定时器

	/*--------------------输出比较结构体初始化-------------------*/	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //使能输出比较
	TIM_OCInitStructure.TIM_Pulse = 0;  //设置初始占空比
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //输出极性:TIM输出比较极性高(高电平有效)
	TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);  //初始化输出比较通道4
	TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);  //使能通道4的CCR4上的预装载寄存器
	TIM_Cmd(GENERAL_TIM, ENABLE);  //使能TIM
}

/**
  * @brief	定时器PWM模式初始化
  *	@param	无
  *	@retval 无
  */
void TIM_PWM_Init()
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_Mode_Config();
}

/**
 * @brief	蜂鸣器响
 */
void beep_beat(uint16_t tone)
{
	TIM_SetAutoreload(GENERAL_TIM, tone);
	TIM_SetCompare4(GENERAL_TIM, tone/2);
}

/**
 * @brief	蜂鸣器停
 */
void stop_beat(void)
{
	TIM_SetAutoreload(GENERAL_TIM, 0);
	TIM_SetCompare4(GENERAL_TIM, 0);
}



#ifdef	C8T6

void motor_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO初始化结构体
	//输出比较通道4 GPIO初始化
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK, ENABLE);	 //使能外设总线时钟
	GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH4_PIN;		//设置GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //设置输出频率
	GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);	 //初始化GPIO
	
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

