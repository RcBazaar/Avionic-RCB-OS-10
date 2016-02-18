#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleVTailMixerSet[]={" V 尾混控","V-Tail Mixer Set"};
CSTR VTailMixer[]={"混控开关:","V-Tail Mixer:"};
CSTR VEle2Ele[]={"升降>升降:","Ele>Ele:"};
CSTR VRud2Ele[]={"方向>升降:","Rud>Ele:"};
CSTR VEle2Rud[]={"升降>方向:","Ele>Rud:"};
CSTR VRud2Rud[]={"方向>方向:","Rud>Rud:"};

CMENUITEM MenuVtailSet[]={
	{TitleVTailMixerSet	,0,0,0,5,0,0,0},
	{VTailMixer	,0,PageOptionOnOff,0,1,&Model.Vtail.Enable,0,0,0},
	{VEle2Ele	,0,0, -MAX_VTL,+MAX_VTL,&Model.Vtail.E2E,0,0,+MAX_VTL},
	{VRud2Ele	,0,0, -MAX_VTL,+MAX_VTL,&Model.Vtail.R2E,0,0,-MAX_VTL},
	{VEle2Rud	,0,0, -MAX_VTL,+MAX_VTL,&Model.Vtail.E2R,0,0,+MAX_VTL},
	{VRud2Rud	,0,0, -MAX_VTL,+MAX_VTL,&Model.Vtail.R2R,0,0,+MAX_VTL},
};

//  模型参数调节菜单处理过程
u32 PageVtailSet(u8 event)
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
		ms.Total=MenuVtailSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	//根据使能开关确定菜单数
	if(Model.Vtail.Enable)	ms.Total=MenuVtailSet[0].Max;
	else					ms.Total=1;
	
	//菜单数变化重绘
	if(ms.Param!=ms.Total)
	{
		LCD_Clr_All();//清屏
		ms.Param=ms.Total;
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuVtailSet,&ms);
		
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0; 
		//关闭其他混控
		Model.Delta.Enable = 0;
		Model.FlpAil.Enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
