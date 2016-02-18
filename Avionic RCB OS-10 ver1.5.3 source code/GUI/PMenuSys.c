#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

s16 PageMenuSysReset;
char PageMenuSysVol[6];

CSTR TitleControllerSet[]={"ң��������","Transmitter Setting"};
CSTR TrimStepAdj[]={"΢������:","Trim Step:"};
CSTR StickMode[]={"ҡ��ģʽ:","Stick Mode:"};
CSTR StickDeadZone[]={"ҡ������:","Stick Centering:"};
CSTR Vibration[]={"����ʾ:","Vibration:"};
CSTR Sound[]={"������ʾ:","Sound:"};
CSTR KeyVolume[]={"��������:","Key Volume:"};
CSTR KeyTone[]={"��������:","Key Tone:"};
CSTR Backlight[]={"��������:","Backlight:"};
CSTR Contrast[]={"�� �� ��:","Contrast:"};
CSTR BacklightOff[]={"����ر�:","Backlight ON:"};
CSTR BatType[]={"�������:","Battery:"};
CSTR AlarmVoltage[]={"������ѹ:","Alarm Volt:"};
CSTR Language[]={"����ѡ��:","Language:"};
CSTR StickCalibration[]={"ҡ��У׼","Stick Calibration"};
CSTR Reset[]={"�ָ�Ĭ��ֵ","Reset"};

CSTR ControllerReseted[]={"ң�����ѻָ�Ĭ��ֵ!","Controller Reseted!"};

CMENUITEM MenuSys[]={
	{TitleControllerSet	,0,0,0,12,0,0,0},
	{TrimStepAdj		,0,0					, 1,10,&TxSys.TrimStep,0,0,2},
	{StickMode			,0,PageOptionStkType	, 0, 3,&TxSys.StkType,0,0},
	{StickDeadZone		,0,0					, 0,50,&TxSys.StkDa,0,0,0},
	{Vibration			,0,PageOptionOnOff		, 0, 1,&TxSys.Vibrator,0,0,1},
	{Sound				,0,PageOptionOnOff		, 0, 1,&TxSys.Music,0,0,1},
	{KeyVolume			,0,0					, 0,10,&TxSys.KeyBeep,0,0,8},
	{KeyTone			,0,0					, 0,30,&TxSys.KeyTone,0,0,20},
	{Backlight			,0,0					, 0,10,&TxSys.Light,0,0,8},
	{Contrast			,0,0					, 0,10,&TxSys.Contrast,0,0,4},
	{BacklightOff		,0,PageOptionLightOff	, 0, 6,&TxSys.LightOff,0,0,3},
	//{BatType			,0,PageOptionTxBatType	, 0, 2,&TxSys.BatType,0,0,0},
	{AlarmVoltage		,0,0					,65,126,&TxSys.BatWarn,0,PageMenuSysVol,74},
	//{Language			,0,PageOptionLanguage	, 0, 1,&TxSys.Language,0,0,1},
	{StickCalibration	,PageStkCal,0,0,0,0,0,0},
	//{Reset			,0,PageOptionOkExe2		, 0, 1,&PageMenuSysReset,0,0,0},
};

//  ��������ƺͰ�������
u32 PageMenuSys(u8 event)
{
	static MENUSTAT ms;
	static s16 pre_stktype = 0;
	static s16 pre_language = 0;
		
	//��ʼ���������
	if(event==PV_INIT)
	{
		LCD_Clr_All();//����
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total = MenuSys[0].Max;
		ms.DrawMask=PD_ALL;	
		
		pre_stktype = TxSys.StkType;
		pre_language = TxSys.Language;
		
		return 1;
	}
	
	//����ѹתΪ�ַ���
	PageMenuSysVol[0]=TxSys.BatWarn>=100?('0'+TxSys.BatWarn/100):(' ');
	PageMenuSysVol[1]=TxSys.BatWarn>=10?('0'+TxSys.BatWarn/10%10):(' ');
	PageMenuSysVol[2]='.';	
	PageMenuSysVol[3]=TxSys.BatWarn%10+'0';	
	PageMenuSysVol[4]='V';
	PageMenuSysVol[5]=0;
	
	//���������ֿ��ص���������
	if(SwTstZYSState()==ZYS_LEFT)//����ģʽ1-3
	{
		if(TxSys.StkType<pre_stktype)//��
			TxSys.StkType = 1;
		if(TxSys.StkType>pre_stktype)//��
		{
			if(TxSys.StkType<=1)
				TxSys.StkType = 1;
			else
				TxSys.StkType = 3;
		}
	}
	if(SwTstZYSState()==ZYS_RIGHT)//����ģʽ0-2
	{
		if(TxSys.StkType<pre_stktype)//��
		{
			if(TxSys.StkType>=2)
				TxSys.StkType = 2;
			else
				TxSys.StkType = 0;
		}
		if(TxSys.StkType>pre_stktype)//��
			TxSys.StkType = 2;
	}
	pre_stktype = TxSys.StkType;	
	
	//�����л�ʱ���»���ȫ��Ԫ��
	if(TxSys.Language!=pre_language)
	{
		ms.DrawMask=PD_ALL;	
		pre_language = TxSys.Language;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuSys,&ms);
	
	if(MenuSys[ms.iFocus+1].SubItem)//�Ӳ˵����ӹ���
	{
		if(KeyTstDown(KEY_ENT))
		{
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuSys[ms.iFocus+1].SubItem,PV_INIT);
		}
	}
	else if(MenuSys[ms.iFocus+1].pValue==&PageMenuSysReset)
	{
		if(KeyTstHold(KEY_ENT))//��λ����
		{
			if(PageMenuSysReset==1)
			{
				PageMenuSysReset=0;
				TxSys = TxSysDef;
				PageAlert(ControllerReseted[TxSys.Language],1000,PV_INIT);
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
