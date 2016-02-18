#include "Page.h"
#include "cfg.h"
#include "lcd.h"
#include "time.h"
#include "key.h"
#include "beep.h"
#include "adc.h"
#include "ff.h"
#include "Tx.h"
#include <string.h>

u32 PageMainDrawMask;
u16 TxBatVol;
u16 TxBatLowCnt,RxBatLowCnt;
u8 BatChecked;
u8 FlyModeIcon[7][40];
static s16 TimerFocus;

static FIL file;

//  ���Ƶ�ص���״̬
#define LCD_TXBAT_X			8		//�������ص�����ʾλ��
#define LCD_TXBAT_Y			1
#define LCD_RXBAT_X			53		//���ջ���ص�����ʾλ��
#define LCD_RXBAT_Y			1
#define BAT_TST_CYCLE		500		//��ѹ������ڣ���λms
#define BAT_LOW_CNT			5		//�͵�ѹ�澯�˲�����

void DrawBattery(void)
{
	static u8 txbatflash = 0;//�������ѹ��ʱ��˸
	static u8 rxbatflash = 0;//���ջ���ѹ��ʱ��˸
	
/*************************************************///�������ѹ��ʾ
    if(TxBatLowCnt>=BAT_LOW_CNT)//��ѹ��ʱ��˸
		txbatflash=!txbatflash;
    else						
		txbatflash=1;

	LcdDrawMiniNum(LCD_TXBAT_X-8,LCD_TXBAT_Y,LCD_MN_CHAR('T'),0);
	LcdDrawMiniNum(LCD_TXBAT_X-4,LCD_TXBAT_Y,LCD_MN_COL,0);
	
	LCD_Fill_Rect(LCD_TXBAT_X,LCD_TXBAT_Y,LCD_TXBAT_X+22,LCD_TXBAT_Y+5,0);
    if(txbatflash)
    {
    	LcdDrawMiniInt(LCD_TXBAT_X+18,LCD_TXBAT_Y,TxBatVol,0,2,0,1,0);
    	LcdDrawMiniNum(LCD_TXBAT_X+18,LCD_TXBAT_Y,LCD_MN_CHAR('V'),0);
    }
	
/************************************************************///���ջ���ѹ��ʾ
    if(RxBatLowCnt>=BAT_LOW_CNT)//��ѹ��ʱ��˸
		rxbatflash=!rxbatflash;
    else						
		rxbatflash=1;

	LcdDrawMiniNum(LCD_RXBAT_X-8,LCD_RXBAT_Y,LCD_MN_CHAR('R'),0);
	LcdDrawMiniNum(LCD_RXBAT_X-4,LCD_RXBAT_Y,LCD_MN_COL,0);
	
	LCD_Fill_Rect(LCD_RXBAT_X,LCD_RXBAT_Y,LCD_RXBAT_X+22,LCD_RXBAT_Y+5,0);
    if(rxbatflash)
    {
    	LcdDrawMiniInt(LCD_RXBAT_X+18,LCD_RXBAT_Y,RxBatVol,0,2,0,1,0);
    	LcdDrawMiniNum(LCD_RXBAT_X+18,LCD_RXBAT_Y,LCD_MN_CHAR('V'),0);
    }	
}

//��ص������͸澯
void BatteryHandler(void)
{
	static u32 batcnt = 0;
	static u8 BatLowMusicCnt = 0;
	u16 voltdev = 0;
	
	//��ؼ���Ƶ��
	if(batcnt>Time_NowMs())	return;
	batcnt=Time_NowMs()+BAT_TST_CYCLE;//500ms
	
	//ȡ�õ�ѹ
	PageMainDrawMask|=PMD_BAT;
	
	if(TxBatVol>=650)
	{
		if(BatChecked==0)
		{
			BatChecked = 1;
			if(TxBatVol>TxSys.BatVoltRec)	voltdev = TxBatVol-TxSys.BatVoltRec;
			else	voltdev = TxSys.BatVoltRec-TxBatVol;
			if(voltdev>=50)	TxSys.BatTime = 0;//���0.5V
		}
	
		TxSys.BatVoltRec = TxBatVol;//���µ�ѹ��¼
	}
	
	//�������ص�ѹ�澯
	if(TxBatVol/10<TxSys.BatWarn)
	{	
		if(TxBatLowCnt>BAT_LOW_CNT)//���˵�ص�ѹ����
		{
			BatLowMusicCnt++;
			if(BatLowMusicCnt>10)
			{
				BeepMusic(MusicBatLow);//�澯
				BatLowMusicCnt=0;
			}
		}
		else TxBatLowCnt++;
	}
	else TxBatLowCnt=0;
	
	//���ջ���ص�ѹ�澯
	if(RxBatVol)
	{
		if((RxBatVol/10<Model.RxBatWarn))
		{	
			if(RxBatLowCnt>BAT_LOW_CNT)//���˵�ص�ѹ����
			{
				BatLowMusicCnt++;
				if(BatLowMusicCnt>10)
				{
					BeepMusic(MusicBatLow);//�澯
					BatLowMusicCnt=0;
				}
			}
			else RxBatLowCnt++;
		}
		else RxBatLowCnt=0;
	}
}

#define LCD_SIG_X	116
#define LCD_SIG_Y	1	
#define SIG_LEVEL5	0x1F
#define SIG_LEVEL4	0x0F
#define SIG_LEVEL3	0x07
#define SIG_LEVEL2	0x03
#define SIG_LEVEL1	0x01

void Disp_SignalStrength(void)
{
	LCD_Fill_Rect(LCD_SIG_X,LCD_SIG_Y,LCD_SIG_X+10,LCD_SIG_Y+5,0);	
	
	if(RxRssiValue>-95)	LcdDrawMaskY(LCD_SIG_X,LCD_SIG_Y,SIG_LEVEL1,5,0);
	if(RxRssiValue>-85)	LcdDrawMaskY(LCD_SIG_X+2,LCD_SIG_Y,SIG_LEVEL2,5,0);
	if(RxRssiValue>-80)	LcdDrawMaskY(LCD_SIG_X+4,LCD_SIG_Y,SIG_LEVEL3,5,0);
	if(RxRssiValue>-75)	LcdDrawMaskY(LCD_SIG_X+6,LCD_SIG_Y,SIG_LEVEL4,5,0);
	if(RxRssiValue>-70)	LcdDrawMaskY(LCD_SIG_X+8,LCD_SIG_Y,SIG_LEVEL5,5,0);
}

//����΢����
#define LCD_TRIM_X1		2
#define LCD_TRIM_Y1		59
#define LCD_TRIM_X2		57
#define LCD_TRIM_Y2		7
#define LCD_TRIM_X3		75
#define LCD_TRIM_Y3		59
#define LCD_TRIM_X4		66
#define LCD_TRIM_Y4		7
#define LCD_TRIM_W		48	//΢�����ȣ�ʵ�ʿ��Ϊ������+3
#define	LCD_TRIM_SIZE	5

#define LCD_TRIM_ICONL	0x1f                          
#define LCD_TRIM_ICONM	0x15
#define LCD_TRIM_ICONH	0x11
#define LCD_TRIM_ICONHB	0x1f
#define LCD_TRIM_ICONR	0x1f

void DrawTrimBarH(u16 x,u16 y,s8 trim)
{
	s16 i,p,s;
	    
	//������״����ֹλ�� 
    p=trim;
    p=p*LCD_TRIM_W/2/50;
    if(trim>0)
    {
    	if(p==0) p=1;//������ʾ1��
    	s=LCD_TRIM_W/2+1;
    	p+=LCD_TRIM_W/2+1;
    }
    else if(trim<0)
    {
    	if(p==0) p=-1;//������ʾ1��
    	s=p+LCD_TRIM_W/2;
    	p=LCD_TRIM_W/2;
    }
    else
    {
    	s=p=LCD_TRIM_W/2;
    }
    
    //������״��
    for(i=0;i<s          ;i++)   LcdDrawMaskY(x+1+i,y,LCD_TRIM_ICONH ,LCD_TRIM_SIZE,0);
    for(   ;i<p          ;i++)   LcdDrawMaskY(x+1+i,y,LCD_TRIM_ICONHB,LCD_TRIM_SIZE,0);
    for(   ;i<=LCD_TRIM_W;i++)   LcdDrawMaskY(x+1+i,y,LCD_TRIM_ICONH ,LCD_TRIM_SIZE,0);
    LcdDrawMaskY(x,y,LCD_TRIM_ICONL,LCD_TRIM_SIZE,0);	//���
    LcdDrawMaskY(x+1+LCD_TRIM_W/2,y,LCD_TRIM_ICONM,LCD_TRIM_SIZE,0); //�е�
    LcdDrawMaskY(x+2+LCD_TRIM_W,y,LCD_TRIM_ICONR,LCD_TRIM_SIZE,0);   //�ұ�    
}

void DrawTrimBarV(u16 x,u16 y,s8 trim)
{
	s16 i,p,s;
	    
	//������״����ֹλ�� 
    p=-trim;
    p=p*LCD_TRIM_W/2/50;
    if(trim<0)
    {
    	if(p==0) p=1;//������ʾ1��
    	s=LCD_TRIM_W/2+1;
    	p+=LCD_TRIM_W/2+1;
    }
    else if(trim>0)
    {
    	if(p==0) p=-1;//������ʾ1��
    	s=p+LCD_TRIM_W/2;
    	p=LCD_TRIM_W/2;
    }
    else
    {
    	s=p=0;
    }

    //������״��
    for(i=0;i<s          ;i++)   LcdDrawMaskX(x,y+1+i,LCD_TRIM_ICONH ,LCD_TRIM_SIZE,0);
    for(   ;i<p          ;i++)   LcdDrawMaskX(x,y+1+i,LCD_TRIM_ICONHB,LCD_TRIM_SIZE,0);
    for(   ;i<=LCD_TRIM_W;i++)   LcdDrawMaskX(x,y+1+i,LCD_TRIM_ICONH ,LCD_TRIM_SIZE,0);
    LcdDrawMaskX(x,y,LCD_TRIM_ICONL,LCD_TRIM_SIZE,0);	//�±�
    LcdDrawMaskX(x,y+1+LCD_TRIM_W/2,LCD_TRIM_ICONM,LCD_TRIM_SIZE,0); //�е�
    LcdDrawMaskX(x,y+2+LCD_TRIM_W,LCD_TRIM_ICONR,LCD_TRIM_SIZE,0);   //�ϱ�    
}

void DrawTrimBar(void)
{     
    DrawTrimBarH(LCD_TRIM_X1,LCD_TRIM_Y1,Model.Trim[3]);
    DrawTrimBarV(LCD_TRIM_X2,LCD_TRIM_Y2,Model.Trim[1]);
    DrawTrimBarH(LCD_TRIM_X3,LCD_TRIM_Y3,Model.Trim[0]);
    DrawTrimBarV(LCD_TRIM_X4,LCD_TRIM_Y4,Model.Trim[2]);
}

//��ʱ��
s8 PageMainTimerSel;
u8 PageMainTimerCol;

void DrawUserTimer(u8 y)
{
	u8 x;
	u16 m,s;
	TIMERRUN *pt=&TxTimer;
	s32 timer=pt->Cnt;
	u8 sp;
	u8 ascii[2] = {0,0};

	x = 0;
	//����ͼ��		
	LcdDrawClock(x,y);
	x += 8;
	LcdDrawMiniNum(x,y+1,3,0);
	x += 6;
	
	//���Ʒ���
	if(timer<0)
	{
		Disp_String_6x8(y,x,"-",0,TimerFocus==2);
		timer=-timer;
	}
	else
	{
		Disp_String_6x8(y,x," ",0,TimerFocus==2);
	}
	x += 6;
	
	//����MS
	m=timer/60%100;
	s=timer%60;
	
	sp = pt->Run ? (PageMainTimerCol?1:0) : 1;//�����˸ͼ�����
	ascii[0] = '0'+m/10;
	Disp_String_6x8(y,x,ascii,0,TimerFocus==2);
	x += 6;
	ascii[0] = '0'+m%10;
	Disp_String_6x8(y,x,ascii,0,TimerFocus==2);
	x += 6;
	if(sp)	Disp_String_6x8(y,x,":",0,TimerFocus==2);
	else 	Disp_String_6x8(y,x," ",0,TimerFocus==2);
	x += 5;
	ascii[0] = '0'+s/10;
	Disp_String_6x8(y,x,ascii,0,TimerFocus==2);
	x += 6;
	ascii[0] = '0'+s%10;
	Disp_String_6x8(y,x,ascii,0,TimerFocus==2);	
}

void DrawBatTimer(u8 y)
{
	u8 x;
	u16 h,m;
	u8 ascii[2] = {0,0};
	static u16 Last_T = 0;
	
	if(Last_T==TxSys.BatTime)	return;
	Last_T = TxSys.BatTime;
	
	x = 0;
	//����ͼ��		
	LcdDrawThunder(x,y);
	x += 8;
	LcdDrawMiniNum(x,y+1,2,0);
	x += 12;
	
	h = TxSys.BatTime/3600%100;
	m = TxSys.BatTime/60%60;
	
	ascii[0] = '0'+h/10;
	Disp_String_6x8(y,x,ascii,0,TimerFocus==1);
	x += 6;
	ascii[0] = '0'+h%10;
	Disp_String_6x8(y,x,ascii,0,TimerFocus==1);
	x += 6;
	Disp_String_6x8(y,x,":",0,TimerFocus==1);
	x += 5;
	ascii[0] = '0'+m/10;
	Disp_String_6x8(y,x,ascii,0,TimerFocus==1);
	x += 6;
	ascii[0] = '0'+m%10;
	Disp_String_6x8(y,x,ascii,0,TimerFocus==1);	
}

//������ʱ��
void DrawRunTimer(u8 y)
{
	u8 x;
	u16 m,s;
	u8 sp;
	u8 ascii[2] = {0,0};
	
	x = 0;
	//����ͼ��		
	LcdDrawTxRun(x,y);
	x += 8;
	LcdDrawMiniNum(x,y+1,1,0);
	x += 12;
	
	//����ʱ��
	m = TxRunTime/60%100;
	s = TxRunTime%60;
	
	//���ƿ���ʱ��
	sp = PageMainTimerCol?1:0;//�����˸ͼ�����
	ascii[0] = '0'+m/10;
	Disp_String_6x8(y,x,ascii,0,TimerFocus==0);
	x += 6;
	ascii[0] = '0'+m%10;
	Disp_String_6x8(y,x,ascii,0,TimerFocus==0);
	x += 6;
	if(sp)	Disp_String_6x8(y,x,":",0,TimerFocus==0);
	else 	Disp_String_6x8(y,x," ",0,TimerFocus==0);
	x += 5;
	ascii[0] = '0'+s/10;
	Disp_String_6x8(y,x,ascii,0,TimerFocus==0);
	x += 6;
	ascii[0] = '0'+s%10;
	Disp_String_6x8(y,x,ascii,0,TimerFocus==0);	
}

#define AUX3_X		55
#define AUX3_Y		57
#define AUX4_X		72
#define AUX4_Y		57
#define AUXX_MAX	50

void DrawAux(void)
{
	u8 ydev;
	
	ydev = (MixerValue[CH_AUX3]+STK_TRV)*AUXX_MAX/1024;
	LCD_Draw_Line(AUX3_X,AUX3_Y,AUX3_X,AUX3_Y-AUXX_MAX,0);
	LCD_Draw_Line(AUX3_X,AUX3_Y,AUX3_X,AUX3_Y-ydev,1);
	LCD_Draw_Line(AUX3_X-1,AUX3_Y-AUXX_MAX/2,AUX3_X,AUX3_Y-AUXX_MAX/2,1);

	ydev = (MixerValue[CH_AUX4]+STK_TRV)*AUXX_MAX/1024;
	LCD_Draw_Line(AUX4_X,AUX4_Y,AUX4_X,AUX4_Y-AUXX_MAX,0);
	LCD_Draw_Line(AUX4_X,AUX4_Y,AUX4_X,AUX4_Y-ydev,1);
	LCD_Draw_Line(AUX4_X,AUX4_Y-AUXX_MAX/2,AUX4_X+1,AUX4_Y-AUXX_MAX/2,1);
}

//���ƽ�����Ϣ
void DrawInfo(void)
{
	static u32 target = 0;
	
	if(target>Time_NowMs())	return;
	else target = Time_NowMs()+500;//500ms
	
	//΢�������Զ�����������
	if(PageMainTrimShowCnt)	PageMainTrimShowCnt--;
	    
	LCD_Fill_Rect(10,15,44,25,0);
	if(Model.PpmIn&&(SwValue(SWD_TRAINER)<0))//������ģʽ
	{
		if(TxSys.Language==0)
			Disp_GB2312_String(15,10,"������",0,0);
		if(TxSys.Language==1)
			Disp_GB2312_String(15,10,"TRAIN",0,0);
	}
	//���Ʒ���ģʽ
	else if(Model.Type==MT_HELI)//ֱ�������Ʒ���ģʽ
	{		
		LcdDrawIcon(10,15,32,10,FlyModeIcon[FlyMode]);
	}
	else//�̶�����ƴ�С��
	{
		LcdDrawIcon(10,15,32,10,FlyModeIcon[4+DrMode]);
	}
    
	//�����˸����
	PageMainTimerCol=!PageMainTimerCol;
	
	//���ƿ���ʱ��
	DrawRunTimer(32);
	
	//���Ƶ�ؼ�ʱ��
	DrawBatTimer(40);
	
	//���ƶ�ʱ��	
	DrawUserTimer(48);
	
	Disp_SignalStrength();
	
	//���Ƹ���ͨ��
	//DrawAux();
}

// ��������ƺͰ�������
u32 PageMain(u8 event)
{
	FRESULT res;
	UINT br;
	char file_name[20];
	//static u32 ParamRstTarget;
	//static u8 ParamRstState = 0;
	
	if(event==PV_INIT)
	{
		LCD_Clr_All();//����

		if(TxSys.Language==0)
			res = f_open(&file,"res/flymode.bmp",FA_OPEN_EXISTING|FA_READ);//��ͼƬ�ļ�(����)
		if(TxSys.Language==1)
			res = f_open(&file,"res/flymode1.bmp",FA_OPEN_EXISTING|FA_READ);//��ͼƬ�ļ�(English)
		if(res==FR_OK)
		{
			res = f_lseek(&file,62);//62��BMP�ļ�ͷ�ĳ���
			res = f_read(&file,FlyModeIcon,sizeof(FlyModeIcon),&br);
			f_close(&file);
		}

		PageMainDrawMask = PMD_ALL;//��Ҫ�������н���Ҫ��
		BatteryHandler();//����һ�ε�ѹ���
		TimerFocus = -1;
		
		return 1;
	}
	
	//�������
	if(PageMainDrawMask)
	{	
		//ģ��ͼ��
		if(PageMainDrawMask&PMD_ICON)	
		{
			strcpy(file_name,"icon/");
			strcat(file_name,Model.Icon);
			strcat(file_name,".bmp");
			LCD_Disp_BMP(9,74,file_name);//����ģ��ͼ�꣨��Ļ�Ҳࣩ
		}
		//ģ������
		if(PageMainDrawMask&PMD_NAME)	//��ʾģ����
		{
			u16 x=100-strlen(Model.Name)*6/2;                                                                            
			Disp_String_6x8(48,x,(u8 *)Model.Name,0,0);
		}
		
		if(PageMainDrawMask&PMD_BAT)	DrawBattery();//���Ƶ��
		if(PageMainDrawMask&PMD_TRIM)	DrawTrimBar();//����΢����
		
		PageMainDrawMask = 0;
	} 
	
	DrawInfo();//��ʱ�������
	
	//��������
	if(KeyTstHold(KEY_ENT))
	{
		BeepMusic(MusicEnter);
		PageEnter(PageMenuMain,PV_INIT);
		PageMainDrawMask=PMD_ALL;
	}
	
	//���δ������
	KeyClearDown(KEY_MENUALL);

	return 0;
}

