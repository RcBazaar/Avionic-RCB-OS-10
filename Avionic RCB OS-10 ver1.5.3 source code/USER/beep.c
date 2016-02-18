#include "beep.h"
#include "time.h"
#include "cfg.h"

//////////////////////////////////////////////
//
// ���ִ�����
//
const u8 *pBeepMusic=0;
static u8 BeepCnt,BeepDivider,BeepShortCnt=0;
const s16 BeepMusicTable[]={
	//   1    2    3 -  4    5    6    7 -
	-1, 349, 392, 440, 466, 523, 578, 659, -1,-1,
	-1, 698, 784, 880, 932,1046,1175,1318, -1,-1,
	-1,1397,1568,1760,1865,2960,3170,3280,
};

//////////////////////////////////////////////
//
// �����б�
//
//��ʽ��4�ֽ�һ��Ԫ�������ǣ�
//	����:1�����ִ���50ms
//	Ƶ��:1 2 3...11 12 13...21 22 23..����������������Ķ�����
//	����:(0~100)
//  �𶯣�0|1
//��������4������Ϊ0��β
const u8 MusicStartup[]={4,11,80,0, 4,13,90,0, 4,15,90,0, 2,21,100,1, 2,21,80,1, 2,21,60,0, 2,21,40,1, 2,21,20,1, 2,21,10,0, 0,0,0,0};
const u8 MusicTrimZero[]={1,15,80,0, 1,16,80,0, 2,15,80,0, 0,0,0,0};
const u8 MusicTrimMax[]={1,5,80,0, 1,12,0,0, 2,5,60,0, 0,0,0,0};
const u8 MusicEnter[]={1,21,80,0, 2,23,60,0, 0,0,0,0};
const u8 MusicBatLow[]={1,15,80,1, 1,13,60,1, 1,11,60,0, 1,11,0,0, 1,11,60,1, 1,11,0,0, 1,11,60,1, 0,0,0,0};
const u8 MusicTimeout[]={1,11,80,1, 1,13,70,1, 1,15,60,0, 1,13,50,1, 1,11,40,1,  0,0,0,0};

void Beep_Cofiguration(void)
{
	//BEEP-------------PA1(TIM2_CH2)
	//VIBRATOR---------PA8
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//��������TIM2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//��������GPIOA
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 0x05; //72MHz/(5+1)=12MHz 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_SetCompare2(TIM2,0);
	
	TIM_Cmd(TIM2,ENABLE);  //ʹ��TIM2
}

void Beep_Set_Vibration(u8 state)
{
	state ? GPIO_SetBits(GPIOA,GPIO_Pin_8) : GPIO_ResetBits(GPIOA,GPIO_Pin_8);	
}

//����Ƶ�ʺ���ȳ���
void Beep(u16 frequency,u8 volume)
{
    /* volume is between 0 and 100 */
    /* period = 14400000 / frequency */
    /* A Period of 65535 gives a ~ 220Hz tone */
    /* The Devo buzzer reaches max-volume with a pw ~ 100us.  That is max volume */
    /* use quadratic to approximate exponential volume control */
    u32 period = 12000000 / frequency;
    /* Taylor series: x + x^2/2 + x^3/6 + x^4/24 */
    u32 duty_cycle = (volume == 100) ? (period >> 1) : (u32)volume * volume * volume * 12 / 10000;
	TIM_SetAutoreload(TIM2,period);
    TIM_SetCompare2(TIM2,duty_cycle);
}


void BeepMusic(const u8 *music)
{	
	BeepShortCnt=0;//ȡ��������
	BeepCnt=BeepDivider=0;
	pBeepMusic=music;
	if(TxSys.Music)	Beep(BeepMusicTable[pBeepMusic[1]],pBeepMusic[2]);
	if(TxSys.Vibrator)	Beep_Set_Vibration(pBeepMusic[3]);
}

//����Ƶ�ʺ��������tm����
void BeepShort(u16 frequency,u8 volume,u8 tm)
{
	BeepCnt=BeepDivider=0;
	BeepShortCnt=tm;
	if(TxSys.Music)  Beep(frequency,volume);
}

void BeepHandler(void)
{	
	static u32 target = 0;
	
	if(target>Time_NowMs())	return;
	target = Time_NowMs()+1;//ÿ1msִ��һ��
/*********************************************/
	if(BeepShortCnt)//�����д���
	{
		BeepShortCnt--;
		if(BeepShortCnt==0)
		{
			TIM_SetCompare2(TIM2,0);
		}
		return;
	}
/**********************************************/
	if(BeepDivider++<50)
		return;	
	else
		BeepDivider=0;	

	if(pBeepMusic && pBeepMusic[0])//�������д���
	{
		if(BeepCnt++>=pBeepMusic[0])
		{											
			pBeepMusic+=4;
			BeepCnt=0;
			if(TxSys.Music) Beep(BeepMusicTable[pBeepMusic[1]],pBeepMusic[2]);
			if(TxSys.Vibrator)	Beep_Set_Vibration(pBeepMusic[3]);
		}
	}
	else
	{
		pBeepMusic=0;
		TIM_SetCompare2(TIM2,0);
		Beep_Set_Vibration(0);
	}
}



