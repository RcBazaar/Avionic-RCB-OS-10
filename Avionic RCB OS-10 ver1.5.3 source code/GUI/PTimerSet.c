#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

s16 PageTimeReset;
char PageAlarmTime[6];

CSTR TitleTimerSet[]={"��ʱ������","Timer Setting"};
CSTR TimerMode[]={"����:","Mode:"};
CSTR TimerSwitch[]={"���ƿ���:","Switch:"};
CSTR ThrPosition[]={"����λ��:","Thr Pos:"};
CSTR AlarmTime[]={"����ʱ��:","Alarm Time:"};
CSTR TimeReset[]={"��λ:","Reset:"};

CSTR TimeReseted[]={"��ʱ���Ѹ�λ","Time Reseted"};

CMENUITEM MenuTimerSet[]={
	{TitleTimerSet,0,0,0,5,0,0,0},
	{TimerMode	,0,PageOptionTmrType	, 0, 2,&Model.Timer.Type,0,0,0},
	{TimerSwitch,0,PageOptionTmrSw		, 0, 4,&Model.Timer.Sw,0,0,0},
	{ThrPosition,0,0,-100,100,&Model.Timer.Thr,0,0,-90},
	{AlarmTime	,PageEditTime,0,0,0,0,0,PageAlarmTime},
	{TimeReset	,0,PageOptionOkExe2,0,1,&PageTimeReset,0,0,0},
};

u32 PageTimerSet(u8 event)
{
	u8 Minute,Second;
	static MENUSTAT ms;
	
	if(event==PV_INIT)
	{
		LCD_Clr_All();//����
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuTimerSet[0].Max;
		ms.DrawMask=PD_ALL;	
		
		Minute = Model.Timer.Time/60%100;
		Second = Model.Timer.Time%60;
		PageAlarmTime[0] = '0'+Minute/10;
		PageAlarmTime[1] = '0'+Minute%10;
		PageAlarmTime[2] = ':';
		PageAlarmTime[3] = '0'+Second/10;
		PageAlarmTime[4] = '0'+Second%10;
		PageAlarmTime[5] = 0;
		
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuTimerSet,&ms);
	
	if(MenuTimerSet[ms.iFocus+1].SubItem)//�Ӳ˵����ӹ���
	{
		if(KeyTstDown(KEY_ENT))
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuTimerSet[ms.iFocus+1].SubItem,PV_INIT);
		}
	}
	else if(MenuTimerSet[ms.iFocus+1].pValue==&PageTimeReset)//��λ����
	{
		if(KeyTstDown(KEY_ENT))
		{
			if(PageTimeReset==1)
			{
				PageTimeReset = 0;
				TxTimer.Reset = 1;
				PageAlert(TimeReseted[TxSys.Language],1000,PV_INIT);
			}
		}
	}
	
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);

	return 0;
}
