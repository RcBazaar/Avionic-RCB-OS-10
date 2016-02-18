#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"
#include "Tx.h"
#include "time.h"
#include <string.h>

#define BMP_X	50
#define BMP_Y	29

CSTR TitleInBind[]={"正在对码","Binding"};

char PageBindBmpFile[]="res/bind1.bmp";

//  主界面绘制和按键处理
u32 PageBind(u8 event)
{
	char iconfile[16];
	static u32 target = 0;
	static u8  iconidx = 0;
	static u32 iconcnt = 0;
	static u32 BindCnt = 0;
	
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
		
		Disp_GB2312_String(0,3,(u8 *)TitleInBind[TxSys.Language],0,0);
		LCD_Draw_Line(0,14,127,14,1);
		LCD_Draw_Line(0,15,127,15,1);
		LCD_Disp_BMP(BMP_Y,BMP_X,PageBindBmpFile);
		
		iconidx = 1;
		BindCnt = TxBind();

		return 1;
	}	
	
	if(target<=Time_NowMs())
	{
		target = Time_NowMs()+5;//5ms周期
		
		iconcnt++;
		if(iconcnt>=50)
		{
			iconcnt = 0;
			iconidx++;
			if(iconidx>4)
			{
				iconidx = 1;
				LCD_Fill_Rect(BMP_X,BMP_Y,BMP_X+24,BMP_Y+22,0);
			}
			strcpy(iconfile,PageBindBmpFile);
			iconfile[8]='0'+iconidx;
			LCD_Disp_BMP(BMP_Y,BMP_X,iconfile);
		}
		
		if(BindCnt)	BindCnt--; 
	}
	
	//退出对码	
	if(KeyTstDown(KEY_EXT) ||BindCnt==0)
	{
		TxK8taBindCnt = TxF6TbBindCnt = 0;//退出绑定
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}
