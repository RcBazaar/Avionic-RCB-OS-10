#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleDeltaMixerSet[]={"��������","Delta Mixer Set"};
CSTR DeltaMixer[]={"��ؿ���:","Delta Mixer:"};
CSTR DAil2Ail[]={"����>����:","Ail>Ail:"};
CSTR DEle2Ail[]={"����>����:","Ele>Ail:"};
CSTR DAil2Ele[]={"����>����:","Ail>Ele:"};
CSTR DEle2Ele[]={"����>����:","Ele>Ele:"};

CMENUITEM MenuDeltaSet[]={
	{TitleDeltaMixerSet	,0,0,0,5,0,0,0},
	{DeltaMixer	,0,PageOptionOnOff,0,1,&Model.Delta.Enable,0,0,0},
	{DAil2Ail	,0,0	, -MAX_DLT,+MAX_DLT,&Model.Delta.A2A,0,0,+MAX_DLT},
	{DEle2Ail	,0,0	, -MAX_DLT,+MAX_DLT,&Model.Delta.E2A,0,0,-MAX_DLT},
	{DAil2Ele	,0,0	, -MAX_DLT,+MAX_DLT,&Model.Delta.A2E,0,0,+MAX_DLT},
	{DEle2Ele	,0,0	, -MAX_DLT,+MAX_DLT,&Model.Delta.E2E,0,0,+MAX_DLT},
};

//  ģ�Ͳ������ڲ˵��������
u32 PageDeltaSet(u8 event)
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
		ms.Total=MenuDeltaSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	//����ʹ�ܿ���ȷ���˵���
	if(Model.Delta.Enable)	ms.Total=MenuDeltaSet[0].Max;
	else					ms.Total=1;
	
	//�˵����仯�ػ�
	if(ms.Param!=ms.Total)
	{
		LCD_Clr_All();//����
		ms.Param=ms.Total;
		ms.DrawMask=PD_ALL;	
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuDeltaSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		//�ر��������
		Model.Vtail.Enable = 0;
		Model.FlpAil.Enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
