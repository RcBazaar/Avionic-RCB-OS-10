#ifndef _PAGE_H_
#define _PAGE_H_

#include "stm32f10x.h"

#define LCD_MENULINES		3	//每夜菜单个数

//页面处理过程定义
#define PV_RUN		0
#define PV_INIT		1
#define PV_END		2
#define PV_REDRAW	3
#define PV_TIMER	4
#define PV_USER		0x80

typedef u32 (*PAGEPROC)(u8 event);//函数指针

//菜单数据结构定义
typedef const char * CSTR;
typedef struct{
	CSTR *		Title;
	void *		SubItem;	//子功能|子菜单
	CSTR *		Values;		//值列表	 
	s16			Min,Max;	//菜单参数的最大最小值
	s16			*pValue;	//菜单参数对应的内存位置
	void *		Data;	//参数
	char *		Text;	//显示字符串
	s16			Default;//参数默认值
}MENUITEM,*PMENUITEM;
typedef const MENUITEM CMENUITEM;
#define SUBMENU(X)	((CPMENUITEM)((X)->SubItem))
#define SUBPROC(X)	((PAGEPROC)((X)->SubItem))

//菜单状态定义
typedef struct{
	u8 Init;
	u8 DrawMask,iFocus,iStart,Total;//iFocus=光标位置，total=子菜单的个数（行数）
	u8 Param;
}MENUSTAT;

#define PD_TITLE	0x01
#define PD_LIST		0x02
#define PD_IDX		0x04
#define PD_VALUE	0x08
#define PD_USER		0x10
#define PD_ALL		0xFF

//页面控制函数
#define PAGELEVEL	5	//最多允许进入几层UI
extern PAGEPROC PageStack[PAGELEVEL];//原变量在page.c中定义
extern u8 PageStackIdx;
void PageEnter(PAGEPROC page,u8 msg);
void PageGoto(PAGEPROC page,u8 msg);
void PageReturn(u8 msg);
void PageSet(PAGEPROC page,u8 msg);
void PageAlertModel(CSTR text,u32 delay);
void PageAlert(CSTR text,u32 delay,u8 msg);
void PageMenuProc(CMENUITEM *pmi,MENUSTAT *pms);

extern u8 edit_enable;//参数编辑使能标志

//主界面绘制控制
#define PMD_BACK	1
#define PMD_ICON	2
#define PMD_NAME	4
#define PMD_BAT		8
#define PMD_TRIM	0X10
#define PMD_INFO	0x20
#define PMD_ALL		0xffffffff

//系统处理
void SysProc(void);

extern u32 PageMainDrawMask;
u32 PageMain(u8 init);
extern u16 TxBatVol;
void BatteryHandler(void);

//微调控制
#define TRIM_MAX		50
#define TRIM_NUM_HIDE 	99
#define TRIM_NUM_TIME	6	//微调数字自动消隐时间，单位0.25ms
extern u32 PageMainTrimShowCnt;
void TrimProc(void);

//定时器控制
extern u16	TxRunTime;
void TimerProc(void);

//摇杆参数
#define STK_AIL				0
#define STK_ELE				1
#define	STK_THR				2
#define STK_RUD				3
#define STK_NUM				4
extern u16 StickRaw[STK_NUM];
extern s16 StickCal[STK_NUM];
extern s16 StickValue[STK_NUM];
extern s16 StickTrim[STK_NUM];
//摇杆数据处理
void StickHandler(void);

typedef enum{
	ACRO_STICK,
	ACRO_THR,
	HELI_STICK,
	HELI_THR,
	HELI_PIT,
}CurveMaster_t;
extern CurveMaster_t CurveMaster;

//菜单系列
u32 PageMenuMain(u8 event);
u32 PageMenuSys(u8 event);
u32 PageMenuBack(u8 event);
u32 PageMenuMod(u8 event);
u32 PageStkCal(u8 event);
u32 PageMonitor(u8 event);
u32 PageBind(u8 event);
u32 PageIconSel(u8 event);
u32 PageMenuRf(u8 event);
u32 PageEditName(u8 event);
u32 PageEditId(u8 event);
u32 PageModelSel(u8 event);
u32 PageModelCopy(u8 event);
u32 PageHeliSet(u8 event);
u32 PageAeroSet(u8 event);
u32 PageMenuChMap(u8 event);
u32 PageTimerSet(u8 event);
u32 PageEditTime(u8 event);
u32 PageSwDef(u8 event);
u32 PageMenuTrainer(u8 event);
u32 PagePpmIn(u8 event);
u32 PageMenuTrChMap(u8 event);

//模型参数菜单系列
u32 PageNorRevSet(u8 event);
u32 PageEpaSet(u8 event);
u32 PageNeuSet(u8 event);
u32 PageAuxChSet(u8 event);
u32 PageVirtualSw(u8 event);
u32 PageMixerSet(u8 event);
u32 PageDeltaSet(u8 event);
u32 PageVtailSet(u8 event);
u32 PageFlpAilSet(u8 event);
u32 PageThrMixSet(u8 event);
u32 PageYawMixSet(u8 event);
u32 PageAilRudSet(u8 event);
u32 PageGyroSet(u8 event);
u32 PageSwashSet(u8 event);
u32 PageDelaySet(u8 event);

//曲线设置函数
extern CSTR *PageCurveTitle;
extern u8 PageCurveCnt;
extern s16 *PageCurveValue;
u32 PageCurveSet(u8 event);

//关于
u32 PageAbout(u8 init);

extern CSTR PageOptionOnOff[];
extern CSTR PageOptionStkType[];
extern CSTR PageOptionTxBatType[];
extern CSTR PageOptionRxBatType[];
extern CSTR PageOptionLanguage[];
extern CSTR PageOptionOkExe1[];
extern CSTR PageOptionOkExe2[];
extern CSTR PageOptionModType[];
extern CSTR PageOptionSwDef[];
extern CSTR PageOptionProtocol[];
extern CSTR PageOptionRfPwr[];
extern CSTR PageOptionChSelA[];
extern CSTR PageOptionChSelH[];
extern CSTR PageOptionTrChSel[];
extern CSTR PageOptionAuxSrc[];
extern CSTR PageOptionTmrType[];
extern CSTR PageOptionTmrSw[];
extern CSTR PageStkCurveTitle[];
extern CSTR PageAcroThrCurveTitle[];
extern CSTR PageHeliThrCurveTitle[];
extern CSTR PagePitCurveTitle[];
extern CSTR PageOptionSwash[];
extern CSTR PageOptionLightOff[];
extern CSTR PageOptionVirtualSw[];

//通道名称
extern CSTR Ch1[];
extern CSTR Ch2[];
extern CSTR Ch3[];
extern CSTR Ch4[];
extern CSTR Ch5[];
extern CSTR Ch6[];
extern CSTR Ch7[];
extern CSTR Ch8[];
extern CSTR Ch9[];
extern CSTR Ch10[];

#endif//_PAGE_H_

