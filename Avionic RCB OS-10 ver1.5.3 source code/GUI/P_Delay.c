#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

char PageDelayTime[10][5];

CSTR TitleServoDelay[]={"舵机减速","Servo Delay"};

CMENUITEM MenuDelaySet[]={
	{TitleServoDelay	,0,0,0,10,0,0,0},
	{Ch1,0,0	, 0,DELAY_MAX,&Model.Delay[0],0,&PageDelayTime[0][0],0},
	{Ch2,0,0	, 0,DELAY_MAX,&Model.Delay[1],0,&PageDelayTime[1][0],0},
	{Ch3,0,0	, 0,DELAY_MAX,&Model.Delay[2],0,&PageDelayTime[2][0],0},
	{Ch4,0,0	, 0,DELAY_MAX,&Model.Delay[3],0,&PageDelayTime[3][0],0},
	{Ch5,0,0	, 0,DELAY_MAX,&Model.Delay[4],0,&PageDelayTime[4][0],0},
	{Ch6,0,0	, 0,DELAY_MAX,&Model.Delay[5],0,&PageDelayTime[5][0],0},
	{Ch7,0,0	, 0,DELAY_MAX,&Model.Delay[6],0,&PageDelayTime[6][0],0},
	{Ch8,0,0	, 0,DELAY_MAX,&Model.Delay[7],0,&PageDelayTime[7][0],0},
	{Ch9,0,0	, 0,DELAY_MAX,&Model.Delay[8],0,&PageDelayTime[8][0],0},
	{Ch10,0,0	, 0,DELAY_MAX,&Model.Delay[9],0,&PageDelayTime[9][0],0},
};

//  舵机减速菜单处理过程
u32 PageDelaySet(u8 event)
{
	u8 i;
	static MENUSTAT ms;
			
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuDelaySet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	for(i=0;i<10;i++)
	{
		PageDelayTime[i][0] = '0'+Model.Delay[i]/10;
		PageDelayTime[i][1] = '.';
		PageDelayTime[i][2] = '0'+Model.Delay[i]%10;
		PageDelayTime[i][3] = 's';
		PageDelayTime[i][4] = 0;
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuDelaySet,&ms);
		
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
