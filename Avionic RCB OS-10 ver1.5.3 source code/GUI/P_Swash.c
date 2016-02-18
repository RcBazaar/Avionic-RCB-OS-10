#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleSwashSet[]={"斜盘设置","Swash"};
CSTR SwashType[]={"斜盘类型:","Swash Type:"};
CSTR AilRatio[]={"副翼混控比:","Ail:"};
CSTR EleRatio[]={"升降混控比:","Ele:"};
CSTR PitchRatio[]={"螺距混控比:","Pitch:"};

CMENUITEM MenuSwashSet[]={
	{TitleSwashSet	,0,0,0,4,0,0,0},
	{SwashType	,0,PageOptionSwash,0,3,&Model.SwashType,0,0,0},
	{AilRatio	,0,0, -100,+100,&Model.Swash[0],0,0,50},
	{EleRatio	,0,0, -100,+100,&Model.Swash[1],0,0,50},
	{PitchRatio	,0,0, -100,+100,&Model.Swash[2],0,0,50},
};

//  陀螺仪菜单处理
u32 PageSwashSet(u8 event)
{
	static MENUSTAT ms;
			
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuSwashSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuSwashSet,&ms);
		
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
