#ifndef _CFG_H_
#define _CFG_H_

#include "stm32f10x.h"

#define PARAM_TXSYS_ADDRESS 		0x0801A800 	//(0x08000000 + (STM32_FLASH_SIZE-22)*1024)
#define PARAM_MODEL_BASE_ADDRESS	0x0801A850	

//����
#define CHINESE			0
#define ENGLISH			1

//�������ʱ�İ�ȫ����
#define	THRSAFE			-485

#define MODELIDL	6	//ID�ַ�����

typedef struct{
	u8  StkCalied;		//ҡ���Ƿ�У׼
	u32	RfId;//ID
	char RfIdStr[MODELIDL+1];//ID�ַ�
	s16 ModelNo;		//ģ�ͺ�
	s16	TrimStep;	    //΢������
	s16 StkType;		//����
	s16 StkCali[4][3];  //ҡ��У׼ֵ
	s16 StkDa;			//ҡ������
	s16	Vibrator;		//����￪��
	s16	Music;			//���ֿ���
	s16 KeyBeep;		//����������
	s16 KeyTone;		//��������
	s16 Light;			//LCD����
	s16 Contrast;		//LCD�Աȶ�
	s16 LightOff;		//LCD����ʱ��
	s16 BatType;		//�������
	s16 BatWarn;		//��ѹ����ֵ
	s16 Language;		//����
	u16	BatTime;		//���ʹ��ʱ��
	u16	BatVoltRec;		//��ص�ѹ��¼
	u32	crc;			//CRCУ��ֵ
}TXSYS;

extern const TXSYS TxSysDef;
extern TXSYS TxSys;

/*********************************************************************/
//ģ�͹ز���
#define MODELNL		9	//ģ�����ĳ���
#define MODCFG_NUM	30	//���֧�ֵ�ģ����

//ģ������
typedef enum{
	MT_AERO,
	MT_HELI,
	MT_WING,
	MT_VTAIL,
	MT_NUM
}MODELTYPE;
/*********************************/

/*********************************************************************/
//��ص�ѹ����
#define VOLTAGE_NUMERATOR 	210			//Compute voltage from y = 2.1592x + 0.2493
#define VOLTAGE_OFFSET    	249
/*********************************/

/*********************************************************************/
//��ʱ����ز���
//��ʱ������
#define TMR_INH		0	//��ֹ
#define TMR_CLK		1	//��ʱ��
#define TMR_DCK		2	//����ʱ

//��ʱ�����ƿ���
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

extern TIMERRUN TxTimer;	//��ʱ������
/*********************************/

/*********************************************************************/
//�����ز���
#define	MAX_EPA				120
#define EPA_SIZE			100			//EPA����
#define	NEU_STEP			2			//�����㲽��
#define	MAX_DLT				100			//���������г�
#define	MAX_VTL				100			//Vβ����г�
#define	MAX_FAM				100			//���������г�
#define	MAX_THM				100			//���Ż���г�
#define	MAX_YWM				100			//ת�����г�
#define MAX_ARM				100			//���������г�
#define	DELAY_MAX			99			//ͨ����������
#define	MAX_AUX				100			//AUXͨ������
#define	TRV_AUX				120			//AUXͨ������
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
//ҡ����ز���
#define STKMOVTST	6	//ҡ�˾�ֹ�����ֵ,��ֵԽ��Խ�ٶ�

#define	STK_TRV		512	//ҡ���г�
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
#define ICONNL		8	//ͼ�����Ƶ���󳤶�
#define ICON_NUM	20	//���֧�ֵ�ͼ���
/*********************************/

/*********************************************************************/
//ҡ��������ز���
#define CURVE_NUM	9	//ҡ�����ߵĵ���	
#define CURVE_DS	(CURVE_NUM+3)//���ݴ洢�ռ��С��������9����������Լ������3��������
/*********************************/

/*********************************************************************/
//���ض���(SwDef)
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
	s16		ChMap[10];				//ͨ��ӳ��
	s16		TrChMap[10];			//��������ͨ��ӳ��
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
	u32		crc;//CRCУ��ֵ
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

