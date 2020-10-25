/**
* @Platform    Keil 5
* @Fielname    task_scheduler.c
* @brief       任务调度器
* @Author      Longer95479
* @Email       371573369@qq.com
* @Date        2020/9/22
*
*/

/*!
* @file_note    各个任务全部执行一遍的时间不能超过任务时钟节拍周期
*              子任务运行时间方差较大的任务要放在靠后的位置
*              运行时间越短的任务放在越靠前
* 
*/

#include "stm32f10x.h"
#include "stdio.h"

#include "GLOBAL.h"
#include "task_scheduler.h"
#include "led.h"
#include "clock.h"
#include "key.h"
#include "LMT70.h"
#include "oled.h"
#include "buzzer.h"

/**
 * @brief       定时器TIM3 HAL函数声明
 */
static void TIM3_Int_Init(uint16_t arr,uint16_t psc);

/**********************************************************************/
/*************************任务实例初始化处*****************************/
/**********************************************************************/   
/**
* @brief       任务个数
*/
#define TASK_NUM        8

/**
* @brief       任务活跃数，当不为0时激活任务调度器
*/
static uint8_t active_task_num = 0;

/**
* @brief       任务间通讯结构体，1 表示资源已生成，0 表示资源还未生成
*/



/**
* @brief       任务入口函数声明
*/
void task0_entry(void *arg);
void task1_entry(void *arg);
void task2_entry(void *arg);
void task3_entry(void *arg);
void task4_entry(void *arg);
void task5_entry(void *arg);
void task6_entry(void *arg);
void task7_entry(void *arg);

/**
* @brief       任务实例定时器初始值
*/
typedef enum {
  TASK0_TIMER_INIT_VAL = 25,
  TASK1_TIMER_INIT_VAL = 200,
  TASK2_TIMER_INIT_VAL = 20,
  TASK3_TIMER_INIT_VAL = 200,
  TASK4_TIMER_INIT_VAL = 100,
  TASK5_TIMER_INIT_VAL = 20,
  TASK6_TIMER_INIT_VAL = 200,
	TASK7_TIMER_INIT_VAL = 20
    
} task_timer_init_val_instance;


/**
* @brief       任务控制块实例，对其初始化
* @note        列的入口函数顺序就是任务执行的顺序，若要调整优先级，调整列顺序即可
*
*/
static task_t tasks[TASK_NUM] = {
  {TASK_DELAY, TASK0_TIMER_INIT_VAL, TASK0_TIMER_INIT_VAL, NULL, task0_entry},
  {TASK_DELAY, TASK1_TIMER_INIT_VAL, TASK1_TIMER_INIT_VAL, NULL, task1_entry},
  {TASK_DELAY, TASK2_TIMER_INIT_VAL, TASK2_TIMER_INIT_VAL, NULL, task2_entry},
  {TASK_DELAY, TASK3_TIMER_INIT_VAL, TASK3_TIMER_INIT_VAL, NULL, task3_entry},
  {TASK_DELAY, TASK4_TIMER_INIT_VAL, TASK4_TIMER_INIT_VAL, NULL, task4_entry},
  {TASK_DELAY, TASK5_TIMER_INIT_VAL, TASK5_TIMER_INIT_VAL, NULL, task5_entry},
  {TASK_DELAY, TASK6_TIMER_INIT_VAL, TASK6_TIMER_INIT_VAL, NULL, task6_entry},
	{TASK_DELAY, TASK7_TIMER_INIT_VAL, TASK7_TIMER_INIT_VAL, NULL, task7_entry}
};   
/***********************************************************************/
/***********************************************************************/

/**
* @brief       子任务计时函数声明
*/
static void PIT1_start_count(void);
static uint32_t PIT1_get_time(void);

/**********************************************************************/
/*************************任务入口函数*********************************/
/**********************************************************************/
/**
* @brief       任务0入口函数，一个节拍翻转一次灯，作为节拍直观监测
*/
void task0_entry(void *arg)
{ 
	static int8_t if_on = 0;
	
	if (if_on) {
		Led_Set(LED1, 1);
		if_on = 0;
	}
	else {
		Led_Set(LED1, 0);
		if_on = 1;
	}
	
  //printf("task0 is running\n");
}


/**
* @brief       任务1入口函数
*/
void task1_entry(void *arg)
{
#ifdef TASK_TIME  
  PIT1_start_count();
#endif  //TASK_TIME
    
	update_clock();
	
#ifdef  TASK_TIME
    printf("task1_subtask0 running time: %u.\n", PIT1_get_time());
#endif  //TASK_TIME
}


/**
* @brief       任务2入口函数
*/
void task2_entry(void *arg)
{
#ifdef  TASK_TIME
    PIT1_start_count();
#endif  //TASK_TIME
    
	if (!KEY1) {	//低电平按下
		if (key_1 == 2)
			key_1 = 1;
		else
			key_1 = 2;
	}
	
	if (!KEY2) {	//低电平按下
		if (key_2 == 2)
			key_2 = 1;
		else
			key_2 = 2;
	}
	
    
#ifdef  TASK_TIME
    printf("task2_subtask0 running time: %u.\n", PIT1_get_time());
#endif
          
}


/**
* @brief       任务3入口函数，生成 theta
*/
void task3_entry(void *arg)
{
	check_alarm();
}


/**
 * @brief				任务4入口函数
 */
void task4_entry(void *arg)
{
	get_temp();
	uart_send_temp(temp);
}


/**
 * @brief				任务5入口函数
 */
#define ON 1
#define OFF 0
void task5_entry(void *arg)
{
	static int cnt = 0;
	static int oled_status = ON;
	
	switch (oled_status) {
		
		case OFF:
			if (key_1 == 1 || key_2 == 1) {
				oled_turn_on();
				oled_status = ON;
				cnt = 0;
				key_1 = 0;
				key_2 = 0;
			}
			break;
		
		case ON:
			cnt++;
			if (key_1 == 1 || key_2 == 1) {
				cnt = 0;
			}
			if (cnt >= 10 * rest_screen_time) {
				oled_turn_off();
				oled_status = OFF;
			}
			break;
	}
	
	
}


/**
 * @brief				任务6入口函数
 */
void task6_entry(void *arg)
{
	check_temp();
}


/**
 * @brief				任务7入口函数
 */
void task7_entry(void *arg)
{
	if (alarm_ringing == 1) {
		if (key_1 == 1) {
			key_1 = 0;
			alarm_ringing = 0;
			#ifdef	VET6
			stop_beat();
			#endif
			#ifdef	C8T6
			motor_stop();
			#endif
		}
	}
}


/***********************************************************************/
/***********************************************************************/





/**
* @brief       任务状态更新函数
* @param       
* @return
* @example
* @note        该函数放置在中断函数里，作为任务节拍的处理内容
*
*/
void task_rhythm(void)
{
  for (int i = 0; i < TASK_NUM; i++) {
    
    if (tasks[i].task_timer != 0) {
      tasks[i].task_timer--;
      
      if(tasks[i].task_timer == 0) {
        tasks[i].task_status_flag = TASK_RUN;
        tasks[i].task_timer = tasks[i].task_timer_init_val;
        active_task_num++;
      }
      
    }    
  }  
  //printf("\n*********\n");      //only for debug
}


/**
* @brief       任务处理函数
* @param
* @return
* @exanple
* @note        任务输入的参数在此处不是定义，而是实例，因此依需求而变。
*
*/
void task_process(void)
{
  while(active_task_num != 0) {
    for (int i = 0; i < TASK_NUM; i++) {
      
      if (tasks[i].task_status_flag == TASK_RUN) {
        tasks[i].task_entry(tasks[i].arg);
        tasks[i].task_status_flag = TASK_DELAY;
        active_task_num--;
      }
      
    }
  }
}

/**
* @brief       任务节拍中断的初始化
* @param       ms：节拍周期
* @return
* @example
* @note        使用 TIM3
*
*/
void task_rhythm_init(uint32_t ms)
{
  TIM3_Int_Init(ms * 10,7199);
}


/**
* @brief       PIT1 计时，用于子任务运行时长的测试
* @param      
* @return
* @example
* @note        使用 PIT1 
* @date        2020/7/7
*
*/
static void PIT1_start_count(void)
{
 ; 
}

/**
* @brief       获取运行时间
* @param
* @return      运行时间，单位为 ms，
* @example
* @note
*
*/
static uint32_t PIT1_get_time(void)
{
	return 0;
}


/*----------------------------------------------------------------------------------*/
/**
 * @brief	TIM3初始化用于触发中断
 * @note	计数器频率=72M/(7199+1)=10kHz,10KHZ是0.1ms，一个定时器中断的时间=5000*（1/10）=500ms
 */
static void TIM3_Int_Init(uint16_t arr,uint16_t psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //TIM3的时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割（互补输出，死区才用得到）
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 	TIM_Cmd(TIM3, ENABLE);                    //使能TIMx计数器	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级,2位响应优先级
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
			 
}

//定时器3中断服务程序 
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update))  //检查TIM3更新中断发生与否
	{
		task_rhythm();
		task_process();
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
	}
}


