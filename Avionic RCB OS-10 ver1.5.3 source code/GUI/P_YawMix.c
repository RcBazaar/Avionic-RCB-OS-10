#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleYawMixerSet[]={"������>�������","Ail/Rud>Ele"};
CSTR AilLeft[]={"����-��:","Ail-Left:"};
CSTR RudLeft[]={"����-��:","Rud-Left:"};
CSTR XxxRight[]={"    -��:","  -Right:"};

CMENUITEM MenuYawMixSet[]={
	{TitleYawMixerSet	,0,0,0,4,0,0,0},
	{AilLeft	,0,0, -MAX_YWM,+MAX_YWM,&Model.YawMix.Al2E,0,0,0},
	{XxxRight	,0,0, -MAX_YWM,+MAX_YWM,&Model.YawMix.Ar2E,0,0,0},
	{RudLeft	,0,0, -MAX_YWM,+MAX_YWM,&Model.YawMix.Rl2E,0,0,0},
	{XxxRight	,0,0, -MAX_YWM,+MAX_YWM,&Model.YawMix.Rr2E,0,0,0},
};

//  ģ�Ͳ������ڲ˵��������
u32 PageYawMixSet(u8 event)
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
		ms.Total=MenuYawMixSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}	
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuYawMixSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
