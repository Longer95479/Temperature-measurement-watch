#ifndef _CLOCK_H
#define _CLOCK_H

/**
 * @brief	设置时钟的初始时间
 */
void clock_init(void);

/**
 * @brief	时钟更新
 * @note	放在任务中，每秒执行一次
 */
void update_clock(void);


/**
 * @brief	oled显示时间
 */
void oled_draw_clock(void);

/**
 * @brief	设置时间
 */
void set_clock(void);

/**
 * @brief	设置闹铃时间
 */
void set_alarm_clock(void);

/**
 * @brief	时钟菜单
 */
void clock_menu(void);

/**
 * @brief	检测是否闹铃
 */
void check_alarm(void);

#endif
