#include "cc2500.h"
#include "rf_spi.h"
#include "time.h"

//��������PA
void PA_TXEN(void)
{
	RF_TXEN_1;
	RF_RXEN_0;
}

//��������PA
void PA_RXEN(void)
{
	RF_TXEN_0;
	RF_RXEN_1;
}

//�ر�PA
void PA_OFF(void)
{
	RF_TXEN_0;
	RF_RXEN_0;
}

//дCC2500���üĴ���
void CC2500_Write_Reg(unsigned char adress,unsigned char data)
{
	RF_CS_0;
	while(RF_MISO);
	RF_SPI_Write_Byte(adress);
	RF_SPI_Write_Byte(data);
	RF_CS_1;
}

//��CC2500���ú����ݼĴ���
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

//��CC2500״̬�Ĵ���
unsigned char CC2500_Read_Staus(unsigned char adress)
{
	unsigned char temp_adress,value;
	temp_adress=(adress|READ_BURST);//״̬�Ĵ�����ַ
	RF_CS_0;
	while(RF_MISO);
	RF_SPI_Write_Byte(temp_adress);
	value=RF_SPI_Read_Byte();
	RF_CS_1;
	return  value;
}

//��ȡRSSI��ֵ
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

//дCC2500�˲�����
void CC2500_Write_Com(unsigned char strope)
{
	RF_CS_0;
	while(RF_MISO);
	RF_SPI_Write_Byte(strope);
	RF_CS_1;
}

//CC2500��λ����
void CC2500_Reset(void)
{
	RF_CS_0; 
	CC2500_Write_Com(CCxxx0_SRES);//д�븴λ����
	RF_CS_1;
}

/************************************
* �� �� ����CC2500-Idle Mode			*
* ��������������CC2500�����ڿ���ģʽ	*
* �����f�����˺�����ʹCC2500ֱ�Ӵӷ�	*
*		   ��ģʽ�����ģʽ�лؿ���	*
*		   ģʽ��Ӧ���ڲ�����գ���	*
*		   Ҫ����ٷ��ط������ՠ�	*
*		   ̬						*
* ��ڲ�������						*
* ���ز�������						*
************************************/
void CC2500_Idle_Mode(void)
{
	CC2500_Write_Com(CCxxx0_SIDLE);		//0x36 �������״̬
}

/****************************************
* �� �� ����void CC2500_Tx_Mode(void)	*
* ��������������CC2500�����ڷ���ģʽ		*
* ����˵�����˺�����ʹTRW-24Cֱ�Ӵӽ���	*
*		   ģʽ�����ģʽ�лط���ģʽ		*
* ��ڲ�������							*
* ���ز�������							*
****************************************/
void CC2500_Tx_Mode(void)
{
	PA_TXEN();//������PA
	CC2500_Write_Com(CCxxx0_SIDLE);		//0x36 �������״̬
	CC2500_Write_Com(CCxxx0_STX);		//0x35 ������
}

/****************************************
* �� �� ����void CC2500_Rx_Mode(void)	*
* ��������������CC2500�����ڽ���ģʽ		*
* ����˵�����˺�����ʹCC2500ֱ�Ӵӷ���	*
*		   ģʽ�лؿ���ģʽ�����ģʽ		*
* ��ڲ�������							*
* ���ز�������							*
****************************************/
void CC2500_Rx_Mode(void)
{
	PA_RXEN();//������PA
	CC2500_Write_Com(CCxxx0_SIDLE);		//0x36 �������״̬
	CC2500_Write_Com(CCxxx0_SFRX);
	CC2500_Write_Com(CCxxx0_SRX);		//0x34 ������
}

/****************************************
* �� �� ����CC2500_Burst_Write_Reg()		*
* ��������������д���ݵ�ָ����ַ			*
* ��ڲ�������ַ���ݡ�д�����ݡ�д�����	*
* ���ز�������							*
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
* �� �� ����void CC2500_RFSend_Pac()		*
* ������������Ƶ�������ݰ�				*
* ��ڲ��������ݰ������ݳ���				*
* ���ز�������							*
****************************************/
void CC2500_RFSend_Pac(unsigned char *Buffer,unsigned char size) 
{
	PA_TXEN();//������PA
	CC2500_Write_Com(CCxxx0_SIDLE);	//0x36 �������״̬
	CC2500_Write_Com(CCxxx0_SFTX); 	//��շ��ͻ���
	CC2500_Burst_Write_Reg(CCxxx0_TXFIFO, Buffer, size);
	CC2500_Write_Com(CCxxx0_STX);
}

/************************************************
* �� �� ����void CC2500_Burst_Read_Reg(uchar addr*
*	,uchar *buffer, uchar count)				*
* ����������SPI���������üĴ���					*
* ��ڲ�������ַ���������ݺ��ݴ�Ļ�����������		*
*						���ø���					*
* ���ز�������									*
************************************************/
void CC2500_Burst_Read_Reg(unsigned char address,unsigned char *buffer,unsigned char count) 
{
	unsigned char i,temp;
	temp = (address|READ_BURST);//����������0xC0
	RF_CS_0;
	while(RF_MISO);
	RF_SPI_Write_Byte(temp); 
	for(i=0;i<count;i++) 
	{
		buffer[i] = RF_SPI_Read_Byte();
	}
	RF_CS_1;
}
