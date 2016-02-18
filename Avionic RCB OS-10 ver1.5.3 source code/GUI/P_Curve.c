#include "Page.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "cfg.h"
#include "time.h"

CurveMaster_t CurveMaster;

CSTR CurveParamReseted[]={"曲线参数已复位!","Param Reseted!"};

//曲线设置
#define LCD_CURVE_LINES 6//一个页面最多能显示几个曲线点
#define LCD_CV_X 79
#define LCD_CV_Y 14
#define LCD_CV_W 48//曲线绘制区宽度
#define LCD_CV_H 48//曲线绘制区高度
#define	LCD_CV_IX 44
#define CURVE_FUNC 5//除曲线点外还需显示的可调参数个数

CSTR *PageCurveTitle;
u8 PageCurveCnt;
s16 *PageCurveValue;

//  根据EPA/EXP调节曲线
s32 CurveExpo(s16 i,s16 e)
{
	return e*i*i*i+(100-e)*i;
}

void CurveMake(s16 *curve,s16 epa,s16 exp)
{
	s32 i;
	s32 tc[CURVE_NUM/2];
	s32 expmax;
	
	if(exp<0)
	{
		expmax = CurveExpo(4,-exp);
		for(i=0;i<CURVE_NUM;i++)	
		{
			curve[i]=500+CurveExpo(i-4,-exp)*epa*5/expmax;
		}
		for(i=1;i<CURVE_NUM/2;i++)
		{
			tc[i] = curve[i];
			curve[i] = curve[CURVE_NUM/2+i]-500;
			curve[CURVE_NUM/2+i] = tc[i]+500;
		}
	}
	else
	{
		expmax = CurveExpo(4,exp);
		for(i=0;i<CURVE_NUM;i++)	
		{
			curve[i]=500+CurveExpo(i-4,exp)*epa*5/expmax;
		}
	}
}

void CurveDrawStkPos(s16 v)
{
	u8 x = LCD_CV_X+(LCD_CV_W/2)+(LCD_CV_W/2)*v/512;
	
	LcdDrawMaskY(x,LCD_CV_Y+1,0x55,7,0);
	LcdDrawMaskY(x,LCD_CV_Y+8,0x2A,7,0);
	LcdDrawMaskY(x,LCD_CV_Y+15,0x55,7,0);
	LcdDrawMaskY(x,LCD_CV_Y+22,0x2A,7,0);
	LcdDrawMaskY(x,LCD_CV_Y+29,0x55,7,0);
	LcdDrawMaskY(x,LCD_CV_Y+36,0x2A,7,0);
	LcdDrawMaskY(x,LCD_CV_Y+43,0x15,5,0);
}

u32 PageCurveSet(u8 event)
{
	u8 LcdBw = 0;
	u16 i,x,y,idx;
	u16 x0,y0;
	s16 v,delta=0;
	static u8 DrawMask;
	static u8 iStk;
	static u8 iFocus,Total,iStart;
	static u8 edit_enable = 0;
	static u32 target = 0;
	static u8 focus_flash = 1;
	static u16 focus_flash_cnt = 0;
	static u32 ParamRstTarget;
	static u8 ParamRstState = 0;
	
	if(target<=Time_NowMs())
	{
		target = Time_NowMs()+100;
		
		if(edit_enable)
		{
			focus_flash_cnt++;
			if(focus_flash_cnt>=10)
			{
				focus_flash_cnt = 0;
				focus_flash = !focus_flash;
			}
		}
		else
			focus_flash = 1;
		DrawMask |= PD_USER|PD_LIST|PD_TITLE;
	}
	
	if(event==PV_INIT)
	{
		LCD_Clr_All();//清屏
		iStk=0;
		iFocus=CURVE_NUM;
		iStart=0;
		Total=CURVE_NUM+CURVE_FUNC;
		DrawMask=PD_ALL;	
		
		LCD_Draw_Line(0,13,LCD_CV_X-2,13,1);
		LCD_Draw_Line(0,14,LCD_CV_X-2,14,1);
		
		return 1;
	}
	
	if(event==PV_USER)
	{
		LCD_Clr_All();//清屏
		DrawMask = PD_ALL;
	}

	//开始绘图
	if(DrawMask)    
	{
		//标题绘制
		if(DrawMask&PD_TITLE)
		{
			LcdBw = (iFocus==CURVE_NUM+4)&&focus_flash;
			LCD_Fill_Rect(0,0,82,10,LcdBw);//清空标题显示区
			Disp_GB2312_String(0,2,(u8 *)PageCurveTitle[iStk],0,LcdBw);
		}
		
		//曲线点列表、参数值
		if(DrawMask&(PD_LIST|PD_VALUE))
		{			
			for(i=0,y=18;i<LCD_CURVE_LINES ;i++,y+=7)//LCD_CURVE_LINES
			{
				idx=iStart+i;
				LcdBw = (idx==iFocus)&&focus_flash;
				LCD_Fill_Rect(LCD_CV_IX,y,LCD_CV_IX+31,y+7,LcdBw);//绘制选中框
				LcdDrawMiniInt(LCD_CV_IX+3,y+1,idx+1,1,0,0xff,0,LcdBw);	//绘制曲线序号
				LcdDrawMiniNum(LCD_CV_IX+7,y+1,LCD_MN_COL,LcdBw);		//绘制分隔点
				LcdDrawMiniInt(LCD_CV_IX+30,y+1,PageCurveValue[iStk*CURVE_DS+idx],0,1,0,1,LcdBw);	//绘制曲线值
				LcdBw=0;
			}
			
			//绘制EXP DR菜单
			LcdBw = (iFocus==CURVE_NUM)&&focus_flash;
			LCD_Fill_Rect(3,20,LCD_CV_IX-4,26,LcdBw);	
			LcdDrawMiniEng(5,21,"DR\x12",LcdBw);
			LcdDrawMiniInt(40,21,PageCurveValue[iStk*CURVE_DS+CURVE_NUM],0,0,1,1,LcdBw);//绘制EPA
			
			LcdBw = (iFocus==CURVE_NUM+1)&&focus_flash;
			LCD_Fill_Rect(3,30,LCD_CV_IX-4,36,LcdBw);	
			LcdDrawMiniEng(5,31,"EXP\x12",LcdBw);
			LcdDrawMiniInt(40,31,PageCurveValue[iStk*CURVE_DS+CURVE_NUM+1],0,0,1,1,LcdBw);//绘制EXP

			LcdBw = (iFocus==CURVE_NUM+2)&&focus_flash;
			LCD_Fill_Rect(3,40,LCD_CV_IX-4,46,LcdBw);	
			LcdDrawMiniEng(5,41,"LEVEL",LcdBw);
			
			LcdBw = (iFocus==CURVE_NUM+3)&&focus_flash;
			LCD_Fill_Rect(3,50,LCD_CV_IX-4,56,LcdBw);	
			LcdDrawMiniEng(5,51,"RESET",LcdBw);
			
			LcdBw=0;
		}
		//曲线
		if(DrawMask&PD_USER)
		{
			//绘制曲线
			LCD_Fill_Rect(LCD_CV_X,LCD_CV_Y,LCD_CV_X+LCD_CV_W,LCD_CV_Y+LCD_CV_H,0);//清空曲线绘图区
			LCD_Draw_Rect(LCD_CV_X,LCD_CV_Y,LCD_CV_X+LCD_CV_W,LCD_CV_Y+LCD_CV_H,1);//曲线绘图框
			LCD_Draw_Line(LCD_CV_X,LCD_CV_Y+LCD_CV_H/2,LCD_CV_X+LCD_CV_W,LCD_CV_Y+LCD_CV_H/2,1);
			LCD_Draw_Line(LCD_CV_X+LCD_CV_W/2,LCD_CV_Y,LCD_CV_X+LCD_CV_W/2,LCD_CV_Y+LCD_CV_H,1);
			
			for(i=0,x0=y0=0;i<CURVE_NUM;i++)//9点连线
			{
				x=i*LCD_CV_W/(CURVE_NUM-1);
				y=LCD_CV_H-(s32)PageCurveValue[iStk*CURVE_DS+i]*LCD_CV_H/CURVE_TRV;
				if(y>LCD_CV_H) y=LCD_CV_H;
				LCD_Draw_Line(LCD_CV_X+x0,LCD_CV_Y+y0,LCD_CV_X+x,LCD_CV_Y+y,1);
				if(i==iFocus) LCD_Draw_Line(LCD_CV_X+x,LCD_CV_Y,LCD_CV_X+x,LCD_CV_Y+LCD_CV_H,1);
				x0=x;
				y0=y;
			}
			
			if((CurveMaster==ACRO_STICK)||(CurveMaster==HELI_STICK))
			{
				if(iStk<=2)	CurveDrawStkPos(StickValue[0]);//副翼摇杆值
				else if(iStk<=5)	CurveDrawStkPos(StickValue[1]);//升降摇杆值
				else	CurveDrawStkPos(StickValue[3]);//方向摇杆值
			}
			if((CurveMaster==ACRO_THR)||(CurveMaster==HELI_THR)||(CurveMaster==HELI_PIT))
			{
				CurveDrawStkPos(StickValue[2]);//油门摇杆值
			}
		}
	
		DrawMask = 0;
	}
	
	if(KeyTstDown(KEY_ENT))
	{
		edit_enable = !edit_enable;
		DrawMask |= PD_LIST;
	}
	
	if(KeyTstHold(KEY_ENT))//参数复位
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
				if(iFocus<CURVE_NUM)//复位曲线点的值
				{
					PageCurveValue[iStk*CURVE_DS+iFocus] = iFocus*125;
					DrawMask |= PD_VALUE|PD_USER;
				}
				else//复位EXP、DR等的值
				{
					if(iFocus==CURVE_NUM+1)//EXP
					{
						v = PageCurveValue[iStk*CURVE_DS+iFocus] = 0;
						CurveMake(&PageCurveValue[iStk*CURVE_DS],PageCurveValue[iStk*CURVE_DS+CURVE_NUM],v);
						DrawMask |= PD_VALUE|PD_USER;
					}
					if(iFocus==CURVE_NUM)//DR
					{
						v = PageCurveValue[iStk*CURVE_DS+iFocus] = 100;
						CurveMake(&PageCurveValue[iStk*CURVE_DS],v,PageCurveValue[iStk*CURVE_DS+CURVE_NUM+1]);
						DrawMask |= PD_VALUE|PD_USER;
					}
				}
				edit_enable = 0;
				ParamRstState = 0;
				if(TxSys.KeyBeep)	BeepShort(1000+TxSys.KeyTone*30,30+TxSys.KeyBeep*6,20);
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
			if(iFocus<CURVE_NUM)//调节曲线点的值
			{
				v = PageCurveValue[iStk*CURVE_DS+iFocus];
				if(v>0)
				{
					if(KeyTstHold(KEY_DOWN))
					{
						if(v<=10)	v = 0;
						else	v-=10;
					}
					else
					{
						v--;
					}						
				}					
				PageCurveValue[iStk*CURVE_DS+iFocus] = v;
				DrawMask |= PD_VALUE|PD_USER;
			}
			else//调节EXP、DR等的值
			{
				if(iFocus==CURVE_NUM+4)//Stick select
				{
					if(iStk>0)	iStk--;
					else	iStk = PageCurveCnt-1;
					DrawMask = PD_ALL;
				}
				if(iFocus==CURVE_NUM+3)//RESET
				{
					for(i=0;i<CURVE_NUM;i++)	PageCurveValue[iStk*CURVE_DS+i]=i*125;
					PageCurveValue[iStk*CURVE_DS+CURVE_NUM  ]=100;		//DR
					PageCurveValue[iStk*CURVE_DS+CURVE_NUM+1]=0;		//EXP
					PageAlert(CurveParamReseted[TxSys.Language],500,PV_USER);
				}
				if(iFocus==CURVE_NUM+2)//LEVEL
				{
					if(KeyTstHold(KEY_DOWN))	delta = -10;
					else	delta = -1;
					for(i=0;i<CURVE_NUM;i++)	
					{
						v = PageCurveValue[iStk*CURVE_DS+i];
						v+=delta;
						if(v>1000)	v=1000;
						if(v<0)		v=0;
						PageCurveValue[iStk*CURVE_DS+i] = v;
					}
					DrawMask |= PD_VALUE|PD_USER;
				}
				if(iFocus==CURVE_NUM+1)//EXP
				{
					v = PageCurveValue[iStk*CURVE_DS+iFocus];
					if(v>-50)
					{
						if(KeyTstHold(KEY_DOWN))
						{
							if(v+50<=10)	v = -50;
							else	v-=10;                                                         
						}
						else
						{
							v--;
						}
					}
					if(PageCurveValue[iStk*CURVE_DS+CURVE_NUM+1]!=v)
					{
						CurveMake(&PageCurveValue[iStk*CURVE_DS],PageCurveValue[iStk*CURVE_DS+CURVE_NUM],v);
						DrawMask |= PD_VALUE|PD_USER;
					}
					PageCurveValue[iStk*CURVE_DS+iFocus] = v;
				}
				if(iFocus==CURVE_NUM)//DR
				{
					v = PageCurveValue[iStk*CURVE_DS+iFocus];
					if(v>-100)
					{
						if(KeyTstHold(KEY_DOWN))
						{
							if(v+100<=10)	v = -100;
							else	v-=10;
						}
						else
						{
							v--;
						}
					}
					if(PageCurveValue[iStk*CURVE_DS+CURVE_NUM]!=v)
					{
						CurveMake(&PageCurveValue[iStk*CURVE_DS],v,PageCurveValue[iStk*CURVE_DS+CURVE_NUM+1]);
						DrawMask |= PD_VALUE|PD_USER;
					}
					PageCurveValue[iStk*CURVE_DS+iFocus] = v;
				}
			}
		}	
		else
		{
			if(iFocus<Total-1) iFocus++;
			else		 		iFocus=0;
			DrawMask |= PD_LIST;
		}
	}
	if(KeyTstDown(KEY_UP))
	{
		if(edit_enable)
		{
			if(iFocus<CURVE_NUM)//调节曲线点的值
			{
				v = PageCurveValue[iStk*CURVE_DS+iFocus];
				if(v<CURVE_TRV)
				{
					if(KeyTstHold(KEY_UP))
					{
						if(CURVE_TRV-v<=10)	v = CURVE_TRV;
						else	v+=10;
					}
					else
					{
						v++;	
					}
				}					
				PageCurveValue[iStk*CURVE_DS+iFocus] = v;
				DrawMask |= PD_VALUE|PD_USER;
			}
			else//调节EXP、DR等的值
			{
				if(iFocus==CURVE_NUM+4)//Stick select
				{
					iStk++;
					if(iStk>=PageCurveCnt) iStk=0;
					DrawMask = PD_ALL;
				}
				if(iFocus==CURVE_NUM+3)//RESET
				{
					for(i=0;i<CURVE_NUM;i++)	PageCurveValue[iStk*CURVE_DS+i]=i*125;
					PageCurveValue[iStk*CURVE_DS+CURVE_NUM  ]=100;		//DR
					PageCurveValue[iStk*CURVE_DS+CURVE_NUM+1]=0;		//EXP
					PageAlert(CurveParamReseted[TxSys.Language],500,PV_USER);
				}
				if(iFocus==CURVE_NUM+2)//LEVEL
				{
					if(KeyTstHold(KEY_UP))	delta = 10;
					else	delta = 1;
					for(i=0;i<CURVE_NUM;i++)	
					{
						v = PageCurveValue[iStk*CURVE_DS+i];
						v+=delta;
						if(v>1000)	v=1000;
						if(v<0)		v=0;
						PageCurveValue[iStk*CURVE_DS+i] = v;
					}
					DrawMask |= PD_VALUE|PD_USER;
				}
				if(iFocus==CURVE_NUM+1)//EXP
				{
					v = PageCurveValue[iStk*CURVE_DS+iFocus];
					if(v<50)
					{
						if(KeyTstHold(KEY_UP))
						{
							if(50-v<=10)	v = 50;
							else	v+=10;                                                         
						}
						else
						{
							v++;
						}
					}
					if(PageCurveValue[iStk*CURVE_DS+CURVE_NUM+1]!=v)
					{
						CurveMake(&PageCurveValue[iStk*CURVE_DS],PageCurveValue[iStk*CURVE_DS+CURVE_NUM],v);
						DrawMask |= PD_VALUE|PD_USER;
					}
					PageCurveValue[iStk*CURVE_DS+iFocus] = v;
				}
				if(iFocus==CURVE_NUM)//DR
				{
					v = PageCurveValue[iStk*CURVE_DS+iFocus];
					if(v<100)
					{
						if(KeyTstHold(KEY_UP))
						{
							if(100-v<=10)	v = 100;
							else	v+=10;
						}
						else
						{
							v++;
						}
					}
					if(PageCurveValue[iStk*CURVE_DS+CURVE_NUM]!=v)
					{
						CurveMake(&PageCurveValue[iStk*CURVE_DS],v,PageCurveValue[iStk*CURVE_DS+CURVE_NUM+1]);
						DrawMask |= PD_VALUE|PD_USER;
					}
					PageCurveValue[iStk*CURVE_DS+iFocus] = v;
				}
			}
		}
		else
		{
			if(iFocus>0) iFocus--;
			else		 iFocus=Total-1;
			DrawMask |= PD_LIST;
		}
	}
	
	if(iFocus>=Total)							iFocus=Total-1;
	if(iFocus>=CURVE_NUM)						iStart=0;
	else if(iFocus<iStart) 						iStart=iFocus;
	else if(iFocus>iStart+LCD_CURVE_LINES-1) 	iStart=iFocus-LCD_CURVE_LINES+1;	

	//返回键处理
	if(KeyTstDown(KEY_EXT) )
	{
		edit_enable = 0;
		PageReturn(PV_INIT);
	}
	
	KeyClearDown(KEY_MENUALL);	
	
	return 0;
}
