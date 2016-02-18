#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleTrainerSet[]={"教练功能设置","Trainer Set"};
CSTR PpmIn[]={"PPM输入","PPM In"};
CSTR PpmMonitor[]={"PPM监视器","PPM Monitor"};
CSTR TrainerChMap[]={"教练通道映射","Trainer Ch Map"};

CMENUITEM MenuTrainer[]={
	{TitleTrainerSet,0,0,0,3,0,0,0},
	{PpmIn			,0,PageOptionOnOff,0,1,&Model.PpmIn,0,0,0},
	{PpmMonitor	 	,PagePpmIn,0,0,0,0,0,0},
	{TrainerChMap	,PageMenuTrChMap,0,0,0,0,0,0},
};

u32 PageMenuTrainer(u8 event)
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
		ms.Total=MenuTrainer[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuTrainer,&ms);
	
	//回车键处理
	if(KeyTstDown(KEY_ENT))
	{
		//如果有子菜单或子功能，进入
		if(MenuTrainer[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuTrainer[ms.iFocus+1].SubItem,PV_INIT);
		}
	}
	
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);

	return 0;
}







