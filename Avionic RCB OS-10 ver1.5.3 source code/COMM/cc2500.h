#ifndef _CC2500_H_
#define _CC2500_H_

//�Ĵ������������
#define CCxxx0_IOCFG2			0x00	// GDO2���������
#define CCxxx0_IOCFG1			0x01	// GDO1���������
#define CCxxx0_IOCFG0			0x02	// GDO0���������
#define CCxxx0_FIFOTHR			0x03	// RX FIFO ��TX FIFO����
#define CCxxx0_SYNC1			0x04	// ͬ���֣����ֽ�
#define CCxxx0_SYNC0			0x05	// ͬ���֣����ֽ�
#define CCxxx0_PKTLEN			0x06	// ���ݰ�����
#define CCxxx0_PKTCTRL1			0x07	// ���ݰ��Զ�����
#define CCxxx0_PKTCTRL0			0x08	// ���ݰ��Զ�����
#define CCxxx0_ADDR				0x09	// �豸��ַ
#define CCxxx0_CHANNR			0x0A	// �ŵ���
#define CCxxx0_FSCTRL1			0x0B	// Ƶ�ʺϳ�������
#define CCxxx0_FSCTRL0			0x0C	// Ƶ�ʺϳ�������
#define CCxxx0_FREQ2			0x0D	// Ƶ�ʿ����ָ��ֽ�
#define CCxxx0_FREQ1			0x0E	// Ƶ�ʿ������м��ֽ�
#define CCxxx0_FREQ0			0x0F	// Ƶ�ʿ����ֵ��ֽ�
#define CCxxx0_MDMCFG4			0x10	// ����������
#define CCxxx0_MDMCFG3			0x11	// ����������
#define CCxxx0_MDMCFG2			0x12	// ����������
#define CCxxx0_MDMCFG1			0x13	// ����������
#define CCxxx0_MDMCFG0			0x14	// ����������
#define CCxxx0_DEVIATN			0x15	// ��������������
#define CCxxx0_MCSM2			0x16	// ��ͨ�ſ���״̬������
#define CCxxx0_MCSM1			0x17	// ��ͨ�ſ���״̬������
#define CCxxx0_MCSM0			0x18	// ��ͨ�ſ���״̬������
#define CCxxx0_FOCCFG			0x19	// Ƶ��ƫ�Ʋ�������
#define CCxxx0_BSCFG			0x1A	// λͬ������
#define CCxxx0_AGCCTRL2			0x1B	// AGC����
#define CCxxx0_AGCCTRL1			0x1C	// AGC����
#define CCxxx0_AGCCTRL0			0x1D	// AGC����
#define CCxxx0_WOREVT1			0x1E	// ���ֽ�ʱ��0��ͣ
#define CCxxx0_WOREVT0			0x1F	// ���ֽ�ʱ��0��ͣ
#define CCxxx0_WORCTRL			0x20	// ��Ų��������
#define CCxxx0_FREND1			0x21	// ǰĩ��RX����
#define CCxxx0_FREND0			0x22	// ǰĩ��TX����
#define CCxxx0_FSCAL3			0x23	// Ƶ�ʺϳ���У׼
#define CCxxx0_FSCAL2			0x24	// Ƶ�ʺϳ���У׼
#define CCxxx0_FSCAL1			0x25	// Ƶ�ʺϳ���У׼
#define CCxxx0_FSCAL0			0x26	// Ƶ�ʺϳ���У׼
#define CCxxx0_RCCTRL1			0x27	// RC��������
#define CCxxx0_RCCTRL0			0x28	// RC��������
#define CCxxx0_FSTEST			0x29	// Ƶ�ʺϳ���У׼����
#define CCxxx0_PTEST			0x2A	// ��Ʒ����
#define CCxxx0_AGCTEST			0x2B	// AGC����
#define CCxxx0_TEST2			0x2C	// ��ͬ�Ĳ�������
#define CCxxx0_TEST1			0x2D	// ��ͬ�Ĳ�������
#define CCxxx0_TEST0			0x2E	// ��ͬ�Ĳ�������

//�����˲�
#define CCxxx0_SRES				0x30	// ����оƬ
#define CCxxx0_SFSTXON			0x31	// ������У׼Ƶ�ʺϳ���(��MCSM0.FS_AUTOCAL=1)
#define CCxxx0_SXOFF			0x32	// �رվ�������
#define CCxxx0_SCAL				0x33	//  У׼Ƶ�ʺϳ������ضϣ���������������
#define CCxxx0_SRX				0x34	// ����RX������һ״̬Ϊ������MCSM0.FS_AUTOCAL=1����������У׼
#define CCxxx0_STX				0x35	// ����״̬������TX����MCSM0.FS_AUTOCAL=1��������У׼��
										// ����RX״̬��CCA���ã����ŵ�Ϊ�������TX
#define CCxxx0_SIDLE			0x36	// �뿪RX/TX���ض�Ƶ�ʺϳ������뿪��Ų�����ģʽ������
#define CCxxx0_SAFC				0x37	//  ����23.1���г���Ƶ�ʺϳ�����AFC����
#define CCxxx0_SWOR				0x38	//  ����28.5���������Զ�RXѡ�����У���Ų����
#define CCxxx0_SPWD				0x39	// ��CSnΪ��ʱ���빦�ʽ���ģʽ
#define CCxxx0_SFRX				0x3A	// ��RX FIFO����
#define CCxxx0_SFTX				0x3B	// ��TX FIFO����
#define CCxxx0_SWORRST			0x3C	// ����������ʵʱ��ʱ��
#define CCxxx0_SNOP				0x3D	// �޲�����������Ϊ���򵥵� �����˲������Ϊ2�ֽ�
#define CCxxx0_PARTNUM			0x30	// CC2500����ɲ�����Ŀ
#define CCxxx0_VERSION			0x31	//��ǰ�汾��
#define CCxxx0_FREQEST			0x32	//Ƶ��ƫ�ƹ���
#define CCxxx0_LQI				0x33	//���������Ľ��������
#define CCxxx0_RSSI				0x34	//�����ź�ǿ��ָʾ
#define CCxxx0_MARCSTATE		0x35	//����״̬��״̬
#define CCxxx0_WORTIME1			0x36	//WOR��ʱ�����ֽ�
#define CCxxx0_WORTIME0			0x37	//WOR��ʱ�����ֽ�
#define CCxxx0_PKTSTATUS		0x38	//��ǰGDOx״̬�����ݰ�״̬
#define CCxxx0_VCO_VC_DAC		0x39	//PLLУ׼ģ��ĵ�ǰ�趨
#define CCxxx0_TXBYTES			0x3A	//TX FIFO�е�����ͱ�����
#define CCxxx0_RXBYTES			0x3B	//RX FIFO�е�����ͱ�����
#define CCxxx0_RCCTRL1_STATUS	0x3C
#define CCxxx0_RCCTRL0_STATUS	0x3D

#define CCxxx0_PATABLE			0x3E	//���书��
#define CCxxx0_TXFIFO			0x3F
#define CCxxx0_RXFIFO			0x3F

//��־����
#define	WRITE_BURST				0x40	//����д��
#define	READ_SINGLE 			0x80	//�����ֽ�
#define	READ_BURST     			0xC0	//������
#define	BYTES_IN_RXFIFO			0x7F	//���ջ���������Ч�ֽ���
#define BYTES_IN_TXFIFO			0x7F	//���ͻ���������Ч�ֽ���
#define	CRC_OK            		0x80	//CRCУ��ͨ��λ��־
#define	Range_TEST				0xC6	//Range test PATABLE value

//��������PA
void PA_TXEN(void);

//��������PA
void PA_RXEN(void);

//�ر�PA
void PA_OFF(void);

//дCC2500���üĴ���
void CC2500_Write_Reg(unsigned char adress,unsigned char data);

//��CC2500�Ĵ���
unsigned char CC2500_Read_Reg(unsigned char adress);

//��CC2500״̬�Ĵ���
unsigned char CC2500_Read_Staus(unsigned char adress);

//��ȡRSSI��ֵ
short int CC2500_Get_RSSI(unsigned char ch);

//дCC2500�˲�����
void CC2500_Write_Com(unsigned char strope);

//CC2500��λ����
void CC2500_Reset(void);

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
void CC2500_Idle_Mode(void);

/****************************************
* �� �� ����void CC2500_Tx_Mode(void)	*
* ��������������CC2500�����ڷ���ģʽ		*
* ����˵�����˺�����ʹTRW-24Cֱ�Ӵӽ���	*
*		   ģʽ�����ģʽ�лط���ģʽ		*
* ��ڲ�������							*
* ���ز�������							*
****************************************/
void CC2500_Tx_Mode(void);

/****************************************
* �� �� ����void CC2500_Rx_Mode(void)	*
* ��������������CC2500�����ڽ���ģʽ		*
* ����˵�����˺�����ʹCC2500ֱ�Ӵӷ���	*
*		   ģʽ�лؿ���ģʽ�����ģʽ		*
* ��ڲ�������							*
* ���ز�������							*
****************************************/
void CC2500_Rx_Mode(void);

/****************************************
* �� �� ����CC2500_Burst_Write_Reg()		*
* ��������������д���ݵ�ָ����ַ			*
* ��ڲ�������ַ���ݡ�д�����ݡ�д�����	*
* ���ز�������							*
****************************************/
void CC2500_Burst_Write_Reg(unsigned char address,unsigned char *buffer,unsigned char count);

/****************************************
* �� �� ����void CC2500_RFSend_Pac()		*
* ������������Ƶ�������ݰ�				*
* ��ڲ��������ݰ������ݳ���				*
* ���ز�������							*
****************************************/
void CC2500_RFSend_Pac(unsigned char *Buffer,unsigned char size);

/************************************************
* �� �� ����void CC2500_Burst_Read_Reg(uchar addr*
*	,uchar *buffer, uchar count)				*
* ����������SPI���������üĴ���					*
* ��ڲ�������ַ���������ݺ��ݴ�Ļ�����������		*
*						���ø���					*
* ���ز�������									*
************************************************/
void CC2500_Burst_Read_Reg(unsigned char address,unsigned char *buffer,unsigned char count); 

#endif




