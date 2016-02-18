#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x.h"

//KEY_ENT_IN---------------PC4
//KEY_EXT_IN---------------PC5
//KEY_UP_IN----------------PC14
//KEY_DOWN_IN--------------PC13
//KEY_TRIM_RL_IN-----------PA15
//KEY_TRIM_RR_IN-----------PC10
//KEY_TRIM_TD_IN-----------PC11
//KEY_TRIM_TU_IN-----------PB5
//KEY_TRIM_ED_IN-----------PB7
//KEY_TRIM_EU_IN-----------PB6
//KEY_TRIM_AL_IN-----------PB9
//KEY_TRIM_AR_IN-----------PB8

//KEY_ZYS_IN---------------PC12

#define KEY_WHEEL_IN	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)
#define KEY_ENT_IN		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)
#define KEY_EXT_IN		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)
#define KEY_UP_IN		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define KEY_DOWN_IN		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)
#define KEY_TRIM_RL_IN	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)
#define KEY_TRIM_RR_IN	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)
#define KEY_TRIM_TD_IN	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)
#define KEY_TRIM_TU_IN	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)
#define KEY_TRIM_ED_IN	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define KEY_TRIM_EU_IN	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define KEY_TRIM_AL_IN	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
#define KEY_TRIM_AR_IN	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)

#define KEY_ZYS_IN		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)

//�������� 
#define KEY_TRIM_TU	0x0001
#define KEY_TRIM_TD	0x0002
#define KEY_TRIM_EU	0x0004
#define KEY_TRIM_ED	0x0008
#define KEY_DOWN	0x0010
#define KEY_UP		0x0020
#define KEY_TRIM_AL	0x0400
#define KEY_TRIM_AR	0x0800
#define KEY_EXT		0x1000
#define KEY_ENT		0x2000
#define KEY_TRIM_RL	0x4000
#define KEY_TRIM_RR	0x8000
#define	KEY_MASK	0xFFFF
#define	KEY_MENUALL	(KEY_EXT|KEY_ENT|KEY_DOWN|KEY_UP)

//������
#define ZYS_LEFT	0	//����ģʽ
#define ZYS_RIGHT	1	//����ģʽ

//��������
#define SW_MASK		0x0003

//����״̬λ������(������������(SW_MASK<<SW_XX))
#define	SW_SB		0
#define SW_SC		2
#define SW_SA		4
#define SW_SD		6
#define SW_SE		8
#define SW_SF		10

//���ο��ص�����״̬
#define SW_STA_UP		0x0002	
#define SW_STA_MD		0x0003
#define SW_STA_DW		0x0001

#define	KEY_CONT_TRIG		(800/25)	//�����೤ʱ�䴥������
#define	KEY_CONT_FREQ		(140/25)	//����ʱ������һ��

void Key_Configuration(void);
u16 Key_ScanOnce(void);
u16 Switch_ScanOnce(void);
void KeyScanHandler(void);
u8 KeyTstDown(u16 mask);
void KeyClearDown(u16 mask);
u8 KeyTstHold(u16 mask);
void KeyFlush(void);
void KeyStopCnt(void);
u16 SwTstState(u16 mask);
s16 SwValue(s16 swpos);
u8 SwTstZYSState(void);

#endif

