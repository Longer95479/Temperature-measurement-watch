#ifndef	LMT70_H
#define LMT70_H

void LMT70_ad_init(void);
void get_temp(void);
void oled_print_temp(void);

/**
 * @brief	设置温度阈值
 */
void set_temp_threshold(void);

/**
 * @brief	设置温度阈值初始值
 * @note	单位摄氏度
 */
void temp_thrshold_init(float temp_thre);

/**
 * @brief	检测温度是否超过阈值
 */
void check_temp(void);

/**
 * @brief	画实时温度折线
 */
void draw_real_temp_line(void);

#endif
