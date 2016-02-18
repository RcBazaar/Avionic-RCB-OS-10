#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"
#include "ff.h"
#include <string.h>

CSTR TitleModelIconSel[]={"ѡ��ģ��ͼ��","Symbol"};
CSTR NoIconFile[]={"û��ͼ���ļ�...","No Symbol File..."};

char PageIconFiles[ICON_NUM][13];
u16	PageIconFileCnt,PageIconFileIdx,PageIconFileStart;
static DIR dirs;
static FILINFO finfo;

u32 PageIconSel(u8 event)
{	
	static u32 DrawMask;
	int i,y,idx;
	u8 LcdBw = 0;
	char file[20];
	
	if(event==PV_INIT)
	{
		LCD_Clr_All();//����
		Disp_GB2312_String(0,3,(u8 *)TitleModelIconSel[TxSys.Language],0,0);
		LCD_Draw_Line(0,14,127,14,1);
		LCD_Draw_Line(0,15,127,15,1);
		
		DrawMask=PD_ALL;
		
		PageIconFileCnt=0;
		PageIconFileIdx=0;
		PageIconFileStart=0;

		//ö������ģ��ͼ��
		if(f_opendir(&dirs,"icon")==FR_OK)
    	{
			f_readdir(&dirs,&finfo);
			while((finfo.fname[0])&&(PageIconFileCnt<ICON_NUM))
        	{
				strcpy(PageIconFiles[PageIconFileCnt],finfo.fname);//����ͼƬ��
            	if(strncasecmp(PageIconFiles[PageIconFileCnt],Model.Icon,strlen(Model.Icon))==0)//Ѱ�ҵ�ǰͼ���ļ�
            	{
            		PageIconFileIdx = PageIconFileCnt;
            	}
            	PageIconFileCnt++;   
				f_readdir(&dirs,&finfo);
            }
        }

		//�ж��Ƿ���ͼ���ļ�
		if(PageIconFileCnt==0)
		{
			PageAlert(NoIconFile[TxSys.Language],1000,PV_END);
		}

		return 1;
	}	

	//���ƽ���
	if(DrawMask)
	{		
		//��ʼλ������
		if(PageIconFileIdx<PageIconFileStart) 					PageIconFileStart=PageIconFileIdx;
		if(PageIconFileIdx>PageIconFileStart+LCD_MENULINES-1)	PageIconFileStart=PageIconFileIdx-LCD_MENULINES+1;	
	
		//�˵���������
		if(DrawMask&PD_IDX)
		{
			if(PageIconFileCnt>9)
			{
				LcdDrawMiniInt(110,4,PageIconFileIdx+1,2,0,0xff,1,0);
				LcdDrawMiniNum(110,4,LCD_MN_SPA,0);
				LcdDrawMiniInt(114,4,PageIconFileCnt,2,0,0xff,0,0);
			}
			else
			{
				LcdDrawMiniInt(120,4,PageIconFileIdx+1,2,0,0xff,1,0);
				LcdDrawMiniNum(120,4,LCD_MN_SPA,0);
				LcdDrawMiniInt(124,4,PageIconFileCnt,1,0,0xff,0,0);
			}
		}
		
		//�����ļ��б��ģ��ͼ��
		if(DrawMask&PD_LIST)
		{
			//��ͼ��
			strcpy(file,"icon/");
			strcat(file,PageIconFiles[PageIconFileIdx]);
			LCD_Fill_Rect(74,22,127,63,0);//��ջ�ͼ��
			LCD_Disp_BMP(22,74,file);
			
			//��ʾ�б�
			for(i=0,y=18;i<LCD_MENULINES;i++,y+=16)
			{
				idx=PageIconFileStart+i;
				strcpy(file,PageIconFiles[idx]);
				file[strlen(file)-4]=0;
				LcdBw=(idx==PageIconFileIdx);				
				LCD_Fill_Rect(0,y-1,70,y+13,LcdBw);		//����ѡ�п�
				LcdDrawMiniInt(2,y+4,idx+1,2,0,0xff,0,LcdBw);//�������
				LcdDrawMiniNum(10,y+4,LCD_MN_DOT,LcdBw);		//���Ʒָ���
				Disp_GB2312_String(y,14,(u8 *)file,0,LcdBw);	//��������
				LcdBw=0;
			}
		}

		DrawMask=0;
	}
	
	//���°�������
	if(KeyTstDown(KEY_DOWN))
	{
		if(PageIconFileIdx<PageIconFileCnt-1)	PageIconFileIdx++;
		else		 							PageIconFileIdx=0;
		DrawMask=PD_LIST|PD_IDX;
	}
	if(KeyTstDown(KEY_UP))
	{
		if(PageIconFileIdx>0)	PageIconFileIdx--;
		else		 			PageIconFileIdx=PageIconFileCnt-1;
		DrawMask=PD_LIST|PD_IDX;
	}
	
	//ѡ��λͼ���˳�
	if(KeyTstDown(KEY_EXT))
	{
		PageIconFiles[PageIconFileIdx][strlen(PageIconFiles[PageIconFileIdx])-4]=0;
		strcpy(Model.Icon,PageIconFiles[PageIconFileIdx]);
		PageReturn(PV_INIT);
	}

	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}
