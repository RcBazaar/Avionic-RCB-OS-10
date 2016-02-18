#include "Page.h"
#include "string.h"
#include "lcd.h"
#include "beep.h"
#include "time.h"
#include "key.h"
#include "cfg.h"
#include "Tx.h"

PAGEPROC PageStack[PAGELEVEL];//页面处理函数堆栈区
u8 PageStackIdx;//页面处理函数堆栈区索引

u8 edit_enable;//参数编辑使能标志
volatile u8 LcdBw;
char *PageAlertText;//警告对话框内显示的内容
u32   PageAlertDelay;
u8	  PageAlertMsg;

void PageEnter(PAGEPROC page,u8 msg)//入栈
{
	if(PageStackIdx>=PAGELEVEL-1)	return;
	PageStack[++PageStackIdx]=page;
	page(msg);
}

void PageGoto(PAGEPROC page,u8 msg)//当前堆栈元素替换
{
	PageStack[PageStackIdx]=page;	
	page(msg);
}

void PageReturn(u8 msg)//出栈
{
	if(PageStackIdx>0)	PageStackIdx--;
	PageStack[PageStackIdx](msg);
}

void PageSet(PAGEPROC page,u8 msg)//设置首个堆栈元素
{
	PageStack[PageStackIdx=0]=page;
	page(msg);
}

void PageAlertDraw(void)
{
	int s,p,l;		
	l=6*strlen(PageAlertText);//警告内容字数*每个字的列数（6）=警告字所占列数
	s=(LCD_WIDTH-l)/2;
	p=(LCD_WIDTH+l)/2;

	if(PageAlertDelay==0)//警告对话框绘制(有按钮)
	{
		LCD_Fill_Rect(s-6,9,p+6,56,0);
		LCD_Fill_Rect(s-5,10,p+5,55,1);
		LCD_Fill_Rect(s-4,11,p+3,53,0);
		Disp_GB2312_String(18,s,(u8 *)PageAlertText,0,0);
		Disp_String_6x8(36,LCD_WIDTH/2-16," EXT ",0,1);	//绘制按钮	
	}
	else//警告对话框绘制(无按钮)
	{
		LCD_Fill_Rect(s-6,9,p+6,41,0);
		LCD_Fill_Rect(s-5,10,p+5,40,1);
		LCD_Fill_Rect(s-4,11,p+3,38,0);
		Disp_GB2312_String(18,s,(u8 *)PageAlertText,0,0);
	}
}

u32 PageAlertProc(u8 event)
{
	static u32 starttime = 0;
	if(event==PV_INIT)
	{
		PageAlertDraw();
		starttime = Time_NowMs();
		return 1;
	}
	
	if(PageAlertDelay==0)
	{
		//按键处理
		if(KeyTstDown(KEY_EXT))
		{
			PageReturn(PageAlertMsg);
		}
	}
	else
	{
		if(Time_NowMs()-starttime>PageAlertDelay)
		{
			PageReturn(PageAlertMsg);
		}
	}
	return 0;
}

//显示提示框后退回到上一个界面
void PageAlert(CSTR text,u32 delay,u8 retmsg)
{
	if(text==0)	return;
	PageAlertText=(char*)text;
	PageAlertDelay=delay;
	PageAlertMsg=retmsg;
	PageEnter(PageAlertProc,PV_INIT);
}

//显示提示框后不退回到上一个界面
void PageAlertModel(CSTR text,u32 delay)
{
	if(text==0)	return;
	PageAlertText=(char*)text;
	PageAlertDelay=delay;
	PageAlertDraw();

	if(PageAlertDelay==0)
	{
		while(!KeyTstDown(KEY_EXT)) 
		{
			SysProc();
		}
	}
	else
	{
		while(delay--)
		{
			SysProc();
		}
	}		
	KeyFlush();	
}

//  菜单通用过程处理
void PageMenuProc(CMENUITEM *pmi,MENUSTAT *pms)
{	
	int i,s,y,idx;
	s16 v;
	s16 language_offset;
	static u32 ParamRstTarget;
	static u8 ParamRstState = 0;
	static u32 Target = 0;
	MENUITEM *mi=(MENUITEM *)&pmi[pms->iFocus+1];//此处的&是取地址
	
	//开始绘图
	if(pms->DrawMask)    
	{
		if(pms->DrawMask&PD_TITLE)//标题绘制
		{
			LCD_Fill_Rect(0,0,127,13,0);
			Disp_GB2312_String(0,3,(u8 *)pmi[0].Title[TxSys.Language],0,0);
			LCD_Draw_Line(0,14,127,14,1);
			LCD_Draw_Line(0,15,127,15,1);
		}
		
		if(pms->DrawMask&PD_IDX)//菜单项索引号
		{
			if(pms->Total>9)
			{
				LcdDrawMiniInt(110,4,pms->iFocus+1,2,0,0xff,1,0);
				LcdDrawMiniNum(110,4,LCD_MN_SPA,0);
				LcdDrawMiniInt(114,4,pms->Total,2,0,0xff,0,0);
			}
			else
			{
				LcdDrawMiniInt(120,4,pms->iFocus+1,2,0,0xff,1,0);
				LcdDrawMiniNum(120,4,LCD_MN_SPA,0);
				LcdDrawMiniInt(124,4,pms->Total,1,0,0xff,0,0);
			}
		}
		
		if(pms->DrawMask&(PD_LIST|PD_VALUE))//菜单列表
		{
			for(i=0,y=18;i<LCD_MENULINES && i<pms->Total;i++,y+=16)//LCD_MENULINES=3
			{
				idx = pms->iStart+i;
				LcdBw=(idx==pms->iFocus);//被选中的菜单要反显
				idx++;
				
				//如果是绘制整个列表才需要绘制以下内容
				if(pms->DrawMask&PD_LIST || LcdBw)
				{
					LCD_Fill_Rect(0,y-1,127,y+13,LcdBw);		//绘制选中框
					LcdDrawMiniInt(2,y+4,idx,2,0,0xff,0,LcdBw);	//绘制序号
					LcdDrawMiniNum(10,y+4,LCD_MN_DOT,LcdBw);	//绘制分隔点
					Disp_GB2312_String(y,13,(u8 *)pmi[idx].Title[TxSys.Language],0,LcdBw);
					
					if(pmi[idx].SubItem) LcdDrawArrow(117,y+2,LcdBw);//有子功能的加箭头提示
	
					if(pmi[idx].Text)//有文本的选项显示文本
					{
						s=110-6*strlen(pmi[idx].Text);
						Disp_GB2312_String(y,s,(u8 *)pmi[idx].Text,edit_enable&LcdBw,LcdBw);
					}
					else if(pmi[idx].pValue)//没有文本的选项显示值
					{
						if(pmi[idx].pValue)
						{
							v = *pmi[idx].pValue;
							if(v>pmi[idx].Max) v=pmi[idx].Max;
							if(v<pmi[idx].Min) v=pmi[idx].Min;
							*pmi[idx].pValue=v;
							
							//文字选项的显示文字
							if(pmi[idx].Values)
							{
								language_offset = (pmi[idx].Max+1)*TxSys.Language;
								s=118-6*strlen(pmi[idx].Values[v+language_offset]);
								Disp_GB2312_String(y,s,(u8 *)pmi[idx].Values[v+language_offset],edit_enable&LcdBw,LcdBw);
							}
							else//数字选项的显示数字
							{
								if(pmi[idx].SubItem)	s = 90;
								else	s = 96;
								LcdDrawInt(s,y,v,3,(u32)pmi[idx].Data,0,0,edit_enable&LcdBw,LcdBw);
							}
							
							//选中的时候显示编辑状态
							if(LcdBw)
							{
								if(v==pmi[idx].Max)
									LcdDraw_ValueArrow(122,y+2,1);
								else if(v==pmi[idx].Min)
									LcdDraw_ValueArrow(122,y+2,2);
								else
									LcdDraw_ValueArrow(122,y+2,0);
							}
						}
					}
				}
				LcdBw=0;
			}
		}
		
		//完成绘图		
		pms->DrawMask=0;
	}
	
	if(mi->pValue)//存在参数
	{
		if(KeyTstDown(KEY_ENT))//参数选中
		{
			edit_enable = !edit_enable;             
			pms->DrawMask = PD_LIST;
		}
	}
	
	if(mi->SubItem)	edit_enable = 0;
	
	if(edit_enable)
	{
		if((mi->pValue==&Model.Timer.Thr)||(mi->pValue==&Model.ThrHold))//该二项直接读取油门的值而不是用户输入
		{
			if(Target<=Time_NowMs())
			{
				Target = Time_NowMs()+500;//100ms
				*mi->pValue = MixerValue[CH_THR]*100/STK_TRV;
				pms->DrawMask = PD_LIST;
			}
		}
		else if(mi->pValue==&Model.VirtualSwVal[0])
		{
			if(Target<=Time_NowMs())
			{
				Target = Time_NowMs()+500;//100ms
				*mi->pValue = StickValue[Model.VirtualSwStk[0]]*100/STK_TRV;
				pms->DrawMask = PD_LIST;
			}
		}
		else if(mi->pValue==&Model.VirtualSwVal[1])
		{
			if(Target<=Time_NowMs())
			{
				Target = Time_NowMs()+500;//100ms
				*mi->pValue = StickValue[Model.VirtualSwStk[1]]*100/STK_TRV;
				pms->DrawMask = PD_LIST;
			}
		}
	}
	
	if(KeyTstHold(KEY_ENT))//参数复位
	{
		if(mi->pValue)
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
					*mi->pValue = mi->Default;
					edit_enable = 0;
					ParamRstState = 0;
					pms->DrawMask|=PD_VALUE;
					if(TxSys.KeyBeep)	BeepShort(1000+TxSys.KeyTone*30,30+TxSys.KeyBeep*6,20);
				}
			}
		}
	}
	else
	{
		ParamRstState = 0;
	}
	
	//上下按键处理
	if(KeyTstDown(KEY_DOWN))
	{
		if(edit_enable)
		{
			if((mi->pValue)&&(mi->pValue!=&Model.Timer.Thr)&&(mi->pValue!=&Model.ThrHold))
			{
				if(*mi->pValue>mi->Min) 
				{
					if(KeyTstHold(KEY_DOWN))
					{
						if((*mi->pValue-mi->Min)<=10)	*mi->pValue = mi->Min;
						else	(*mi->pValue)-=10;
					}
					else
					{
						(*mi->pValue)--;
					}
					pms->DrawMask|=PD_VALUE;
				}
			}
		}
		else
		{
			if(pms->iFocus<pms->Total-1) pms->iFocus++;
			else	pms->iFocus=0;
			pms->DrawMask=PD_LIST|PD_IDX;
		}
	}
	if(KeyTstDown(KEY_UP))
	{
		if(edit_enable)
		{
			if((mi->pValue)&&(mi->pValue!=&Model.Timer.Thr)&&(mi->pValue!=&Model.ThrHold))
			{
				if(*mi->pValue<mi->Max) 
				{
					if(KeyTstHold(KEY_UP))
					{
						if((mi->Max-*mi->pValue)<=10)	*mi->pValue = mi->Max;
						else	(*mi->pValue)+=10;
					}
					else
					{
						(*mi->pValue)++;
					}
					pms->DrawMask|=PD_VALUE;
				}
			}
		}
		else
		{
			if(pms->iFocus>0) pms->iFocus--;
			else	pms->iFocus=pms->Total-1;
			pms->DrawMask=PD_LIST|PD_IDX;
		}
	}
	
	if(pms->iFocus>=pms->Total)	pms->iFocus=pms->Total-1;
	if(pms->iFocus<pms->iStart)	pms->iStart=pms->iFocus;
	if(pms->iFocus>pms->iStart+LCD_MENULINES-1) pms->iStart=pms->iFocus-LCD_MENULINES+1;	
}	

//菜单选项
CSTR PageOptionOnOff[]={"关闭","开启"," OFF"," ON"};
CSTR PageOptionOkExe1[]={"不执行","执行","全部","NO","YES","ALL"};
CSTR PageOptionOkExe2[]={"不执行","执行","NO","YES"};
CSTR PageOptionStkType[]={"模式1","模式2","模式3","模式4","MODE1","MODE2","MODE3","MODE4"};
CSTR PageOptionTxBatType[]={"2S 锂电","3S 锂电","4S 锂电","2S LIPO","3S LIPO","4S LIPO"};
CSTR PageOptionRxBatType[]={"4S 镍电","5S 镍电","1S 锂电","2S 锂电","4S NI-MH","5S NI-MH","1S LIPO","2S LIPO"};
CSTR PageOptionLanguage[]={"中文","英文","CHINESE","ENGLISH"};
CSTR PageOptionModType[]={"固定翼","直升机","ACRO","HELI"};
CSTR PageOptionSwDef[]={"禁用","SA","SB","SC","SD","SE","SF","INH","SA","SB","SC","SD","SE","SF"};
CSTR PageOptionProtocol[]={"关闭","PPM","RCB-OS10","RCB6i","OFF","PPM","RCB-OS10","RCB6i"};
CSTR PageOptionRfPwr[]={"3mW","5mW","8mW","13mW","20mW","31mW","50mW","100mW","3mW","5mW","8mW","13mW","20mW","31mW","50mW","100mW"};
CSTR PageOptionChSelA[]={"副翼","升降","油门","方向","起落架","襟翼","辅助1","辅助2","辅助3","辅助4","自动","AIL","ELE","THR","RUD","GEAR","FLAP","AUX1","AUX2","AUX3","AUX4","AUTO"};
CSTR PageOptionChSelH[]={"副翼","升降","油门","方向","陀螺","螺距","辅助1","辅助2","辅助3","辅助4","自动","AIL","ELE","THR","RUD","GYRO","PITCH","AUX1","AUX2","AUX3","AUX4","AUTO"};
CSTR PageOptionTrChSel[]={"PPM_CH1","PPM_CH2","PPM_CH3","PPM_CH4","PPM_CH5","PPM_CH6","PPM_CH7","PPM_CH8","PPM_CH9","PPM_CH10","禁用","PPM_CH1","PPM_CH2","PPM_CH3","PPM_CH4","PPM_CH5","PPM_CH6","PPM_CH7","PPM_CH8","PPM_CH9","PPM_CH10","INH"};
CSTR PageOptionAuxSrc[]={"0%","+100%","-100%","重力X","重力Y","0%","+100%","-100%","GRAV X","GRAV Y"};
CSTR PageOptionTmrType[]={"禁用","计时","倒计时","INH","COUNT UP","COUNT DOWN"};
CSTR PageOptionTmrSw[]={"油门","SA","SB","SC","SD","THR","SA","SB","SC","SD"};
CSTR PageOptionSwash[]={"单舵机","120CCPM","140CCPM","90CCPM","SINGLE","120CCPM","140CCPM","90CCPM"};
CSTR PageOptionLightOff[]={"常亮","10秒","20秒","30秒","40秒","50秒","60秒","EVER","10 S","20 S","30 S","40 S","50 S","60 S"};
CSTR PageStkCurveTitle[]={"副翼DR0曲线","副翼DR1曲线","副翼DR2曲线","升降DR0曲线","升降DR1曲线","升降DR2曲线","方向DR0曲线","方向DR1曲线","方向DR2曲线","AIL DR0 CURVE","AIL DR1 CURVE","AIL DR2 CURVE","ELE DR0 CURVE","ELE DR1 CURVE","ELE DR2 CURVE","RUD DR0 CURVE","RUD DR1 CURVE","RUD DR2 CURVE"};
CSTR PageAcroThrCurveTitle[]={"油门曲线","Throttle curve"};
CSTR PageHeliThrCurveTitle[]={"常规油门曲线","特技1油门曲线","特技2油门曲线","Throttle curve-Normal","Throttle curve-IdleUp1","Throttle curve-IdleUp2"};
CSTR PagePitCurveTitle[]={"常规螺距曲线","特技1螺距曲线","特技2螺距曲线","锁定螺距曲线","Pitch curve-Normal","Pitch curve-IdleUp1","Pitch curve-IdleUp2","Pitch curve-Hold"};
CSTR PageOptionVirtualSw[]={"副翼","升降","油门","方向","AIL","ELE","THR","RUD"};

CSTR Ch1[]={"通道1:","CH1:"};
CSTR Ch2[]={"通道2:","CH2:"};
CSTR Ch3[]={"通道3:","CH3:"};
CSTR Ch4[]={"通道4:","CH4:"};
CSTR Ch5[]={"通道5:","CH5:"};
CSTR Ch6[]={"通道6:","CH6:"};
CSTR Ch7[]={"通道7:","CH7:"};
CSTR Ch8[]={"通道8:","CH8:"};
CSTR Ch9[]={"通道9:","CH9:"};
CSTR Ch10[]={"通道10:","CH10:"};
