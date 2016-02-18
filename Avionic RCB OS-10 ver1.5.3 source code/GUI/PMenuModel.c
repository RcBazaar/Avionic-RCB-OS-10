#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"
#include <string.h>

char PageMenuModelCur[10];
s16 PageMenuModelReset;

CSTR TitleModelSet[]={"ģ������","Model Setting"};
CSTR ModelSelect[]={"ѡ��:","Model No:"};
CSTR ModelName[]={"����:","Name:"};
CSTR ModelType[]={"����:","Type:"};
CSTR ModelIcon[]={"ͼ��:","Symbol:"};
CSTR Copy[]={"ģ�͸���","Model Copy"};
CSTR RfSet[]={"��Ƶ����","Bind & RF"};
CSTR ChMap[]={"ͨ��ӳ��","Channel Map"};
CSTR SwitchSelect[]={"����ѡ��","Switch Map"};
CSTR Trainer[]={"��������","Trainer"};
CSTR ModelReset[]={"����ģ��","Model Reset"};

CSTR ModelReseted[]={"ģ��������!","Model Reseted!"};
CSTR AllModelReseted[]={"����ģ��������!","All Model Reseted!"};

CMENUITEM MenuMod[]={
	{TitleModelSet	,0,0,0,10,0,0,0},
	{ModelSelect	,PageModelSel			, 0,0,0,0,0,PageMenuModelCur},
	{ModelName		,PageEditName			, 0,0,0,0,0,Model.Name},
	{ModelType		,0,PageOptionModType	, 0,1,&Model.Type,0,0,0},
	{ModelIcon		,PageIconSel			, 0,0,0,0,0,Model.Icon},
	{Copy			,PageModelCopy,0,0,0,0,0,0},
	{RfSet			,PageMenuRf	  ,0,0,0,0,0,0},
	{ChMap			,PageMenuChMap,0,0,0,0,0,0},
	{SwitchSelect	,PageSwDef	  ,0,0,0,0,0,0},
	{Trainer		,PageMenuTrainer,0,0,0,0,0,0},
	{ModelReset 	,0,PageOptionOkExe1,0,2,&PageMenuModelReset,0,0,0},
};

//  ģ�ͻ������ò˵��������
void PageMenuModMarkNo(void)
{
	if(TxSys.Language==0)
	{
		strcpy(PageMenuModelCur,"00��ģ��");
		PageMenuModelCur[0]=TxSys.ModelNo/10+'0';
		PageMenuModelCur[1]=TxSys.ModelNo%10+'0';
	}
	if(TxSys.Language==1)
	{
		strcpy(PageMenuModelCur,"00");
		PageMenuModelCur[0]=TxSys.ModelNo/10+'0';
		PageMenuModelCur[1]=TxSys.ModelNo%10+'0';
	}
}

u32 PageMenuMod(u8 event)
{
	static MENUSTAT ms;
	
	PageMenuModMarkNo();
		
	if(event==PV_INIT)
	{
		LCD_Clr_All();//����
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuMod[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuMod,&ms);
	
	if(MenuMod[ms.iFocus+1].SubItem)//�Ӳ˵����ӹ���
	{
		if(KeyTstDown(KEY_ENT))
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuMod[ms.iFocus+1].SubItem,PV_INIT);
		}
	}	
	else if(MenuMod[ms.iFocus+1].pValue==&PageMenuModelReset)//��λ����
	{
		if(KeyTstHold(KEY_ENT))
		{
			if(PageMenuModelReset==1)
			{
				PageMenuModelReset=0;
				Model = ModelDef;
				Model.Name[5]=TxSys.ModelNo/10+'0';
				Model.Name[6]=TxSys.ModelNo%10+'0';
				Model.Name[7]=0;
				PageAlert(ModelReseted[TxSys.Language],1000,PV_INIT);
			}
			if(PageMenuModelReset==2)
			{
				PageMenuModelReset=0;
				ModelFormat();
				PageAlert(AllModelReseted[TxSys.Language],1000,PV_INIT);
			}
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
