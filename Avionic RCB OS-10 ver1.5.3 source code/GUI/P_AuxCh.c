#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleAuxSet[]={"辅助通道设置","Auxillary"};
CSTR GearL[]={"起落架-低:","Gear-Low:"};
CSTR FlapL[]={"襟 翼 -低:","Flap-Low:"};
CSTR Aux1L[]={"辅助1 -低:","AUX1-Low:"};
CSTR Aux2L[]={"辅助2 -低:","AUX2-Low:"};
CSTR XxxxM[]={"      -中:"," -Middle:"};
CSTR XxxxH[]={"      -高:","   -High:"};

CMENUITEM MenuAeroAuxChSet[]={
	{TitleAuxSet,0,0,0,12,0,0,0},
	{GearL,0,0,-TRV_AUX,TRV_AUX,&Model.Gear[0],0,0,-100},
	{XxxxM,0,0,-TRV_AUX,TRV_AUX,&Model.Gear[1],0,0,0},
	{XxxxH,0,0,-TRV_AUX,TRV_AUX,&Model.Gear[2],0,0, 100},
	{FlapL,0,0,-TRV_AUX,TRV_AUX,&Model.Flap[0],0,0,-100},
	{XxxxM,0,0,-TRV_AUX,TRV_AUX,&Model.Flap[1],0,0,0},
	{XxxxH,0,0,-TRV_AUX,TRV_AUX,&Model.Flap[2],0,0, 100},
	{Aux1L,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[0][0],0,0,-100},
	{XxxxM,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[0][1],0,0,0},
	{XxxxH,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[0][2],0,0, 100},
	{Aux2L,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[1][0],0,0,-100},
	{XxxxM,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[1][1],0,0,0},
	{XxxxH,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[1][2],0,0, 100},
};

CMENUITEM MenuHeliAuxChSet[]={
	{TitleAuxSet,0,0,0,6,0,0,0},
	{Aux1L,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[0][0],0,0,-100},
	{XxxxM,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[0][1],0,0,0},
	{XxxxH,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[0][2],0,0, 100},
	{Aux2L,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[1][0],0,0,-100},
	{XxxxM,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[1][1],0,0,0},
	{XxxxH,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[1][2],0,0, 100},
};

//  辅助通道数调节菜单处理过程
CMENUITEM *MenuAux;
u32 PageAuxChSet(u8 event)
{
	static MENUSTAT ms;
			
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
		if(ms.Param!=Model.Type)//模型类型变化要重新初始化
		{
			ms.Init=0;
			ms.Param=Model.Type;
		}
		
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		MenuAux=(Model.Type==MT_AERO)?MenuAeroAuxChSet:MenuHeliAuxChSet;
		ms.Total=MenuAux[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuAux,&ms);
		
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
