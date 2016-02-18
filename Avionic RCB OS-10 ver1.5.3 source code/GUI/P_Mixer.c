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

CSTR TitleMixerSet[]={"混控设置","Mixer Set"};
CSTR DeltaSet[]={"三角翼混控","Delta"};
CSTR VTailSet[]={" V 尾 混控","V-Tail"};
CSTR FlpAilSet[]={"襟副翼混控","Flaperon"};
CSTR ThrSet[]={"油门>平衡混控","Thr Mixer"};
CSTR YawSet[]={"副翼方向>升降混控","Ail/Rud=>Ele"};
CSTR AilDiffSet[]={"副翼差动","Ail Diff"};
CSTR FlapDiffSet[]={"襟翼差动","Flap Diff"};
CSTR AilRudSet[]={"副翼方向混控","Rud&Ail"};
CSTR ButterflySet[]={"蝴蝶混控","Butterfly"};
CSTR FreeSet[]={"自定义混控","Free Mixer"};

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

//  混控菜单处理过程
u32 PageMixerSet(u8 event)
{
	static MENUSTAT ms;
	s16 language_offset;
			
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
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
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	 
	}
	
	if(ms.DrawMask==PD_ALL)
	{
		//读取各混控状态
		language_offset = 2*TxSys.Language;
		strcpy(MenuMixerVtailStat,PageOptionOnOff[Model.Vtail.Enable+language_offset]);
		strcpy(MenuMixerDeltaStat,PageOptionOnOff[Model.Delta.Enable+language_offset]);
		strcpy(MenuMixerFlpAilStat,PageOptionOnOff[Model.FlpAil.Enable+language_offset]);
		strcpy(MenuMixerAilRudStat,PageOptionOnOff[Model.AilRud.Enable+language_offset]);
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuMixerSet,&ms);
		
	//回车键处理
	if(KeyTstDown(KEY_ENT))
	{
		//如果有子菜单或子功能，进入
		if(MenuMixerSet[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuMixerSet[ms.iFocus+1].SubItem,PV_INIT);
		}				
	}	
	
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
