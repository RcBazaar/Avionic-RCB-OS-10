#ifndef _MX25L3205D_H_
#define _MX25L3205D_H_

#include "stm32f10x.h"

#define COM_WREN	0x06	//ʹ��WEL=1,дʹ��
#define COM_WRDI	0x04	//ʹ��WEL=0,д��ֹ
#define COM_RDID	0x9F	//��ID(3Bytes)
#define COM_RDSR	0x05	//��״̬�Ĵ���
#define COM_EWSR	0x50	//ʹ��д״̬�Ĵ���
#define COM_WRSR	0x01	//д״̬�Ĵ���
#define COM_READ	0x03	//������
#define COM_FREAD	0x0B	//���ٶ�����
#define COM_SE		0x20	//��������
#define COM_BE32	0x52	//32KB�����
#define COM_BE64	0xD8	//64KB�����
#define COM_CE		0x60	//оƬ����
#define COM_BP		0x02	//�ֽڱ��(ÿ��ֻ��д1Bytes)
#define COM_AAIP	0xAD	//AAI�������
#define COM_EBSY	0x70	//��AAI����ڼ�ʹ��SO�����RY/BY#״̬
#define COM_DBSY	0x80	//��AAI����ڼ��ֹSO�����RY/BY#״̬

void SST25_Unlock(void);//����flash
void SST25_Init(void);//��ʼ��
void SST25_Write_Enable(void);//ʹ��WEL=1,дʹ��
void SST25_Write_Disable(void);//ʹ��WEL=0,д��ֹ
void SST25_Sector_Erase(u32	addr);//��������
void SST25_Block_Erase(u32 addr);//�����
void SST25_Chip_Erase(void);//оƬ����
void SST25_Write_Data(u32 addr,u8 *pBuf,u16 length);
u32 SST25_Read_ID(void);//��оƬID
u8 SST25_Read_Status(void);//��״̬
void SST25_Write_Status(u8 status);//д״̬
void SST25_Read_Data(u32 addr,u8 *pBuf,u16 length);//������,�������25M
void SST25_FRead_Data(u32 addr,u8 *pBuf,u16 length);//������,�������80M
void SPI_Flash_Write_NoCheck(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite);//�޲���д
void SPI_Flash_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite);//������д

#endif


