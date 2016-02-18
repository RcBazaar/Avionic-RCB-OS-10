#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "adc.h"
#include "cfg.h"
#include "Tx.h"
#include <string.h>
#include <stdlib.h>
#include "time.h"

CSTR TitleEditIdCode[]={"编辑通讯ID","Edit Id Code"};
CSTR EditAutoId[]={"(自动)","(Auto)"};

extern u32 TxDevoRndId;

char PageEditIdBuf[MODELIDL+1];
u32 PageEditIdValue;
u8  PageEditStillSeed;                                                                                                                                                                                                         
#define PAGE_EDIT_LEFT	46
#define PAGE_EDIT_TOP	34
#define PAGE_EDIT_WIDTH	MODELIDL*6

//  编辑模型名称
u32 PageEditId(u8 event)
{
	u8 i;
	u32 mask = 100000;
	u32 seed;
	char c;
	static u32 DrawMask,EditIdx;
	static u8 edit_enable = 0;
	
	if(event==PV_INIT)
	{		
		LCD_Clr_All();//清屏
		
		Disp_GB2312_String(0,3,(u8 *)TitleEditIdCode[TxSys.Language],0,0);
		LCD_Draw_Line(0,14,127,14,1);
		LCD_Draw_Line(0,15,127,15,1);
		
		EditIdx=0;
		
		//把随机ID拷贝进编辑缓冲区
		if(PageEditIdValue==0)
		{			
			for(i=0;i<MODELIDL;i++)
			{
				PageEditIdBuf[i]=TxRndId/mask%10+'0';
				mask/=10;
			}
			Disp_GB2312_String(0,76,(u8 *)EditAutoId[TxSys.Language],0,0);
		}
		else strcpy(PageEditIdBuf,TxSys.RfIdStr);
		
		LCD_Draw_Line(0,14,128,14,1);
		LCD_Draw_Line(0,15,128,15,1);

		DrawMask=PD_ALL;

		return 1;
	}	
	
	if(DrawMask)
	{
		if(DrawMask&PD_IDX)
		{			
			LCD_Fill_Rect(PAGE_EDIT_LEFT,PAGE_EDIT_TOP-8,PAGE_EDIT_LEFT+PAGE_EDIT_WIDTH,PAGE_EDIT_TOP-1,0);			
			LcdDraw_EditArrow(PAGE_EDIT_LEFT+EditIdx*6,PAGE_EDIT_TOP-12);	
		}
		if(DrawMask&PD_VALUE)
		{
			Disp_GB2312_String(PAGE_EDIT_TOP,PAGE_EDIT_LEFT,(u8 *)PageEditIdBuf,0,0);	
			LCD_Draw_Line(PAGE_EDIT_LEFT,PAGE_EDIT_TOP+13,PAGE_EDIT_LEFT+PAGE_EDIT_WIDTH,PAGE_EDIT_TOP+13,1);
			LCD_Draw_Line(PAGE_EDIT_LEFT,PAGE_EDIT_TOP+14,PAGE_EDIT_LEFT+PAGE_EDIT_WIDTH,PAGE_EDIT_TOP+14,1);
		}
		DrawMask=0;
	}

	if(KeyTstDown(KEY_ENT))
	{
		edit_enable = !edit_enable;
	}
	
	//焦点移动与字符编辑
	if(KeyTstDown(KEY_DOWN))
	{
		if(edit_enable)
		{
			c = PageEditIdBuf[EditIdx];
			if(c>'0')
			{
				c--;
				DrawMask=PD_VALUE;
			}
			PageEditIdBuf[EditIdx] = c;
			
			//生成静态跳频种子
			ADCHandler();
			seed = PageEditIdValue+Time_NowMs();
			srand(seed);
			PageEditStillSeed = rand()%256;
		}
		else
		{
			if(EditIdx>0)	EditIdx--;
			else	EditIdx = MODELIDL-1;
			DrawMask=PD_IDX;

		}
	}
	if(KeyTstDown(KEY_UP))
	{
		if(edit_enable)
		{
			c = PageEditIdBuf[EditIdx];
			if(c<'9')
			{
				c++;
				DrawMask=PD_VALUE;
			}
			PageEditIdBuf[EditIdx] = c;
			
			//生成静态跳频种子
			ADCHandler();
			seed = PageEditIdValue+Time_NowMs();
			srand(seed);
			PageEditStillSeed = rand()%256;
		}
		else
		{
			if(EditIdx<MODELIDL-1)	EditIdx++;
			else	EditIdx = 0;
			DrawMask=PD_IDX;
		}
	}
	
	//确定键处理
	if(KeyTstHold(KEY_ENT))
	{
		PageEditIdValue = 0;
		for(i=0;i<MODELIDL;i++)
		{
			PageEditIdValue+=((u32)(PageEditIdBuf[i]-'0'))*mask;	
			mask/=10;
		}
		edit_enable = 0;
		PageReturn(PV_REDRAW);
	}

	//返回键处理
	if(KeyTstDown(KEY_EXT) || event==PV_END)
	{
		edit_enable = 0;
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);
	
	return 0;
}
