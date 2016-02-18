#include "Page.h"
#include "cfg.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"
#include "ff.h"
#include "Tx.h"
#include <string.h>

CSTR TitleModelSelect[]={"模型选择","Model Select"};
CSTR NoCfgFile[]={"没有配置文件...","No Cfg File"};
CSTR ModelChanged[]={"模型已切换!","Model Changed!"};

char PageModFiles[MODCFG_NUM][MODELNL+6+1];//长度16
u16	PageModFileCnt,PageModFileIdx,PageModFileStart;

u32 PageModelSel(u8 event)
{	
	static u32 DrawMask;
	u8 LcdBw = 0;
	u16 idx,i,y;
	u32 ParamModAddr = 0;
	const MODEL *pModel = 0;
	
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
		
		Disp_GB2312_String(0,3,(u8 *)TitleModelSelect[TxSys.Language],0,0);
		LCD_Draw_Line(0,14,127,14,1);
		LCD_Draw_Line(0,15,127,15,1);
		
		PageModFileCnt=0;
		PageModFileIdx=0;
		PageModFileStart=0;
		
		DrawMask=PD_ALL;
		
		for(i=1;i<=MODCFG_NUM;i++)
		{
			ParamModAddr = PARAM_MODEL_BASE_ADDRESS+(i-1)*sizeof(MODEL);
			pModel = (const MODEL *)ParamModAddr;
			if(MODEL_checkValid(pModel)==0)
			{
				PageModFiles[PageModFileCnt][0]='0'+i/10;
				PageModFiles[PageModFileCnt][1]='0'+i%10;
				PageModFiles[PageModFileCnt][2]='.';
				strcpy(&PageModFiles[PageModFileCnt][3],pModel->Name);
				
				if(i==TxSys.ModelNo)//寻找当前配置文件
				{
					PageModFileIdx = PageModFileCnt;
				}
				
				PageModFileCnt++;
			}
		}
		
		//判断是否有配置文件
		if(PageModFileCnt==0)
		{
			PageAlert(NoCfgFile[TxSys.Language],1000,PV_END);
		}
		
		return 1;
	}	

	//绘制界面
	if(DrawMask)
	{		
		//起始位置整理
		if(PageModFileIdx<PageModFileStart) 				PageModFileStart=PageModFileIdx;
		if(PageModFileIdx>PageModFileStart+LCD_MENULINES-1)	PageModFileStart=PageModFileIdx-LCD_MENULINES+1;	

		//菜单项索引号
		if(DrawMask&PD_IDX)
		{
			if(PageModFileCnt>9)
			{
				LcdDrawMiniInt(110,4,PageModFileIdx+1,2,0,0xff,1,0);
				LcdDrawMiniNum(110,4,LCD_MN_SPA,0);
				LcdDrawMiniInt(114,4,PageModFileCnt,2,0,0xff,0,0);
			}
			else
			{
				LcdDrawMiniInt(120,4,PageModFileIdx+1,2,0,0xff,1,0);
				LcdDrawMiniNum(120,4,LCD_MN_SPA,0);
				LcdDrawMiniInt(124,4,PageModFileCnt,1,0,0xff,0,0);
			}
		}

		//绘制文件列表和模型图标
		if(DrawMask&PD_LIST)
		{	
			//显示列表
			for(i=0,y=18;i<LCD_MENULINES;i++,y+=16)
			{
				idx = PageModFileStart+i;
				LcdBw=(idx==PageModFileIdx);
				LCD_Fill_Rect(0,y-1,127,y+13,LcdBw);//绘制选中框	
				Disp_GB2312_String(y,3,(u8 *)PageModFiles[idx],0,LcdBw);//绘制名称					
				LcdBw = 0;
			}
		}

		DrawMask=0;
	}
	
	//上下按键处理
	if(KeyTstDown(KEY_DOWN))
	{
		if(PageModFileIdx<PageModFileCnt-1)	PageModFileIdx++;
		else		 							PageModFileIdx=0;
		DrawMask=PD_LIST|PD_IDX;
	}
	if(KeyTstDown(KEY_UP))
	{
		if(PageModFileIdx>0)	PageModFileIdx--;
		else		 			PageModFileIdx=PageModFileCnt-1;
		DrawMask=PD_LIST|PD_IDX;
	}

	//选中位图
	if(KeyTstDown(KEY_ENT))
	{
		//取得选中的模型号
		PageModFileIdx = 10*(PageModFiles[PageModFileIdx][0]-'0')+(PageModFiles[PageModFileIdx][1]-'0');
		
		//保存现有模型的配置
		ModelSave(TxSys.ModelNo);
		
		//加载新遥控器配置
		ModelLoad(TxSys.ModelNo=PageModFileIdx,1);
		
		PageAlert(ModelChanged[TxSys.Language],1000,PV_END);
	}
	
	//退出消息
	if(KeyTstDown(KEY_EXT) || event==PV_END)	PageReturn(PV_INIT);//这里不要用REDRAW，要用INIT，让模型菜单初始化
	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}
