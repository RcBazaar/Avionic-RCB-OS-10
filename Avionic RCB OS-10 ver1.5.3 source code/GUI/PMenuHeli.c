#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleHeliParamSet[]={"ֱ������������","Heli Setting"};
CSTR HeliNorRevSet[]={"��������","Ch Reverse"};
CSTR HeliEpaSet[]={"�г�����","Endpoints"};
CSTR HeliNeuSet[]={"�е����","Sub Trim"};
CSTR HeliGyroSet[]={"������","Gyro Setting"};
CSTR HeliStickCurveSet[]={"ҡ������","Stick Curve"};
CSTR HeliThrCurveSet[]={"��������","Throttle Curve"};
CSTR HeliPitchCurveSet[]={"�ݾ�����","Pitch Curve"};
CSTR HeliThrHold[]={"��������:","Throttle Hold:"};
CSTR HeliViryualSw[]={"���⿪��","Virtual Switch"};
CSTR HeliAuxSet[]={"����ͨ��","Auxillary"};
CSTR HeliSwashMixer[]={"б�̻��","Swash"};
CSTR HeliServoDelay[]={"�������","Servo Delay"};

CMENUITEM MenuHeliSet[]={
	{TitleHeliParamSet	,0,0,0,12,0,0,0},
	{HeliNorRevSet		,PageNorRevSet		, 0,0,0,0,0,0},
	{HeliEpaSet			,PageEpaSet			, 0,0,0,0,0,0},
	{HeliNeuSet			,PageNeuSet			, 0,0,0,0,0,0},
	{HeliGyroSet		,PageGyroSet		, 0,0,0,0,0,0},
	{HeliStickCurveSet	,PageCurveSet		, 0,1,9,0,0,0},
	{HeliThrCurveSet	,PageCurveSet		, 0,2,3,0,0,0},
	{HeliPitchCurveSet	,PageCurveSet		, 0,3,4,0,0,0},
	{HeliThrHold		,0,0,-100,100,&Model.ThrHold,0,0,-10},
	{HeliViryualSw 		,PageVirtualSw		, 0,0,0,0,0,0},
	{HeliAuxSet 		,PageAuxChSet		, 0,0,0,0,0,0},
	{HeliSwashMixer		,PageSwashSet		, 0,0,0,0,0,0},
	{HeliServoDelay		,PageDelaySet		, 0,0,0,0,0,0},
};

//  ģ�Ͳ������ڲ˵��������
u32 PageHeliSet(u8 event)
{
	static MENUSTAT ms;
	s16 language_offset;
			
	if(event==PV_INIT)
	{
		LCD_Clr_All();//����
		if(!ms.Init)
		{
			ms.iFocus=ms.iStart=0;
			ms.Init=1;
		}
		ms.Total=MenuHeliSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuHeliSet,&ms);
		
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{
		//������Ӳ˵����ӹ��ܣ�����
		if(MenuHeliSet[ms.iFocus+1].SubItem)
		{ 
			if(MenuHeliSet[ms.iFocus+1].SubItem==PageCurveSet)
			{
				PageCurveCnt = MenuHeliSet[ms.iFocus+1].Max;//��������
				language_offset = PageCurveCnt*TxSys.Language;
				
				if(MenuHeliSet[ms.iFocus+1].Min==1)//ֱ����ҡ������
				{
					PageCurveTitle = PageStkCurveTitle+language_offset;//�������������б�
					PageCurveValue = &Model.StkCurve[0][0][0];
					CurveMaster = HELI_STICK;
				}
				else if(MenuHeliSet[ms.iFocus+1].Min==2)//ֱ������������
				{
					PageCurveTitle = PageHeliThrCurveTitle+language_offset;
					PageCurveValue = &Model.ThrCurve[0][0];
					CurveMaster = HELI_THR;
				}
				else//ֱ�����ݾ�����
				{
					PageCurveTitle = PagePitCurveTitle+language_offset;
					PageCurveValue = &Model.PitCurve[0][0];
					CurveMaster = HELI_PIT;
				}
			}
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuHeliSet[ms.iFocus+1].SubItem,PV_INIT);
		}				
	}

	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
