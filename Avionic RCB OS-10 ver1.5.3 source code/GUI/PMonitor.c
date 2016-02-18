#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"
#include "time.h"
#include "ppm_decode.h"
#include "Tx.h"

#define LCD_MON_TMR		50

#define LCD_MON_ICONL	0x1f
#define LCD_MON_ICONM	0x15
#define LCD_MON_ICONH	0x11
#define LCD_MON_ICONHB	0x1f
#define LCD_MON_ICONR	0x1f
#define LCD_MON_W		60
#define	LCD_MON_SIZE	5

void DrawChMonitor(u16 x,u16 y,s16 idx,s16 value)
{
	s16 i,p,s;
	
	//ͨ���޷�
	if(value>TX_TRV)	value=TX_TRV;
	if(value<-TX_TRV)	value=-TX_TRV;
	
	//������״����ֹλ�� 
    p=value*LCD_MON_W/2/TX_TRV;
    if(value>0)
    {
    	s=LCD_MON_W/2+1;
    	p+=LCD_MON_W/2+1;
    }
    else if(value<0)
    {
    	s=p+LCD_MON_W/2;
    	p=LCD_MON_W/2;
    }
    else
    {
    	s=p=LCD_MON_W/2;
    }
    
    //��ʾͨ������
	if(idx<10)
	{
		LcdDrawMiniNum(x- 2,y,LCD_MN_SP,0);
		LcdDrawMiniNum(x+ 2,y,LCD_MN_CHAR('C'),0);
		LcdDrawMiniNum(x+ 6,y,LCD_MN_CHAR('H'),0);
		LcdDrawMiniNum(x+10,y,idx,0);
	}
	else
	{
		LcdDrawMiniNum(x- 2,y,LCD_MN_CHAR('C'),0);
		LcdDrawMiniNum(x+ 2,y,LCD_MN_CHAR('H'),0);
		LcdDrawMiniNum(x+6,y,idx/10,0);
		LcdDrawMiniNum(x+10,y,idx%10,0);
	}
    
    x+=16;
    
    //������״��
    for(i=0;i<s          ;i++)   LcdDrawMaskY(x+1+i,y,LCD_MON_ICONH ,LCD_MON_SIZE,0);
    for(   ;i<p          ;i++)   LcdDrawMaskY(x+1+i,y,LCD_MON_ICONHB,LCD_MON_SIZE,0);
    for(   ;i<=LCD_MON_W;i++)   LcdDrawMaskY(x+1+i,y,LCD_MON_ICONH ,LCD_MON_SIZE,0);
    LcdDrawMaskY(x,y,LCD_MON_ICONL,LCD_MON_SIZE,0);	//���
    LcdDrawMaskY(x+1+LCD_MON_W/2,y,LCD_MON_ICONM,LCD_MON_SIZE,0); //�е�
    LcdDrawMaskY(x+2+LCD_MON_W,y,LCD_MON_ICONR,LCD_MON_SIZE,0);   //�ұ�    
    LcdDrawMiniInt(x+LCD_MON_W+5,y,(s32)value*100/STK_TRV,3,0,1,0,0);
}

//ͨ��������
u32 PageMonitor(u8 event)
{	
	static u32 montmr = 0;
	static u8 iStart = 0;
	u8 i;
	
	if(event==PV_INIT)
	{
		montmr=Time_NowMs()+LCD_MON_TMR;
		LCD_Clr_All();//����
		iStart = 0;
		
		return 1;
	}	

	if(montmr<=Time_NowMs())
	{		
		montmr += LCD_MON_TMR;
		for(i=0;i<8;i++)
		{
			DrawChMonitor(18,2+i*8,i+iStart+1,TxChValue[i+iStart]);
		}		
	}
	
	if(KeyTstDown(KEY_DOWN))
	{
		if(iStart<2)	iStart++;
	}
	if(KeyTstDown(KEY_UP))
	{
		if(iStart>0)	iStart--;
	}

	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}

//PPMͨ��������
u32 PagePpmIn(u8 event)
{	
	static u32 montmr = 0;
	static u8 iStart = 0;
	u8 i;
	
	if(event==PV_INIT)
	{
		montmr=Time_NowMs()+LCD_MON_TMR;
		LCD_Clr_All();//����
		iStart = 0;
		
		return 1;
	}
	
	if(montmr<=Time_NowMs())
	{		
		montmr += LCD_MON_TMR;//50ִ��һ��
		
		for(i=0;i<8;i++)
		{
			DrawChMonitor(18,2+i*8,i+iStart+1,PpmInValue[i+iStart]);
		}		
	}
	
	if(KeyTstDown(KEY_DOWN))
	{
		if(iStart>0)	iStart--;
	}
	if(KeyTstDown(KEY_UP))
	{
		if(iStart<2)	iStart++;
	}
	
	//���ؼ�����
	if(KeyTstDown(KEY_EXT))
	{
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}
