#ifndef _TX_H_
#define _TX_H_

#include "stm32f10x.h"

#define MAP_CH_NUM	10		//�ɹ�ӳ���ͨ����,�������Զ�ͨ��

#define RF_POWER_NUM	8	//��Ƶ���ʼ���
#define RF_BIND_POWER	0x8D//��ʱʹ�õķ��书��

//********************************************************************
//����ͨ������
#define	TX_CH_NUM	10	//�豸ͨ����(ָң����֧�ֲ�����ͨ��������ͨ��Э���ͨ�������ܲ�ͬ)
#define	TX_TRV		614	//512*120%
#define TX_BIND_WAIT	((u32)-1)
//********************************

//********************************************************************
//������Ʋ���
#define SERVO_NEU	1500
#define SERVO_TRV	500
//********************************

//********************************************************************
//ҡ��ͨ������
#define CHROCKDELAY	1	//����Զ����Ե��ٶȣ�ֵԽ���ٶ�Խ��
#define CHROCKSTEP	3
//********************************

//******************************************************************
//����ģʽ����
#define FM_NOR		0
#define	FM_IDL1		1
#define	FM_IDL2		2
#define	FM_HLD		3
//********************************

extern s32 MixerValue[MAP_CH_NUM];
extern s16 TxChValue[TX_CH_NUM];
extern s16 Std_TxChValue[TX_CH_NUM];
extern u8  TxPacket[32];
extern u8 RfPowTable[RF_POWER_NUM];

extern u8 FlyMode,DrMode;

extern u32 TxRndId;
extern s32 TxValueSum;

extern u16 TxK8taBindCnt;//�󶨼���
extern u16 TxF6TbBindCnt;//�󶨼���

extern u16 RxBatVol;
extern s16 RxRssiValue;

//�ɹ�ӳ���ͨ�����
enum{
	CH_AIL=0,
	CH_ELE,
	CH_THR,
	CH_RUD,
	CH_GEAR,
	CH_FLAP,
	CH_AUX1,
	CH_AUX2,
	CH_AUX3,
	CH_AUX4,
	CH_AUTO
};

void TxTimerCfg(void);
void TxTimerStart(u16 us, u16 (*cb)(void));
void TxTimerStop(void);

void TxProcTimerInit(void);

u32 TxLoad(u8 idx);
u32 TxBind(void);
void TxClose(void);
void TxMixer(void);

void TxF6tbBuildId(void);
void TxK8taBuildId(void);
void RxK8taUnpacking(void);

void PowerManager(void);

#endif//_TX_H_
