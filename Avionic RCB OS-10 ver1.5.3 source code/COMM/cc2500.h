#ifndef _CC2500_H_
#define _CC2500_H_

//寄存器控制命令定义
#define CCxxx0_IOCFG2			0x00	// GDO2输出脚配置
#define CCxxx0_IOCFG1			0x01	// GDO1输出脚配置
#define CCxxx0_IOCFG0			0x02	// GDO0输出脚配置
#define CCxxx0_FIFOTHR			0x03	// RX FIFO 和TX FIFO门限
#define CCxxx0_SYNC1			0x04	// 同步字，高字节
#define CCxxx0_SYNC0			0x05	// 同步字，低字节
#define CCxxx0_PKTLEN			0x06	// 数据包长度
#define CCxxx0_PKTCTRL1			0x07	// 数据包自动控制
#define CCxxx0_PKTCTRL0			0x08	// 数据包自动控制
#define CCxxx0_ADDR				0x09	// 设备地址
#define CCxxx0_CHANNR			0x0A	// 信道数
#define CCxxx0_FSCTRL1			0x0B	// 频率合成器控制
#define CCxxx0_FSCTRL0			0x0C	// 频率合成器控制
#define CCxxx0_FREQ2			0x0D	// 频率控制字高字节
#define CCxxx0_FREQ1			0x0E	// 频率控制字中间字节
#define CCxxx0_FREQ0			0x0F	// 频率控制字低字节
#define CCxxx0_MDMCFG4			0x10	// 调制器配置
#define CCxxx0_MDMCFG3			0x11	// 调制器配置
#define CCxxx0_MDMCFG2			0x12	// 调制器配置
#define CCxxx0_MDMCFG1			0x13	// 调制器配置
#define CCxxx0_MDMCFG0			0x14	// 调制器配置
#define CCxxx0_DEVIATN			0x15	// 调制器背离设置
#define CCxxx0_MCSM2			0x16	// 主通信控制状态机配置
#define CCxxx0_MCSM1			0x17	// 主通信控制状态机配置
#define CCxxx0_MCSM0			0x18	// 主通信控制状态机配置
#define CCxxx0_FOCCFG			0x19	// 频率偏移补偿配置
#define CCxxx0_BSCFG			0x1A	// 位同步配置
#define CCxxx0_AGCCTRL2			0x1B	// AGC控制
#define CCxxx0_AGCCTRL1			0x1C	// AGC控制
#define CCxxx0_AGCCTRL0			0x1D	// AGC控制
#define CCxxx0_WOREVT1			0x1E	// 高字节时间0暂停
#define CCxxx0_WOREVT0			0x1F	// 低字节时间0暂停
#define CCxxx0_WORCTRL			0x20	// 电磁波激活控制
#define CCxxx0_FREND1			0x21	// 前末端RX配置
#define CCxxx0_FREND0			0x22	// 前末端TX配置
#define CCxxx0_FSCAL3			0x23	// 频率合成器校准
#define CCxxx0_FSCAL2			0x24	// 频率合成器校准
#define CCxxx0_FSCAL1			0x25	// 频率合成器校准
#define CCxxx0_FSCAL0			0x26	// 频率合成器校准
#define CCxxx0_RCCTRL1			0x27	// RC振荡器配置
#define CCxxx0_RCCTRL0			0x28	// RC振荡器配置
#define CCxxx0_FSTEST			0x29	// 频率合成器校准控制
#define CCxxx0_PTEST			0x2A	// 产品测试
#define CCxxx0_AGCTEST			0x2B	// AGC测试
#define CCxxx0_TEST2			0x2C	// 不同的测试设置
#define CCxxx0_TEST1			0x2D	// 不同的测试设置
#define CCxxx0_TEST0			0x2E	// 不同的测试设置

//命令滤波
#define CCxxx0_SRES				0x30	// 重启芯片
#define CCxxx0_SFSTXON			0x31	// 开启和校准频率合成器(若MCSM0.FS_AUTOCAL=1)
#define CCxxx0_SXOFF			0x32	// 关闭晶体振荡器
#define CCxxx0_SCAL				0x33	//  校准频率合成器并关断（开启快速启动）
#define CCxxx0_SRX				0x34	// 启用RX。若上一状态为空闲且MCSM0.FS_AUTOCAL=1则首先运行校准
#define CCxxx0_STX				0x35	// 空闲状态：启用TX，若MCSM0.FS_AUTOCAL=1首先运行校准。
										// 若在RX状态且CCA启用，若信道为空则进入TX
#define CCxxx0_SIDLE			0x36	// 离开RX/TX，关断频率合成器并离开电磁波激活模式若可用
#define CCxxx0_SAFC				0x37	//  运行23.1节列出的频率合成器的AFC调节
#define CCxxx0_SWOR				0x38	//  运行28.5节描述的自动RX选举序列（电磁波激活）
#define CCxxx0_SPWD				0x39	// 当CSn为高时进入功率降低模式
#define CCxxx0_SFRX				0x3A	// 清RX FIFO缓冲
#define CCxxx0_SFTX				0x3B	// 清TX FIFO缓冲
#define CCxxx0_SWORRST			0x3C	// 重新设置真实时间时钟
#define CCxxx0_SNOP				0x3D	// 无操作。可用来为更简单的 件将滤波命令变为2字节
#define CCxxx0_PARTNUM			0x30	// CC2500的组成部分数目
#define CCxxx0_VERSION			0x31	//当前版本号
#define CCxxx0_FREQEST			0x32	//频率偏移估计
#define CCxxx0_LQI				0x33	//连接质量的解调器估计
#define CCxxx0_RSSI				0x34	//接收信号强度指示
#define CCxxx0_MARCSTATE		0x35	//控制状态机状态
#define CCxxx0_WORTIME1			0x36	//WOR计时器高字节
#define CCxxx0_WORTIME0			0x37	//WOR计时器低字节
#define CCxxx0_PKTSTATUS		0x38	//当前GDOx状态的数据包状态
#define CCxxx0_VCO_VC_DAC		0x39	//PLL校准模块的当前设定
#define CCxxx0_TXBYTES			0x3A	//TX FIFO中的下溢和比特数
#define CCxxx0_RXBYTES			0x3B	//RX FIFO中的下溢和比特数
#define CCxxx0_RCCTRL1_STATUS	0x3C
#define CCxxx0_RCCTRL0_STATUS	0x3D

#define CCxxx0_PATABLE			0x3E	//发射功率
#define CCxxx0_TXFIFO			0x3F
#define CCxxx0_RXFIFO			0x3F

//标志定义
#define	WRITE_BURST				0x40	//连续写入
#define	READ_SINGLE 			0x80	//读单字节
#define	READ_BURST     			0xC0	//连续读
#define	BYTES_IN_RXFIFO			0x7F	//接收缓冲区的有效字节数
#define BYTES_IN_TXFIFO			0x7F	//发送缓冲区的有效字节数
#define	CRC_OK            		0x80	//CRC校验通过位标志
#define	Range_TEST				0xC6	//Range test PATABLE value

//开启发射PA
void PA_TXEN(void);

//开启接收PA
void PA_RXEN(void);

//关闭PA
void PA_OFF(void);

//写CC2500配置寄存器
void CC2500_Write_Reg(unsigned char adress,unsigned char data);

//读CC2500寄存器
unsigned char CC2500_Read_Reg(unsigned char adress);

//读CC2500状态寄存器
unsigned char CC2500_Read_Staus(unsigned char adress);

//读取RSSI的值
short int CC2500_Get_RSSI(unsigned char ch);

//写CC2500滤波命令
void CC2500_Write_Com(unsigned char strope);

//CC2500复位函数
void CC2500_Reset(void);

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
void CC2500_Idle_Mode(void);

/****************************************
* 函 数 名：void CC2500_Tx_Mode(void)	*
* 功能描述：配置CC2500工作在发射模式		*
* 函数说明：此函数可使TRW-24C直接从接收	*
*		   模式或空闲模式切回发射模式		*
* 入口参数：无							*
* 返回参数：无							*
****************************************/
void CC2500_Tx_Mode(void);

/****************************************
* 函 数 名：void CC2500_Rx_Mode(void)	*
* 功能描述：配置CC2500工作在接收模式		*
* 函数说明：此函数可使CC2500直接从发射	*
*		   模式切回空闲模式或接收模式		*
* 入口参数：无							*
* 返回参数：无							*
****************************************/
void CC2500_Rx_Mode(void);

/****************************************
* 函 数 名：CC2500_Burst_Write_Reg()		*
* 功能描述：连续写数据到指定地址			*
* 入口参数：地址数据、写入数据、写入个数	*
* 返回参数：无							*
****************************************/
void CC2500_Burst_Write_Reg(unsigned char address,unsigned char *buffer,unsigned char count);

/****************************************
* 函 数 名：void CC2500_RFSend_Pac()		*
* 功能描述：射频发送数据包				*
* 入口参数：数据包、数据长度				*
* 返回参数：无							*
****************************************/
void CC2500_RFSend_Pac(unsigned char *Buffer,unsigned char size);

/************************************************
* 函 数 名：void CC2500_Burst_Read_Reg(uchar addr*
*	,uchar *buffer, uchar count)				*
* 功能描述：SPI连续读配置寄存器					*
* 入口参数：地址，读出数据后暂存的缓冲区，读出		*
*						配置个数					*
* 返回参数：无									*
************************************************/
void CC2500_Burst_Read_Reg(unsigned char address,unsigned char *buffer,unsigned char count); 

#endif




