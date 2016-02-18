#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"
#include "string.h"

CSTR TitleEditModelName[]={"编辑模型名称","Edit Model Name"};

char PageEditNameBuf[MODELNL+1];
#define PAGE_EDIT_LEFT	38
#define PAGE_EDIT_TOP	34
#define PAGE_EDIT_WIDTH	MODELNL*6

u32 PageEditName(u8 event)
{
	static u8 DrawMask,EditIdx;	 
	static u8 edit_enable = 0;
	u8 i,se;
	char c;
	
	if(event==PV_INIT)
	{		
		LCD_Clr_All();//清屏
		
		Disp_GB2312_String(0,3,(u8 *)TitleEditModelName[TxSys.Language],0,0);
		LCD_Draw_Line(0,14,127,14,1);
		LCD_Draw_Line(0,15,127,15,1);

		DrawMask=PD_ALL;
		EditIdx=0;
		
		for(i=se=0;i<MODELNL;i++)//把字符串拷贝进编辑缓冲区，长度不足的用空格填充
		{
			if(Model.Name[i]==0 || se)
			{
				PageEditNameBuf[i]=' ';
				se=1;
			}
			else
			{
				PageEditNameBuf[i]=Model.Name[i];		
			}
		}

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
			Disp_GB2312_String(PAGE_EDIT_TOP,PAGE_EDIT_LEFT,(u8 *)PageEditNameBuf,0,0);	
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
			c = PageEditNameBuf[EditIdx];
			if(c>' ')
			{
				if(KeyTstHold(KEY_DOWN))
				{
					if(c-' '<=10)	c = ' ';
					else	c-=10;
				}
				else
				{
					c--;
				}
			}
			else
			{
				c='~';
			}
			PageEditNameBuf[EditIdx] = c;
			
			DrawMask=PD_VALUE;
		}
		else
		{
			if(EditIdx>0)	EditIdx--;
			else	EditIdx = MODELNL-1;
			DrawMask=PD_IDX;
				
		}
	}
	if(KeyTstDown(KEY_UP))
	{
		if(edit_enable)
		{
			c = PageEditNameBuf[EditIdx];
			if(c<'~')
			{
				if(KeyTstHold(KEY_UP))
				{
					if('~'-c<=10)	c = '~';
					else	c+=10;
				}
				else
				{
					c++;
				}
			}
			else
			{
				c=' ';
			}
			PageEditNameBuf[EditIdx]=c;
			
			DrawMask=PD_VALUE;
		}
		else
		{
			if(EditIdx<MODELNL-1)	EditIdx++;
			else	EditIdx = 0;
			DrawMask=PD_IDX;
		}
	}
	
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		//拷贝结果并清除尾部空格
		char *p=PageEditNameBuf;
		for(p=PageEditNameBuf; *p ; p++);
		for(p--; *p==' ' && p>PageEditNameBuf ; p--) *p=0;
		strncpy(Model.Name,PageEditNameBuf,MODELNL);
		
		edit_enable = 0;
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}
