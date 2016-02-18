#ifndef _MX25L3205D_H_
#define _MX25L3205D_H_

#include "stm32f10x.h"

#define COM_WREN	0x06	//使得WEL=1,写使能
#define COM_WRDI	0x04	//使得WEL=0,写禁止
#define COM_RDID	0x9F	//读ID(3Bytes)
#define COM_RDSR	0x05	//读状态寄存器
#define COM_EWSR	0x50	//使能写状态寄存器
#define COM_WRSR	0x01	//写状态寄存器
#define COM_READ	0x03	//读数据
#define COM_FREAD	0x0B	//快速读数据
#define COM_SE		0x20	//扇区擦除
#define COM_BE32	0x52	//32KB块擦除
#define COM_BE64	0xD8	//64KB块擦除
#define COM_CE		0x60	//芯片擦除
#define COM_BP		0x02	//字节编程(每次只能写1Bytes)
#define COM_AAIP	0xAD	//AAI连续编程
#define COM_EBSY	0x70	//在AAI编程期间使能SO以输出RY/BY#状态
#define COM_DBSY	0x80	//在AAI编程期间禁止SO以输出RY/BY#状态

void SST25_Unlock(void);//解锁flash
void SST25_Init(void);//初始化
void SST25_Write_Enable(void);//使得WEL=1,写使能
void SST25_Write_Disable(void);//使得WEL=0,写禁止
void SST25_Sector_Erase(u32	addr);//扇区擦除
void SST25_Block_Erase(u32 addr);//块擦除
void SST25_Chip_Erase(void);//芯片擦除
void SST25_Write_Data(u32 addr,u8 *pBuf,u16 length);
u32 SST25_Read_ID(void);//读芯片ID
u8 SST25_Read_Status(void);//读状态
void SST25_Write_Status(u8 status);//写状态
void SST25_Read_Data(u32 addr,u8 *pBuf,u16 length);//读数据,最大速率25M
void SST25_FRead_Data(u32 addr,u8 *pBuf,u16 length);//读数据,最大速率80M
void SPI_Flash_Write_NoCheck(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite);//无擦除写
void SPI_Flash_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite);//带擦除写

#endif


