#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"
#include "string.h"

char MenuMixerDeltaStat[6];
char MenuMixerVtailStat[6];
char MenuMixerFlpAilStat[6];
char MenuMixerAilRudStat[6];

CSTR TitleMixerSet[]={"�������","Mixer Set"};
CSTR DeltaSet[]={"��������","Delta"};
CSTR VTailSet[]={" V β ���","V-Tail"};
CSTR FlpAilSet[]={"������","Flaperon"};
CSTR ThrSet[]={"����>ƽ����","Thr Mixer"};
CSTR YawSet[]={"������>�������","Ail/Rud=>Ele"};
CSTR AilDiffSet[]={"����","Ail Diff"};
CSTR FlapDiffSet[]={"����","Flap Diff"};
CSTR AilRudSet[]={"��������","Rud&Ail"};
CSTR ButterflySet[]={"�������","Butterfly"};
CSTR FreeSet[]={"�Զ�����","Free Mixer"};

CMENUITEM MenuMixerSet[]={
	{TitleMixerSet	,0,0,0,6,0,0,0},
	{DeltaSet	,PageDeltaSet,0,0,0,0,0,MenuMixerDeltaStat},//Ail&Ele
	{VTailSet	,PageVtailSet,0,0,0,0,0,MenuMixerVtailStat},//Rud&Ele
	{FlpAilSet	,PageFlpAilSet,0,0,0,0,0,MenuMixerFlpAilStat},//Flap&Ail
	{ThrSet  	,PageThrMixSet,0,0,0,0,0,0},
	{YawSet  	,PageYawMixSet,0,0,0,0,0,0},
	{AilRudSet  ,PageAilRudSet,0,0,0,0,0,MenuMixerAilRudStat},//Ail&Rud
	{FreeSet  	,0,0,0,0,0,0,0},
};

//  ��ز˵��������
u32 PageMixerSet(u8 event)
{
	static MENUSTAT ms;
	s16 language_offset;
			
	if(event==PV_INIT)
	{
		LCD_Clr_All();//����
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuMixerSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	 
	}
	
	if(ms.DrawMask==PD_ALL)
	{
		//��ȡ�����״̬
		language_offset = 2*TxSys.Language;
		strcpy(MenuMixerVtailStat,PageOptionOnOff[Model.Vtail.Enable+language_offset]);
		strcpy(MenuMixerDeltaStat,PageOptionOnOff[Model.Delta.Enable+language_offset]);
		strcpy(MenuMixerFlpAilStat,PageOptionOnOff[Model.FlpAil.Enable+language_offset]);
		strcpy(MenuMixerAilRudStat,PageOptionOnOff[Model.AilRud.Enable+language_offset]);
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuMixerSet,&ms);
		
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{
		//������Ӳ˵����ӹ��ܣ�����
		if(MenuMixerSet[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuMixerSet[ms.iFocus+1].SubItem,PV_INIT);
		}				
	}	
	
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
