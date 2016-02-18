#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"
#include <string.h>

char PageMenuModelCur[10];
s16 PageMenuModelReset;

CSTR TitleModelSet[]={"模型设置","Model Setting"};
CSTR ModelSelect[]={"选择:","Model No:"};
CSTR ModelName[]={"名称:","Name:"};
CSTR ModelType[]={"类型:","Type:"};
CSTR ModelIcon[]={"图标:","Symbol:"};
CSTR Copy[]={"模型复制","Model Copy"};
CSTR RfSet[]={"射频设置","Bind & RF"};
CSTR ChMap[]={"通道映射","Channel Map"};
CSTR SwitchSelect[]={"开关选择","Switch Map"};
CSTR Trainer[]={"教练功能","Trainer"};
CSTR ModelReset[]={"重置模型","Model Reset"};

CSTR ModelReseted[]={"模型已重置!","Model Reseted!"};
CSTR AllModelReseted[]={"所有模型已重置!","All Model Reseted!"};

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

//  模型基本配置菜单处理过程
void PageMenuModMarkNo(void)
{
	if(TxSys.Language==0)
	{
		strcpy(PageMenuModelCur,"00号模型");
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
		LCD_Clr_All();//清屏
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
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuMod,&ms);
	
	if(MenuMod[ms.iFocus+1].SubItem)//子菜单或子功能
	{
		if(KeyTstDown(KEY_ENT))
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuMod[ms.iFocus+1].SubItem,PV_INIT);
		}
	}	
	else if(MenuMod[ms.iFocus+1].pValue==&PageMenuModelReset)//复位功能
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
	
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);

	return 0;
}
