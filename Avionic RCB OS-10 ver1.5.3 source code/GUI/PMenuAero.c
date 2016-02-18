#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"
#include <string.h>

CSTR TitleAeroParamSet[]={"固定翼参数调节","Acro Setting"};
CSTR NorRevSet[]={"正反设置","Ch Reverse"};
CSTR EpaSet[]={"行程设置","Endpoints"};
CSTR NeuSet[]={"辅助微调","Sub Trim"};
CSTR StickCurveSet[]={"摇杆曲线","Stick Curve"};
CSTR ThrCurveSet[]={"油门曲线","Throttle Curve"};
CSTR ThrHold[]={"油门锁定:","Throttle Hold:"};
CSTR ViryualSw[]={"虚拟开关","Virtual Switch"};
CSTR AuxSet[]={"辅助通道","Auxillary"};
CSTR MixerSet[]={"混控设置","Mixer Set"};
CSTR ServoDelay[]={"舵机减速","Servo Delay"};

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

//  模型参数调节菜单处理过程
u32 PageAeroSet(u8 event)
{
	static MENUSTAT ms;
	s16 language_offset;
			
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
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
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuAeroSet,&ms);
		
	//回车键处理
	if(KeyTstDown(KEY_ENT))
	{
		//如果有子菜单或子功能，进入
		if(MenuAeroSet[ms.iFocus+1].SubItem)
		{
			if(MenuAeroSet[ms.iFocus+1].SubItem==PageCurveSet)
			{
				PageCurveCnt = MenuAeroSet[ms.iFocus+1].Max;//曲线条数，固定翼分大中小舵，故有3*3条
				language_offset = PageCurveCnt*TxSys.Language;
				if(MenuAeroSet[ms.iFocus+1].Min==1)//固定翼摇杆曲线
				{
					PageCurveTitle = PageStkCurveTitle+language_offset;//加载曲线名称列表
					PageCurveValue = &Model.StkCurve[0][0][0];
					CurveMaster = ACRO_STICK;
				}
				else//固定翼油门曲线
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
	
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}

	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
