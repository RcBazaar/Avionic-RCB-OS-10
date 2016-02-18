#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleTrainerSet[]={"������������","Trainer Set"};
CSTR PpmIn[]={"PPM����","PPM In"};
CSTR PpmMonitor[]={"PPM������","PPM Monitor"};
CSTR TrainerChMap[]={"����ͨ��ӳ��","Trainer Ch Map"};

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
		LCD_Clr_All();//����
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
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuTrainer,&ms);
	
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{
		//������Ӳ˵����ӹ��ܣ�����
		if(MenuTrainer[ms.iFocus+1].SubItem)
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuTrainer[ms.iFocus+1].SubItem,PV_INIT);
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







