#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"
#include <string.h>

CSTR TitleAeroParamSet[]={"�̶����������","Acro Setting"};
CSTR NorRevSet[]={"��������","Ch Reverse"};
CSTR EpaSet[]={"�г�����","Endpoints"};
CSTR NeuSet[]={"����΢��","Sub Trim"};
CSTR StickCurveSet[]={"ҡ������","Stick Curve"};
CSTR ThrCurveSet[]={"��������","Throttle Curve"};
CSTR ThrHold[]={"��������:","Throttle Hold:"};
CSTR ViryualSw[]={"���⿪��","Virtual Switch"};
CSTR AuxSet[]={"����ͨ��","Auxillary"};
CSTR MixerSet[]={"�������","Mixer Set"};
CSTR ServoDelay[]={"�������","Servo Delay"};

CMENUITEM MenuAeroSet[]={
	{TitleAeroParamSet,0				,0,0,10,0,0,0},
	{NorRevSet		,PageNorRevSet		,0,0,0,0,0,0},
	{EpaSet			,PageEpaSet			,0,0,0,0,0,0},
	{NeuSet			,PageNeuSet			,0,0,0,0,0,0},
	{StickCurveSet	,PageCurveSet		,0,1,9,0,0,0},
	{ThrCurveSet	,PageCurveSet		,0,2,1,0,0,0},
	{ThrHold		,0,0,-100,100,&Model.ThrHold,0,0,-20},
	{ViryualSw 		,PageVirtualSw		,0,0,0,0,0,0},
	{AuxSet 		,PageAuxChSet		,0,0,0,0,0,0},
	{MixerSet 		,PageMixerSet		,0,0,0,0,0,0},
	{ServoDelay		,PageDelaySet		,0,0,0,0,0,0},
};

//  ģ�Ͳ������ڲ˵��������
u32 PageAeroSet(u8 event)
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
		ms.Total=MenuAeroSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//����
		ms.DrawMask=PD_ALL;	
	}
	
	//�˵�ͨ�ô������
	PageMenuProc(MenuAeroSet,&ms);
		
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{
		//������Ӳ˵����ӹ��ܣ�����
		if(MenuAeroSet[ms.iFocus+1].SubItem)
		{
			if(MenuAeroSet[ms.iFocus+1].SubItem==PageCurveSet)
			{
				PageCurveCnt = MenuAeroSet[ms.iFocus+1].Max;//�����������̶���ִ���С�棬����3*3��
				language_offset = PageCurveCnt*TxSys.Language;
				if(MenuAeroSet[ms.iFocus+1].Min==1)//�̶���ҡ������
				{
					PageCurveTitle = PageStkCurveTitle+language_offset;//�������������б�
					PageCurveValue = &Model.StkCurve[0][0][0];
					CurveMaster = ACRO_STICK;
				}
				else//�̶�����������
				{
					PageCurveTitle = PageAcroThrCurveTitle+language_offset;
					PageCurveValue = &Model.ThrCurve[0][0];
					CurveMaster = ACRO_THR;
				}
			}
			BeepMusic(MusicEnter);			
			PageEnter((PAGEPROC)MenuAeroSet[ms.iFocus+1].SubItem,PV_INIT);
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
