#ifndef _CLOCK_H
#define _CLOCK_H

/**
 * @brief	����ʱ�ӵĳ�ʼʱ��
 */
void clock_init(void);

/**
 * @brief	ʱ�Ӹ���
 * @note	���������У�ÿ��ִ��һ��
 */
void update_clock(void);


/**
 * @brief	oled��ʾʱ��
 */
void oled_draw_clock(void);

/**
 * @brief	����ʱ��
 */
void set_clock(void);

/**
 * @brief	��������ʱ��
 */
void set_alarm_clock(void);

/**
 * @brief	ʱ�Ӳ˵�
 */
void clock_menu(void);

/**
 * @brief	����Ƿ�����
 */
void check_alarm(void);

#endif
