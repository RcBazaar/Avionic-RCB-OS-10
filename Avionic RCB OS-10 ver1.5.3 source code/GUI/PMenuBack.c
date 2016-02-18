#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

char PageMenuSysRxVol[6];

CSTR TitleReceiverSet[]={"接收机设置","Receiver Setting"};
CSTR RxBatType[]={"电池类型:","Battery:"};
CSTR RxAlarmVoltage[]={"报警电压:","Alarm Volt:"};
CSTR FallSave[]={"失控保护:","Fail Safe:"};

CMENUITEM MenuBack[]={
	{TitleReceiverSet,0,0,0,1,0,0,0},
	//{RxBatType		,0,PageOptionRxBatType,0,3,&Model.RxBatType,0,0,3},
	{RxAlarmVoltage	,0,0,37,222,&Model.RxBatWarn,0,PageMenuSysRxVol,37},
	{FallSave		,0,PageOptionOnOff,0,0,0,0,0},
};

u32 PageMenuBack(u8 event)
{
	static MENUSTAT ms;
	
	//初始化界面参数
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total = MenuBack[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}                                                                                                                                                                                                                                               
	
	//将电压转为字符串
	PageMenuSysRxVol[0]=Model.RxBatWarn>=100?('0'+Model.RxBatWarn/100):(' ');
	PageMenuSysRxVol[1]=Model.RxBatWarn>=10?('0'+Model.RxBatWarn/10%10):(' ');
	PageMenuSysRxVol[2]='.';	
	PageMenuSysRxVol[3]=Model.RxBatWarn%10+'0';	
	PageMenuSysRxVol[4]='V';
	PageMenuSysRxVol[5]=0;
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuBack,&ms);
	
	//回车键处理
	if(KeyTstDown(KEY_ENT))
	{
		//如果有子菜单或子功能，进入
		if(MenuBack[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuBack[ms.iFocus+1].SubItem,PV_INIT);
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
