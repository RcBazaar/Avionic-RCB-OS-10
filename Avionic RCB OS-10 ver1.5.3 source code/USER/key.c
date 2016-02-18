#include "key.h"
#include "time.h"
#include "beep.h"
#include "lcd.h"
#include "adc.h"
#include "cfg.h"

//矩阵键盘按键状态
/*
	RUD+ RUD- ENT EXIT	AIL- AIL+   NC  NC		NC NC  UP  DOWN ELE- ELE+ THR- THR+
	x    x    x    x      x    x     1    1      1	1	x	 x	  x	   x	x	 x    (b)
*/

//开关状态(所有开关均按照三段开关定义)
/*
	NC	NC	SF	SE	SD	SA	SC	SB
	11	11	xx	xx	xx	xx	xx	xx	(b)
*/

volatile u16 KeyDown = KEY_MASK,KeyHold = KEY_MASK;
volatile u8 ZYS_State = ZYS_LEFT;//左右手状态,默认左手模式
volatile u16 SwState = KEY_MASK;//开关状态
u16 KeyPressCnt;

//键盘扫描初始化
void Key_Configuration(void)
{
	//KEY_ENT_IN---------------PC4
	//KEY_EXT_IN---------------PC5
	//KEY_UP_IN----------------PC14
	//KEY_DOWN_IN--------------PC13
	//KEY_TRIM_RL_IN-----------PA15
	//KEY_TRIM_RR_IN-----------PC10
	//KEY_TRIM_TD_IN-----------PC11
	//KEY_TRIM_TU_IN-----------PB5
	//KEY_TRIM_ED_IN-----------PB7
	//KEY_TRIM_EU_IN-----------PB6
	//KEY_TRIM_AL_IN-----------PB9
	//KEY_TRIM_AR_IN-----------PB8
	
	//KEY_ZYS_IN---------------PC12
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
}

//矩阵键盘扫描得到键值
u16 Key_ScanOnce(void)
{
    u16 result = 0;
	
	if(!KEY_ENT_IN)	result = KEY_ENT;
	else if(!KEY_EXT_IN)	result = KEY_EXT;
	else if(!KEY_UP_IN)		result = KEY_UP;
	else if(!KEY_DOWN_IN)		result = KEY_DOWN;
	else if(!KEY_TRIM_RL_IN)	result = KEY_TRIM_RL;
	else if(!KEY_TRIM_RR_IN)	result = KEY_TRIM_RR;
	else if(!KEY_TRIM_TD_IN)	result = KEY_TRIM_TD;
	else if(!KEY_TRIM_TU_IN)	result = KEY_TRIM_TU;
	else if(!KEY_TRIM_ED_IN)	result = KEY_TRIM_ED;
	else if(!KEY_TRIM_EU_IN)	result = KEY_TRIM_EU;
	else if(!KEY_TRIM_AL_IN)	result = KEY_TRIM_AL;
	else if(!KEY_TRIM_AR_IN)	result = KEY_TRIM_AR;
	else	result = 0;
	
    return result;
}

//开关扫描
u16 Switch_ScanOnce(void)
{
	s16 v;
	u16 result = 0;
	
	//SA
	if(ADC_AvgValue[AD_SA]<=1024)	result |= SW_STA_UP<<SW_SA;
	else if(ADC_AvgValue[AD_SA]>=3072)	result |= SW_STA_DW<<SW_SA;
	else	result |= SW_STA_MD<<SW_SA;
	//SB
	if(ADC_AvgValue[AD_SB]<=1024)	result |= SW_STA_UP<<SW_SB;
	else if(ADC_AvgValue[AD_SB]>=3072)	result |= SW_STA_DW<<SW_SB;
	else	result |= SW_STA_MD<<SW_SB;
	//SC
	if(ADC_AvgValue[AD_SC]<=1024)	result |= SW_STA_UP<<SW_SC;
	else if(ADC_AvgValue[AD_SC]>=3072)	result |= SW_STA_DW<<SW_SC;
	else	result |= SW_STA_MD<<SW_SC;
	//SD
	if(ADC_AvgValue[AD_SD]<=1024)	result |= SW_STA_UP<<SW_SD;
	else if(ADC_AvgValue[AD_SD]>=3072)	result |= SW_STA_DW<<SW_SD;
	else	result |= SW_STA_MD<<SW_SD;
	//SE
	v = Model.VirtualSwVal[0]*STK_TRV/100;
	if(StickValue[Model.VirtualSwStk[0]]<=v)	result |= SW_STA_DW<<SW_SE;
	else	result |= SW_STA_UP<<SW_SE;
	//SF
	v = Model.VirtualSwVal[1]*STK_TRV/100;
	if(StickValue[Model.VirtualSwStk[1]]<=v)	result |= SW_STA_DW<<SW_SF;
	else	result |= SW_STA_UP<<SW_SF;
	
	return result;
}

//按键、开关处理函数
void KeyScanHandler(void)
{
	u16 key_press;
	u16 sw_state;
	static u16 key_buf = 0;
	static u16 sw_buf = 0;
	static u32 target = 0;//按键扫描分频系数
	
	if(target>Time_NowMs())	return;	
	target = Time_NowMs()+20;//按键扫描分频，20ms扫描一次
/*****************************************************************/
	key_press = Key_ScanOnce();
	if(key_press==key_buf)//按键消抖	
	{
		if(key_press)//有键按下
		{
			if(KeyPressCnt==0)//按键刚开始按下
			{
				KeyDown &= ~key_press;
				if(TxSys.KeyBeep)	BeepShort(1000+TxSys.KeyTone*30,30+TxSys.KeyBeep*6,20);
				LightStartTmr = Time_NowMs();
			}
			else
			{
				if(KeyPressCnt>=KEY_CONT_TRIG)//连按处理
				{				
					KeyHold &= ~key_press;
					if(key_press!=KEY_ENT && key_press!=KEY_EXT)	KeyDown &= ~key_press;
					KeyPressCnt = KEY_CONT_TRIG-KEY_CONT_FREQ;
					LightStartTmr = Time_NowMs();
				}
			}
			KeyPressCnt++;
		}
		else
		{
			KeyPressCnt = 0;
			KeyHold = KEY_MASK;
		}
	}
	else
	{
		key_buf = key_press;
	}
/*****************************************************************/	
	sw_state = Switch_ScanOnce();
	if(sw_state==sw_buf)//消抖
	{
		SwState = sw_state;
	}
	else
		sw_buf = sw_state;
/*****************************************************************/
	if(!KEY_ZYS_IN)	ZYS_State = ZYS_LEFT;//左右手开关闭合，左手模式
	else	ZYS_State = ZYS_RIGHT;//左右手开关未闭合，右手模式
}

u8 KeyTstDown(u16 mask)
{
	if((KeyDown&mask)==0)
	{
		KeyDown |= mask;
		return 1;
	}
	return 0;
}

void KeyClearDown(u16 mask)
{
	KeyDown |= mask;	
}

u8 KeyTstHold(u16 mask)
{
	if((KeyHold&mask)==0)
	{
		return 1;
	}
	return 0;
}

void KeyFlush(void)
{
	KeyDown=KeyHold=KEY_MASK;
	KeyPressCnt = 0;
}

void KeyStopCnt(void)
{
	KeyPressCnt=1;
}

u16 SwTstState(u16 mask)
{
	return (SwState>>mask)&SW_MASK;
}

//获取映射之后的开关状态
s16 SwValue(s16 swpos)
{
	u16 state;
	s16 result;
	
	switch(Model.SwDef[swpos])
	{
		case 1://SA
			state = SwTstState(SW_SA);break;
		case 2://SB
			state = SwTstState(SW_SB);break;
		case 3://SC
			state = SwTstState(SW_SC);break;
		case 4://SD
			state = SwTstState(SW_SD);break;
		case 5://SE
			state = SwTstState(SW_SE);break;
		case 6://SF
			state = SwTstState(SW_SF);break;
		default://INH
			if(swpos==SWD_IDLE)	state = SW_STA_UP;
			else	state = SW_STA_MD;
	}
	
	if(state==SW_STA_DW)	result = -1;
	else if(state==SW_STA_MD)	result = 0;
	else	result = 1;
	
	return result;
}

//检测左右手
u8 SwTstZYSState(void)
{
	return ZYS_State;
}

