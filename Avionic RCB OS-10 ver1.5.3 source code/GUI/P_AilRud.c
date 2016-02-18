#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleAilRudSet[]={"副翼方向混控","Ail&Rud"};
CSTR AilRudMixer[]={"混控开关:","Ail&Rud Mixer:"};
CSTR Ail22Ail[]={"副翼>副翼:","Ail>Ail:"};
CSTR Ail2Rud[]={"副翼>方向:","Ail>Rud:"};
CSTR Rud2Ail[]={"方向>副翼:","Rud>Ail:"};
CSTR Rud2Rud[]={"方向>方向:","Rud>Rud:"};

CMENUITEM MenuAilRudSet[]={
	{TitleAilRudSet	,0,0,0,5,0,0,0},
	{AilRudMixer,0,PageOptionOnOff,0,1,&Model.AilRud.Enable,0,0,0},
	{Ail22Ail	,0,0, -MAX_ARM,+MAX_ARM,&Model.AilRud.A2A,0,0,+MAX_ARM},
	{Ail2Rud	,0,0, -MAX_ARM,+MAX_ARM,&Model.AilRud.A2R,0,0,-MAX_ARM},
	{Rud2Ail	,0,0, -MAX_ARM,+MAX_ARM,&Model.AilRud.R2A,0,0,+MAX_ARM},
	{Rud2Rud	,0,0, -MAX_ARM,+MAX_ARM,&Model.AilRud.R2R,0,0,+MAX_ARM},
};

u32 PageAilRudSet(u8 event)
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
		ms.Total=MenuAilRudSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	//根据使能开关确定菜单数
	if(Model.AilRud.Enable)	ms.Total=MenuAilRudSet[0].Max;
	else					ms.Total=1;
	
	//菜单数变化重绘
	if(ms.Param!=ms.Total)
	{
		LCD_Clr_All();//清屏
		ms.Param=ms.Total;
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuAilRudSet,&ms);
	
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}








