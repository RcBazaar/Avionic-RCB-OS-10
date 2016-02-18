#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CMENUITEM *MenuChMap;

CSTR TitleAeroChMap[]={"�̶���ͨ��ӳ��","Aero Ch Map"};
CSTR TitleHeliChMap[]={"ֱ����ͨ��ӳ��","Heli Ch Map"};

CMENUITEM MenuChMapA[]={
	{TitleAeroChMap	,0,0,0,10,0,0,0},
	{Ch1,0,PageOptionChSelA,0,10,&Model.ChMap[0],0,0,0},
	{Ch2,0,PageOptionChSelA,0,10,&Model.ChMap[1],0,0,1},
	{Ch3,0,PageOptionChSelA,0,10,&Model.ChMap[2],0,0,2},
	{Ch4,0,PageOptionChSelA,0,10,&Model.ChMap[3],0,0,3},
	{Ch5,0,PageOptionChSelA,0,10,&Model.ChMap[4],0,0,4},
	{Ch6,0,PageOptionChSelA,0,10,&Model.ChMap[5],0,0,5},
	{Ch7,0,PageOptionChSelA,0,10,&Model.ChMap[6],0,0,6},
	{Ch8,0,PageOptionChSelA,0,10,&Model.ChMap[7],0,0,7},
	{Ch9,0,PageOptionChSelA,0,10,&Model.ChMap[8],0,0,8},
	{Ch10,0,PageOptionChSelA,0,10,&Model.ChMap[9],0,0,9},
};

CMENUITEM MenuChMapH[]={
	{TitleHeliChMap	,0,0,0,10,0,0,0},
	{Ch1,0,PageOptionChSelH,0,10,&Model.ChMap[0],0,0,0},
	{Ch2,0,PageOptionChSelH,0,10,&Model.ChMap[1],0,0,1},
	{Ch3,0,PageOptionChSelH,0,10,&Model.ChMap[2],0,0,2},
	{Ch4,0,PageOptionChSelH,0,10,&Model.ChMap[3],0,0,3},
	{Ch5,0,PageOptionChSelH,0,10,&Model.ChMap[4],0,0,4},
	{Ch6,0,PageOptionChSelH,0,10,&Model.ChMap[5],0,0,5},
	{Ch7,0,PageOptionChSelH,0,10,&Model.ChMap[6],0,0,6},
	{Ch8,0,PageOptionChSelH,0,10,&Model.ChMap[7],0,0,7},
	{Ch9,0,PageOptionChSelH,0,10,&Model.ChMap[8],0,0,8},
	{Ch10,0,PageOptionChSelH,0,10,&Model.ChMap[9],0,0,9},
};

//  ģ��ͨ��ӳ�䴦�����
u32 PageMenuChMap(u8 event)
{
	static MENUSTAT ms;
			
	if(event==PV_INIT)
	{
		if(Model.Type==MT_AERO)	MenuChMap=MenuChMapA;
		else					MenuChMap=MenuChMapH;
		
		LCD_Clr_All();//����
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuChMap[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuChMap,&ms);
		
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{
		//������Ӳ˵����ӹ��ܣ�����
		if(MenuChMap[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuChMap[ms.iFocus+1].SubItem,PV_INIT);
		}		
	}
	
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
