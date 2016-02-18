#include "Tx.h"
#include "Page.h"
#include "cfg.h"
#include "adc.h"
#include "key.h"
#include "time.h"
#include "ppm_decode.h"
#include <string.h>
#include <stdlib.h>

CSTR TxInternalError[]={"发射模块内部错误!","Tx Internal Error!"};
CSTR LoadSuccess[]={"加载成功!"," Loaded"};
CSTR RfClosed[]={"RF已关闭!","RF Closed!"};
CSTR LoadFail[]={"加载失败!"," Load Fail!"};
CSTR RfCloseFail[]={"RF关闭失败!","RF Close Fail!"};

u32 TxRndId;//随机ID
s32 TxValueSum;//通道总和
s16 TxChValue[TX_CH_NUM];//未标准化通道值
s16	Std_TxChValue[TX_CH_NUM];//标准化的通道值,可直接控制舵机
u8 TxPacket[32];//发射缓冲区
u8 TxPrevIdx;//当前使用的通信协议号
u8 RfPowTable[RF_POWER_NUM]={0x8D,0xC6,0x97,0x6E,0x7F,0xA9,0xBB,0xFF};

//PPM协议接口
u32 TxPpmOpen(void);
u32 TxPpmBind(void);
void TxPpmClose(void);
//F6TB协议接口
u32 TxF6tbOpen(void);
u32 TxF6tbBind(void);
void TxF6tbClose(void);
//K8TA协议接口
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

//Tx时间处理函数指针TxTimerCallBackFunc
u16 (*TxTimerCallBackFunc)(void);

//Tx时间配置函数,使用TIM4_CH1比较功能
void TxTimerCfg(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//开启外设TIM4
	
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1; //72MHz/72=1MHz 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //比较输出禁止
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Disable);//禁用TIM4在CCR1上的预装载寄存器
	
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_ITConfig(TIM4,TIM_IT_CC1,DISABLE);//禁用TIM4 CCR1中断
	
	TIM_Cmd(TIM4,ENABLE); //使能TIM4
}

//Tx延时启动函数
void TxTimerStart(u16 us, u16 (*cb)(void))
{
	if(! cb)	return;
    TxTimerCallBackFunc = cb;
	TIM_SetCompare1(TIM4,us + TIM_GetCounter(TIM4));
    TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);//清除比较中断标志
    TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);//使能TIM4 CCR1中断
}

//Tx时间停止函数
void TxTimerStop(void) 
{
	TxTimerCallBackFunc = NULL;
	TIM_ITConfig(TIM4,TIM_IT_CC1,DISABLE );//禁用TIM4 CCR1中断
}

void TIM4_IRQHandler(void)
{
	u16 us;
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1)!=RESET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);//清除比较中断标志
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

//TIM1用作tx数据处理周期定时器
void TxProcTimerInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//开启外设TIMx
	
	TIM_TimeBaseStructure.TIM_Period = 10000;//10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1; //72MHz/72=1MHz 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;//TIMx中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);//使能TIM_IT_Update中断
	
	TIM_Cmd(TIM1,ENABLE); //使能TIMx
}

void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);//清除比较中断标志
		
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

	while(TxRndId==0)//产生动态非零ID，每次上电仅产生一次
	{
		ADCHandler();//先进行一次ADC处理
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
		TxK8taBuildId();//标准化ID
		TxF6tbBuildId();//标准化ID
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

//根据EPA/REV/SUBTRIM进行i通道比例调整
//      (调整顺序不能变)
//输入: cv 通道的值
//    : idx 通道号
void TxSetCh(u8 idx,s32 cv)
{
	static u8 DelayRun = 0;
	static u32 TxDelayCnt = 0;
	s32 dv,ds;

	//延时处理定时器
	if(idx==0xFF)
	{
		if(TxDelayCnt<=Time_NowMs())
		{
			TxDelayCnt = Time_NowMs()+15;//15ms执行一次延时处理
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

	//4.限幅
	if(cv>TX_TRV)	cv=TX_TRV;
	if(cv<-TX_TRV)	cv=-TX_TRV;

	//5.写入通道缓冲区
	if(Model.Delay[idx]>0 && Model.Delay[idx]<=DELAY_MAX)//有效性判断，以免配置文件出错
	{
		if(DelayRun)//延时通道处理
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
	else TxChValue[idx]=cv;	//直接赋值
	
	Std_TxChValue[idx] = SERVO_NEU+(s32)TxChValue[idx]*SERVO_TRV/STK_TRV;//转换为标准舵机控制信号
}

//曲线函数(点数CURVE_NUM)
#define CURVE_SEG	(STK_TRV*2/(CURVE_NUM-1))	//9点曲线就是8段
s32 TxMixCurve(s32 v,s16 *curve)
{
	u8	i;
	s32 x1,y2,y1;
	
	//把值转换为全正值
	if(v<-STK_TRV)	v=-STK_TRV;
	if(v>+STK_TRV)	v=+STK_TRV;
	v+=STK_TRV;

	//寻找区间
	for(i=0,x1=0;i<CURVE_NUM-1;i++,x1+=CURVE_SEG)
	{
		//线性插值
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

	//如果没有转换成功，返回原值
	return v;
}

//混控函数
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

	//测试用舵量摇摆
	if(ChRockDelay<=Time_NowMs())
	{
		ChRockDelay=Time_NowMs()+CHROCKDELAY;
		
		ChRock+=ChRockStep;
		if(ChRock> STK_TRV)	ChRockStep=-CHROCKSTEP;
		if(ChRock<-STK_TRV)	ChRockStep=+CHROCKSTEP;
	}

	//处理延时
	TxSetCh(-1,0);

	//检测大小舵
	DrMode = SwValue(SWD_DR)+1;

	//检测锁定
	FlyMode=(SwValue(SWD_HOLD)<0)?FM_HLD:FM_NOR;

	if(Model.Type==MT_AERO)//固定翼
	{
		//输入4个通道
		MixerValue[CH_AIL]=StickTrim[0]+TxMixCurve(StickValue[0], Model.StkCurve[0][DrMode]);
		MixerValue[CH_ELE]=StickTrim[1]+TxMixCurve(StickValue[1], Model.StkCurve[1][DrMode]);
		MixerValue[CH_THR]=StickTrim[2]+TxMixCurve(StickValue[2], Model.ThrCurve[0]);
		MixerValue[CH_RUD]=StickTrim[3]+TxMixCurve(StickValue[3], Model.StkCurve[2][DrMode]);

		//起落架和襟翼
		MixerValue[CH_GEAR]=Model.Gear[SwValue(SWD_GEAR)+1]*STK_TRV/MAX_AUX;
		MixerValue[CH_FLAP]=Model.Flap[SwValue(SWD_FLAP)+1]*STK_TRV/MAX_AUX;

		//锁定油门处理
		if(FlyMode==FM_HLD)		MixerValue[CH_THR]=Model.ThrHold*STK_TRV/100;
		
		//油门混控计算
		{
			Thr = MixerValue[CH_THR]+STK_TRV;
			MixerValue[CH_AIL]+=Thr*Model.ThrMix[0]/MAX_THM;
			MixerValue[CH_RUD]+=Thr*Model.ThrMix[1]/MAX_THM;
			MixerValue[CH_ELE]+=Thr*Model.ThrMix[2]/MAX_THM;		
		}
		
		//转向混控
		{
			if(MixerValue[CH_AIL]>=0)	ea= MixerValue[CH_AIL]*Model.YawMix.Ar2E;
			else						ea=-MixerValue[CH_AIL]*Model.YawMix.Al2E;
			if(MixerValue[CH_RUD]>=0)	er= MixerValue[CH_RUD]*Model.YawMix.Rr2E;
			else						er=-MixerValue[CH_RUD]*Model.YawMix.Rl2E;
			MixerValue[CH_ELE]+=(ea+er)/MAX_YWM;			
		}
		
		//三角翼混控
		if(Model.Delta.Enable)
		{
			a=MixerValue[CH_AIL]*Model.Delta.A2A + MixerValue[CH_ELE]*Model.Delta.E2A;
			e=MixerValue[CH_AIL]*Model.Delta.A2E + MixerValue[CH_ELE]*Model.Delta.E2E;
			MixerValue[CH_AIL]=a/MAX_DLT;
			MixerValue[CH_ELE]=e/MAX_DLT;
		}

		//V尾混控
		if(Model.Vtail.Enable)
		{
			r=MixerValue[CH_RUD]*Model.Vtail.R2R + MixerValue[CH_ELE]*Model.Vtail.E2R;
			e=MixerValue[CH_RUD]*Model.Vtail.R2E + MixerValue[CH_ELE]*Model.Vtail.E2E;
			MixerValue[CH_RUD]=r/MAX_VTL;
			MixerValue[CH_ELE]=e/MAX_VTL;
		}

		//襟翼副翼混控
		if(Model.FlpAil.Enable)
		{
			a=MixerValue[CH_AIL]*Model.FlpAil.A2A + MixerValue[CH_FLAP]*Model.FlpAil.F2A;
			f=MixerValue[CH_AIL]*Model.FlpAil.A2F + MixerValue[CH_FLAP]*Model.FlpAil.F2F;
			MixerValue[CH_AIL]=a/MAX_FAM;
			MixerValue[CH_FLAP]=f/MAX_FAM;
		}
		
		//副翼方向混控
		if(Model.AilRud.Enable)
		{
			a = MixerValue[CH_AIL]*Model.AilRud.A2A + MixerValue[CH_RUD]*Model.AilRud.R2A;
			r = MixerValue[CH_AIL]*Model.AilRud.A2R + MixerValue[CH_RUD]*Model.AilRud.R2R;
			MixerValue[CH_AIL] = a/MAX_ARM;
			MixerValue[CH_RUD] = r/MAX_ARM;
		}
	}
	else//直升机
	{
		//检测特技开关
		if(FlyMode==FM_NOR)
		{
			if(SwValue(SWD_IDLE)==0)	FlyMode = FM_IDL1;
			if(SwValue(SWD_IDLE)==-1)	FlyMode = FM_IDL2;
		}

		//输入6个基本通道
		Thr=StickTrim[2]+StickValue[2];
		Ail=StickTrim[0]+TxMixCurve(StickValue[0], Model.StkCurve[0][DrMode]);
		Ele=StickTrim[1]+TxMixCurve(StickValue[1], Model.StkCurve[1][DrMode]);
		Rud=StickTrim[3]+TxMixCurve(StickValue[3], Model.StkCurve[2][DrMode]);
		
		Pit=TxMixCurve(Thr , Model.PitCurve[FlyMode]);
		
		if(FlyMode!=FM_HLD)	Thr=TxMixCurve(Thr , Model.ThrCurve[FlyMode]);//直升机模式下无锁定油门曲线
		
		Gyo=Model.Gyro[FlyMode]*STK_TRV/MAX_GYRO;

		//锁定油门处理
		if(FlyMode==FM_HLD)		Thr=Model.ThrHold*STK_TRV/100;

		//斜盘混控比率调整
		Ail=Ail*Model.Swash[0]/100;
		Ele=Ele*Model.Swash[1]/100;
		Pit=Pit*Model.Swash[2]/100;

		//斜盘混控
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
		else//单舵机斜盘
		{
			MixerValue[CH_AIL]	=Ail;
			MixerValue[CH_ELE]	=Ele;
			MixerValue[CH_FLAP]	=Pit;
		}             

		//设定油门|方向|陀螺仪
		MixerValue[CH_THR]	=Thr;
		MixerValue[CH_RUD]	=Rud;
		MixerValue[CH_GEAR]	=Gyo;
	}

	//4个辅助通道	
	MixerValue[CH_AUX1] = Model.Aux[0][SwValue(SWD_AUX1)+1]*STK_TRV/MAX_AUX;
	MixerValue[CH_AUX2] = Model.Aux[1][SwValue(SWD_AUX2)+1]*STK_TRV/MAX_AUX;
	
	if(ADC_AvgValue[AD_DL]>=4000)	MixerValue[CH_AUX3] = -STK_TRV;
	else if(ADC_AvgValue[AD_DL]<=96)	MixerValue[CH_AUX3] = STK_TRV;
	else	MixerValue[CH_AUX3] = 512-1024*(ADC_AvgValue[AD_DL]-96)/(4000-96);
	
	if(ADC_AvgValue[AD_DR]>=4000)	MixerValue[CH_AUX4] = -STK_TRV;
	else if(ADC_AvgValue[AD_DR]<=96)	MixerValue[CH_AUX4] = STK_TRV;
	else	MixerValue[CH_AUX4] = 512-1024*(ADC_AvgValue[AD_DR]-96)/(4000-96);

	if(Model.PpmIn)//PPM输入开启
	{
		for(i=0;i<PPMINNUM;i++)//PPM数据反向换算
		{
			PpmInValue[i] = (s32)(Std_PpmInValue[i]-SERVO_NEU)*STK_TRV/SERVO_TRV;//反向换算
		}
	}
	
	//通道映射输出
	TxValueSum = 0;
	for(i=0;i<TX_CH_NUM;i++)
	{
		if(Model.ChMap[i]==CH_AUTO)//测试通道
		{
			TxSetCh(i,ChRock);
		}
		else
		{
			TxSetCh(i,MixerValue[Model.ChMap[i]]);
		}
		
		if(Model.PpmIn&&(SwValue(SWD_TRAINER)<0))//教练机通道映射
		{	
			TxChValue[i] = PpmInValue[Model.TrChMap[i]];
			Std_TxChValue[i] = Std_PpmInValue[Model.TrChMap[i]];
		}
		
		TxValueSum += TxChValue[i]/STKMOVTST;
	}
}
