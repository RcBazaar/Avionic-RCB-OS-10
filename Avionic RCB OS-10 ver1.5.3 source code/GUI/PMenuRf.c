#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"
#include "Tx.h"
#include <string.h>

s16 PageMenuRfProto;
extern u32 PageEditIdValue;
extern u8  PageEditStillSeed; 

CSTR TitleRfSet[]={"射频设置","Bind & RF"};
CSTR PageMenuRfBindTitle[]={"启动对码","Bind"};
CSTR Protocol[]={"通讯协议:","Protocol:"};
CSTR RfPower[]={"发射功率:","Rf Power:"};
CSTR IDCode[]={"识 别 码:","Id Code:"};
CSTR SaveRfCfg[]={"变更射频设置","Save Rf Cfg"};
CSTR EnterBind[]={"启动对码","Bind"};
CSTR AutoId[]={"(自动)","(Auto)"};
CSTR NotSaveRfCfg[]={"没有保存改动!","Changes Not Saved!"};

CMENUITEM MenuRf[]={
	{TitleRfSet,0,0,0,4,0,0,0},
	{PageMenuRfBindTitle,PageBind,0,0,0,0,0,0},
	{Protocol			,0,PageOptionProtocol	, 0,3,&PageMenuRfProto,0,0,0},
	{RfPower			,0,PageOptionRfPwr		, 0,7,&Model.RfPwr,0,0,4},
	{IDCode				,PageEditId,0,0,0,0,0,TxSys.RfIdStr},
};

//模型基本配置菜单处理过程
void PageMenuRfIdStr(void)
{
	u8 i;
	u32 mask=100000;
	
	if(PageEditIdValue==0)
	{
		strcpy(TxSys.RfIdStr,AutoId[TxSys.Language]);//开机后第一次进入更改ID时先加载一个随机值
		return;
	}
	for(i=0;i<MODELIDL;i++)
	{
		TxSys.RfIdStr[i]=PageEditIdValue/mask%10+'0';	
		mask/=10;
	}
}

u32 PageMenuRf(u8 event)
{
	static u8 CfgChanged;
	static MENUSTAT ms;
			
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuRf[0].Max;
		ms.DrawMask=PD_ALL;	
		PageMenuRfProto=Model.Protocol;
		PageEditIdValue=TxSys.RfId;
		PageEditStillSeed = Model.StillSeed; 
		CfgChanged=1;
		return 1;
	}
	
	//检测配置变化
	if(PageMenuRfProto!=Model.Protocol || PageEditIdValue!=TxSys.RfId)
	{
		if(!CfgChanged) 
		{
			PageMenuRfBindTitle[0] = SaveRfCfg[0];
			PageMenuRfBindTitle[1] = SaveRfCfg[1];
			ms.DrawMask=PD_ALL;	
		}
		CfgChanged=1;		
	}
	else
	{
		if(CfgChanged)
		{
			PageMenuRfBindTitle[0] = EnterBind[0];
			PageMenuRfBindTitle[1] = EnterBind[1];
			ms.DrawMask=PD_ALL;	
		}
		CfgChanged=0;
	}
	
	//将ID字符化
	PageMenuRfIdStr();
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuRf,&ms);
	
	//回车键处理
	if(KeyTstDown(KEY_ENT))
	{			
		//进入对码
		if(MenuRf[ms.iFocus+1].SubItem==PageBind)
		{
			if(CfgChanged)
			{
				TxSys.RfId=PageEditIdValue;
				Model.StillSeed = PageEditStillSeed;  
				Model.Protocol=PageMenuRfProto;
				TxLoad(Model.Protocol);
			}
			else
			{
				BeepMusic(MusicEnter);	
				PageEnter(PageBind,PV_INIT);
			}
		}		
		if(MenuRf[ms.iFocus+1].SubItem==PageEditId)
		{
			BeepMusic(MusicEnter);
			PageEnter(PageEditId,PV_INIT);
		}
	}	
	
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		if(CfgChanged)
		{
			PageAlert(NotSaveRfCfg[TxSys.Language],1000,PV_END);
		}
		else PageReturn(PV_INIT);
	}
	
	if(event==PV_END)
	{
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
