#ifndef _TX_H_
#define _TX_H_

#include "stm32f10x.h"

#define MAP_CH_NUM	10		//可供映射的通道数,不包括自动通道

#define RF_POWER_NUM	8	//射频功率级数
#define RF_BIND_POWER	0x8D//绑定时使用的发射功率

//********************************************************************
//发射通道参数
#define	TX_CH_NUM	10	//设备通道数(指遥控器支持操作的通道数，与通信协议的通道数可能不同)
#define	TX_TRV		614	//512*120%
#define TX_BIND_WAIT	((u32)-1)
//********************************

//********************************************************************
//舵机控制参数
#define SERVO_NEU	1500
#define SERVO_TRV	500
//********************************

//********************************************************************
//摇摆通道参数
#define CHROCKDELAY	1	//舵机自动测试的速度，值越大速度越慢
#define CHROCKSTEP	3
//********************************

//******************************************************************
//飞行模式参数
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

extern u16 TxK8taBindCnt;//绑定计数
extern u16 TxF6TbBindCnt;//绑定计数

extern u16 RxBatVol;
extern s16 RxRssiValue;

//可供映射的通道编号
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
