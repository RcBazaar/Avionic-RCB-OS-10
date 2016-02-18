#include "adc.h"
#include "time.h"

volatile uint16_t ADC_ConvertedValue[ADC_AVGNUM][ADC_CHNUM];
volatile uint16_t ADC_AvgValue[ADC_CHNUM];

void ADC1_DMA_Configuration(void)
{
	//AD_BAT----------------->PA0(ADC_IN0)
	//AD_DL------------------>PA2(ADC_IN2)
	//AD_DR------------------>PA3(ADC_IN3)
	//AD_RV------------------>PA4(ADC_IN4)
	//AD_RH------------------>PA5(ADC_IN5)
	//AD_SD------------------>PA6(ADC_IN6)
	//AD_SC------------------>PA7(ADC_IN7)
	//AD_LV------------------>PC0(ADC_IN10)
	//AD_LH------------------>PC1(ADC_IN11)
	//AD_SA------------------>PC2(ADC_IN12)
	//AD_SB------------------>PC3(ADC_IN13)
	
	GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	/* GPIO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	/* DMA1 clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* ADC1 DeInit */  
	ADC_DeInit(ADC1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADC_AVGNUM*ADC_CHNUM;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
	ADC_InitStructure.ADC_NbrOfChannel = ADC_CHNUM;//转换的通道数
	ADC_Init(ADC1, &ADC_InitStructure); 
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //ADC时钟6分频
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 6, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 7, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 8, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 9, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 10, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 11, ADC_SampleTime_239Cycles5);
	
	ADC_DMACmd(ADC1, ENABLE);//启动ADC的DMA传输
	
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);//开始校准
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//软件启动ADC
}

void ADCHandler(void)
{
	uint8_t i,j;
	uint16_t ADC_SumValue[ADC_CHNUM] = {0,0,0,0,0,0,0,0,0,0,0};
	
	for(i=0;i<ADC_CHNUM;i++)
	{
		for(j=0;j<ADC_AVGNUM;j++)
		{
			ADC_SumValue[i]+=ADC_ConvertedValue[j][i];
		}
	}
	for(i=0;i<ADC_CHNUM;i++)
	{
		ADC_AvgValue[i] = ADC_SumValue[i]/ADC_AVGNUM;
	}
}











