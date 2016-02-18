#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleGyroSet[]={"陀螺仪设置","Gyro Setting"};
CSTR Normal[]={"常规:","Normal:"};
CSTR IdleUp1[]={"特技1:","Idle-Up 1:"};
CSTR IdleUp2[]={"特技2:","Idle-Up 2:"};
CSTR Hold[]={"油门锁定:","Throttle hold:"};

CMENUITEM MenuGyroSet[]={
	{TitleGyroSet	,0,0,0,4,0,0,0},
	{Normal	,0,0	, -TRV_GYRO,+TRV_GYRO,&Model.Gyro[0],0,0,50},
	{IdleUp1,0,0	, -TRV_GYRO,+TRV_GYRO,&Model.Gyro[1],0,0,50},
	{IdleUp2,0,0	, -TRV_GYRO,+TRV_GYRO,&Model.Gyro[2],0,0,50},
	{Hold	,0,0	, -TRV_GYRO,+TRV_GYRO,&Model.Gyro[3],0,0,50},
};

//  陀螺仪菜单处理
u32 PageGyroSet(u8 event)
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
		ms.Total=MenuGyroSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuGyroSet,&ms);
		
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
