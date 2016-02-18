#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"
#include "string.h"
#include "time.h"

CSTR TitleEditTime[]={"定时时间","Alarm Time"};

u8 Minute,Second;
char min[3],sec[3];

u32 PageEditTime(u8 event)
{
	static u8 DrawMask;	 
	static u8 iFocus;
	static u8 edit_enable = 0;
	static u32 target = 0;
	static u8 focus_flash = 1;
	static u32 ParamRstTarget;
	static u8 ParamRstState = 0;
	
	if(event==PV_INIT)
	{		
		LCD_Clr_All();//清屏

		iFocus = 0;
		
		Minute = Model.Timer.Time/60%100;
		Second = Model.Timer.Time%60;
		
		DrawMask = PD_ALL;
		
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		DrawMask = PD_ALL;	
	}

	if(DrawMask)
	{
		if(DrawMask&PD_TITLE)
		{
			Disp_GB2312_String(0,3,(u8 *)TitleEditTime[TxSys.Language],0,0);
			LCD_Draw_Line(0,14,127,14,1);
			LCD_Draw_Line(0,15,127,15,1);
			
			Disp_GB2312_String(28,56,":",0,0);
		}
		
		if((DrawMask&PD_IDX)||(DrawMask&PD_VALUE))
		{
			min[0] = '0'+Minute/10;
			min[1] = '0'+Minute%10;
			min[2] = 0;
			sec[0] = '0'+Second/10;
			sec[1] = '0'+Second%10;
			sec[2] = 0;
			
			Disp_GB2312_String(29,40,(u8 *)min,0,(iFocus==0)&&focus_flash);
			Disp_GB2312_String(29,64,(u8 *)sec,0,(iFocus==1)&&focus_flash);
		}
		
		DrawMask = 0;
	}
	
	if(target<=Time_NowMs())
	{
		target = Time_NowMs()+500;
		if(edit_enable)
			focus_flash = !focus_flash;
		else
			focus_flash = 1;
		DrawMask = PD_IDX;
	}
	
	if(KeyTstDown(KEY_ENT))
	{
		edit_enable = !edit_enable;
	}
	
	if(KeyTstHold(KEY_ENT))//参数复位
	{
		if(ParamRstState==0)
		{
			ParamRstTarget = Time_NowMs();
			ParamRstState = 1;
		}
		else
		{
			if(Time_NowMs()-ParamRstTarget>=1000)//1s
			{
				if(iFocus==0)	Minute = 0;
				if(iFocus==1)	Second = 0;
				edit_enable = 0;
				ParamRstState = 0;
				DrawMask = PD_VALUE;
				if(TxSys.KeyBeep)	BeepShort(1000+TxSys.KeyTone*30,30+TxSys.KeyBeep*6,20);
			}
		}
	}
	else
	{
		ParamRstState = 0;
	}
	
	if(KeyTstDown(KEY_DOWN))
	{
		if(edit_enable)
		{
			if(iFocus==0)
			{
				if(Minute>0)
				{
					if(KeyTstHold(KEY_DOWN))
					{
						if(Minute<=10)	Minute = 0;
						else	Minute-=10;
					}
					else
					{
						Minute--;
					}
					DrawMask = PD_VALUE;
				}
			}
			else
			{
				if(Second>0)
				{
					if(KeyTstHold(KEY_DOWN))
					{
						if(Second<=10)	Second = 0;
						else	Second-=10;
					}
					else
					{
						Second--;
					}
					DrawMask = PD_VALUE;
				}
			}
		}
		else
		{
			if(iFocus>0)	iFocus--;
			else	iFocus = 1;
			DrawMask = PD_IDX;
		}
	}
	
	if(KeyTstDown(KEY_UP))
	{
		if(edit_enable)
		{
			if(iFocus==0)
			{
				if(Minute<99)
				{
					if(KeyTstHold(KEY_UP))
					{
						if(99-Minute<=10)	Minute = 99;
						else	Minute+=10;
					}
					else
					{
						Minute++;
					}
					DrawMask = PD_VALUE;
				}
			}
			else
			{
				if(Second<59)
				{
					if(KeyTstHold(KEY_ENT))
					{
						if(59-Second<=10)	Second = 59;
						else	Second+=10;
					}
					else
					{
						Second++;
					}
					DrawMask = PD_VALUE;
				}
			}
		}
		else
		{
			if(iFocus<1)	iFocus++;
			else	iFocus = 0;
			DrawMask = PD_IDX;
		}
	}
	
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		Minute = (min[0]-'0')*10+min[1]-'0';
		Second = (sec[0]-'0')*10+sec[1]-'0';
		Model.Timer.Time = Minute*60+Second;
		
		edit_enable = 0;
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}
