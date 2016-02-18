#include "Page.h"
#include "string.h"
#include "lcd.h"
#include "beep.h"
#include "time.h"
#include "key.h"
#include "cfg.h"
#include "Tx.h"

PAGEPROC PageStack[PAGELEVEL];//ҳ�洦������ջ��
u8 PageStackIdx;//ҳ�洦������ջ������

u8 edit_enable;//�����༭ʹ�ܱ�־
volatile u8 LcdBw;
char *PageAlertText;//����Ի�������ʾ������
u32   PageAlertDelay;
u8	  PageAlertMsg;

void PageEnter(PAGEPROC page,u8 msg)//��ջ
{
	if(PageStackIdx>=PAGELEVEL-1)	return;
	PageStack[++PageStackIdx]=page;
	page(msg);
}

void PageGoto(PAGEPROC page,u8 msg)//��ǰ��ջԪ���滻
{
	PageStack[PageStackIdx]=page;	
	page(msg);
}

void PageReturn(u8 msg)//��ջ
{
	if(PageStackIdx>0)	PageStackIdx--;
	PageStack[PageStackIdx](msg);
}

void PageSet(PAGEPROC page,u8 msg)//�����׸���ջԪ��
{
	PageStack[PageStackIdx=0]=page;
	page(msg);
}

void PageAlertDraw(void)
{
	int s,p,l;		
	l=6*strlen(PageAlertText);//������������*ÿ���ֵ�������6��=��������ռ����
	s=(LCD_WIDTH-l)/2;
	p=(LCD_WIDTH+l)/2;

	if(PageAlertDelay==0)//����Ի������(�а�ť)
	{
		LCD_Fill_Rect(s-6,9,p+6,56,0);
		LCD_Fill_Rect(s-5,10,p+5,55,1);
		LCD_Fill_Rect(s-4,11,p+3,53,0);
		Disp_GB2312_String(18,s,(u8 *)PageAlertText,0,0);
		Disp_String_6x8(36,LCD_WIDTH/2-16," EXT ",0,1);	//���ư�ť	
	}
	else//����Ի������(�ް�ť)
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
		//��������
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

//��ʾ��ʾ����˻ص���һ������
void PageAlert(CSTR text,u32 delay,u8 retmsg)
{
	if(text==0)	return;
	PageAlertText=(char*)text;
	PageAlertDelay=delay;
	PageAlertMsg=retmsg;
	PageEnter(PageAlertProc,PV_INIT);
}

//��ʾ��ʾ����˻ص���һ������
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

//  �˵�ͨ�ù��̴���
void PageMenuProc(CMENUITEM *pmi,MENUSTAT *pms)
{	
	int i,s,y,idx;
	s16 v;
	s16 language_offset;
	static u32 ParamRstTarget;
	static u8 ParamRstState = 0;
	static u32 Target = 0;
	MENUITEM *mi=(MENUITEM *)&pmi[pms->iFocus+1];//�˴���&��ȡ��ַ
	
	//��ʼ��ͼ
	if(pms->DrawMask)    
	{
		if(pms->DrawMask&PD_TITLE)//�������
		{
			LCD_Fill_Rect(0,0,127,13,0);
			Disp_GB2312_String(0,3,(u8 *)pmi[0].Title[TxSys.Language],0,0);
			LCD_Draw_Line(0,14,127,14,1);
			LCD_Draw_Line(0,15,127,15,1);
		}
		
		if(pms->DrawMask&PD_IDX)//�˵���������
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
		
		if(pms->DrawMask&(PD_LIST|PD_VALUE))//�˵��б�
		{
			for(i=0,y=18;i<LCD_MENULINES && i<pms->Total;i++,y+=16)//LCD_MENULINES=3
			{
				idx = pms->iStart+i;
				LcdBw=(idx==pms->iFocus);//��ѡ�еĲ˵�Ҫ����
				idx++;
				
				//����ǻ��������б����Ҫ������������
				if(pms->DrawMask&PD_LIST || LcdBw)
				{
					LCD_Fill_Rect(0,y-1,127,y+13,LcdBw);		//����ѡ�п�
					LcdDrawMiniInt(2,y+4,idx,2,0,0xff,0,LcdBw);	//�������
					LcdDrawMiniNum(10,y+4,LCD_MN_DOT,LcdBw);	//���Ʒָ���
					Disp_GB2312_String(y,13,(u8 *)pmi[idx].Title[TxSys.Language],0,LcdBw);
					
					if(pmi[idx].SubItem) LcdDrawArrow(117,y+2,LcdBw);//���ӹ��ܵļӼ�ͷ��ʾ
	
					if(pmi[idx].Text)//���ı���ѡ����ʾ�ı�
					{
						s=110-6*strlen(pmi[idx].Text);
						Disp_GB2312_String(y,s,(u8 *)pmi[idx].Text,edit_enable&LcdBw,LcdBw);
					}
					else if(pmi[idx].pValue)//û���ı���ѡ����ʾֵ
					{
						if(pmi[idx].pValue)
						{
							v = *pmi[idx].pValue;
							if(v>pmi[idx].Max) v=pmi[idx].Max;
							if(v<pmi[idx].Min) v=pmi[idx].Min;
							*pmi[idx].pValue=v;
							
							//����ѡ�����ʾ����
							if(pmi[idx].Values)
							{
								language_offset = (pmi[idx].Max+1)*TxSys.Language;
								s=118-6*strlen(pmi[idx].Values[v+language_offset]);
								Disp_GB2312_String(y,s,(u8 *)pmi[idx].Values[v+language_offset],edit_enable&LcdBw,LcdBw);
							}
							else//����ѡ�����ʾ����
							{
								if(pmi[idx].SubItem)	s = 90;
								else	s = 96;
								LcdDrawInt(s,y,v,3,(u32)pmi[idx].Data,0,0,edit_enable&LcdBw,LcdBw);
							}
							
							//ѡ�е�ʱ����ʾ�༭״̬
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
		
		//��ɻ�ͼ		
		pms->DrawMask=0;
	}
	
	if(mi->pValue)//���ڲ���
	{
		if(KeyTstDown(KEY_ENT))//����ѡ��
		{
			edit_enable = !edit_enable;             
			pms->DrawMask = PD_LIST;
		}
	}
	
	if(mi->SubItem)	edit_enable = 0;
	
	if(edit_enable)
	{
		if((mi->pValue==&Model.Timer.Thr)||(mi->pValue==&Model.ThrHold))//�ö���ֱ�Ӷ�ȡ���ŵ�ֵ�������û�����
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
	
	if(KeyTstHold(KEY_ENT))//������λ
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
	
	//���°�������
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

//�˵�ѡ��
CSTR PageOptionOnOff[]={"�ر�","����"," OFF"," ON"};
CSTR PageOptionOkExe1[]={"��ִ��","ִ��","ȫ��","NO","YES","ALL"};
CSTR PageOptionOkExe2[]={"��ִ��","ִ��","NO","YES"};
CSTR PageOptionStkType[]={"ģʽ1","ģʽ2","ģʽ3","ģʽ4","MODE1","MODE2","MODE3","MODE4"};
CSTR PageOptionTxBatType[]={"2S ﮵�","3S ﮵�","4S ﮵�","2S LIPO","3S LIPO","4S LIPO"};
CSTR PageOptionRxBatType[]={"4S ����","5S ����","1S ﮵�","2S ﮵�","4S NI-MH","5S NI-MH","1S LIPO","2S LIPO"};
CSTR PageOptionLanguage[]={"����","Ӣ��","CHINESE","ENGLISH"};
CSTR PageOptionModType[]={"�̶���","ֱ����","ACRO","HELI"};
CSTR PageOptionSwDef[]={"����","SA","SB","SC","SD","SE","SF","INH","SA","SB","SC","SD","SE","SF"};
CSTR PageOptionProtocol[]={"�ر�","PPM","RCB-OS10","RCB6i","OFF","PPM","RCB-OS10","RCB6i"};
CSTR PageOptionRfPwr[]={"3mW","5mW","8mW","13mW","20mW","31mW","50mW","100mW","3mW","5mW","8mW","13mW","20mW","31mW","50mW","100mW"};
CSTR PageOptionChSelA[]={"����","����","����","����","�����","����","����1","����2","����3","����4","�Զ�","AIL","ELE","THR","RUD","GEAR","FLAP","AUX1","AUX2","AUX3","AUX4","AUTO"};
CSTR PageOptionChSelH[]={"����","����","����","����","����","�ݾ�","����1","����2","����3","����4","�Զ�","AIL","ELE","THR","RUD","GYRO","PITCH","AUX1","AUX2","AUX3","AUX4","AUTO"};
CSTR PageOptionTrChSel[]={"PPM_CH1","PPM_CH2","PPM_CH3","PPM_CH4","PPM_CH5","PPM_CH6","PPM_CH7","PPM_CH8","PPM_CH9","PPM_CH10","����","PPM_CH1","PPM_CH2","PPM_CH3","PPM_CH4","PPM_CH5","PPM_CH6","PPM_CH7","PPM_CH8","PPM_CH9","PPM_CH10","INH"};
CSTR PageOptionAuxSrc[]={"0%","+100%","-100%","����X","����Y","0%","+100%","-100%","GRAV X","GRAV Y"};
CSTR PageOptionTmrType[]={"����","��ʱ","����ʱ","INH","COUNT UP","COUNT DOWN"};
CSTR PageOptionTmrSw[]={"����","SA","SB","SC","SD","THR","SA","SB","SC","SD"};
CSTR PageOptionSwash[]={"�����","120CCPM","140CCPM","90CCPM","SINGLE","120CCPM","140CCPM","90CCPM"};
CSTR PageOptionLightOff[]={"����","10��","20��","30��","40��","50��","60��","EVER","10 S","20 S","30 S","40 S","50 S","60 S"};
CSTR PageStkCurveTitle[]={"����DR0����","����DR1����","����DR2����","����DR0����","����DR1����","����DR2����","����DR0����","����DR1����","����DR2����","AIL DR0 CURVE","AIL DR1 CURVE","AIL DR2 CURVE","ELE DR0 CURVE","ELE DR1 CURVE","ELE DR2 CURVE","RUD DR0 CURVE","RUD DR1 CURVE","RUD DR2 CURVE"};
CSTR PageAcroThrCurveTitle[]={"��������","Throttle curve"};
CSTR PageHeliThrCurveTitle[]={"������������","�ؼ�1��������","�ؼ�2��������","Throttle curve-Normal","Throttle curve-IdleUp1","Throttle curve-IdleUp2"};
CSTR PagePitCurveTitle[]={"�����ݾ�����","�ؼ�1�ݾ�����","�ؼ�2�ݾ�����","�����ݾ�����","Pitch curve-Normal","Pitch curve-IdleUp1","Pitch curve-IdleUp2","Pitch curve-Hold"};
CSTR PageOptionVirtualSw[]={"����","����","����","����","AIL","ELE","THR","RUD"};

CSTR Ch1[]={"ͨ��1:","CH1:"};
CSTR Ch2[]={"ͨ��2:","CH2:"};
CSTR Ch3[]={"ͨ��3:","CH3:"};
CSTR Ch4[]={"ͨ��4:","CH4:"};
CSTR Ch5[]={"ͨ��5:","CH5:"};
CSTR Ch6[]={"ͨ��6:","CH6:"};
CSTR Ch7[]={"ͨ��7:","CH7:"};
CSTR Ch8[]={"ͨ��8:","CH8:"};
CSTR Ch9[]={"ͨ��9:","CH9:"};
CSTR Ch10[]={"ͨ��10:","CH10:"};
