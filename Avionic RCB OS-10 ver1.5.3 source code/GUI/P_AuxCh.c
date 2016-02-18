#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleAuxSet[]={"����ͨ������","Auxillary"};
CSTR GearL[]={"�����-��:","Gear-Low:"};
CSTR FlapL[]={"�� �� -��:","Flap-Low:"};
CSTR Aux1L[]={"����1 -��:","AUX1-Low:"};
CSTR Aux2L[]={"����2 -��:","AUX2-Low:"};
CSTR XxxxM[]={"      -��:"," -Middle:"};
CSTR XxxxH[]={"      -��:","   -High:"};

CMENUITEM MenuAeroAuxChSet[]={
	{TitleAuxSet,0,0,0,12,0,0,0},
	{GearL,0,0,-TRV_AUX,TRV_AUX,&Model.Gear[0],0,0,-100},
	{XxxxM,0,0,-TRV_AUX,TRV_AUX,&Model.Gear[1],0,0,0},
	{XxxxH,0,0,-TRV_AUX,TRV_AUX,&Model.Gear[2],0,0, 100},
	{FlapL,0,0,-TRV_AUX,TRV_AUX,&Model.Flap[0],0,0,-100},
	{XxxxM,0,0,-TRV_AUX,TRV_AUX,&Model.Flap[1],0,0,0},
	{XxxxH,0,0,-TRV_AUX,TRV_AUX,&Model.Flap[2],0,0, 100},
	{Aux1L,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[0][0],0,0,-100},
	{XxxxM,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[0][1],0,0,0},
	{XxxxH,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[0][2],0,0, 100},
	{Aux2L,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[1][0],0,0,-100},
	{XxxxM,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[1][1],0,0,0},
	{XxxxH,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[1][2],0,0, 100},
};

CMENUITEM MenuHeliAuxChSet[]={
	{TitleAuxSet,0,0,0,6,0,0,0},
	{Aux1L,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[0][0],0,0,-100},
	{XxxxM,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[0][1],0,0,0},
	{XxxxH,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[0][2],0,0, 100},
	{Aux2L,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[1][0],0,0,-100},
	{XxxxM,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[1][1],0,0,0},
	{XxxxH,0,0,-TRV_AUX,TRV_AUX,&Model.Aux[1][2],0,0, 100},
};

//  ����ͨ�������ڲ˵��������
CMENUITEM *MenuAux;
u32 PageAuxChSet(u8 event)
{
	static MENUSTAT ms;
			
	if(event==PV_INIT)
	{
		LCD_Clr_All();//����
		if(ms.Param!=Model.Type)//ģ�����ͱ仯Ҫ���³�ʼ��
		{
			ms.Init=0;
			ms.Param=Model.Type;
		}
		
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		MenuAux=(Model.Type==MT_AERO)?MenuAeroAuxChSet:MenuHeliAuxChSet;
		ms.Total=MenuAux[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuAux,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
