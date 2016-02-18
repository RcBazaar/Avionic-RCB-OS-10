#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleFlyDream[]={"Avionic RCB-OS10","Avionic RCB-OS10"};
CSTR ModelParamSet[]={"ģ�Ͳ�������","Parameter Setting"};
CSTR ModelAdvSet[]={"ģ�͸߼�����","Model Setting"};
CSTR ControllerSet[]={"ң��������","Transmitter Setting"};
CSTR TimerSet[]={"��ʱ������","Timer Setting"};
CSTR ReceiverSet[]={"���ջ�����","Receiver Setting"};
CSTR Monitor[]={"ͨ��������","Monitor"};
CSTR About[]={"����...","About"};

CMENUITEM MenuMain[]={
	{TitleFlyDream,0,0,0,7,0,0,0},
	{ModelParamSet		,PageAeroSet,0,0,0,0,0,0},
	{ModelAdvSet		,PageMenuMod,0,0,0,0,0,0},
	{ControllerSet		,PageMenuSys,0,0,0,0,0,0},
	{TimerSet			,PageTimerSet,0,0,0,0,0,0},
	{ReceiverSet		,PageMenuBack,0,0,0,0,0,0},
	{Monitor			,PageMonitor,0,0,0,0,0,0},
	{About				,PageAbout,0,0,0,0,0,0},	
};

//  ��������ƺͰ�������
u32 PageMenuMain(u8 event)
{
	static MENUSTAT ms;
	
	if(event==PV_INIT)
	{
		LCD_Clr_All();//����
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;//�˵��ѳ�ʼ�����
		}
		ms.Total=MenuMain[0].Max;//�������Ӳ˵�
		ms.DrawMask=PD_ALL;	
		return 1;
	}	
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuMain,&ms);
		
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{
		//������Ӳ˵����ӹ��ܣ�����
		if(MenuMain[ms.iFocus+1].SubItem==PageAeroSet)//ģ������
		{
			BeepMusic(MusicEnter);
			if(Model.Type==MT_AERO)	PageEnter(PageAeroSet,PV_INIT);
			else					PageEnter(PageHeliSet,PV_INIT);
		}
		else if(MenuMain[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);
			PageEnter((PAGEPROC)MenuMain[ms.iFocus+1].SubItem,PV_INIT);
		}	 
	}
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);

	return 0;
}
