#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleEpaSet[]={"�г�����","Endpoints"};
CSTR Ch1L[]={"ͨ��1 -��:","Ch1 -Low:"};
CSTR Ch2L[]={"ͨ��2 -��:","Ch2 -Low:"};
CSTR Ch3L[]={"ͨ��3 -��:","Ch3 -Low:"};
CSTR Ch4L[]={"ͨ��4 -��:","Ch4 -Low:"};
CSTR Ch5L[]={"ͨ��5 -��:","Ch5 -Low:"};
CSTR Ch6L[]={"ͨ��6 -��:","Ch6 -Low:"};
CSTR Ch7L[]={"ͨ��7 -��:","Ch7 -Low:"};
CSTR Ch8L[]={"ͨ��8 -��:","Ch8 -Low:"};
CSTR Ch9L[]={"ͨ��9 -��:","Ch9 -Low:"};
CSTR Ch10L[]={"ͨ��10-��:","Ch10-Low:"};
CSTR ChxH[]={"      -��:","    -High:"};

CMENUITEM MenuEpaSet[]={
	{TitleEpaSet	,0,0,0,20,0,0,0},
	{Ch1L	,0,0	, 0,+MAX_EPA,&Model.Epa[0][0],0,0,100},
	{ChxH	,0,0	, 0,+MAX_EPA,&Model.Epa[0][1],0,0,100},
	{Ch2L	,0,0	, 0,+MAX_EPA,&Model.Epa[1][0],0,0,100},
	{ChxH	,0,0	, 0,+MAX_EPA,&Model.Epa[1][1],0,0,100},
	{Ch3L	,0,0	, 0,+MAX_EPA,&Model.Epa[2][0],0,0,100},
	{ChxH	,0,0	, 0,+MAX_EPA,&Model.Epa[2][1],0,0,100},
	{Ch4L	,0,0	, 0,+MAX_EPA,&Model.Epa[3][0],0,0,100},
	{ChxH	,0,0	, 0,+MAX_EPA,&Model.Epa[3][1],0,0,100},
	{Ch5L	,0,0	, 0,+MAX_EPA,&Model.Epa[4][0],0,0,100},
	{ChxH	,0,0	, 0,+MAX_EPA,&Model.Epa[4][1],0,0,100},
	{Ch6L	,0,0	, 0,+MAX_EPA,&Model.Epa[5][0],0,0,100},
	{ChxH	,0,0	, 0,+MAX_EPA,&Model.Epa[5][1],0,0,100},
	{Ch7L	,0,0	, 0,+MAX_EPA,&Model.Epa[6][0],0,0,100},
	{ChxH	,0,0	, 0,+MAX_EPA,&Model.Epa[6][1],0,0,100},
	{Ch8L	,0,0	, 0,+MAX_EPA,&Model.Epa[7][0],0,0,100},
	{ChxH	,0,0	, 0,+MAX_EPA,&Model.Epa[7][1],0,0,100},
	{Ch9L	,0,0	, 0,+MAX_EPA,&Model.Epa[8][0],0,0,100},
	{ChxH	,0,0	, 0,+MAX_EPA,&Model.Epa[8][1],0,0,100},
	{Ch10L	,0,0	, 0,+MAX_EPA,&Model.Epa[9][0],0,0,100},
	{ChxH	,0,0	, 0,+MAX_EPA,&Model.Epa[9][1],0,0,100},
};

//ģ�Ͳ������ڲ˵��������
u32 PageEpaSet(u8 event)
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
		ms.Total=MenuEpaSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuEpaSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
