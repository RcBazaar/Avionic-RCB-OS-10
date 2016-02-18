#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"

#include "Tx.h"

//  主界面绘制和按键处理
u32 PageAbout(u8 event)
{	
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏	
		
		if(TxSys.Language==CHINESE)
		{
			Disp_GB2312_String(0,0,"版本:  V1.0",0,0);
			Disp_GB2312_String(12,0,"10通遥控器",0,0);
			Disp_GB2312_String(24,0,"内置双协议,兼容RCB6i",0,0);
		}
		if(TxSys.Language==ENGLISH)
		{
			Disp_GB2312_String(0,0,"Version:  V1.0",0,0);
			Disp_GB2312_String(12,0,"Transmitter with 10",0,0);
			Disp_GB2312_String(24,0,"channels,two protocols",0,0);
			Disp_GB2312_String(36,0,"embedded,compatible",0,0);
			Disp_GB2312_String(48,0,"with RCB6i.",0,0);
		}
		
		return 1;
	}	
	
	
	//返回键处理
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}

