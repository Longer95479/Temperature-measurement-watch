#include"wheel.h"
#include"delay.h"
u16 ad_value[5][8];//采集AD的初始值


#if adc_select_mode==1//多通道采集
void adc_gpio_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能 GPIOA 时钟
	GPIO_InitStruct.GPIO_Pin=adc_pin0|adc_pin7|adc_pin6|adc_pin5|adc_pin4|adc_pin3|adc_pin2|adc_pin1;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;//配置模拟输入
	GPIO_Init( adc_gpio, &GPIO_InitStruct);//初始化8个ADC通道引脚

}
void adc_config_Init()
{
	adc_gpio_Init();
	ADC_InitTypeDef ADC_InitStruct;
	ADC_DeInit(adc);//复位ADC，设为缺省值，这个地方可以没有该配置
	RCC_APB2PeriphClockCmd(RCC_ADC1_APB2Periph,ENABLE);//开启ADC1对应的时钟
    adc_dma_init();//DMA初始化
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;//ADC设为独立模式
	ADC_InitStruct.ADC_ContinuousConvMode=ENABLE;//连续转换
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;//ADC数据为右对齐
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStruct.ADC_NbrOfChannel=8;//转换通道的个数为8个
	ADC_InitStruct.ADC_ScanConvMode=ENABLE;//使能扫描模式（多通道模式）
	ADC_Init(adc, &ADC_InitStruct);//初始化ADC结构体
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//设置 ADC 分频因子 6 (72M/6=12,ADC 最大时钟不能超过 14M)
	ADC_Cmd(adc, ENABLE);
	// 配置 ADC 通道的转换顺序和采样时间 
	ADC_RegularChannelConfig(adc, ADC_Channel_0, 1,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_1, 2,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_2, 3,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_3, 4,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_4, 5,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_5, 6,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_6, 7,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(adc, ADC_Channel_7, 8,ADC_SampleTime_239Cycles5);
	ADC_ResetCalibration(adc);//开启复位校准
	while(ADC_GetResetCalibrationStatus(adc));//等待复位校准结束
	ADC_StartCalibration(adc);//开启 AD 校准
	while(ADC_GetCalibrationStatus(adc));//等待校准结束	
	ADC_DMACmd(adc, ENABLE); // 使能 ADC DMA 请求 
	DMA_Cmd(adc_dma_channel, ENABLE);  //使能DMA对应的通道 
	ADC_SoftwareStartConvCmd(adc, ENABLE);// 由于没有采用外部触发，所以使用软件触发 ADC 转换 
}

	void adc_dma_init()
{
	DMA_InitTypeDef DMA_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//使能对应的DMA1的时钟
	// 复位 DMA 控制器
    DMA_DeInit(adc_dma_channel); 
	DMA_InitStruct.DMA_BufferSize=adc_size;//// 缓冲区大小，应该等于数据目的地的大小
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;//数据源自外设
	DMA_InitStruct.DMA_M2M =DMA_M2M_Disable;//// 禁止存储器到存储器模式，因为是从外设到存储器 
	DMA_InitStruct.DMA_MemoryBaseAddr=(u32)(&ad_value);//存储器地址
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;//设置内存数据大小，与外设数据相同
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;//内存地址递增
	DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;//循环传输模式
	DMA_InitStruct.DMA_PeripheralBaseAddr= (u32)&(ADC1->DR) ;//外设地址
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;//外设数据大小
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable; // 外设寄存器只有一个，地址不用递增
	DMA_InitStruct.DMA_Priority=DMA_Priority_High;// DMA 传输通道优先级为高，当使用一个 DMA通道时，优先级设置不影响 	
	NVIC_DMA_config();
    DMA_ITConfig( DMA1_Channel1,DMA_IT_TC, ENABLE);//DMA传输完成中断
	DMA_Init(adc_dma_channel, & DMA_InitStruct); // 初始化 DMA 

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

#elif adc_select_mode==2//单通道采集，由于单通道配置与ADC多通道配置基本相同，相同的配置就不再有注释





void adc_gpio_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;//修改gpio
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,DISABLE);
	GPIO_InitStruct.GPIO_Pin=adc_pin3;//使能 GPIOA 时钟
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;//配置模拟输入
	GPIO_Init( adc_gpio, &GPIO_InitStruct);//初始化8个ADC通道引脚

}
void adc_config_Init()
{
	adc_gpio_Init();
	ADC_InitTypeDef ADC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_ADC1_APB2Periph,ENABLE);
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;//单次转换
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_NbrOfChannel=1;
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;//单通道模式
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
  * @brief  获得 ADC 值
  * @param  Ch ：通道值 0~3
  * @retval None
  */
uint16_t Get_Adc(uint8_t Ch)
{
	//通道 3,规则采样顺序值为 1,采样时间为 55.5 周期
	ADC_RegularChannelConfig(adc, Ch, 1, ADC_SampleTime_55Cycles5 );
	ADC_SoftwareStartConvCmd(adc, ENABLE); 						//使能软件转换功能
	while(!ADC_GetFlagStatus(adc, ADC_FLAG_EOC ));					//等待转换结束
	return ADC_GetConversionValue(adc); 							//返回最近一次 ADC1 规则组的转换结果
}
/**
  * @brief  计算平均值
  * @param  Ch 		：通道值 0~3 
			Times ：取Times次的平均 
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
	return temp_val/Times;		//求平均
}

/**
  * @brief  多通道计算平均值
  * @param  channel ：均值滤波的通道数
			Times ：取Times次的平均 
			ad[Times][channel]:adc采集的初始值
			adtrue[channel]： 滤波后的电压值
  * @retval None
  */
void Get_more_Adc_Average(u16 Times,u16 channel,u16 ad[Times][channel],float adtrue[channel])
{
	u8 i,j;
	for(j=0;j<channel;j++)

	{	
		for(i=0;i<Times;i++)//ad值五次求和
		{
		  adtrue[j]+=ad[i][j];		
		}
		adtrue[j]=adtrue[j]/(5*4096*1.0)*3.3;//均值滤波
	}
}


/**
 * @brief	根据给定的组数，得到电位器的位置
 */
int get_wheel_position(int group)
{
	#ifdef	VET6
	int advalue=Get_Adc_Average(15,5);//单通道采集ad值
	#endif
	#ifdef	C8T6
	int advalue=Get_Adc_Average(3,5);//单通道采集ad值
	#endif
	return advalue / (4096 / group);
	
}
	




