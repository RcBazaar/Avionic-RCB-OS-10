#include "Ext_Flash.h"
#include "spi.h"

u8 SPI_FLASH_BUF[4096];//4KB读写缓冲区

void SST25_Write_Enable(void)//使得WEL=1,写使能
{
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_WREN);//0x06
	SPI_FLASH_CSN_1;
}

void SST25_Write_Disable(void)//使得WEL=0,写禁止
{
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_WRDI);//0x04
	SPI_FLASH_CSN_1;
}

u32 SST25_Read_ID(void)//读芯片ID
{
	u32 result = 0;
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_RDID);//0x9F
	result |= ((u32)SPI2_Read_Byte())<<16;
	result |= ((u32)SPI2_Read_Byte())<<8;
	result |= (u32)SPI2_Read_Byte();
	SPI_FLASH_CSN_1;
	return result;
}

u8 SST25_Read_Status(void)//读状态
{
	u8 status;
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_RDSR);//0x05
	status = SPI2_Read_Byte();
	SPI_FLASH_CSN_1;
	return status;
}

void SST25_Write_Status(u8 status)//写状态
{
	SST25_Write_Enable();//使得WEL=1,写使能
	
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_EWSR);//0x50
	SPI_FLASH_CSN_1;
	
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_WRSR);//0x01
	SPI2_Write_Byte(status);
	SPI_FLASH_CSN_1;
	
	while(SST25_Read_Status()&0x01);//等待操作完成
}

void SST25_Unlock(void)//解锁flash
{
	SST25_Write_Status(0x00);
}

void SST25_Init(void)
{
	SST25_Unlock();
	while(SST25_Read_ID()!=0x00BF2541);//检测不到SPI Flash
}

void SST25_Sector_Erase(u32	addr)//扇区擦除
{
	SST25_Write_Enable();//使得WEL=1,写使能
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_SE);
	SPI2_Write_Byte((addr&0xFF0000)>>16);
	SPI2_Write_Byte((addr&0xFF00)>>8);
	SPI2_Write_Byte(addr&0xFF);
	SPI_FLASH_CSN_1;
	while(SST25_Read_Status()&0x01);//等待操作完成
}

void SST25_Block_Erase(u32 addr)//块擦除
{
	SST25_Write_Enable();//使得WEL=1,写使能
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_BE32);
	SPI2_Write_Byte((addr&0xFF0000)>>16);
	SPI2_Write_Byte((addr&0xFF00)>>8);
	SPI2_Write_Byte(addr&0xFF);
	SPI_FLASH_CSN_1;
	while(SST25_Read_Status()&0x01);//等待操作完成
}

void SST25_Chip_Erase(void)//芯片擦除
{
	SST25_Write_Enable();//使得WEL=1,写使能
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_CE);
	SPI_FLASH_CSN_1;
	while(SST25_Read_Status()&0x01);//等待操作完成
}

void SST25_Write_Data(u32 addr,u8 *pBuf,u16 length)
{
	u16 i;
	u16 halfword_amount,siglebyte_amount;
	
	halfword_amount = length/2;
	siglebyte_amount = length%2;
	
	SST25_Write_Enable();
	for(i=0;i<halfword_amount;i++)
	{
		if(i==0)
		{
			SPI_FLASH_CSN_0;
			SPI2_Write_Byte(COM_AAIP);//0xAD
			SPI2_Write_Byte((addr&0xFF0000)>>16);
			SPI2_Write_Byte((addr&0xFF00)>>8);
			SPI2_Write_Byte(addr&0xFF);
			SPI2_Write_Byte(pBuf[2*i]);
			SPI2_Write_Byte(pBuf[2*i+1]);
			SPI_FLASH_CSN_1;
			while(SST25_Read_Status()&0x01);//等待操作完成
		}
		else
		{
			SPI_FLASH_CSN_0;
			SPI2_Write_Byte(COM_AAIP);//0xAD
			SPI2_Write_Byte(pBuf[2*i]);
			SPI2_Write_Byte(pBuf[2*i+1]);
			SPI_FLASH_CSN_1;
			while(SST25_Read_Status()&0x01);//等待操作完成
		}
	}
	SST25_Write_Disable();
	
	if(siglebyte_amount)//剩余单个字节要写
	{
		addr += 2*halfword_amount;
		SST25_Write_Enable();
		SPI_FLASH_CSN_0;
		SPI2_Write_Byte(COM_BP);//0x02
		SPI2_Write_Byte((addr&0xFF0000)>>16);
		SPI2_Write_Byte((addr&0xFF00)>>8);
		SPI2_Write_Byte(addr&0xFF);
		SPI2_Write_Byte(pBuf[length-1]);//写入最后一个字节
		SPI_FLASH_CSN_1;
		while(SST25_Read_Status()&0x01);//等待操作完成
		SST25_Write_Disable();
	}
}

void SST25_Read_Data(u32 addr,u8 *pBuf,u16 length)//读数据,最大速率25M
{
	u16 i;
	
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_READ);//0x03
	SPI2_Write_Byte((addr&0xFF0000)>>16);
	SPI2_Write_Byte((addr&0xFF00)>>8);
	SPI2_Write_Byte(addr&0xFF);
	for(i=0;i<length;i++)
	{
		pBuf[i] = SPI2_Read_Byte();
	}
	SPI_FLASH_CSN_1;
}

void SST25_FRead_Data(u32 addr,u8 *pBuf,u16 length)//读数据,最大速率80M
{
	u16 i;
	
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_FREAD);//0x0B
	SPI2_Write_Byte((addr&0xFF0000)>>16);
	SPI2_Write_Byte((addr&0xFF00)>>8);
	SPI2_Write_Byte(addr&0xFF);
	SPI2_Write_Byte(0xFF);//空操作
	for(i=0;i<length;i++)
	{
		pBuf[i] = SPI2_Read_Byte();
	}
	SPI_FLASH_CSN_1;
}

//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void SPI_Flash_Write_NoCheck(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不超过该页
	while(1)
	{	   
		SST25_Write_Data(WriteAddr,pBuffer,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	}	    
} 

//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)  		   
void SPI_Flash_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=WriteAddr/4096;//扇区地址 0~511
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		SST25_Read_Data(secpos*4096,SPI_FLASH_BUF,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			SST25_Sector_Erase(secpos*4096);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			SPI_Flash_Write_NoCheck(secpos*4096,SPI_FLASH_BUF,4096);//写入整个扇区  

		}else SPI_Flash_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	} 	 
}

