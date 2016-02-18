#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleSwitchDef[]={"����ѡ��","Switch Map"};
CSTR ThrHoldSw[]={"��������:","Thr Hold Sw:"};
CSTR IdleModeSw[]={"�ؼ�ģʽ:","Idle Mode Sw:"};
CSTR DRModeSw[]={"��С��:","D/R Mode Sw:"};
CSTR GearSw[]={"�����:","Gear Sw:"};
CSTR FlapSw[]={"��  ��:","Flap Sw:"};
CSTR Aux1Sw[]={"����ͨ��1:","Aux1 Sw:"};
CSTR Aux2Sw[]={"����ͨ��2:","Aux2 Sw:"};
CSTR TrainerSw[]={"��������:","Trainer Sw:"};

CMENUITEM MenuSwDef[]={
	{TitleSwitchDef,0,0,0,8,0,0,0},
	{ThrHoldSw	,0,PageOptionSwDef	, 0,6,&Model.SwDef[SWD_HOLD],0,0,2},
	{IdleModeSw	,0,PageOptionSwDef	, 0,6,&Model.SwDef[SWD_IDLE],0,0,0},
	{DRModeSw	,0,PageOptionSwDef	, 0,6,&Model.SwDef[SWD_DR]	,0,0,0},
	{GearSw		,0,PageOptionSwDef	, 0,6,&Model.SwDef[SWD_GEAR],0,0,1},
	{FlapSw		,0,PageOptionSwDef	, 0,6,&Model.SwDef[SWD_FLAP],0,0,4},
	{Aux1Sw	 	,0,PageOptionSwDef	, 0,6,&Model.SwDef[SWD_AUX1],0,0,0},
	{Aux2Sw		,0,PageOptionSwDef	, 0,6,&Model.SwDef[SWD_AUX2],0,0,0},
	{TrainerSw	,0,PageOptionSwDef	, 0,6,&Model.SwDef[SWD_TRAINER],0,0,0},
};


//  ģ�Ϳ���ѡ�������
u32 PageSwDef(u8 event)
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
		ms.Total=MenuSwDef[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuSwDef,&ms);
		
	//�س������� ���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
