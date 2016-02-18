#include "cfg.h"
#include "time.h"
#include "beep.h"
#include "lcd.h"
#include "key.h"
#include "Tx.h"

u16	TxRunTime;
s32	LastCnt;


//��ʱ��������
TIMERRUN TxTimer={0,0,0,1,&Model.Timer};

// ��ʱ������
void TimerRun(void)
{	
	TIMERRUN *pt=&TxTimer;
	
	//�����ж�
	if(pt->Cfg->Type==TMR_INH)
	{
		pt->Run = 0;
		return;
	}
	
	//��λ�ж�
	if(pt->Reset)
	{
		pt->Reset=0;
		if(pt->Cfg->Type==TMR_CLK)	pt->Cnt=0;//��������ʱ����
		else if(pt->Cfg->Type==TMR_DCK)	pt->Cnt=pt->Cfg->Time;//�ݼ�����ʱ�ָ������ֵ
	}
	
	//�����ж�
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
	
	//�����еĲ�����
	if(!pt->Run)	return;
	
	//�������������
	if(pt->Cfg->Type==TMR_CLK)
	{
		pt->Cnt++;
		pt->Alert=(pt->Cnt+60>=pt->Cfg->Time);//ʣ��ʱ�䲻��1����
	}
	else if(pt->Cfg->Type==TMR_DCK)
	{
		pt->Cnt--;
		pt->Alert=(pt->Cnt<=60);//ʣ��ʱ�䲻��1����
	}
	
	//�澯����	
	if(pt->Alert)
	{
		if(pt->Cnt+10>=pt->Cfg->Time)//ʣ��ʱ�䲻��10s
		{
			if(pt->Cnt!=LastCnt)//ÿ1s����һ��
			{
				BeepMusic(MusicTimeout);
				LightStartTmr = Time_NowMs();//����رռ���������
			}
		}
		else
		{
			if(pt->Cnt%10==0)//ÿ10s����һ��
			{
				BeepMusic(MusicTimeout);
				LightStartTmr = Time_NowMs();//����رռ���������
			}
		}
	}
	LastCnt = pt->Cnt;
}

void TimerProc(void)
{
	static u32 timercnt = 0;
	
	//���Ƶ��
	if(timercnt>Time_NowMs())	return;
	timercnt=Time_NowMs()+1000;//1s
	
	TxRunTime++;
	TxSys.BatTime++;
		
	TimerRun();
}
