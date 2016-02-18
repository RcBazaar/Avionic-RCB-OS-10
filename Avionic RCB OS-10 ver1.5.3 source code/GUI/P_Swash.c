#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleSwashSet[]={"б������","Swash"};
CSTR SwashType[]={"б������:","Swash Type:"};
CSTR AilRatio[]={"�����ر�:","Ail:"};
CSTR EleRatio[]={"������ر�:","Ele:"};
CSTR PitchRatio[]={"�ݾ��ر�:","Pitch:"};

CMENUITEM MenuSwashSet[]={
	{TitleSwashSet	,0,0,0,4,0,0,0},
	{SwashType	,0,PageOptionSwash,0,3,&Model.SwashType,0,0,0},
	{AilRatio	,0,0, -100,+100,&Model.Swash[0],0,0,50},
	{EleRatio	,0,0, -100,+100,&Model.Swash[1],0,0,50},
	{PitchRatio	,0,0, -100,+100,&Model.Swash[2],0,0,50},
};

//  �����ǲ˵�����
u32 PageSwashSet(u8 event)
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
		ms.Total=MenuSwashSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuSwashSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
