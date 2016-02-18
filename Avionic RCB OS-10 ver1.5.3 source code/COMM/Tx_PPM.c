#include "Tx.h"

#define PPM_PLUS	400	//PPM 脉冲400us,用于分割通道
#define PPM_MID		1500
#define PPM_FRAME	20000

#define PPM_OUT_H()		GPIO_SetBits(GPIOA,GPIO_Pin_10)
#define PPM_OUT_L()		GPIO_ResetBits(GPIOA,GPIO_Pin_10)

u8 TxPpmPin,TxPpmIdx;

static u16 TxPpmCallback(void)
{
	static u16 ppmtime = 0;
	u16 t;
	
	if(TxPpmPin)//输出高电平	
	{
		PPM_OUT_H();
		TxPpmPin = 0;
		return PPM_PLUS;
	}
	else	//输出低电平
	{
		PPM_OUT_L();
		TxPpmPin = 1;		
		if(TxPpmIdx>=TX_CH_NUM)	//输出完最后一个脉冲
		{
			t=PPM_FRAME-PPM_PLUS*TX_CH_NUM-ppmtime;	//计算帧间隔时间
			ppmtime=0;								//清空通道累加器
			TxPpmIdx=0;								//回归CH1
		}
		else	//PPM脉冲序列
		{
			t = Std_TxChValue[TxPpmIdx]-PPM_PLUS;	//计算本通道时间		
			ppmtime+=t;								//累加本通道时间
			TxPpmIdx++;								//CH步进
		}
		
		return t;
	}
}
	
u32 TxPpmOpen(void)
{
	//PPM_ONT-----------------PA10
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_10);
    
    //50ms后开始输出PPM信号
    TxTimerStart(50000,TxPpmCallback);
    
    TxPpmPin = 0;
    TxPpmIdx = TX_CH_NUM+1;

	return 1;
}

void TxPpmClose(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//将PPM管脚设置为高阻态
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}

u32 TxPpmBind(void)
{
	return 0;//返回对码界面显示秒数
}

