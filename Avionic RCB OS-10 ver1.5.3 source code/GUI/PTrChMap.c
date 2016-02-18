#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleTrainerChMap[]={"教练通道映射","Trainer Ch Map"};

CMENUITEM MenuTrChMap[]={
	{TitleTrainerChMap	,0,0,0,10,0,0,0},
	{Ch1,0,PageOptionTrChSel, 0,10,&Model.TrChMap[0],0,0,0},
	{Ch2,0,PageOptionTrChSel, 0,10,&Model.TrChMap[1],0,0,1},
	{Ch3,0,PageOptionTrChSel, 0,10,&Model.TrChMap[2],0,0,2},
	{Ch4,0,PageOptionTrChSel, 0,10,&Model.TrChMap[3],0,0,3},
	{Ch5,0,PageOptionTrChSel, 0,10,&Model.TrChMap[4],0,0,4},
	{Ch6,0,PageOptionTrChSel, 0,10,&Model.TrChMap[5],0,0,5},
	{Ch7,0,PageOptionTrChSel, 0,10,&Model.TrChMap[6],0,0,6},
	{Ch8,0,PageOptionTrChSel, 0,10,&Model.TrChMap[7],0,0,7},
	{Ch9,0,PageOptionTrChSel, 0,10,&Model.TrChMap[8],0,0,8},
	{Ch10,0,PageOptionTrChSel,0,10,&Model.TrChMap[9],0,0,9},
};

u32 PageMenuTrChMap(u8 event)
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
		ms.Total=MenuTrChMap[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		ms.DrawMask = PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuTrChMap,&ms);
		
	//回车键处理
	if(KeyTstDown(KEY_ENT))
	{
		//如果有子菜单或子功能，进入
		if(MenuTrChMap[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuTrChMap[ms.iFocus+1].SubItem,PV_INIT);
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
