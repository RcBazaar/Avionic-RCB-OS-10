#ifndef _CFG_H_
#define _CFG_H_

#include "stm32f10x.h"

#define PARAM_TXSYS_ADDRESS 		0x0801A800 	//(0x08000000 + (STM32_FLASH_SIZE-22)*1024)
#define PARAM_MODEL_BASE_ADDRESS	0x0801A850	

//ÓïÑÔ
#define CHINESE			0
#define ENGLISH			1

//¿ª»ú¼ì²âÊ±µÄ°²È«ÓÍÃÅ
#define	THRSAFE			-485

#define MODELIDL	6	//ID×Ö·û³¤¶È

typedef struct{
	u8  StkCalied;		//Ò¡¸ËÊÇ·ñÐ£×¼
	u32	RfId;//ID
	char RfIdStr[MODELIDL+1];//ID×Ö·û
	s16 ModelNo;		//Ä£ÐÍºÅ
	s16	TrimStep;	    //Î¢µ÷²½³¤
	s16 StkType;		//ÊÖÐÍ
	s16 StkCali[4][3];  //Ò¡¸ËÐ£×¼Öµ
	s16 StkDa;			//Ò¡¸ËËÀÇø
	s16	Vibrator;		//Õð¶¯Âí´ï¿ª¹Ø
	s16	Music;			//ÒôÀÖ¿ª¹Ø
	s16 KeyBeep;		//°´¼üÒô¿ª¹Ø
	s16 KeyTone;		//°´¼üÒôµ÷
	s16 Light;			//LCD±³¹â
	s16 Contrast;		//LCD¶Ô±È¶È
	s16 LightOff;		//LCD±³¹âÊ±¼ä
	s16 BatType;		//µç³ØÀàÐÍ
	s16 BatWarn;		//µçÑ¹±¨¾¯Öµ
	s16 Language;		//ÓïÑÔ
	u16	BatTime;		//µç³ØÊ¹ÓÃÊ±¼ä
	u16	BatVoltRec;		//µç³ØµçÑ¹¼ÇÂ¼
	u32	crc;			//CRCÐ£ÑéÖµ
}TXSYS;

extern const TXSYS TxSysDef;
extern TXSYS TxSys;

/*********************************************************************/
//Ä£ÐÍ¹Ø²ÎÊý
#define MODELNL		9	//Ä£ÐÍÃûµÄ³¤¶È
#define MODCFG_NUM	30	//×î¶àÖ§³ÖµÄÄ£ÐÍÊý

//Ä£ÐÍÀàÐÍ
typedef enum{
	MT_AERO,
	MT_HELI,
	MT_WING,
	MT_VTAIL,
	MT_NUM
}MODELTYPE;
/*********************************/

/*********************************************************************/
//µç³ØµçÑ¹²ÎÊý
#define VOLTAGE_NUMERATOR 	210			//Compute voltage from y = 2.1592x + 0.2493
#define VOLTAGE_OFFSET    	249
/*********************************/

/*********************************************************************/
//¶¨Ê±Æ÷Ïà¹Ø²ÎÊý
//¶¨Ê±Æ÷ÀàÐÍ
#define TMR_INH		0	//½ûÖ¹
#define TMR_CLK		1	//ÕýÊ±Æ÷
#define TMR_DCK		2	//µ¹¼ÆÊ±

//¶¨Ê±Æ÷¿ØÖÆ¿ª¹Ø
#define TMRSW_THR	0	
#define TMRSW_SA	1
#define TMRSW_SB	2
#define TMRSW_SC	3
#define TMRSW_SD	4

typedef struct{
	s16 Type;
	s16	Sw;
	s16 Thr;
	s16 Time;
}TIMER;

typedef struct{
	s32	Cnt;
	u8	Run;
	u8	Alert;
	u8  Reset;
	TIMER *Cfg;
}TIMERRUN;

extern TIMERRUN TxTimer;	//¶¨Ê±¼ÆÊýÆ÷
/*********************************/

/*********************************************************************/
//»ì¿ØÏà¹Ø²ÎÊý
#define	MAX_EPA				120
#define EPA_SIZE			100			//EPAÂú·ù
#define	NEU_STEP			2			//ÖÐÁ¢µã²½³¤
#define	MAX_DLT				100			//Èý½ÇÒí»ì¿ØÐÐ³Ì
#define	MAX_VTL				100			//VÎ²»ì¿ØÐÐ³Ì
#define	MAX_FAM				100			//½óÒí¸±Òí»ì¿ØÐÐ³Ì
#define	MAX_THM				100			//ÓÍÃÅ»ì¿ØÐÐ³Ì
#define	MAX_YWM				100			//×ªÏò»ì¿ØÐÐ³Ì
#define MAX_ARM				100			//¸±Òí·½Ïò»ì¿ØÐÐ³Ì
#define	DELAY_MAX			99			//Í¨µÀ¼õËÙÉÏÏÞ
#define	MAX_AUX				100			//AUXÍ¨µÀÉÏÏÞ
#define	TRV_AUX				120			//AUXÍ¨µÀÉÏÏÞ
#define	MAX_GYRO			100
#define	TRV_GYRO			120
#define CURVE_TRV			1000

typedef struct{
	s16	Enable;
	s16 A2A,E2A,A2E,E2E;
}DELTAMIX;

typedef struct{
	s16	Enable;
	s16 E2E,R2E,E2R,R2R;
}VTAILMIX;

typedef struct{
	s16	Enable;
	s16 A2A,A2F,F2A,F2F;
}FLAPAILMIX;

typedef struct{
	s16 Rl2E,Rr2E,Al2E,Ar2E;
}YAWMIX;

typedef struct{
	s16 Enable;
	s16 A2A,A2R,R2A,R2R;
}AILRUDMIX;
/*********************************/

/*********************************************************************/
//Ò¡¸ËÏà¹Ø²ÎÊý
#define STKMOVTST	6	//Ò¡¸Ë¾²Ö¹¼ì²âãÐÖµ,ÊýÖµÔ½´óÔ½³Ù¶Û

#define	STK_TRV		512	//Ò¡¸ËÐÐ³Ì
#define STK_AIL		0
#define STK_ELE		1
#define	STK_THR		2
#define STK_RUD		3
#define STK_NUM		4
extern u16 StickRaw[STK_NUM];
extern s16 StickCal[STK_NUM];
extern s16 StickValue[STK_NUM];
/*********************************/

/*********************************************************************/
#define ICONNL		8	//Í¼±êÃû³ÆµÄ×î´ó³¤¶È
#define ICON_NUM	20	//×î¶àÖ§³ÖµÄÍ¼±êÊ
/*********************************/

/*********************************************************************/
//Ò¡¸ËÇúÏßÏà¹Ø²ÎÊý
#define CURVE_NUM	9	//Ò¡¸ËÇúÏßµÄµãÊý	
#define CURVE_DS	(CURVE_NUM+3)//Êý¾Ý´æ´¢¿Õ¼ä´óÐ¡£¨ÇúÏßÉÏ9¸öµãµÄÊý¾ÝÒÔ¼°¶îÍâµÄ3¸ö²ÎÊý£©
/*********************************/

/*********************************************************************/
//¿ª¹Ø¶¨Òå(SwDef)
#define SWD_HOLD	0
#define SWD_IDLE	1
#define SWD_DR		2
#define SWD_GEAR	3
#define SWD_FLAP	4
#define SWD_AUX1	5
#define SWD_AUX2	6
#define SWD_TRAINER	7
/*********************************/

typedef struct{
	char 	Name[MODELNL+1];
	char 	Icon[ICONNL+1];
	s16		Type;
	s8		Trim[4];
	s16		Protocol;
	s16		RfPwr;
	s16		ChMap[10];				//Í¨µÀÓ³Éä
	s16		TrChMap[10];			//½ÌÁ·¹¦ÄÜÍ¨µÀÓ³Éä
	TIMER	Timer;
	s16		Rev[10];
	s16		Epa[10][2];
	s16		Neu[10];
	s16		ThrHold;
	DELTAMIX Delta;
	VTAILMIX Vtail;
	FLAPAILMIX	FlpAil;
	YAWMIX		YawMix;
	s16			ThrMix[3];
	AILRUDMIX	AilRud;
	s16		StkCurve[3][3][CURVE_DS];
	s16		ThrCurve[3][CURVE_DS];
	s16		PitCurve[4][CURVE_DS];
	s16		Gyro[4];
	s16		SwashType;
	s16		Swash[3];
	s16		Gear[3];
	s16		Flap[3];
	s16		Aux[6][3];
	s16		Delay[10];
	s16		SwDef[8];
	s16		PpmIn;
	u8 		StillSeed;
	s16		RxBatType;
	s16		RxBatWarn;
	s16		VirtualSwStk[2];
	s16		VirtualSwVal[2];
	u32		crc;//CRCÐ£ÑéÖµ
}MODEL;

extern MODEL Model;
extern const MODEL ModelDef;

void LoadCfg(void);
void SaveCfg(void);

int32_t MODEL_checkValid(const MODEL *param);
void ModelLoad(u16 id,u8 txopen);
void ModelSave(u16 id);
void ModelCopy(u8 from,u8 to);
void ModelFormat(void);

#endif

