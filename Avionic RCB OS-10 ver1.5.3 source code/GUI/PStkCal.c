#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"
#include "time.h"

CSTR TitleStickCal[]={"ҡ��У׼","Stick Calibration"};
CSTR StickCalStep1[]={" ������ҡ������","Center all sticks"};
CSTR StickCalStep2[]={" ������ҡ���г�"," Extreme sticks  "};
CSTR StickCalENT[]={" Ȼ��ENT��","  then press ENT"};
CSTR StickCalDone[]={"ҡ��У׼���!","Calibration Done"};

//  ҡ��У׼
u32 PageStkCal(u8 event)
{
	u8 i;
	static s8 StkCalStep,Step;
	static s16 stkmax[STK_NUM],stkmin[STK_NUM],stkmid[STK_NUM];
	
	if(event==PV_INIT)
	{
		StkCalStep=0;
		Step=-1;
		
		LCD_Clr_All();//����
		
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
	
	//��¼�����Сֵ
	for(i=0;i<STK_NUM;i++)
	{
		if(StickRaw[i]>stkmax[i])	stkmax[i]=StickRaw[i];
		if(StickRaw[i]<stkmin[i])	stkmin[i]=StickRaw[i];
	}
		
	//���ؼ�����
	if(KeyTstDown(KEY_EXT) || event==PV_END)
	{
		PageReturn(PV_INIT);
	}
	
	//�س�������
	if(KeyTstDown(KEY_ENT))
	{
		StkCalStep++;
		if(StkCalStep==1)//��¼�е�
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
