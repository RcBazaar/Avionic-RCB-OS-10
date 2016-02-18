#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleEpaSet[]={"行程设置","Endpoints"};
CSTR Ch1L[]={"通道1 -低:","Ch1 -Low:"};
CSTR Ch2L[]={"通道2 -低:","Ch2 -Low:"};
CSTR Ch3L[]={"通道3 -低:","Ch3 -Low:"};
CSTR Ch4L[]={"通道4 -低:","Ch4 -Low:"};
CSTR Ch5L[]={"通道5 -低:","Ch5 -Low:"};
CSTR Ch6L[]={"通道6 -低:","Ch6 -Low:"};
CSTR Ch7L[]={"通道7 -低:","Ch7 -Low:"};
CSTR Ch8L[]={"通道8 -低:","Ch8 -Low:"};
CSTR Ch9L[]={"通道9 -低:","Ch9 -Low:"};
CSTR Ch10L[]={"通道10-低:","Ch10-Low:"};
CSTR ChxH[]={"      -高:","    -High:"};

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

//模型参数调节菜单处理过程
u32 PageEpaSet(u8 event)
{
	static MENUSTAT ms;
			
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
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
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuEpaSet,&ms);
		
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
