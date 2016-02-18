#include "cc2500.h"
#include "rf_spi.h"
#include "time.h"

//开启发射PA
void PA_TXEN(void)
{
	RF_TXEN_1;
	RF_RXEN_0;
}

//开启接收PA
void PA_RXEN(void)
{
	RF_TXEN_0;
	RF_RXEN_1;
}

//关闭PA
void PA_OFF(void)
{
	RF_TXEN_0;
	RF_RXEN_0;
}

//写CC2500配置寄存器
void CC2500_Write_Reg(unsigned char adress,unsigned char data)
{
	RF_CS_0;
	while(RF_MISO);
	RF_SPI_Write_Byte(adress);
	RF_SPI_Write_Byte(data);
	RF_CS_1;
}

//读CC2500配置和数据寄存器
unsigned char CC2500_Read_Reg(unsigned char adress)
{
	unsigned char temp_adress,value;
	temp_adress=(adress|READ_SINGLE);
	RF_CS_0;
	while(RF_MISO);
	RF_SPI_Write_Byte(temp_adress);
	value=RF_SPI_Read_Byte();
	RF_CS_1;
	return  value;
}

//读CC2500状态寄存器
unsigned char CC2500_Read_Staus(unsigned char adress)
{
	unsigned char temp_adress,value;
	temp_adress=(adress|READ_BURST);//状态寄存器地址
	RF_CS_0;
	while(RF_MISO);
	RF_SPI_Write_Byte(temp_adress);
	value=RF_SPI_Read_Byte();
	RF_CS_1;
	return  value;
}

//读取RSSI的值
short int CC2500_Get_RSSI(unsigned char ch)
{
	short int result;
	unsigned char RSSI;
	CC2500_Write_Reg(CCxxx0_CHANNR, ch);
	CC2500_Rx_Mode();
	RSSI = CC2500_Read_Staus(CCxxx0_RSSI);
	DelayMs(1);
	RSSI = CC2500_Read_Staus(CCxxx0_RSSI);
	if(RSSI>=128)
		result = (short int)(RSSI-256)/2-72;
	else
		result = (short int)RSSI/2-72; 
	return result;
}

//写CC2500滤波命令
void CC2500_Write_Com(unsigned char strope)
{
	RF_CS_0;
	while(RF_MISO);
	RF_SPI_Write_Byte(strope);
	RF_CS_1;
}

//CC2500复位函数
void CC2500_Reset(void)
{
	RF_CS_0; 
	CC2500_Write_Com(CCxxx0_SRES);//写入复位命令
	RF_CS_1;
}

/************************************
* 函 数 名：CC2500-Idle Mode			*
* 功能描述：配置CC2500工作在空闲模式	*
* 函數說明：此函数可使CC2500直接从发	*
*		   射模式或接收模式切回空闲	*
*		   模式，应用于不想接收，但	*
*		   要求快速返回发射或接收狀	*
*		   态						*
* 入口参数：无						*
* 返回参数：无						*
************************************/
void CC2500_Idle_Mode(void)
{
	CC2500_Write_Com(CCxxx0_SIDLE);		//0x36 进入空闲状态
}

/****************************************
* 函 数 名：void CC2500_Tx_Mode(void)	*
* 功能描述：配置CC2500工作在发射模式		*
* 函数说明：此函数可使TRW-24C直接从接收	*
*		   模式或空闲模式切回发射模式		*
* 入口参数：无							*
* 返回参数：无							*
****************************************/
void CC2500_Tx_Mode(void)
{
	PA_TXEN();//开发射PA
	CC2500_Write_Com(CCxxx0_SIDLE);		//0x36 进入空闲状态
	CC2500_Write_Com(CCxxx0_STX);		//0x35 开发射
}

/****************************************
* 函 数 名：void CC2500_Rx_Mode(void)	*
* 功能描述：配置CC2500工作在接收模式		*
* 函数说明：此函数可使CC2500直接从发射	*
*		   模式切回空闲模式或接收模式		*
* 入口参数：无							*
* 返回参数：无							*
****************************************/
void CC2500_Rx_Mode(void)
{
	PA_RXEN();//开接收PA
	CC2500_Write_Com(CCxxx0_SIDLE);		//0x36 进入空闲状态
	CC2500_Write_Com(CCxxx0_SFRX);
	CC2500_Write_Com(CCxxx0_SRX);		//0x34 开接收
}

/****************************************
* 函 数 名：CC2500_Burst_Write_Reg()		*
* 功能描述：连续写数据到指定地址			*
* 入口参数：地址数据、写入数据、写入个数	*
* 返回参数：无							*
****************************************/
void CC2500_Burst_Write_Reg(unsigned char address,unsigned char *buffer,unsigned char count)
{
	unsigned char i, temp;
	temp = (address|WRITE_BURST);	//0x40
	RF_CS_0;
	while(RF_MISO);
	RF_SPI_Write_Byte(temp);
	for(i=0;i<count;i++)
	{
		RF_SPI_Write_Byte(buffer[i]);
	}
	RF_CS_1;
}

/****************************************
* 函 数 名：void CC2500_RFSend_Pac()		*
* 功能描述：射频发送数据包				*
* 入口参数：数据包、数据长度				*
* 返回参数：无							*
****************************************/
void CC2500_RFSend_Pac(unsigned char *Buffer,unsigned char size) 
{
	PA_TXEN();//开发射PA
	CC2500_Write_Com(CCxxx0_SIDLE);	//0x36 进入空闲状态
	CC2500_Write_Com(CCxxx0_SFTX); 	//清空发送缓存
	CC2500_Burst_Write_Reg(CCxxx0_TXFIFO, Buffer, size);
	CC2500_Write_Com(CCxxx0_STX);
}

/************************************************
* 函 数 名：void CC2500_Burst_Read_Reg(uchar addr*
*	,uchar *buffer, uchar count)				*
* 功能描述：SPI连续读配置寄存器					*
* 入口参数：地址，读出数据后暂存的缓冲区，读出		*
*						配置个数					*
* 返回参数：无									*
************************************************/
void CC2500_Burst_Read_Reg(unsigned char address,unsigned char *buffer,unsigned char count) 
{
	unsigned char i,temp;
	temp = (address|READ_BURST);//连续读命令0xC0
	RF_CS_0;
	while(RF_MISO);
	RF_SPI_Write_Byte(temp); 
	for(i=0;i<count;i++) 
	{
		buffer[i] = RF_SPI_Read_Byte();
	}
	RF_CS_1;
}
