#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleYawMixerSet[]={"副翼方向>升降混控","Ail/Rud>Ele"};
CSTR AilLeft[]={"副翼-左:","Ail-Left:"};
CSTR RudLeft[]={"方向-左:","Rud-Left:"};
CSTR XxxRight[]={"    -右:","  -Right:"};

CMENUITEM MenuYawMixSet[]={
	{TitleYawMixerSet	,0,0,0,4,0,0,0},
	{AilLeft	,0,0, -MAX_YWM,+MAX_YWM,&Model.YawMix.Al2E,0,0,0},
	{XxxRight	,0,0, -MAX_YWM,+MAX_YWM,&Model.YawMix.Ar2E,0,0,0},
	{RudLeft	,0,0, -MAX_YWM,+MAX_YWM,&Model.YawMix.Rl2E,0,0,0},
	{XxxRight	,0,0, -MAX_YWM,+MAX_YWM,&Model.YawMix.Rr2E,0,0,0},
};

//  模型参数调节菜单处理过程
u32 PageYawMixSet(u8 event)
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
		ms.Total=MenuYawMixSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}	
	
	//菜单通用处理过程
	PageMenuProc(MenuYawMixSet,&ms);
		
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
