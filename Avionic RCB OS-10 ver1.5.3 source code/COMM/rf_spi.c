#include "rf_spi.h"

void RF_SPI_Configuration(void)
{
	/*RF_CS----------------PC6*/
	/*RF_MOSI--------------PB4*/
	/*RF_SCK---------------PB3*/
	/*RF_MISO--------------PD2*/
	/*RF_GDO0--------------PC7*/
	/*RF_TXEN--------------PC8*/
	/*RF_RXEN--------------PC9*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD,&GPIO_InitStructure);
}

/********************************
*		SPI写数据函数		  	*
* 功能描述：通过SPI写一个字节 	*
* 入口参数：写入的字节			*
* 出口参数：无					*
********************************/
void RF_SPI_Write_Byte(u8 data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		RF_SCLK_0;
	    (data&0x80)?RF_MOSI_1:RF_MOSI_0;
		RF_SCLK_1;
        data = data<<1;
	}
	RF_SCLK_0;
}

/********************************
*		SPI读数据函数		  	*
* 功能描述：通过SPI读一个字节		*
* 入口参数：无					*
* 出口参数：读取的字节			*
********************************/
u8 RF_SPI_Read_Byte(void)
{
	u8 i;
    u8 data = 0;
	for(i=0;i<8;i++)
	{
		RF_SCLK_0;
		data = data<<1;
		if(RF_MISO)
		{
			data |= 0x01;
		}
		RF_SCLK_1;
	}
	RF_SCLK_0;
	return data;
}
