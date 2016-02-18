#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"
#include "time.h"

CSTR TitleStickCal[]={"摇杆校准","Stick Calibration"};
CSTR StickCalStep1[]={" 将所有摇杆置中","Center all sticks"};
CSTR StickCalStep2[]={" 打满各摇杆行程"," Extreme sticks  "};
CSTR StickCalENT[]={" 然后按ENT键","  then press ENT"};
CSTR StickCalDone[]={"摇杆校准完成!","Calibration Done"};

//  摇杆校准
u32 PageStkCal(u8 event)
{
	u8 i;
	static s8 StkCalStep,Step;
	static s16 stkmax[STK_NUM],stkmin[STK_NUM],stkmid[STK_NUM];
	
	if(event==PV_INIT)
	{
		StkCalStep=0;
		Step=-1;
		
		LCD_Clr_All();//清屏
		
		Disp_GB2312_String(0,3,(u8 *)TitleStickCal[TxSys.Language],0,0);
		LCD_Draw_Line(0,14,127,14,1);
		LCD_Draw_Line(0,15,127,15,1);
		
		stkmin[0]=5000;
		stkmin[1]=5000;
		stkmin[2]=5000;
		stkmin[3]=5000;
		stkmax[0]=0;
		stkmax[1]=0;
		stkmax[2]=0;
		stkmax[3]=0;
		
		return 1;
	}
		
	if(Step!=StkCalStep)
	{
		if(StkCalStep==0)
		{
			LCD_Disp_BMP(30,0,"res/stk.bmp");
			Disp_GB2312_String(30,24,(u8 *)StickCalStep1[TxSys.Language],0,0);
			Disp_GB2312_String(44,24,(u8 *)StickCalENT[TxSys.Language],0,0);
		}
		if(StkCalStep==1)
		{
			LCD_Disp_BMP(30,0,"res/stkmax.bmp");
			Disp_GB2312_String(30,24,(u8 *)StickCalStep2[TxSys.Language],0,0);
			Disp_GB2312_String(44,24,(u8 *)StickCalENT[TxSys.Language],0,0);
		}
		Step=StkCalStep;
	}
	
	//记录最大最小值
	for(i=0;i<STK_NUM;i++)
	{
		if(StickRaw[i]>stkmax[i])	stkmax[i]=StickRaw[i];
		if(StickRaw[i]<stkmin[i])	stkmin[i]=StickRaw[i];
	}
		
	//返回键处理
	if(KeyTstDown(KEY_EXT) || event==PV_END)
	{
		PageReturn(PV_INIT);
	}
	
	//回车键处理
	if(KeyTstDown(KEY_ENT))
	{
		StkCalStep++;
		if(StkCalStep==1)//记录中点
		{
			for(i=0;i<STK_NUM;i++)
			{
				stkmid[i]=StickRaw[i];
			}
		}
		if(StkCalStep>=2)
		{
			for(i=0;i<STK_NUM;i++)
			{
				TxSys.StkCali[i][1]=stkmid[i];
				TxSys.StkCali[i][0]=stkmid[i]-stkmin[i];
				TxSys.StkCali[i][2]=stkmax[i]-stkmid[i];
			}
			PageAlert(StickCalDone[TxSys.Language],1000,PV_END);
			TxSys.StkCalied = 1;
		}
	}
	
	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}
