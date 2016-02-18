#ifndef _FLASH_H_
#define _FLASH_H_

#include "stm32f10x.h"

#define STM32_FLASH_SIZE 	128 	 	//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_BASE 	0x08000000 	//STM32 FLASH����ʼ��ַ

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif	

u16 STMFLASH_ReadHalfWord(u32 faddr);
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);
void STMFLASH_WriteBuff(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);

#endif
