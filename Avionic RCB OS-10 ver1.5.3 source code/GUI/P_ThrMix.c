#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleThrMixerSet[]={"����>ƽ����","Thr Mixer Set"};
CSTR AntiRoll_Ail[]={"����Ť-����:"," Roll-Ail:"};
CSTR AntiYaw_Rud[]={"��ƫ��-����:","  Yaw-Rud:"};
CSTR AntiPitch_Ele[]={"�����-����:","Pitch-Ele:"};

CMENUITEM MenuThrMixSet[]={
	{TitleThrMixerSet	,0,0,0,3,0,0,0},
	{AntiRoll_Ail	,0,0, -MAX_THM,+MAX_THM,&Model.ThrMix[0],0,0,0},
	{AntiYaw_Rud	,0,0, -MAX_THM,+MAX_THM,&Model.ThrMix[1],0,0,0},
	{AntiPitch_Ele	,0,0, -MAX_THM,+MAX_THM,&Model.ThrMix[2],0,0,0},
};

//  ���Ż�ز˵��������
u32 PageThrMixSet(u8 event)
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
		ms.Param=0xff;
		ms.Total=MenuThrMixSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
		
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuThrMixSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
