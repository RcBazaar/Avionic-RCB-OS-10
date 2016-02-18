#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleVTailMixerSet[]={" V β���","V-Tail Mixer Set"};
CSTR VTailMixer[]={"��ؿ���:","V-Tail Mixer:"};
CSTR VEle2Ele[]={"����>����:","Ele>Ele:"};
CSTR VRud2Ele[]={"����>����:","Rud>Ele:"};
CSTR VEle2Rud[]={"����>����:","Ele>Rud:"};
CSTR VRud2Rud[]={"����>����:","Rud>Rud:"};

CMENUITEM MenuVtailSet[]={
	{TitleVTailMixerSet	,0,0,0,5,0,0,0},
	{VTailMixer	,0,PageOptionOnOff,0,1,&Model.Vtail.Enable,0,0,0},
	{VEle2Ele	,0,0, -MAX_VTL,+MAX_VTL,&Model.Vtail.E2E,0,0,+MAX_VTL},
	{VRud2Ele	,0,0, -MAX_VTL,+MAX_VTL,&Model.Vtail.R2E,0,0,-MAX_VTL},
	{VEle2Rud	,0,0, -MAX_VTL,+MAX_VTL,&Model.Vtail.E2R,0,0,+MAX_VTL},
	{VRud2Rud	,0,0, -MAX_VTL,+MAX_VTL,&Model.Vtail.R2R,0,0,+MAX_VTL},
};

//  ģ�Ͳ������ڲ˵��������
u32 PageVtailSet(u8 event)
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
		ms.Total=MenuVtailSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	//����ʹ�ܿ���ȷ���˵���
	if(Model.Vtail.Enable)	ms.Total=MenuVtailSet[0].Max;
	else					ms.Total=1;
	
	//�˵����仯�ػ�
	if(ms.Param!=ms.Total)
	{
		LCD_Clr_All();//����
		ms.Param=ms.Total;
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuVtailSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0; 
		//�ر��������
		Model.Delta.Enable = 0;
		Model.FlpAil.Enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
