#ifndef	LMT70_H
#define LMT70_H

void LMT70_ad_init(void);
void get_temp(void);
void oled_print_temp(void);

/**
 * @brief	�����¶���ֵ
 */
void set_temp_threshold(void);

/**
 * @brief	�����¶���ֵ��ʼֵ
 * @note	��λ���϶�
 */
void temp_thrshold_init(float temp_thre);

/**
 * @brief	����¶��Ƿ񳬹���ֵ
 */
void check_temp(void);

/**
 * @brief	��ʵʱ�¶�����
 */
void draw_real_temp_line(void);

#endif
