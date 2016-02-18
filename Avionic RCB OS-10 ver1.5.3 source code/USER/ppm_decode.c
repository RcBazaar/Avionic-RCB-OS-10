#include "ppm_decode.h"

s16 PpmInValue[PPMINNUM];//未标准化的PPM输入信号
s16 Std_PpmInValue[PPMINNUM];//标准PPM输入信号

// PPM输入中断初始化
// 利用TIM4作为计时器，TIM4是发射回调函数定时器
void PpmInCfg(void)
{	
	u8 i;
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);//PAx与中断线EXTIx连接
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = DISABLE;//中断关闭
  	EXTI_Init(&EXTI_InitStructure);
	
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//EXTI9_5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
    
	for(i=0;i<PPMINNUM;i++)
	{
		PpmInValue[i] = 0;
		Std_PpmInValue[i] = PPMIN_NEU;
	}
}

void PpmInCtl(u8 onoff)
{
	u8 i;
	static u8 ppm_onoff = 0;//首次默认是关闭
	EXTI_InitTypeDef EXTI_InitStructure;
	
	if(ppm_onoff==onoff)	return;
	else	ppm_onoff = onoff;
	
	if(onoff)
	{ 
		EXTI_ClearITPendingBit(EXTI_Line9);  //清除LINE9上的中断标志位
		
		EXTI_InitStructure.EXTI_Line = EXTI_Line9;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断开启
		EXTI_Init(&EXTI_InitStructure);
	}
	else
	{
		EXTI_InitStructure.EXTI_Line = EXTI_Line9;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;//中断关闭
		EXTI_Init(&EXTI_InitStructure);
		
		for(i=0;i<PPMINNUM;i++) PpmInValue[i] = 0;		
	}
}
