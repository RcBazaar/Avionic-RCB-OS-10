#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleFlpAilSet[]={"������","FlapAil Mixer Set"};
CSTR FlpAilMixer[]={"��ؿ���:","Flaperon Mixer:"};
CSTR Ail2Ail[]={"����>����:"," Ail>Ail:"};
CSTR Ail2Flap[]={"����>����:","Ail>Flap:"};
CSTR Flap2Ail[]={"����>����:","Flap>Ail:"};
CSTR Flap2Flap[]={"����>����:","Flap>Ail:"};

CMENUITEM MenuFlpAilSet[]={
	{TitleFlpAilSet	,0,0,0,5,0,0,0},
	{FlpAilMixer,0,PageOptionOnOff,0,1,&Model.FlpAil.Enable,0,0,0},
	{Ail2Ail	,0,0, -MAX_FAM,+MAX_FAM,&Model.FlpAil.A2A,0,0,+MAX_VTL},
	{Ail2Flap	,0,0, -MAX_FAM,+MAX_FAM,&Model.FlpAil.A2F,0,0,-MAX_FAM},
	{Flap2Ail	,0,0, -MAX_FAM,+MAX_FAM,&Model.FlpAil.F2A,0,0,+MAX_FAM},
	{Flap2Flap	,0,0, -MAX_FAM,+MAX_FAM,&Model.FlpAil.F2F,0,0,+MAX_FAM},
};

//ģ�Ͳ������ڲ˵��������
u32 PageFlpAilSet(u8 event)
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
		ms.Total=MenuFlpAilSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	//����ʹ�ܿ���ȷ���˵���
	if(Model.FlpAil.Enable)	ms.Total=MenuFlpAilSet[0].Max;
	else					ms.Total=1;
	
	//�˵����仯�ػ�
	if(ms.Param!=ms.Total)
	{
		LCD_Clr_All();//����
		ms.Param=ms.Total;
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuFlpAilSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		//�ر��������
		Model.Delta.Enable = 0;
		Model.Vtail.Enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
