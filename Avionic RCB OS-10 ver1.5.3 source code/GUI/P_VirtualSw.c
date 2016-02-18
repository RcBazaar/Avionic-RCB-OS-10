#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleVirtualSw[]={"虚拟开关","Virtual Switch"};
CSTR VSEStick[]={"SE -摇杆:","SE -Stick:"};
CSTR VSFStick[]={"SF -摇杆:","SF -Stick:"};
CSTR VSXValue[]={"     -值:","   -Value:"};

CMENUITEM MenuVirtualSw[]={
	{TitleVirtualSw,0,0,0,4,0,0,0},
	{VSEStick,0,PageOptionVirtualSw,   0,3  ,&Model.VirtualSwStk[0],0,0,0},
	{VSXValue,0,0,-100,100,&Model.VirtualSwVal[0],0,0,0},
	{VSFStick,0,PageOptionVirtualSw,   0,3  ,&Model.VirtualSwStk[1],0,0,3},
	{VSXValue,0,0,-100,100,&Model.VirtualSwVal[1],0,0,0},
};

u32 PageVirtualSw(u8 event)
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
		ms.Total=MenuVirtualSw[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuVirtualSw,&ms);
	
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}











