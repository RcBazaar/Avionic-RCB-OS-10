#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleFlpAilSet[]={"襟副翼混控","FlapAil Mixer Set"};
CSTR FlpAilMixer[]={"混控开关:","Flaperon Mixer:"};
CSTR Ail2Ail[]={"副翼>副翼:"," Ail>Ail:"};
CSTR Ail2Flap[]={"副翼>襟翼:","Ail>Flap:"};
CSTR Flap2Ail[]={"襟翼>副翼:","Flap>Ail:"};
CSTR Flap2Flap[]={"襟翼>襟翼:","Flap>Ail:"};

CMENUITEM MenuFlpAilSet[]={
	{TitleFlpAilSet	,0,0,0,5,0,0,0},
	{FlpAilMixer,0,PageOptionOnOff,0,1,&Model.FlpAil.Enable,0,0,0},
	{Ail2Ail	,0,0, -MAX_FAM,+MAX_FAM,&Model.FlpAil.A2A,0,0,+MAX_VTL},
	{Ail2Flap	,0,0, -MAX_FAM,+MAX_FAM,&Model.FlpAil.A2F,0,0,-MAX_FAM},
	{Flap2Ail	,0,0, -MAX_FAM,+MAX_FAM,&Model.FlpAil.F2A,0,0,+MAX_FAM},
	{Flap2Flap	,0,0, -MAX_FAM,+MAX_FAM,&Model.FlpAil.F2F,0,0,+MAX_FAM},
};

//模型参数调节菜单处理过程
u32 PageFlpAilSet(u8 event)
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
		ms.Total=MenuFlpAilSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	//根据使能开关确定菜单数
	if(Model.FlpAil.Enable)	ms.Total=MenuFlpAilSet[0].Max;
	else					ms.Total=1;
	
	//菜单数变化重绘
	if(ms.Param!=ms.Total)
	{
		LCD_Clr_All();//清屏
		ms.Param=ms.Total;
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuFlpAilSet,&ms);
		
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		//关闭其他混控
		Model.Delta.Enable = 0;
		Model.Vtail.Enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
