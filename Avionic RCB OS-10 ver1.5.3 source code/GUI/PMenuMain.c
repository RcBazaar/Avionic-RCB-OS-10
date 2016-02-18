#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleFlyDream[]={"Avionic RCB-OS10","Avionic RCB-OS10"};
CSTR ModelParamSet[]={"模型参数调整","Parameter Setting"};
CSTR ModelAdvSet[]={"模型高级设置","Model Setting"};
CSTR ControllerSet[]={"遥控器设置","Transmitter Setting"};
CSTR TimerSet[]={"定时器设置","Timer Setting"};
CSTR ReceiverSet[]={"接收机设置","Receiver Setting"};
CSTR Monitor[]={"通道监视器","Monitor"};
CSTR About[]={"关于...","About"};

CMENUITEM MenuMain[]={
	{TitleFlyDream,0,0,0,7,0,0,0},
	{ModelParamSet		,PageAeroSet,0,0,0,0,0,0},
	{ModelAdvSet		,PageMenuMod,0,0,0,0,0,0},
	{ControllerSet		,PageMenuSys,0,0,0,0,0,0},
	{TimerSet			,PageTimerSet,0,0,0,0,0,0},
	{ReceiverSet		,PageMenuBack,0,0,0,0,0,0},
	{Monitor			,PageMonitor,0,0,0,0,0,0},
	{About				,PageAbout,0,0,0,0,0,0},	
};

//  主界面绘制和按键处理
u32 PageMenuMain(u8 event)
{
	static MENUSTAT ms;
	
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;//菜单已初始化完成
		}
		ms.Total=MenuMain[0].Max;//共六个子菜单
		ms.DrawMask=PD_ALL;	
		return 1;
	}	
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuMain,&ms);
		
	//回车键处理
	if(KeyTstDown(KEY_ENT))
	{
		//如果有子菜单或子功能，进入
		if(MenuMain[ms.iFocus+1].SubItem==PageAeroSet)//模型设置
		{
			BeepMusic(MusicEnter);
			if(Model.Type==MT_AERO)	PageEnter(PageAeroSet,PV_INIT);
			else					PageEnter(PageHeliSet,PV_INIT);
		}
		else if(MenuMain[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);
			PageEnter((PAGEPROC)MenuMain[ms.iFocus+1].SubItem,PV_INIT);
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
