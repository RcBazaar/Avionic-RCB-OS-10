#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleDeltaMixerSet[]={"三角翼混控","Delta Mixer Set"};
CSTR DeltaMixer[]={"混控开关:","Delta Mixer:"};
CSTR DAil2Ail[]={"副翼>副翼:","Ail>Ail:"};
CSTR DEle2Ail[]={"升降>副翼:","Ele>Ail:"};
CSTR DAil2Ele[]={"副翼>升降:","Ail>Ele:"};
CSTR DEle2Ele[]={"升降>升降:","Ele>Ele:"};

CMENUITEM MenuDeltaSet[]={
	{TitleDeltaMixerSet	,0,0,0,5,0,0,0},
	{DeltaMixer	,0,PageOptionOnOff,0,1,&Model.Delta.Enable,0,0,0},
	{DAil2Ail	,0,0	, -MAX_DLT,+MAX_DLT,&Model.Delta.A2A,0,0,+MAX_DLT},
	{DEle2Ail	,0,0	, -MAX_DLT,+MAX_DLT,&Model.Delta.E2A,0,0,-MAX_DLT},
	{DAil2Ele	,0,0	, -MAX_DLT,+MAX_DLT,&Model.Delta.A2E,0,0,+MAX_DLT},
	{DEle2Ele	,0,0	, -MAX_DLT,+MAX_DLT,&Model.Delta.E2E,0,0,+MAX_DLT},
};

//  模型参数调节菜单处理过程
u32 PageDeltaSet(u8 event)
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
		ms.Param=0xff;
		ms.Total=MenuDeltaSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	//根据使能开关确定菜单数
	if(Model.Delta.Enable)	ms.Total=MenuDeltaSet[0].Max;
	else					ms.Total=1;
	
	//菜单数变化重绘
	if(ms.Param!=ms.Total)
	{
		LCD_Clr_All();//清屏
		ms.Param=ms.Total;
		ms.DrawMask=PD_ALL;	
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuDeltaSet,&ms);
		
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		//关闭其他混控
		Model.Vtail.Enable = 0;
		Model.FlpAil.Enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
