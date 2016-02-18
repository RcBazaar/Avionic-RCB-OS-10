//STM8单片机自带SPI接口编程
#ifndef _RF_SPI_H_
#define _RF_SPI_H_

#include "stm32f10x.h"

/*RF_CS----------------PC6*/
/*RF_MOSI--------------PB4*/
/*RF_SCK---------------PB3*/
/*RF_MISO--------------PD2*/
/*RF_GDO0--------------PC7*/
/*RF_TXEN--------------PC8*/
/*RF_RXEN--------------PC9*/
#define		RF_CS_0		GPIO_ResetBits(GPIOC,GPIO_Pin_6)
#define		RF_CS_1		GPIO_SetBits(GPIOC,GPIO_Pin_6)
#define   	RF_MOSI_0	GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define   	RF_MOSI_1	GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define   	RF_SCLK_0	GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define  	RF_SCLK_1	GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define  	RF_TXEN_0	GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define 	RF_TXEN_1	GPIO_SetBits(GPIOC,GPIO_Pin_8)
#define  	RF_RXEN_0	GPIO_ResetBits(GPIOC,GPIO_Pin_9)
#define 	RF_RXEN_1	GPIO_SetBits(GPIOC,GPIO_Pin_9)

#define		RF_MISO   	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)
#define 	RF_GDO0		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)

void RF_SPI_Configuration(void);

/********************************
*		SPI写数据函数		  	*
* 功能描述：通过SPI写一个字节 	*
* 入口参数：写入的字节			*
* 出口参数：无					*
********************************/
void RF_SPI_Write_Byte(unsigned char data);

/********************************
*		SPI读数据函数		  	*
* 功能描述：通过SPI读一个字节		*
* 入口参数：无					*
* 出口参数：读取的字节			*
********************************/
unsigned char RF_SPI_Read_Byte(void);

#endif
