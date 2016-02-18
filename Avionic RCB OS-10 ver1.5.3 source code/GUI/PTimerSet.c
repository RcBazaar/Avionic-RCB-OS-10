#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

s16 PageTimeReset;
char PageAlarmTime[6];

CSTR TitleTimerSet[]={"定时器设置","Timer Setting"};
CSTR TimerMode[]={"类型:","Mode:"};
CSTR TimerSwitch[]={"控制开关:","Switch:"};
CSTR ThrPosition[]={"油门位置:","Thr Pos:"};
CSTR AlarmTime[]={"提醒时间:","Alarm Time:"};
CSTR TimeReset[]={"复位:","Reset:"};

CSTR TimeReseted[]={"定时器已复位","Time Reseted"};

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
		LCD_Clr_All();//清屏
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
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuTimerSet,&ms);
	
	if(MenuTimerSet[ms.iFocus+1].SubItem)//子菜单或子功能
	{
		if(KeyTstDown(KEY_ENT))
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuTimerSet[ms.iFocus+1].SubItem,PV_INIT);
		}
	}
	else if(MenuTimerSet[ms.iFocus+1].pValue==&PageTimeReset)//复位功能
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
	
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);

	return 0;
}
