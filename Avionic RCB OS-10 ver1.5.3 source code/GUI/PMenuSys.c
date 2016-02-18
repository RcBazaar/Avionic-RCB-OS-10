#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

s16 PageMenuSysReset;
char PageMenuSysVol[6];

CSTR TitleControllerSet[]={"遥控器设置","Transmitter Setting"};
CSTR TrimStepAdj[]={"微调步长:","Trim Step:"};
CSTR StickMode[]={"摇杆模式:","Stick Mode:"};
CSTR StickDeadZone[]={"摇杆死区:","Stick Centering:"};
CSTR Vibration[]={"振动提示:","Vibration:"};
CSTR Sound[]={"声音提示:","Sound:"};
CSTR KeyVolume[]={"按键音量:","Key Volume:"};
CSTR KeyTone[]={"按键音调:","Key Tone:"};
CSTR Backlight[]={"背光亮度:","Backlight:"};
CSTR Contrast[]={"对 比 度:","Contrast:"};
CSTR BacklightOff[]={"背光关闭:","Backlight ON:"};
CSTR BatType[]={"电池类型:","Battery:"};
CSTR AlarmVoltage[]={"报警电压:","Alarm Volt:"};
CSTR Language[]={"语言选择:","Language:"};
CSTR StickCalibration[]={"摇杆校准","Stick Calibration"};
CSTR Reset[]={"恢复默认值","Reset"};

CSTR ControllerReseted[]={"遥控器已恢复默认值!","Controller Reseted!"};

CMENUITEM MenuSys[]={
	{TitleControllerSet	,0,0,0,12,0,0,0},
	{TrimStepAdj		,0,0					, 1,10,&TxSys.TrimStep,0,0,2},
	{StickMode			,0,PageOptionStkType	, 0, 3,&TxSys.StkType,0,0},
	{StickDeadZone		,0,0					, 0,50,&TxSys.StkDa,0,0,0},
	{Vibration			,0,PageOptionOnOff		, 0, 1,&TxSys.Vibrator,0,0,1},
	{Sound				,0,PageOptionOnOff		, 0, 1,&TxSys.Music,0,0,1},
	{KeyVolume			,0,0					, 0,10,&TxSys.KeyBeep,0,0,8},
	{KeyTone			,0,0					, 0,30,&TxSys.KeyTone,0,0,20},
	{Backlight			,0,0					, 0,10,&TxSys.Light,0,0,8},
	{Contrast			,0,0					, 0,10,&TxSys.Contrast,0,0,4},
	{BacklightOff		,0,PageOptionLightOff	, 0, 6,&TxSys.LightOff,0,0,3},
	//{BatType			,0,PageOptionTxBatType	, 0, 2,&TxSys.BatType,0,0,0},
	{AlarmVoltage		,0,0					,65,126,&TxSys.BatWarn,0,PageMenuSysVol,74},
	//{Language			,0,PageOptionLanguage	, 0, 1,&TxSys.Language,0,0,1},
	{StickCalibration	,PageStkCal,0,0,0,0,0,0},
	//{Reset			,0,PageOptionOkExe2		, 0, 1,&PageMenuSysReset,0,0,0},
};

//  主界面绘制和按键处理
u32 PageMenuSys(u8 event)
{
	static MENUSTAT ms;
	static s16 pre_stktype = 0;
	static s16 pre_language = 0;
		
	//初始化界面参数
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total = MenuSys[0].Max;
		ms.DrawMask=PD_ALL;	
		
		pre_stktype = TxSys.StkType;
		pre_language = TxSys.Language;
		
		return 1;
	}
	
	//将电压转为字符串
	PageMenuSysVol[0]=TxSys.BatWarn>=100?('0'+TxSys.BatWarn/100):(' ');
	PageMenuSysVol[1]=TxSys.BatWarn>=10?('0'+TxSys.BatWarn/10%10):(' ');
	PageMenuSysVol[2]='.';	
	PageMenuSysVol[3]=TxSys.BatWarn%10+'0';	
	PageMenuSysVol[4]='V';
	PageMenuSysVol[5]=0;
	
	//根据左右手开关调整左右手
	if(SwTstZYSState()==ZYS_LEFT)//左手模式1-3
	{
		if(TxSys.StkType<pre_stktype)//减
			TxSys.StkType = 1;
		if(TxSys.StkType>pre_stktype)//加
		{
			if(TxSys.StkType<=1)
				TxSys.StkType = 1;
			else
				TxSys.StkType = 3;
		}
	}
	if(SwTstZYSState()==ZYS_RIGHT)//右手模式0-2
	{
		if(TxSys.StkType<pre_stktype)//减
		{
			if(TxSys.StkType>=2)
				TxSys.StkType = 2;
			else
				TxSys.StkType = 0;
		}
		if(TxSys.StkType>pre_stktype)//加
			TxSys.StkType = 2;
	}
	pre_stktype = TxSys.StkType;	
	
	//语言切换时重新绘制全部元素
	if(TxSys.Language!=pre_language)
	{
		ms.DrawMask=PD_ALL;	
		pre_language = TxSys.Language;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuSys,&ms);
	
	if(MenuSys[ms.iFocus+1].SubItem)//子菜单或子功能
	{
		if(KeyTstDown(KEY_ENT))
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuSys[ms.iFocus+1].SubItem,PV_INIT);
		}
	}
	else if(MenuSys[ms.iFocus+1].pValue==&PageMenuSysReset)
	{
		if(KeyTstHold(KEY_ENT))//复位功能
		{
			if(PageMenuSysReset==1)
			{
				PageMenuSysReset=0;
				TxSys = TxSysDef;
				PageAlert(ControllerReseted[TxSys.Language],1000,PV_INIT);
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
