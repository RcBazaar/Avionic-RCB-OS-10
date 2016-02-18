#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

CSTR TitleGyroSet[]={"����������","Gyro Setting"};
CSTR Normal[]={"����:","Normal:"};
CSTR IdleUp1[]={"�ؼ�1:","Idle-Up 1:"};
CSTR IdleUp2[]={"�ؼ�2:","Idle-Up 2:"};
CSTR Hold[]={"��������:","Throttle hold:"};

CMENUITEM MenuGyroSet[]={
	{TitleGyroSet	,0,0,0,4,0,0,0},
	{Normal	,0,0	, -TRV_GYRO,+TRV_GYRO,&Model.Gyro[0],0,0,50},
	{IdleUp1,0,0	, -TRV_GYRO,+TRV_GYRO,&Model.Gyro[1],0,0,50},
	{IdleUp2,0,0	, -TRV_GYRO,+TRV_GYRO,&Model.Gyro[2],0,0,50},
	{Hold	,0,0	, -TRV_GYRO,+TRV_GYRO,&Model.Gyro[3],0,0,50},
};

//  �����ǲ˵�����
u32 PageGyroSet(u8 event)
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
		ms.Total=MenuGyroSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuGyroSet,&ms);
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
