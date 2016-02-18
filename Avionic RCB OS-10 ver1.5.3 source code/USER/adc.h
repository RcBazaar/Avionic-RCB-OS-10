#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f10x.h"

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

#define ADC_TRV			2048	//ADC量程
#define ADC_CHNUM		11		//ADC采样通道数
#define ADC_AVGNUM		10		//ADC采样平均次数
#define AD_POWER_DOWN	808		//4.0V对应AD采样值
#define AD_POWER_LEAST	202		//1.0V对应AD

#define AD_BAT			0
#define AD_DL			1
#define AD_DR			2
#define AD_RV			3
#define AD_RH			4
#define AD_SA			5
#define AD_SB			6
#define AD_LV			7
#define AD_LH			8
#define AD_SD			9
#define AD_SC			10

extern volatile uint16_t ADC_ConvertedValue[ADC_AVGNUM][ADC_CHNUM];
extern volatile uint16_t ADC_AvgValue[ADC_CHNUM];

void ADC1_DMA_Configuration(void);
void ADCHandler(void);

#endif
