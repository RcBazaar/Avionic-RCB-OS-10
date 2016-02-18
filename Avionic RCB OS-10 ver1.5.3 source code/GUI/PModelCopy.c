#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"
#include "string.h"
#include "time.h"

char PageModelNo[10];

CSTR TitleModelCopy[]={"模型复制","Model Copy"};

//模型复制处理过程
u32 PageModelCopy(u8 event)
{
	static u8 DrawMask;
	static u8 iFocus;
	static u8 edit_enable = 0;
	static u8 FromNo,ToNo;
	static u32 target = 0;
	static u8 focus_flash = 1;
	
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
		
		DrawMask = PD_ALL;
		iFocus = 0;
		FromNo = ToNo = TxSys.ModelNo;
		return 1;
	}
	
	if(event==PV_REDRAW)
	{
		LCD_Clr_All();//清屏
		DrawMask = PD_ALL;	
	}
	
	if(DrawMask)
	{
		//标题绘制
		if(DrawMask&PD_TITLE)
		{
			Disp_GB2312_String(0,3,(u8 *)TitleModelCopy[TxSys.Language],0,0);
			LCD_Draw_Line(0,14,127,14,1);
			LCD_Draw_Line(0,15,127,15,1);
			
			LcdDrawArrow(56,28,0);
			
			LCD_Draw_Rect(46,45,84,57,1);
			LCD_Fill_Rect(48,47,82,55,0);
			Disp_String_6x8(48,56,"ENT",0,0);
		}
		
		if((DrawMask&PD_IDX)||(DrawMask&PD_VALUE))
		{
			if(TxSys.Language==0)
			{
				strcpy(PageModelNo,"00号模型");
				
				PageModelNo[0]=FromNo/10+'0';
				PageModelNo[1]=FromNo%10+'0';
				Disp_GB2312_String(26,8,(u8 *)PageModelNo,0,(iFocus==0)&&focus_flash);
				
				PageModelNo[0]=ToNo/10+'0';
				PageModelNo[1]=ToNo%10+'0';
				Disp_GB2312_String(26,72,(u8 *)PageModelNo,0,(iFocus==1)&&focus_flash);
			}
			if(TxSys.Language==1)
			{
				strcpy(PageModelNo,"Model 00");

				PageModelNo[6]=FromNo/10+'0';
				PageModelNo[7]=FromNo%10+'0';
				Disp_GB2312_String(26,8,(u8 *)PageModelNo,0,(iFocus==0)&&focus_flash);
				
				PageModelNo[6]=ToNo/10+'0';
				PageModelNo[7]=ToNo%10+'0';
				Disp_GB2312_String(26,72,(u8 *)PageModelNo,0,(iFocus==1)&&focus_flash);
			}
			
			LCD_Fill_Rect(48,47,82,55,(iFocus==2)&&focus_flash);
			Disp_String_6x8(48,56,"ENT",0,(iFocus==2)&&focus_flash);
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

	if(KeyTstDown(KEY_DOWN))
	{
		if(edit_enable)
		{
			if(iFocus==0)
			{
				if(FromNo>1)
				{
					FromNo--;
					DrawMask = PD_VALUE;
				}
			}
			else if(iFocus==1)
			{
				if(ToNo>1)
				{
					ToNo--;
					DrawMask = PD_VALUE;
				}
			}
		}
		else
		{
			if(iFocus>0)	iFocus--;
			else	iFocus = 2;
			DrawMask = PD_IDX;
		}
	}
	
	if(KeyTstDown(KEY_UP))
	{
		if(edit_enable)
		{
			if(iFocus==0)
			{
				if(FromNo<MODCFG_NUM)
				{
					FromNo++;
					DrawMask = PD_VALUE;
				}
			}
			else if(iFocus==1)
			{
				if(ToNo<MODCFG_NUM)
				{
					ToNo++;
					DrawMask = PD_VALUE;
				}
			}
		}
		else
		{
			if(iFocus<2)	iFocus++;
			else	iFocus = 0;
			DrawMask = PD_IDX;
		}
	}
	
	if(KeyTstDown(KEY_ENT))
	{
		if(iFocus!=2)
		{
			edit_enable = !edit_enable;
		}
		else
		{
			if(FromNo!=ToNo)
			{
				ModelCopy(FromNo,ToNo);
				edit_enable = 0;
				PageReturn(PV_INIT);
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




