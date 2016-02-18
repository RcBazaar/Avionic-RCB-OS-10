#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleAilRudSet[]={"��������","Ail&Rud"};
CSTR AilRudMixer[]={"��ؿ���:","Ail&Rud Mixer:"};
CSTR Ail22Ail[]={"����>����:","Ail>Ail:"};
CSTR Ail2Rud[]={"����>����:","Ail>Rud:"};
CSTR Rud2Ail[]={"����>����:","Rud>Ail:"};
CSTR Rud2Rud[]={"����>����:","Rud>Rud:"};

CMENUITEM MenuAilRudSet[]={
	{TitleAilRudSet	,0,0,0,5,0,0,0},
	{AilRudMixer,0,PageOptionOnOff,0,1,&Model.AilRud.Enable,0,0,0},
	{Ail22Ail	,0,0, -MAX_ARM,+MAX_ARM,&Model.AilRud.A2A,0,0,+MAX_ARM},
	{Ail2Rud	,0,0, -MAX_ARM,+MAX_ARM,&Model.AilRud.A2R,0,0,-MAX_ARM},
	{Rud2Ail	,0,0, -MAX_ARM,+MAX_ARM,&Model.AilRud.R2A,0,0,+MAX_ARM},
	{Rud2Rud	,0,0, -MAX_ARM,+MAX_ARM,&Model.AilRud.R2R,0,0,+MAX_ARM},
};

u32 PageAilRudSet(u8 event)
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
		ms.Total=MenuAilRudSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	//����ʹ�ܿ���ȷ���˵���
	if(Model.AilRud.Enable)	ms.Total=MenuAilRudSet[0].Max;
	else					ms.Total=1;
	
	//�˵����仯�ػ�
	if(ms.Param!=ms.Total)
	{
		LCD_Clr_All();//����
		ms.Param=ms.Total;
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuAilRudSet,&ms);
	
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}








