#include"wheel.h"
#include"delay.h"
u16 ad_value[5][8];//�ɼ�AD�ĳ�ʼֵ


#if adc_select_mode==1//��ͨ���ɼ�
void adc_gpio_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ�� GPIOA ʱ��
	GPIO_InitStruct.GPIO_Pin=adc_pin0|adc_pin7|adc_pin6|adc_pin5|adc_pin4|adc_pin3|adc_pin2|adc_pin1;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;//����ģ������
	GPIO_Init( adc_gpio, &GPIO_InitStruct);//��ʼ��8��ADCͨ������

}
void adc_config_Init()
{
	adc_gpio_Init();
	ADC_InitTypeDef ADC_InitStruct;
	ADC_DeInit(adc);//��λADC����Ϊȱʡֵ������ط�����û�и�����
	RCC_APB2PeriphClockCmd(RCC_ADC1_APB2Periph,ENABLE);//����ADC1��Ӧ��ʱ��
    adc_dma_init();//DMA��ʼ��
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;//ADC��Ϊ����ģʽ
	ADC_InitStruct.ADC_ContinuousConvMode=ENABLE;//����ת��
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;//ADC����Ϊ�Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStruct.ADC_NbrOfChannel=8;//ת��ͨ���ĸ���Ϊ8��
	ADC_InitStruct.ADC_ScanConvMode=ENABLE;//ʹ��ɨ��ģʽ����ͨ��ģʽ��
	ADC_Init(adc, &ADC_InitStruct);//��ʼ��ADC�ṹ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//���� ADC ��Ƶ���� 6 (72M/6=12,ADC ���ʱ�Ӳ��ܳ��� 14M)
	ADC_Cmd(adc, ENABLE);
	// ���� ADC ͨ����ת��˳��Ͳ���ʱ�� 
	ADC_RegularChannelConfig(adc, ADC_Channel_0, 1,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_1, 2,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_2, 3,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_3, 4,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_4, 5,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_5, 6,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_6, 7,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_7, 8,ADC_SampleTime_239Cycles5);
	ADC_ResetCalibration(adc);//������λУ׼
	while(ADC_GetResetCalibrationStatus(adc));//�ȴ���λУ׼����
	ADC_StartCalibration(adc);//���� AD У׼
	while(ADC_GetCalibrationStatus(adc));//�ȴ�У׼����	
	ADC_DMACmd(adc, ENABLE); // ʹ�� ADC DMA ���� 
	DMA_Cmd(adc_dma_channel, ENABLE);  //ʹ��DMA��Ӧ��ͨ�� 
	ADC_SoftwareStartConvCmd(adc, ENABLE);// ����û�в����ⲿ����������ʹ��������� ADC ת�� 
}

	void adc_dma_init()
{
	DMA_InitTypeDef DMA_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//ʹ�ܶ�Ӧ��DMA1��ʱ��
	// ��λ DMA ������
    DMA_DeInit(adc_dma_channel); 
	DMA_InitStruct.DMA_BufferSize=adc_size;//// ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;//����Դ������
	DMA_InitStruct.DMA_M2M =DMA_M2M_Disable;//// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢�� 
	DMA_InitStruct.DMA_MemoryBaseAddr=(u32)(&ad_value);//�洢����ַ
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;//�����ڴ����ݴ�С��������������ͬ
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;//�ڴ��ַ����
	DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;//ѭ������ģʽ
	DMA_InitStruct.DMA_PeripheralBaseAddr= (u32)&(ADC1->DR) ;//�����ַ
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;//�������ݴ�С
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable; // ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStruct.DMA_Priority=DMA_Priority_High;// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ�� DMAͨ��ʱ�����ȼ����ò�Ӱ�� 	
	NVIC_DMA_config();
    DMA_ITConfig( DMA1_Channel1,DMA_IT_TC, ENABLE);//DMA��������ж�
	DMA_Init(adc_dma_channel, & DMA_InitStruct); // ��ʼ�� DMA 

}
static void NVIC_DMA_config()
{
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel=DMA_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;	
	NVIC_Init(&NVIC_InitStruct);
}

#elif adc_select_mode==2//��ͨ���ɼ������ڵ�ͨ��������ADC��ͨ�����û�����ͬ����ͬ�����þͲ�����ע��





void adc_gpio_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;//�޸�gpio
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,DISABLE);
	GPIO_InitStruct.GPIO_Pin=adc_pin3;//ʹ�� GPIOA ʱ��
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;//����ģ������
	GPIO_Init( adc_gpio, &GPIO_InitStruct);//��ʼ��8��ADCͨ������

}
void adc_config_Init()
{
	adc_gpio_Init();
	ADC_InitTypeDef ADC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_ADC1_APB2Periph,ENABLE);
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;//����ת��
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_NbrOfChannel=1;
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;//��ͨ��ģʽ
	ADC_Init(adc, &ADC_InitStruct);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_Cmd(adc, ENABLE);
	ADC_ResetCalibration(adc);
	while(ADC_GetResetCalibrationStatus(adc));
	ADC_StartCalibration(adc);
	while(ADC_GetCalibrationStatus(adc));
	
}



#endif
/**
  * @brief  ��� ADC ֵ
  * @param  Ch ��ͨ��ֵ 0~3
  * @retval None
  */
uint16_t Get_Adc(uint8_t Ch)
{
	//ͨ�� 3,�������˳��ֵΪ 1,����ʱ��Ϊ 55.5 ����
	ADC_RegularChannelConfig(adc, Ch, 1, ADC_SampleTime_55Cycles5 );
	ADC_SoftwareStartConvCmd(adc, ENABLE); 						//ʹ�����ת������
	while(!ADC_GetFlagStatus(adc, ADC_FLAG_EOC ));					//�ȴ�ת������
	return ADC_GetConversionValue(adc); 							//�������һ�� ADC1 �������ת�����
}
/**
  * @brief  ����ƽ��ֵ
  * @param  Ch 		��ͨ��ֵ 0~3 
			Times ��ȡTimes�ε�ƽ�� 
  * @retval None
  */
uint16_t Get_Adc_Average(uint8_t Ch,uint8_t Times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<Times;t++)
	{ 
		temp_val+=Get_Adc(Ch);
		//delay_ms(5);
	}
	return temp_val/Times;		//��ƽ��
}

/**
  * @brief  ��ͨ������ƽ��ֵ
  * @param  channel ����ֵ�˲���ͨ����
			Times ��ȡTimes�ε�ƽ�� 
			ad[Times][channel]:adc�ɼ��ĳ�ʼֵ
			adtrue[channel]�� �˲���ĵ�ѹֵ
  * @retval None
  */
void Get_more_Adc_Average(u16 Times,u16 channel,u16 ad[Times][channel],float adtrue[channel])
{
	u8 i,j;
	for(j=0;j<channel;j++)

	{	
		for(i=0;i<Times;i++)//adֵ������
		{
		  adtrue[j]+=ad[i][j];		
		}
		adtrue[j]=adtrue[j]/(5*4096*1.0)*3.3;//��ֵ�˲�
	}
}


/**
 * @brief	���ݸ������������õ���λ����λ��
 */
int get_wheel_position(int group)
{
	#ifdef	VET6
	int advalue=Get_Adc_Average(15,5);//��ͨ���ɼ�adֵ
	#endif
	#ifdef	C8T6
	int advalue=Get_Adc_Average(3,5);//��ͨ���ɼ�adֵ
	#endif
	return advalue / (4096 / group);
	
}
	




