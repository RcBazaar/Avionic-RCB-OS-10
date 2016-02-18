#include "Tx.h"
#include "Page.h"
#include "cfg.h"
#include "adc.h"
#include "key.h"
#include "time.h"
#include "ppm_decode.h"
#include <string.h>
#include <stdlib.h>

CSTR TxInternalError[]={"����ģ���ڲ�����!","Tx Internal Error!"};
CSTR LoadSuccess[]={"���سɹ�!"," Loaded"};
CSTR RfClosed[]={"RF�ѹر�!","RF Closed!"};
CSTR LoadFail[]={"����ʧ��!"," Load Fail!"};
CSTR RfCloseFail[]={"RF�ر�ʧ��!","RF Close Fail!"};

u32 TxRndId;//���ID
s32 TxValueSum;//ͨ���ܺ�
s16 TxChValue[TX_CH_NUM];//δ��׼��ͨ��ֵ
s16	Std_TxChValue[TX_CH_NUM];//��׼����ͨ��ֵ,��ֱ�ӿ��ƶ��
u8 TxPacket[32];//���仺����
u8 TxPrevIdx;//��ǰʹ�õ�ͨ��Э���
u8 RfPowTable[RF_POWER_NUM]={0x8D,0xC6,0x97,0x6E,0x7F,0xA9,0xBB,0xFF};

//PPMЭ��ӿ�
u32 TxPpmOpen(void);
u32 TxPpmBind(void);
void TxPpmClose(void);
//F6TBЭ��ӿ�
u32 TxF6tbOpen(void);
u32 TxF6tbBind(void);
void TxF6tbClose(void);
//K8TAЭ��ӿ�
u32 TxK8taOpen(void);
u32 TxK8taBind(void);
void TxK8taClose(void);

typedef struct{
	u32 (*Open)(void);
	u32 (*Bind)(void);
	void (*Close)(void);
}TXMODULE;

u32 TxDummy(void)
{
	return 1;
}

void TxDummyClose(void){}

const TXMODULE TxModule[]={
	{TxDummy,TxDummy,TxDummyClose},
	{TxPpmOpen,TxPpmBind,TxPpmClose},
	{TxK8taOpen,TxK8taBind,TxK8taClose},
	{TxF6tbOpen,TxF6tbBind,TxF6tbClose},
};
#define TX_MOD_NUM	sizeof(TxModule)/sizeof(TXMODULE)

//Txʱ�䴦����ָ��TxTimerCallBackFunc
u16 (*TxTimerCallBackFunc)(void);

//Txʱ�����ú���,ʹ��TIM4_CH1�ȽϹ���
void TxTimerCfg(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//��������TIM4
	
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1; //72MHz/72=1MHz 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�Ƚ������ֹ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Disable);//����TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_ITConfig(TIM4,TIM_IT_CC1,DISABLE);//����TIM4 CCR1�ж�
	
	TIM_Cmd(TIM4,ENABLE); //ʹ��TIM4
}

//Tx��ʱ��������
void TxTimerStart(u16 us, u16 (*cb)(void))
{
	if(! cb)	return;
    TxTimerCallBackFunc = cb;
	TIM_SetCompare1(TIM4,us + TIM_GetCounter(TIM4));
    TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);//����Ƚ��жϱ�־
    TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);//ʹ��TIM4 CCR1�ж�
}

//Txʱ��ֹͣ����
void TxTimerStop(void) 
{
	TxTimerCallBackFunc = NULL;
	TIM_ITConfig(TIM4,TIM_IT_CC1,DISABLE );//����TIM4 CCR1�ж�
}

void TIM4_IRQHandler(void)
{
	u16 us;
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1)!=RESET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);//����Ƚ��жϱ�־
		if(TxTimerCallBackFunc)
		{
			us = TxTimerCallBackFunc();
			if(us)
			{
				TIM_SetCompare1(TIM4,us + TIM_GetCounter(TIM4));
				return;
			}
		}
		TxTimerStop();
	}
}

//TIM1����tx���ݴ������ڶ�ʱ��
void TxProcTimerInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//��������TIMx
	
	TIM_TimeBaseStructure.TIM_Period = 10000;//10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1; //72MHz/72=1MHz 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;//TIMx�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);//ʹ��TIM_IT_Update�ж�
	
	TIM_Cmd(TIM1,ENABLE); //ʹ��TIMx
}

void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);//����Ƚ��жϱ�־
		
		ADCHandler();
		PowerManager();
		StickHandler();
		TxMixer();
	}
}

u32 TxLoad(u8 idx)
{
	char msg[16];
	u16 seed;

	while(TxRndId==0)//������̬����ID��ÿ���ϵ������һ��
	{
		ADCHandler();//�Ƚ���һ��ADC����
		seed = ADC_AvgValue[AD_LV]+ADC_AvgValue[AD_RV]+ADC_AvgValue[AD_LH]+ADC_AvgValue[AD_RH]+ADC_AvgValue[AD_BAT];
		srand(seed);
		TxRndId = (u32)rand();
		TxRndId = TxRndId % 1000000;
	}

	if(idx>=TX_MOD_NUM)
	{
		PageAlertModel(TxInternalError[TxSys.Language],0);
		return 0;
	}

	if(idx!=TxPrevIdx)
	{
		strcpy(msg,PageOptionProtocol[idx]);
		TxModule[TxPrevIdx].Close();
		if(TxModule[idx].Open())
		{
			strcat(msg,LoadSuccess[TxSys.Language]);
			PageAlertModel(idx?msg:RfClosed[TxSys.Language],500);
			TxPrevIdx = idx;
			return 1;
		}
		else
		{
			strcat(msg,LoadFail[TxSys.Language]);
			PageAlertModel(idx?msg:RfCloseFail[TxSys.Language],0);
			TxPrevIdx = 0;
			return 0;
		}
	}
	else
	{
		TxK8taBuildId();//��׼��ID
		TxF6tbBuildId();//��׼��ID
	}
	return 2;
}

u32 TxBind(void)
{
	return TxModule[TxPrevIdx].Bind();
}

void TxClose(void)
{
	TxModule[TxPrevIdx].Close();
	TxPrevIdx = 0;
}

//����EPA/REV/SUBTRIM����iͨ����������
//      (����˳���ܱ�)
//����: cv ͨ����ֵ
//    : idx ͨ����
void TxSetCh(u8 idx,s32 cv)
{
	static u8 DelayRun = 0;
	static u32 TxDelayCnt = 0;
	s32 dv,ds;

	//��ʱ����ʱ��
	if(idx==0xFF)
	{
		if(TxDelayCnt<=Time_NowMs())
		{
			TxDelayCnt = Time_NowMs()+15;//15msִ��һ����ʱ����
			DelayRun = 1;
		}
		else	DelayRun = 0;
		return;
	}

	//1.EPA
	if(cv<0) cv*= Model.Epa[idx][0];
	else	 cv*= Model.Epa[idx][1];
	cv/=EPA_SIZE;

	//2.REV
	if(Model.Rev[idx])	cv=-cv;

	//3.NEU
	cv+=Model.Neu[idx]*NEU_STEP;

	//4.�޷�
	if(cv>TX_TRV)	cv=TX_TRV;
	if(cv<-TX_TRV)	cv=-TX_TRV;

	//5.д��ͨ��������
	if(Model.Delay[idx]>0 && Model.Delay[idx]<=DELAY_MAX)//��Ч���жϣ����������ļ�����
	{
		if(DelayRun)//��ʱͨ������
		{
			dv=cv-TxChValue[idx];
			ds=DELAY_MAX-Model.Delay[idx]+1;
			if(dv<ds && dv>-ds)		TxChValue[idx]=cv;
			else
			{
				if(dv<0)	ds=-ds;
				TxChValue[idx]+=ds;
			}
		}
	}
	else TxChValue[idx]=cv;	//ֱ�Ӹ�ֵ
	
	Std_TxChValue[idx] = SERVO_NEU+(s32)TxChValue[idx]*SERVO_TRV/STK_TRV;//ת��Ϊ��׼��������ź�
}

//���ߺ���(����CURVE_NUM)
#define CURVE_SEG	(STK_TRV*2/(CURVE_NUM-1))	//9�����߾���8��
s32 TxMixCurve(s32 v,s16 *curve)
{
	u8	i;
	s32 x1,y2,y1;
	
	//��ֵת��Ϊȫ��ֵ
	if(v<-STK_TRV)	v=-STK_TRV;
	if(v>+STK_TRV)	v=+STK_TRV;
	v+=STK_TRV;

	//Ѱ������
	for(i=0,x1=0;i<CURVE_NUM-1;i++,x1+=CURVE_SEG)
	{
		//���Բ�ֵ
		// y-y1    y2-y1
		//------ = -----
		// x-x1    x2-x1
		//
		// y=y1+(x-x1)*(y2-y1)/(x2-x1)
		if(v>=x1 && v<=(x1+CURVE_SEG))
		{
			y1=(s32)curve[i]*(STK_TRV*2)/CURVE_TRV;
			y2=(s32)curve[i+1]*(STK_TRV*2)/CURVE_TRV;
			v=y1+(v-x1)*(y2-y1)/CURVE_SEG;

			return v-STK_TRV;
		}
	}

	//���û��ת���ɹ�������ԭֵ
	return v;
}

//��غ���
s32 MixerValue[MAP_CH_NUM];
u8 FlyMode=0;
u8 DrMode=0;
void TxMixer(void)
{
	static s16	ChRock=0,ChRockStep=CHROCKSTEP;
	static u32	ChRockDelay=0;
	s32 ea,er;
	s32 a,e,r,f;
	s32 Ail,Ele,Rud,Thr,Pit,Gyo;
	u8 i;

	//�����ö���ҡ��
	if(ChRockDelay<=Time_NowMs())
	{
		ChRockDelay=Time_NowMs()+CHROCKDELAY;
		
		ChRock+=ChRockStep;
		if(ChRock> STK_TRV)	ChRockStep=-CHROCKSTEP;
		if(ChRock<-STK_TRV)	ChRockStep=+CHROCKSTEP;
	}

	//������ʱ
	TxSetCh(-1,0);

	//����С��
	DrMode = SwValue(SWD_DR)+1;

	//�������
	FlyMode=(SwValue(SWD_HOLD)<0)?FM_HLD:FM_NOR;

	if(Model.Type==MT_AERO)//�̶���
	{
		//����4��ͨ��
		MixerValue[CH_AIL]=StickTrim[0]+TxMixCurve(StickValue[0], Model.StkCurve[0][DrMode]);
		MixerValue[CH_ELE]=StickTrim[1]+TxMixCurve(StickValue[1], Model.StkCurve[1][DrMode]);
		MixerValue[CH_THR]=StickTrim[2]+TxMixCurve(StickValue[2], Model.ThrCurve[0]);
		MixerValue[CH_RUD]=StickTrim[3]+TxMixCurve(StickValue[3], Model.StkCurve[2][DrMode]);

		//����ܺͽ���
		MixerValue[CH_GEAR]=Model.Gear[SwValue(SWD_GEAR)+1]*STK_TRV/MAX_AUX;
		MixerValue[CH_FLAP]=Model.Flap[SwValue(SWD_FLAP)+1]*STK_TRV/MAX_AUX;

		//�������Ŵ���
		if(FlyMode==FM_HLD)		MixerValue[CH_THR]=Model.ThrHold*STK_TRV/100;
		
		//���Ż�ؼ���
		{
			Thr = MixerValue[CH_THR]+STK_TRV;
			MixerValue[CH_AIL]+=Thr*Model.ThrMix[0]/MAX_THM;
			MixerValue[CH_RUD]+=Thr*Model.ThrMix[1]/MAX_THM;
			MixerValue[CH_ELE]+=Thr*Model.ThrMix[2]/MAX_THM;		
		}
		
		//ת����
		{
			if(MixerValue[CH_AIL]>=0)	ea= MixerValue[CH_AIL]*Model.YawMix.Ar2E;
			else						ea=-MixerValue[CH_AIL]*Model.YawMix.Al2E;
			if(MixerValue[CH_RUD]>=0)	er= MixerValue[CH_RUD]*Model.YawMix.Rr2E;
			else						er=-MixerValue[CH_RUD]*Model.YawMix.Rl2E;
			MixerValue[CH_ELE]+=(ea+er)/MAX_YWM;			
		}
		
		//��������
		if(Model.Delta.Enable)
		{
			a=MixerValue[CH_AIL]*Model.Delta.A2A + MixerValue[CH_ELE]*Model.Delta.E2A;
			e=MixerValue[CH_AIL]*Model.Delta.A2E + MixerValue[CH_ELE]*Model.Delta.E2E;
			MixerValue[CH_AIL]=a/MAX_DLT;
			MixerValue[CH_ELE]=e/MAX_DLT;
		}

		//Vβ���
		if(Model.Vtail.Enable)
		{
			r=MixerValue[CH_RUD]*Model.Vtail.R2R + MixerValue[CH_ELE]*Model.Vtail.E2R;
			e=MixerValue[CH_RUD]*Model.Vtail.R2E + MixerValue[CH_ELE]*Model.Vtail.E2E;
			MixerValue[CH_RUD]=r/MAX_VTL;
			MixerValue[CH_ELE]=e/MAX_VTL;
		}

		//��������
		if(Model.FlpAil.Enable)
		{
			a=MixerValue[CH_AIL]*Model.FlpAil.A2A + MixerValue[CH_FLAP]*Model.FlpAil.F2A;
			f=MixerValue[CH_AIL]*Model.FlpAil.A2F + MixerValue[CH_FLAP]*Model.FlpAil.F2F;
			MixerValue[CH_AIL]=a/MAX_FAM;
			MixerValue[CH_FLAP]=f/MAX_FAM;
		}
		
		//��������
		if(Model.AilRud.Enable)
		{
			a = MixerValue[CH_AIL]*Model.AilRud.A2A + MixerValue[CH_RUD]*Model.AilRud.R2A;
			r = MixerValue[CH_AIL]*Model.AilRud.A2R + MixerValue[CH_RUD]*Model.AilRud.R2R;
			MixerValue[CH_AIL] = a/MAX_ARM;
			MixerValue[CH_RUD] = r/MAX_ARM;
		}
	}
	else//ֱ����
	{
		//����ؼ�����
		if(FlyMode==FM_NOR)
		{
			if(SwValue(SWD_IDLE)==0)	FlyMode = FM_IDL1;
			if(SwValue(SWD_IDLE)==-1)	FlyMode = FM_IDL2;
		}

		//����6������ͨ��
		Thr=StickTrim[2]+StickValue[2];
		Ail=StickTrim[0]+TxMixCurve(StickValue[0], Model.StkCurve[0][DrMode]);
		Ele=StickTrim[1]+TxMixCurve(StickValue[1], Model.StkCurve[1][DrMode]);
		Rud=StickTrim[3]+TxMixCurve(StickValue[3], Model.StkCurve[2][DrMode]);
		
		Pit=TxMixCurve(Thr , Model.PitCurve[FlyMode]);
		
		if(FlyMode!=FM_HLD)	Thr=TxMixCurve(Thr , Model.ThrCurve[FlyMode]);//ֱ����ģʽ����������������
		
		Gyo=Model.Gyro[FlyMode]*STK_TRV/MAX_GYRO;

		//�������Ŵ���
		if(FlyMode==FM_HLD)		Thr=Model.ThrHold*STK_TRV/100;

		//б�̻�ر��ʵ���
		Ail=Ail*Model.Swash[0]/100;
		Ele=Ele*Model.Swash[1]/100;
		Pit=Pit*Model.Swash[2]/100;

		//б�̻��
		if(Model.SwashType==1)//120 CCPM		sin(120-90)=0.5
		{
			MixerValue[CH_AIL]	=Pit+Ele/2-Ail;
			MixerValue[CH_ELE]	=Pit-Ele;
			MixerValue[CH_FLAP]	=Pit+Ele/2+Ail;
		}
		else if(Model.SwashType==2)//140 CCPM	sin(140-90)=0.766~=36/47
		{
			MixerValue[CH_AIL]	=Pit+Ele*36/47-Ail;
			MixerValue[CH_ELE]	=Pit-Ele;
			MixerValue[CH_FLAP]	=Pit+Ele*36/47+Ail;
		}
		else if(Model.SwashType==3)//90 CCPM	sin(90-90)=0
		{
			MixerValue[CH_AIL]	=Pit-Ail;
			MixerValue[CH_ELE]	=Pit-Ele;
			MixerValue[CH_FLAP]	=Pit+Ail;
		}
		else//�����б��
		{
			MixerValue[CH_AIL]	=Ail;
			MixerValue[CH_ELE]	=Ele;
			MixerValue[CH_FLAP]	=Pit;
		}             

		//�趨����|����|������
		MixerValue[CH_THR]	=Thr;
		MixerValue[CH_RUD]	=Rud;
		MixerValue[CH_GEAR]	=Gyo;
	}

	//4������ͨ��	
	MixerValue[CH_AUX1] = Model.Aux[0][SwValue(SWD_AUX1)+1]*STK_TRV/MAX_AUX;
	MixerValue[CH_AUX2] = Model.Aux[1][SwValue(SWD_AUX2)+1]*STK_TRV/MAX_AUX;
	
	if(ADC_AvgValue[AD_DL]>=4000)	MixerValue[CH_AUX3] = -STK_TRV;
	else if(ADC_AvgValue[AD_DL]<=96)	MixerValue[CH_AUX3] = STK_TRV;
	else	MixerValue[CH_AUX3] = 512-1024*(ADC_AvgValue[AD_DL]-96)/(4000-96);
	
	if(ADC_AvgValue[AD_DR]>=4000)	MixerValue[CH_AUX4] = -STK_TRV;
	else if(ADC_AvgValue[AD_DR]<=96)	MixerValue[CH_AUX4] = STK_TRV;
	else	MixerValue[CH_AUX4] = 512-1024*(ADC_AvgValue[AD_DR]-96)/(4000-96);

	if(Model.PpmIn)//PPM���뿪��
	{
		for(i=0;i<PPMINNUM;i++)//PPM���ݷ�����
		{
			PpmInValue[i] = (s32)(Std_PpmInValue[i]-SERVO_NEU)*STK_TRV/SERVO_TRV;//������
		}
	}
	
	//ͨ��ӳ�����
	TxValueSum = 0;
	for(i=0;i<TX_CH_NUM;i++)
	{
		if(Model.ChMap[i]==CH_AUTO)//����ͨ��
		{
			TxSetCh(i,ChRock);
		}
		else
		{
			TxSetCh(i,MixerValue[Model.ChMap[i]]);
		}
		
		if(Model.PpmIn&&(SwValue(SWD_TRAINER)<0))//������ͨ��ӳ��
		{	
			TxChValue[i] = PpmInValue[Model.TrChMap[i]];
			Std_TxChValue[i] = Std_PpmInValue[Model.TrChMap[i]];
		}
		
		TxValueSum += TxChValue[i]/STKMOVTST;
	}
}
