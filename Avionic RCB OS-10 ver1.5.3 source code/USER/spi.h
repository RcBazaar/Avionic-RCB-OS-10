#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f10x.h"

/*LCD_CS----------------PB10*/
/*LCD_RS(A0)------------PB0*/
#define SET_LCD_CS() GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define CLR_LCD_CS() GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define SET_LCD_RS() GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define CLR_LCD_RS() GPIO_ResetBits(GPIOB,GPIO_Pin_0)

//Flash_CSN----------PB11
#define SPI_FLASH_CSN_0		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
#define SPI_FLASH_CSN_1 	GPIO_SetBits(GPIOB,GPIO_Pin_11);

void SPI2_Configuration(void);
void SPI2_SetSpeed(u8 SpeedSet);
u8 SPI2_ReadWriteByte(u8 data);
void SPI2_Write_Byte(u8 data);
u8 SPI2_Read_Byte(void);

#endif
