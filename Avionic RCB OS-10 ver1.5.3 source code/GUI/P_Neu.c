#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleNeuSet[]={"����΢��","Sub Trim"};

CMENUITEM MenuNeuSet[]={
	{TitleNeuSet,0,0,0,10,0,0,0},
	{Ch1,0,0, -100,+100,&Model.Neu[0],0,0,0},
	{Ch2,0,0, -100,+100,&Model.Neu[1],0,0,0},
	{Ch3,0,0, -100,+100,&Model.Neu[2],0,0,0},
	{Ch4,0,0, -100,+100,&Model.Neu[3],0,0,0},
	{Ch5,0,0, -100,+100,&Model.Neu[4],0,0,0},
	{Ch6,0,0, -100,+100,&Model.Neu[5],0,0,0},
	{Ch7,0,0, -100,+100,&Model.Neu[6],0,0,0},
	{Ch8,0,0, -100,+100,&Model.Neu[7],0,0,0},
	{Ch9,0,0, -100,+100,&Model.Neu[8],0,0,0},
	{Ch10,0,0,-100,+100,&Model.Neu[9],0,0,0},
};

//  ģ�Ͳ������ڲ˵��������
u32 PageNeuSet(u8 event)
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
		ms.Total=MenuNeuSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuNeuSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
