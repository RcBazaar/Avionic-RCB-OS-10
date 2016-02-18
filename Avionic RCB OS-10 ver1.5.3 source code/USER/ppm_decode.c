#include "ppm_decode.h"

s16 PpmInValue[PPMINNUM];//δ��׼����PPM�����ź�
s16 Std_PpmInValue[PPMINNUM];//��׼PPM�����ź�

// PPM�����жϳ�ʼ��
// ����TIM4��Ϊ��ʱ����TIM4�Ƿ���ص�������ʱ��
void PpmInCfg(void)
{	
	u8 i;
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTAʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);//PAx���ж���EXTIx����
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = DISABLE;//�жϹر�
  	EXTI_Init(&EXTI_InitStructure);
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//EXTI9_5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
    
	for(i=0;i<PPMINNUM;i++)
	{
		PpmInValue[i] = 0;
		Std_PpmInValue[i] = PPMIN_NEU;
	}
}

void PpmInCtl(u8 onoff)
{
	u8 i;
	static u8 ppm_onoff = 0;//�״�Ĭ���ǹر�
	EXTI_InitTypeDef EXTI_InitStructure;
	
	if(ppm_onoff==onoff)	return;
	else	ppm_onoff = onoff;
	
	if(onoff)
	{ 
		EXTI_ClearITPendingBit(EXTI_Line9);  //���LINE9�ϵ��жϱ�־λ
		
		EXTI_InitStructure.EXTI_Line = EXTI_Line9;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�жϿ���
		EXTI_Init(&EXTI_InitStructure);
	}
	else
	{
		EXTI_InitStructure.EXTI_Line = EXTI_Line9;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;//�жϹر�
		EXTI_Init(&EXTI_InitStructure);
		
		for(i=0;i<PPMINNUM;i++) PpmInValue[i] = 0;		
	}
}
