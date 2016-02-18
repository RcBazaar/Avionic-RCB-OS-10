#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CMENUITEM *MenuChMap;

CSTR TitleAeroChMap[]={"固定翼通道映射","Aero Ch Map"};
CSTR TitleHeliChMap[]={"直升机通道映射","Heli Ch Map"};

CMENUITEM MenuChMapA[]={
	{TitleAeroChMap	,0,0,0,10,0,0,0},
	{Ch1,0,PageOptionChSelA,0,10,&Model.ChMap[0],0,0,0},
	{Ch2,0,PageOptionChSelA,0,10,&Model.ChMap[1],0,0,1},
	{Ch3,0,PageOptionChSelA,0,10,&Model.ChMap[2],0,0,2},
	{Ch4,0,PageOptionChSelA,0,10,&Model.ChMap[3],0,0,3},
	{Ch5,0,PageOptionChSelA,0,10,&Model.ChMap[4],0,0,4},
	{Ch6,0,PageOptionChSelA,0,10,&Model.ChMap[5],0,0,5},
	{Ch7,0,PageOptionChSelA,0,10,&Model.ChMap[6],0,0,6},
	{Ch8,0,PageOptionChSelA,0,10,&Model.ChMap[7],0,0,7},
	{Ch9,0,PageOptionChSelA,0,10,&Model.ChMap[8],0,0,8},
	{Ch10,0,PageOptionChSelA,0,10,&Model.ChMap[9],0,0,9},
};

CMENUITEM MenuChMapH[]={
	{TitleHeliChMap	,0,0,0,10,0,0,0},
	{Ch1,0,PageOptionChSelH,0,10,&Model.ChMap[0],0,0,0},
	{Ch2,0,PageOptionChSelH,0,10,&Model.ChMap[1],0,0,1},
	{Ch3,0,PageOptionChSelH,0,10,&Model.ChMap[2],0,0,2},
	{Ch4,0,PageOptionChSelH,0,10,&Model.ChMap[3],0,0,3},
	{Ch5,0,PageOptionChSelH,0,10,&Model.ChMap[4],0,0,4},
	{Ch6,0,PageOptionChSelH,0,10,&Model.ChMap[5],0,0,5},
	{Ch7,0,PageOptionChSelH,0,10,&Model.ChMap[6],0,0,6},
	{Ch8,0,PageOptionChSelH,0,10,&Model.ChMap[7],0,0,7},
	{Ch9,0,PageOptionChSelH,0,10,&Model.ChMap[8],0,0,8},
	{Ch10,0,PageOptionChSelH,0,10,&Model.ChMap[9],0,0,9},
};

//  模型通道映射处理过程
u32 PageMenuChMap(u8 event)
{
	static MENUSTAT ms;
			
	if(event==PV_INIT)
	{
		if(Model.Type==MT_AERO)	MenuChMap=MenuChMapA;
		else					MenuChMap=MenuChMapH;
		
		LCD_Clr_All();//清屏
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuChMap[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuChMap,&ms);
		
	//回车键处理
	if(KeyTstDown(KEY_ENT))
	{
		//如果有子菜单或子功能，进入
		if(MenuChMap[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuChMap[ms.iFocus+1].SubItem,PV_INIT);
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
