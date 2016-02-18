#include "cfg.h"
#include "time.h"
#include "beep.h"
#include "lcd.h"
#include "key.h"
#include "Tx.h"

u16	TxRunTime;
s32	LastCnt;


//定时器计数器
TIMERRUN TxTimer={0,0,0,1,&Model.Timer};

// 定时器处理
void TimerRun(void)
{	
	TIMERRUN *pt=&TxTimer;
	
	//禁用判断
	if(pt->Cfg->Type==TMR_INH)
	{
		pt->Run = 0;
		return;
	}
	
	//复位判断
	if(pt->Reset)
	{
		pt->Reset=0;
		if(pt->Cfg->Type==TMR_CLK)	pt->Cnt=0;//递增计数时清零
		else if(pt->Cfg->Type==TMR_DCK)	pt->Cnt=pt->Cfg->Time;//递减计数时恢复到最大值
	}
	
	//运行判断
	if(pt->Cfg->Sw==TMRSW_THR)
	{
		pt->Run=(MixerValue[CH_THR]*100/STK_TRV>=pt->Cfg->Thr);
	}
	else if(pt->Cfg->Sw==TMRSW_SA)
	{
		pt->Run = SwTstState(SW_SA)==SW_STA_DW;
	}
	else if(pt->Cfg->Sw==TMRSW_SB)
	{
		pt->Run = SwTstState(SW_SB)==SW_STA_DW;
	}
	else if(pt->Cfg->Sw==TMRSW_SC)
	{
		pt->Run = SwTstState(SW_SC)==SW_STA_DW;;
	}
	else if(pt->Cfg->Sw==TMRSW_SD)
	{
		pt->Run = SwTstState(SW_SD)==SW_STA_DW;;
	}
	else pt->Run=0;
	
	//不运行的不处理
	if(!pt->Run)	return;
	
	//正反向计数操作
	if(pt->Cfg->Type==TMR_CLK)
	{
		pt->Cnt++;
		pt->Alert=(pt->Cnt+60>=pt->Cfg->Time);//剩余时间不足1分钟
	}
	else if(pt->Cfg->Type==TMR_DCK)
	{
		pt->Cnt--;
		pt->Alert=(pt->Cnt<=60);//剩余时间不足1分钟
	}
	
	//告警操作	
	if(pt->Alert)
	{
		if(pt->Cnt+10>=pt->Cfg->Time)//剩余时间不足10s
		{
			if(pt->Cnt!=LastCnt)//每1s提醒一次
			{
				BeepMusic(MusicTimeout);
				LightStartTmr = Time_NowMs();//背光关闭计数器重置
			}
		}
		else
		{
			if(pt->Cnt%10==0)//每10s提醒一次
			{
				BeepMusic(MusicTimeout);
				LightStartTmr = Time_NowMs();//背光关闭计数器重置
			}
		}
	}
	LastCnt = pt->Cnt;
}

void TimerProc(void)
{
	static u32 timercnt = 0;
	
	//秒分频器
	if(timercnt>Time_NowMs())	return;
	timercnt=Time_NowMs()+1000;//1s
	
	TxRunTime++;
	TxSys.BatTime++;
		
	TimerRun();
}
