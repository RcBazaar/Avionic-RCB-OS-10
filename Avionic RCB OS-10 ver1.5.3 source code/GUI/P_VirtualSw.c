#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleVirtualSw[]={"���⿪��","Virtual Switch"};
CSTR VSEStick[]={"SE -ҡ��:","SE -Stick:"};
CSTR VSFStick[]={"SF -ҡ��:","SF -Stick:"};
CSTR VSXValue[]={"     -ֵ:","   -Value:"};

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
		LCD_Clr_All();//����
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
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuVirtualSw,&ms);
	
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}











