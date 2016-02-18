#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"

CSTR TitleHeliParamSet[]={"直升机参数调节","Heli Setting"};
CSTR HeliNorRevSet[]={"正反设置","Ch Reverse"};
CSTR HeliEpaSet[]={"行程设置","Endpoints"};
CSTR HeliNeuSet[]={"中点调节","Sub Trim"};
CSTR HeliGyroSet[]={"陀螺仪","Gyro Setting"};
CSTR HeliStickCurveSet[]={"摇杆曲线","Stick Curve"};
CSTR HeliThrCurveSet[]={"油门曲线","Throttle Curve"};
CSTR HeliPitchCurveSet[]={"螺距曲线","Pitch Curve"};
CSTR HeliThrHold[]={"油门锁定:","Throttle Hold:"};
CSTR HeliViryualSw[]={"虚拟开关","Virtual Switch"};
CSTR HeliAuxSet[]={"辅助通道","Auxillary"};
CSTR HeliSwashMixer[]={"斜盘混控","Swash"};
CSTR HeliServoDelay[]={"舵机减速","Servo Delay"};

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

//  模型参数调节菜单处理过程
u32 PageHeliSet(u8 event)
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
		ms.Total=MenuHeliSet[0].Max;
		ms.DrawMask=PD_ALL;	
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		ms.DrawMask=PD_ALL;	
	}
	
	//菜单通用处理过程
	PageMenuProc(MenuHeliSet,&ms);
		
	//回车键处理
	if(KeyTstDown(KEY_ENT))
	{
		//如果有子菜单或子功能，进入
		if(MenuHeliSet[ms.iFocus+1].SubItem)
		{ 
			if(MenuHeliSet[ms.iFocus+1].SubItem==PageCurveSet)
			{
				PageCurveCnt = MenuHeliSet[ms.iFocus+1].Max;//曲线条数
				language_offset = PageCurveCnt*TxSys.Language;
				
				if(MenuHeliSet[ms.iFocus+1].Min==1)//直升机摇杆曲线
				{
					PageCurveTitle = PageStkCurveTitle+language_offset;//加载曲线名称列表
					PageCurveValue = &Model.StkCurve[0][0][0];
					CurveMaster = HELI_STICK;
				}
				else if(MenuHeliSet[ms.iFocus+1].Min==2)//直升机油门曲线
				{
					PageCurveTitle = PageHeliThrCurveTitle+language_offset;
					PageCurveValue = &Model.ThrCurve[0][0];
					CurveMaster = HELI_THR;
				}
				else//直升机螺距曲线
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

	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
