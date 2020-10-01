#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include "stm32f10x.h"

/**
 * @brief       ������������̺궨��
 */
#define CREAT_SUBTASK                   static subtask_status_t subtask_status = SUBTASK_STATUS0;
#define SUBTASK_BEGIN                   switch(subtask_status) {
#define SUBTASK_CASE                    case                    //���ô˾�ʱ�ǵ����ð��
#define SUBTASK_BREAK                   break;
#define SUBTASK_END                     }
#define SUBTASK_STATUS_CHANGE_TO         subtask_status =        //�����ô˾�ʱĩβ�ǵ���Ӷ���


/**
 * @brief       ����״̬ö��
 */
typedef enum {
  TASK_RUN,
  TASK_DELAY
} task_status_t;

/**
 * @brief       �����ͬ������
 */
typedef struct {
  uint8_t       flag;
  int16_t       int16_val;
  uint16_t      uint16_val;
  int32_t       int32_val;
  uint32_t      uint32_val;
  float         float_val;
} task_commu_t;

/**
 * @brief       ������״̬ö��
 */
typedef enum {
  SUBTASK_STATUS0,
  SUBTASK_STATUS1,
  SUBTASK_STATUS2,
  SUBTASK_STATUS3,
  SUBTASK_STATUS4,
  SUBTASK_STATUS5,
  SUBTASK_STATUS6,
  SUBTASK_STATUS7  
} subtask_status_t;


/**
 * @brief       ������ƾ��
 */
typedef struct task{
  
  task_status_t task_status_flag; //����״̬
  
  uint32_t task_timer;  //�����ʱ����Ϊ0ʱ����ִ�У���λΪ һ��ʱ�ӽ���
  uint32_t task_timer_init_val;  //�����ʱ���ĳ�ʼֵ
  
  void *arg;    //������ں����Ĳ���
  void (*task_entry) (void *arg);       //������ں���  
  
} task_t;



/**
 * @brief       ����״̬���º���
 * @param       
 * @return
 * @example
 * @note        �ú����������жϺ������Ϊ������ĵĴ�������
 *
 */
void task_rhythm(void);


/**
 * @brief       ��������
 * @param
 * @return
 * @exanple
 * @note        ��������Ĳ����ڴ˴����Ƕ��壬����ʵ���������������䡣
 *
 */
void task_process(void);


/**
 * @brief       ��������жϵĳ�ʼ��
 * @param       ms����������
 * @return
 * @example
 * @note        ʹ�� PIT0
 *
 */
void task_rhythm_init(uint32_t ms);

#endif
