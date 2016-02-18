#include "Page.h"
#include "cfg.h"
#include "beep.h"
#include "lcd.h"
#include "key.h"

u32 PageMainTrimShowCnt;

///////////////////////////////////////////////////////////////////////////////////////
//
//  ΢����������
//
void PageMainTrimBeep(s8 v)
{
	u32 x;
	
	if(!TxSys.Music)	return;
	if(v<0) v=-v;
	x=v;
	x=x*1000/TRIM_MAX+1000;	
	BeepShort(x,80,100);
}

void PageMainTrimShow(s8 v)
{
	//���������ҳ������ʾ
	if(PageStackIdx)	return;
	
	if(v==TRIM_NUM_HIDE)
	{
		LcdDrawMiniNum(59,59, LCD_MN_SP,0);
		LcdDrawMiniNum(63,59, LCD_MN_SP,0);
		LcdDrawMiniNum(67,59, LCD_MN_SP,0);
	}
	else
	{
		LcdDrawMiniInt(59,59, v,2,0,1,0,0);
		PageMainTrimShowCnt=TRIM_NUM_TIME;
	}
}

void PageMainTrimProc(u16 keydec,u16 keyinc,u8 trimidx)
{
	s8 v=Model.Trim[trimidx];

	if(KeyTstDown(keyinc))
	{
		if(v<TRIM_MAX) 
		{
			if(++v==0)
			{
				BeepMusic(MusicTrimZero);
				KeyStopCnt();
			}
			else
			{
				PageMainTrimBeep(v);
			}
			PageMainDrawMask|=PMD_TRIM;
			PageMainTrimShow(v);
		}
		else
		{
			BeepMusic(MusicTrimMax);
			KeyStopCnt();
		}
	}
	if(KeyTstDown(keydec))
	{
		if(v>-TRIM_MAX)
		{
			if(--v==0)
			{
				BeepMusic(MusicTrimZero);
				KeyStopCnt();
			}
			else
			{
				PageMainTrimBeep(v);
			}
			PageMainDrawMask|=PMD_TRIM;
			PageMainTrimShow(v);
		}
		else
		{
			BeepMusic(MusicTrimMax);
			KeyStopCnt();
		}
	}

	Model.Trim[trimidx]=v;	
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  ��������ƺͰ�������
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
void TrimProc(void)
{	
	//�����ҳ������ҳ��������΢�������ڿ�����ʱ��	
	if(PageStack[0]!=PageMain)	return;
	
	//΢����ť����
	PageMainTrimProc(KEY_TRIM_AL,KEY_TRIM_AR,0);
	PageMainTrimProc(KEY_TRIM_TD,KEY_TRIM_TU,1);
	PageMainTrimProc(KEY_TRIM_ED,KEY_TRIM_EU,2);
	PageMainTrimProc(KEY_TRIM_RR,KEY_TRIM_RL,3);	
	
	//΢�������Զ�����
	if(PageMainTrimShowCnt==1)
	{
		PageMainTrimShow(TRIM_NUM_HIDE);
		PageMainDrawMask|=PMD_TRIM;
	}
}
