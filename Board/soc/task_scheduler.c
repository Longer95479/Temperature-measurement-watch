/**
* @Platform    Keil 5
* @Fielname    task_scheduler.c
* @brief       ���������
* @Author      Longer95479
* @Email       371573369@qq.com
* @Date        2020/9/22
*
*/

/*!
* @file_note    ��������ȫ��ִ��һ���ʱ�䲻�ܳ�������ʱ�ӽ�������
*              ����������ʱ�䷽��ϴ������Ҫ���ڿ����λ��
*              ����ʱ��Խ�̵��������Խ��ǰ
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
 * @brief       ��ʱ��TIM3 HAL��������
 */
static void TIM3_Int_Init(uint16_t arr,uint16_t psc);

/**********************************************************************/
/*************************����ʵ����ʼ����*****************************/
/**********************************************************************/   
/**
* @brief       �������
*/
#define TASK_NUM        8

/**
* @brief       �����Ծ��������Ϊ0ʱ�������������
*/
static uint8_t active_task_num = 0;

/**
* @brief       �����ͨѶ�ṹ�壬1 ��ʾ��Դ�����ɣ�0 ��ʾ��Դ��δ����
*/



/**
* @brief       ������ں�������
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
* @brief       ����ʵ����ʱ����ʼֵ
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
* @brief       ������ƿ�ʵ���������ʼ��
* @note        �е���ں���˳���������ִ�е�˳����Ҫ�������ȼ���������˳�򼴿�
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
* @brief       �������ʱ��������
*/
static void PIT1_start_count(void);
static uint32_t PIT1_get_time(void);

/**********************************************************************/
/*************************������ں���*********************************/
/**********************************************************************/
/**
* @brief       ����0��ں�����һ�����ķ�תһ�εƣ���Ϊ����ֱ�ۼ��
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
* @brief       ����1��ں���
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
* @brief       ����2��ں���
*/
void task2_entry(void *arg)
{
#ifdef  TASK_TIME
    PIT1_start_count();
#endif  //TASK_TIME
    
	if (!KEY1) {	//�͵�ƽ����
		if (key_1 == 2)
			key_1 = 1;
		else
			key_1 = 2;
	}
	
	if (!KEY2) {	//�͵�ƽ����
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
* @brief       ����3��ں��������� theta
*/
void task3_entry(void *arg)
{
	check_alarm();
}


/**
 * @brief				����4��ں���
 */
void task4_entry(void *arg)
{
	get_temp();
	uart_send_temp(temp);
}


/**
 * @brief				����5��ں���
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
 * @brief				����6��ں���
 */
void task6_entry(void *arg)
{
	check_temp();
}


/**
 * @brief				����7��ں���
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
* @brief       ����״̬���º���
* @param       
* @return
* @example
* @note        �ú����������жϺ������Ϊ������ĵĴ�������
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
* @brief       ��������
* @param
* @return
* @exanple
* @note        ��������Ĳ����ڴ˴����Ƕ��壬����ʵ���������������䡣
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
* @brief       ��������жϵĳ�ʼ��
* @param       ms����������
* @return
* @example
* @note        ʹ�� TIM3
*
*/
void task_rhythm_init(uint32_t ms)
{
  TIM3_Int_Init(ms * 10,7199);
}


/**
* @brief       PIT1 ��ʱ����������������ʱ���Ĳ���
* @param      
* @return
* @example
* @note        ʹ�� PIT1 
* @date        2020/7/7
*
*/
static void PIT1_start_count(void)
{
 ; 
}

/**
* @brief       ��ȡ����ʱ��
* @param
* @return      ����ʱ�䣬��λΪ ms��
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
 * @brief	TIM3��ʼ�����ڴ����ж�
 * @note	������Ƶ��=72M/(7199+1)=10kHz,10KHZ��0.1ms��һ����ʱ���жϵ�ʱ��=5000*��1/10��=500ms
 */
static void TIM3_Int_Init(uint16_t arr,uint16_t psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //TIM3��ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ����������������õõ���
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 	TIM_Cmd(TIM3, ENABLE);                    //ʹ��TIMx������	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
			 
}

//��ʱ��3�жϷ������ 
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update))  //���TIM3�����жϷ������
	{
		task_rhythm();
		task_process();
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
	}
}


