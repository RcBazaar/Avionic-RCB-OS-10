#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

#define REVBAR_WIDTH	36
#define REVBAR_HELIGHT	4
#define REVBAR_X1	8
#define REVBAR_X2	76
#define REVBAR_Y	24
#define ARROW_X1	0
#define ARROW_X2	64
#define ARROW_Y		22

CSTR TitleNorRevSet[]={"正反设置","Ch Reverse"};

void Disp_NorRev(u8 x,u8 y)
{
	LcdDrawMiniNum(x,y,LCD_MN_CHAR('N'),0);
	x+=4;
	LcdDrawMiniNum(x,y,LCD_MN_CHAR('O'),0);
	x+=4;
	LcdDrawMiniNum(x,y,LCD_MN_CHAR('R'),0);
	x+=12;
	LcdDrawMiniNum(x,y,LCD_MN_CHAR('R'),0);
	x+=4;
	LcdDrawMiniNum(x,y,LCD_MN_CHAR('E'),0);
	x+=4;
	LcdDrawMiniNum(x,y,LCD_MN_CHAR('V'),0);
}

void Disp_RevBar(u8 x,u8 y,u8 state)
{
	LCD_Fill_Rect(x,y,x+REVBAR_WIDTH,y+REVBAR_HELIGHT,1);
	if(state)	LCD_Fill_Rect(x+1,y+1,x+REVBAR_WIDTH/2,y+REVBAR_HELIGHT-1,0);
	else	LCD_Fill_Rect(x+REVBAR_WIDTH/2,y+1,x+REVBAR_WIDTH-1,y+REVBAR_HELIGHT-1,0);
}

//模型参数调节菜单处理过程
u32 PageNorRevSet(u8 event)
{
	static u8 DrawMask;
	static u8 iFocus;
	
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
		DrawMask = PD_ALL;
		iFocus = 0;
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
			LCD_Fill_Rect(0,0,127,13,0);
			Disp_GB2312_String(0,3,(u8 *)TitleNorRevSet[TxSys.Language],0,0);
			LCD_Draw_Line(0,14,127,14,1);
			LCD_Draw_Line(0,15,127,15,1);
			
			Disp_NorRev(24,17);
			Disp_NorRev(92,17);
		}
		
		if(DrawMask&PD_IDX)
		{
			if(iFocus==0)	LcdDraw_RevBarArrow(ARROW_X1,ARROW_Y);
			else	LCD_Fill_Rect(ARROW_X1,ARROW_Y,ARROW_X1+6,ARROW_Y+8,0);
			
			if(iFocus==1)	LcdDraw_RevBarArrow(ARROW_X1,ARROW_Y+8);
			else	LCD_Fill_Rect(ARROW_X1,ARROW_Y+8,ARROW_X1+6,ARROW_Y+16,0);
			
			if(iFocus==2)	LcdDraw_RevBarArrow(ARROW_X1,ARROW_Y+16);
			else	LCD_Fill_Rect(ARROW_X1,ARROW_Y+16,ARROW_X1+6,ARROW_Y+24,0);
			
			if(iFocus==3)	LcdDraw_RevBarArrow(ARROW_X1,ARROW_Y+24);
			else	LCD_Fill_Rect(ARROW_X1,ARROW_Y+24,ARROW_X1+6,ARROW_Y+32,0);
			
			if(iFocus==4)	LcdDraw_RevBarArrow(ARROW_X1,ARROW_Y+32);
			else	LCD_Fill_Rect(ARROW_X1,ARROW_Y+32,ARROW_X1+6,ARROW_Y+40,0);
			
			if(iFocus==5)	LcdDraw_RevBarArrow(ARROW_X2,ARROW_Y);
			else	LCD_Fill_Rect(ARROW_X2,ARROW_Y,ARROW_X2+6,ARROW_Y+8,0);
			
			if(iFocus==6)	LcdDraw_RevBarArrow(ARROW_X2,ARROW_Y+8);
			else	LCD_Fill_Rect(ARROW_X2,ARROW_Y+8,ARROW_X2+6,ARROW_Y+16,0);
			
			if(iFocus==7)	LcdDraw_RevBarArrow(ARROW_X2,ARROW_Y+16);
			else	LCD_Fill_Rect(ARROW_X2,ARROW_Y+16,ARROW_X2+6,ARROW_Y+24,0);
			
			if(iFocus==8)	LcdDraw_RevBarArrow(ARROW_X2,ARROW_Y+24);
			else	LCD_Fill_Rect(ARROW_X2,ARROW_Y+24,ARROW_X2+6,ARROW_Y+32,0);
			
			if(iFocus==9)	LcdDraw_RevBarArrow(ARROW_X2,ARROW_Y+32);
			else	LCD_Fill_Rect(ARROW_X2,ARROW_Y+32,ARROW_X2+6,ARROW_Y+40,0);
		}
			
		if(DrawMask&PD_VALUE)	
		{
			LcdDrawMiniNum(REVBAR_X1,REVBAR_Y,LCD_MN_CHAR('C'),0);
			LcdDrawMiniNum(REVBAR_X1+4,REVBAR_Y,LCD_MN_CHAR('H'),0);
			LcdDrawMiniNum(REVBAR_X1+8,REVBAR_Y,1,0);
			Disp_RevBar(REVBAR_X1+12,REVBAR_Y,Model.Rev[0]);
			
			LcdDrawMiniNum(REVBAR_X1,REVBAR_Y+8,LCD_MN_CHAR('C'),0);
			LcdDrawMiniNum(REVBAR_X1+4,REVBAR_Y+8,LCD_MN_CHAR('H'),0);
			LcdDrawMiniNum(REVBAR_X1+8,REVBAR_Y+8,2,0);
			Disp_RevBar(REVBAR_X1+12,REVBAR_Y+8,Model.Rev[1]);
			
			LcdDrawMiniNum(REVBAR_X1,REVBAR_Y+16,LCD_MN_CHAR('C'),0);
			LcdDrawMiniNum(REVBAR_X1+4,REVBAR_Y+16,LCD_MN_CHAR('H'),0);
			LcdDrawMiniNum(REVBAR_X1+8,REVBAR_Y+16,3,0);
			Disp_RevBar(REVBAR_X1+12,REVBAR_Y+16,Model.Rev[2]);
			
			LcdDrawMiniNum(REVBAR_X1,REVBAR_Y+24,LCD_MN_CHAR('C'),0);
			LcdDrawMiniNum(REVBAR_X1+4,REVBAR_Y+24,LCD_MN_CHAR('H'),0);
			LcdDrawMiniNum(REVBAR_X1+8,REVBAR_Y+24,4,0);
			Disp_RevBar(REVBAR_X1+12,REVBAR_Y+24,Model.Rev[3]);
			
			LcdDrawMiniNum(REVBAR_X1,REVBAR_Y+32,LCD_MN_CHAR('C'),0);
			LcdDrawMiniNum(REVBAR_X1+4,REVBAR_Y+32,LCD_MN_CHAR('H'),0);
			LcdDrawMiniNum(REVBAR_X1+8,REVBAR_Y+32,5,0);
			Disp_RevBar(REVBAR_X1+12,REVBAR_Y+32,Model.Rev[4]);
			
			LcdDrawMiniNum(REVBAR_X2,REVBAR_Y,LCD_MN_CHAR('C'),0);
			LcdDrawMiniNum(REVBAR_X2+4,REVBAR_Y,LCD_MN_CHAR('H'),0);
			LcdDrawMiniNum(REVBAR_X2+8,REVBAR_Y,6,0);
			Disp_RevBar(REVBAR_X2+12,REVBAR_Y,Model.Rev[5]);
			
			LcdDrawMiniNum(REVBAR_X2,REVBAR_Y+8,LCD_MN_CHAR('C'),0);
			LcdDrawMiniNum(REVBAR_X2+4,REVBAR_Y+8,LCD_MN_CHAR('H'),0);
			LcdDrawMiniNum(REVBAR_X2+8,REVBAR_Y+8,7,0);
			Disp_RevBar(REVBAR_X2+12,REVBAR_Y+8,Model.Rev[6]);
			
			LcdDrawMiniNum(REVBAR_X2,REVBAR_Y+16,LCD_MN_CHAR('C'),0);
			LcdDrawMiniNum(REVBAR_X2+4,REVBAR_Y+16,LCD_MN_CHAR('H'),0);
			LcdDrawMiniNum(REVBAR_X2+8,REVBAR_Y+16,8,0);
			Disp_RevBar(REVBAR_X2+12,REVBAR_Y+16,Model.Rev[7]);
			
			LcdDrawMiniNum(REVBAR_X2,REVBAR_Y+24,LCD_MN_CHAR('C'),0);
			LcdDrawMiniNum(REVBAR_X2+4,REVBAR_Y+24,LCD_MN_CHAR('H'),0);
			LcdDrawMiniNum(REVBAR_X2+8,REVBAR_Y+24,9,0);
			Disp_RevBar(REVBAR_X2+12,REVBAR_Y+24,Model.Rev[8]);
			
			LcdDrawMiniNum(REVBAR_X2-4,REVBAR_Y+32,LCD_MN_CHAR('C'),0);
			LcdDrawMiniNum(REVBAR_X2,REVBAR_Y+32,LCD_MN_CHAR('H'),0);
			LcdDrawMiniNum(REVBAR_X2+4,REVBAR_Y+32,1,0);
			LcdDrawMiniNum(REVBAR_X2+8,REVBAR_Y+32,0,0);
			Disp_RevBar(REVBAR_X2+12,REVBAR_Y+32,Model.Rev[9]);
		}
		DrawMask = 0;
	}
		
	if(KeyTstDown(KEY_ENT))
	{
		Model.Rev[iFocus] = !Model.Rev[iFocus];
		DrawMask = PD_VALUE;
	}
	
	//上下按键处理
	if(KeyTstDown(KEY_DOWN))
	{
		if(iFocus<9)	iFocus++;
		else	iFocus = 0;
		DrawMask = PD_IDX;
	}
	if(KeyTstDown(KEY_UP))
	{
		if(iFocus>0)	iFocus--;
		else	iFocus = 9;
		DrawMask = PD_IDX;
	}
	
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_INIT);
	}

	KeyClearDown(KEY_MENUALL);

	
	return 0;
}

