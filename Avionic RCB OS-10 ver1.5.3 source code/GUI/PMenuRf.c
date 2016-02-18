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

CSTR TitleRfSet[]={"��Ƶ����","Bind & RF"};
CSTR PageMenuRfBindTitle[]={"��������","Bind"};
CSTR Protocol[]={"ͨѶЭ��:","Protocol:"};
CSTR RfPower[]={"���书��:","Rf Power:"};
CSTR IDCode[]={"ʶ �� ��:","Id Code:"};
CSTR SaveRfCfg[]={"�����Ƶ����","Save Rf Cfg"};
CSTR EnterBind[]={"��������","Bind"};
CSTR AutoId[]={"(�Զ�)","(Auto)"};
CSTR NotSaveRfCfg[]={"û�б���Ķ�!","Changes Not Saved!"};

CMENUITEM MenuRf[]={
	{TitleRfSet,0,0,0,4,0,0,0},
	{PageMenuRfBindTitle,PageBind,0,0,0,0,0,0},
	{Protocol			,0,PageOptionProtocol	, 0,3,&PageMenuRfProto,0,0,0},
	{RfPower			,0,PageOptionRfPwr		, 0,7,&Model.RfPwr,0,0,4},
	{IDCode				,PageEditId,0,0,0,0,0,TxSys.RfIdStr},
};

//ģ�ͻ������ò˵��������
void PageMenuRfIdStr(void)
{
	u8 i;
	u32 mask=100000;
	
	if(PageEditIdValue==0)
	{
		strcpy(TxSys.RfIdStr,AutoId[TxSys.Language]);//�������һ�ν������IDʱ�ȼ���һ�����ֵ
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
		LCD_Clr_All();//����
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
	
	//������ñ仯
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
	
	//��ID�ַ���
	PageMenuRfIdStr();
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuRf,&ms);
	
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{			
		//�������
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
	
	//���ؼ�����
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
