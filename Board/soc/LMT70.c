/**
 * @brief	测温模块
 */
#define LMT70_GLOBALS

#include "stm32f10x.h"
#include "GLOBAL.h"
#include "LMT70.h"
#include "wheel.h"
#include "oled.h"
#include "clock.h"
#include "buzzer.h"

static char temp_char[4];
 
 
void LMT70_ad_init(void)
{
	#ifdef	VET6
	GPIO_InitTypeDef GPIO_InitStruct;//修改gpio
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;//使能 GPIOA 时钟
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;//配置模拟输入
	GPIO_Init(GPIOC, &GPIO_InitStruct);//初始化ADC通道引脚
	#endif
	
	#ifdef	C8T6
	GPIO_InitTypeDef GPIO_InitStruct;//修改gpio
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;//使能 GPIOA 时钟
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;//配置模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStruct);//初始化ADC通道引脚
	#endif
	
	ADC_InitTypeDef ADC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;//单次转换
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_NbrOfChannel=1;
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;//单通道模式
	ADC_Init(ADC2, &ADC_InitStruct);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_Cmd(ADC2, ENABLE);
	ADC_ResetCalibration(ADC2);
	while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2));
	
}


/**
  * @brief  获得 ADC 值
  * @param  Ch ：通道值 0~3
  * @retval None
  */
uint16_t Get_Adc_sqr(uint8_t Ch, int sqr)
{
	//通道 3,规则采样顺序值为 1,采样时间为 55.5 周期
	ADC_RegularChannelConfig(ADC2, Ch, sqr, ADC_SampleTime_55Cycles5 );
	ADC_SoftwareStartConvCmd(ADC2, ENABLE); 						//使能软件转换功能
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));					//等待转换结束
	return ADC_GetConversionValue(ADC2); 							//返回最近一次 ADC1 规则组的转换结果
}


/**
  * @brief  计算平均值
  * @param  Ch 		：通道值 0~3 
			Times ：取Times次的平均 
  * @retval None
  */
uint16_t Get_Adc_Average_sqr(uint8_t Ch, uint8_t Times, int sqr)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<Times;t++)
	{ 
		temp_val+=Get_Adc_sqr(Ch, sqr);
		//delay_ms(5);
	}
	return temp_val/Times;		//求平均
}


/**
 * @brief	温度入列
 */
static void temp_que_in(float temper) 
{
	for (int i = 0; i < 13; i ++) {
		temp_que[i] = temp_que[i + 1];
	}
	temp_que[13] = temper;
}

static void temp_aver_calcu(float temper)
{
	static int cnt = 0;
	
	temp_que_in(temper);
	cnt++;
	
	if (cnt == 14) {
		cnt = 0;
		
		float sum = 0;
		for (int i = 0; i < 14; i++)
			sum += temp_que[i];
		sum /= 14;
		
		for (int i = 0; i < 9; i++)
			temp_aver[i] = temp_aver[i + 1];
		temp_aver[9] = sum;
		
	}
	
}


void get_temp(void)
{
	#ifdef	VET6
	temp = 1000 * (float)Get_Adc_Average_sqr(12, 3, 1) / 4095.0f * 3.3;
	#endif
	#ifdef	C8T6
	temp = 1000 * (float)Get_Adc_Average_sqr(2, 3, 1) / 4095.0f * 3.3;
	#endif
	temp = 204.6398-temp*temp*7.857923/1000000-temp*0.1777501;
	temp_que_in(temp);
}

void oled_print_temp(void)
{
	
	//*float tem, last_tem;
	
	while (1) {
		/*
		OLED_show6x8string(31, 0, "Temperature");
		
		tem = temp;
		tem *= 100;
		
		if (tem != last_tem) {
			for (int i = 0; i < 4; i++) {
				temp_char[4 - i - 1] = '0' + ((int)tem % 10);
				tem /= 10;
			}
			
			OLED_show8x16char(37, 3, temp_char[0]);
			OLED_show8x16char(37 + 8, 3, temp_char[1]);
			OLED_show8x16char(37 + 8*2, 3, '.');
			OLED_show8x16char(33 + 8*4, 3, temp_char[2]);
			OLED_show8x16char(33 + 8*5, 3, temp_char[3]);
			
			last_tem = tem;
		}
		*/
		
		OLED_Time(hour, minute, second);
		OLED_Temperature(temp);
		
		if (key_2 == 1) {
			key_2 = 0;
			clock_menu();
		}
	}
		
}



/**
 * @brief	设置温度阈值
 */
void set_temp_threshold(void)
{
	int wheel_init_pos, first_equ_init_pos = 1;
	int wheel_pos;
	int item = 0;
	int inter_part, float_part;
	char temp_threshold_chars[3];
	
	inter_part = (int)temp_threshold;
	float_part = temp_threshold - inter_part;
	
	temp_threshold_chars[0] = '0' + temp_threshold / 10;
	temp_threshold_chars[1] = '0' + (int)temp_threshold % 10;
	temp_threshold_chars[2] = '0' + (int)(temp_threshold*10) % 10;
	
	OLED_show6x8string(0, 0, "Set Temp Threshold");
	
	OLED_show8x16char(52, 3, temp_threshold_chars[0]);
	OLED_show8x16char(52 + 8, 3, temp_threshold_chars[1]);
	OLED_show8x16char(52 + 2*8, 3, '.');
	OLED_show8x16char(52 + 4*8, 3, temp_threshold_chars[2]);
	OLED_show8x16char(52 + 6*8, 3, 'd');
	
	wheel_init_pos = get_wheel_position(60);
	
	key_1 = 0;		//进入循环之前要把按键标志位置零
	key_2 = 0;
	while (key_1 != 1) {
		switch (item) {
			case 0:
				wheel_pos = get_wheel_position(60);
				if (wheel_pos != wheel_init_pos) {
					temp_threshold_chars[0] = '0' + wheel_pos / 10;
					temp_threshold_chars[1] = '0' + wheel_pos % 10;
					
					first_equ_init_pos = 0;
				}	
				else if (!first_equ_init_pos) {
					temp_threshold_chars[0] = '0' + wheel_pos / 10;
					temp_threshold_chars[1] = '0' + wheel_pos % 10;
				}	
				
				OLED_show8x16char_inverse(52, 3, temp_threshold_chars[0]);
				OLED_show8x16char_inverse(52 + 8, 3, temp_threshold_chars[1]);
				OLED_show8x16char(52 + 4*8, 3, temp_threshold_chars[2]);
				
				if (key_2 == 1) {
					inter_part = wheel_pos;
					temp_threshold = inter_part + float_part * 0.1;
					item = 1;
					key_2 = 0;
					wheel_init_pos = get_wheel_position(9);
				}
				break;
				
				
			case 1:
				wheel_pos = get_wheel_position(9);
				if (wheel_pos != wheel_init_pos) {
					temp_threshold_chars[2] = '0' + wheel_pos;
					
					first_equ_init_pos = 0;
				}	
				else if (!first_equ_init_pos) {
					temp_threshold_chars[2] = '0' + wheel_pos;
				}	
				
				OLED_show8x16char(52, 3, temp_threshold_chars[0]);
				OLED_show8x16char(52 + 8, 3, temp_threshold_chars[1]);
				OLED_show8x16char_inverse(52 + 4*8, 3, temp_threshold_chars[2]);
				//OLED_show8x16char(33 + 2*8, 3, 's');
				
				if (key_2 == 1) {
					float_part = wheel_pos;
					temp_threshold = inter_part + float_part * 0.1;
					item = 0;
					key_2 = 0;
					wheel_init_pos = get_wheel_position(60);
				}
				break;
				
		}
	}
	
	key_1 = 0;
}

/**
 * @brief	设置温度阈值初始值
 * @note	单位摄氏度
 */
void temp_thrshold_init(float temp_thre)
{
	temp_threshold = temp_thre;
}


/**
 * @brief	检测温度是否超过阈值
 */
void check_temp(void)
{
	if (alarm_ringing == 0) {
		if (temp >= temp_threshold)
			#ifdef	VET6
			beep_beat(DO_m);
			#endif
			#ifdef	C8T6
			motor_shake();
			#endif
		else
			#ifdef	VET6
			stop_beat();
			#endif
			#ifdef	C8T6
			motor_stop();
			#endif
}
}


/**
 * @brief	画实时温度折线
 */
#define	TEMP_L	28
#define	TEMP_H	38
void draw_real_temp_line(void)
{
	int y_f, y_l;
	int num_y;
	float tem, last_tem;
	int wheel_pos;
	
	key_1 = 0;
	while (key_1 != 1) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 128; j++)
				oled_gram[j][i] = 0;
		
		wheel_pos = get_wheel_position(2);
		
		switch (wheel_pos) {
			case 0:
				for (int i = 0; i < 13; i++) {
					y_f = (int)((temp_que[i]-TEMP_L) * 64/(TEMP_H - TEMP_L));
					y_l = (int)((temp_que[i+1]-TEMP_L) * 64/(TEMP_H - TEMP_L));
					
					if (y_f < 0)
						y_f = 0;
					else if (y_f > 63)
						y_f = 63;
					
					if (y_l < 0)
						y_l = 0;
					else if (y_l > 63)
						y_l = 63;
					
					oled_gram_line_update(8*(i + 1), 63-y_f, 8*(i + 2), 63-y_l);
					//oled_gram_line_update(8*(i + 1), 63-2*(i+1), 8*(i + 2), 63-2*(i+2));
				}
				
				
				draw_coordinate(15, TEMP_H - TEMP_L);
				
				
				tem = temp;
				tem *= 100;
				if (tem != last_tem) {
					for (int i = 0; i < 4; i++) {
						temp_char[4 - i - 1] = '0' + ((int)tem % 10);
						tem /= 10;
					}
					
					if (y_l > 32)
						num_y = (63 - y_l) / 8 + 1;
					else
						num_y = (63 - y_l) / 8 - 1;
					
					OLED_show6x8char(87, num_y, temp_char[0]);
					OLED_show6x8char(87 + 8, num_y, temp_char[1]);
					OLED_show6x8char(87 + 8*2, num_y, '.');
					OLED_show6x8char(87 + 8*3, num_y, temp_char[2]);
					OLED_show6x8char(87 + 8*4, num_y, temp_char[3]);
					
					OLED_show6x8string(64, 0, "reatime");
					
					last_tem = tem;
				}
				break;
				
			case 1: ;
			case 2: 
				for (int i = 0; i < 9; i++) {
					y_f = (int)((temp_aver[i]-TEMP_L) * 64/(TEMP_H - TEMP_L));
					y_l = (int)((temp_aver[i+1]-TEMP_L) * 64/(TEMP_H - TEMP_L));
					
					if (y_f < 0)
						y_f = 0;
					else if (y_f > 63)
						y_f = 63;
					
					if (y_l < 0)
						y_l = 0;
					else if (y_l > 63)
						y_l = 63;
					
					oled_gram_line_update(8*(i + 1), 63-y_f, 8*(i + 2), 63-y_l);
					//oled_gram_line_update(8*(i + 1), 63-2*(i+1), 8*(i + 2), 63-2*(i+2));
				}
				
				
				draw_coordinate(11, TEMP_H - TEMP_L);
				
				
				tem = temp;
				tem *= 100;
				if (tem != last_tem) {
					for (int i = 0; i < 4; i++) {
						temp_char[4 - i - 1] = '0' + ((int)tem % 10);
						tem /= 10;
					}
					
					if (y_l > 32)
						num_y = (63 - y_l) / 8 + 1;
					else
						num_y = (63 - y_l) / 8 - 1;
					
					OLED_show6x8char(87, num_y, temp_char[0]);
					OLED_show6x8char(87 + 8, num_y, temp_char[1]);
					OLED_show6x8char(87 + 8*2, num_y, '.');
					OLED_show6x8char(87 + 8*3, num_y, temp_char[2]);
					OLED_show6x8char(87 + 8*4, num_y, temp_char[3]);
					
					OLED_show6x8string(64, 0, "aver");
					
					last_tem = tem;
				}
			
		}
	}
	
}

