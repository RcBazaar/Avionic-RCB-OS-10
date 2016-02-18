#include "Ext_Flash.h"
#include "spi.h"

u8 SPI_FLASH_BUF[4096];//4KB��д������

void SST25_Write_Enable(void)//ʹ��WEL=1,дʹ��
{
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_WREN);//0x06
	SPI_FLASH_CSN_1;
}

void SST25_Write_Disable(void)//ʹ��WEL=0,д��ֹ
{
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_WRDI);//0x04
	SPI_FLASH_CSN_1;
}

u32 SST25_Read_ID(void)//��оƬID
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

u8 SST25_Read_Status(void)//��״̬
{
	u8 status;
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_RDSR);//0x05
	status = SPI2_Read_Byte();
	SPI_FLASH_CSN_1;
	return status;
}

void SST25_Write_Status(u8 status)//д״̬
{
	SST25_Write_Enable();//ʹ��WEL=1,дʹ��
	
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_EWSR);//0x50
	SPI_FLASH_CSN_1;
	
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_WRSR);//0x01
	SPI2_Write_Byte(status);
	SPI_FLASH_CSN_1;
	
	while(SST25_Read_Status()&0x01);//�ȴ��������
}

void SST25_Unlock(void)//����flash
{
	SST25_Write_Status(0x00);
}

void SST25_Init(void)
{
	SST25_Unlock();
	while(SST25_Read_ID()!=0x00BF2541);//��ⲻ��SPI Flash
}

void SST25_Sector_Erase(u32	addr)//��������
{
	SST25_Write_Enable();//ʹ��WEL=1,дʹ��
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_SE);
	SPI2_Write_Byte((addr&0xFF0000)>>16);
	SPI2_Write_Byte((addr&0xFF00)>>8);
	SPI2_Write_Byte(addr&0xFF);
	SPI_FLASH_CSN_1;
	while(SST25_Read_Status()&0x01);//�ȴ��������
}

void SST25_Block_Erase(u32 addr)//�����
{
	SST25_Write_Enable();//ʹ��WEL=1,дʹ��
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_BE32);
	SPI2_Write_Byte((addr&0xFF0000)>>16);
	SPI2_Write_Byte((addr&0xFF00)>>8);
	SPI2_Write_Byte(addr&0xFF);
	SPI_FLASH_CSN_1;
	while(SST25_Read_Status()&0x01);//�ȴ��������
}

void SST25_Chip_Erase(void)//оƬ����
{
	SST25_Write_Enable();//ʹ��WEL=1,дʹ��
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_CE);
	SPI_FLASH_CSN_1;
	while(SST25_Read_Status()&0x01);//�ȴ��������
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
			while(SST25_Read_Status()&0x01);//�ȴ��������
		}
		else
		{
			SPI_FLASH_CSN_0;
			SPI2_Write_Byte(COM_AAIP);//0xAD
			SPI2_Write_Byte(pBuf[2*i]);
			SPI2_Write_Byte(pBuf[2*i+1]);
			SPI_FLASH_CSN_1;
			while(SST25_Read_Status()&0x01);//�ȴ��������
		}
	}
	SST25_Write_Disable();
	
	if(siglebyte_amount)//ʣ�൥���ֽ�Ҫд
	{
		addr += 2*halfword_amount;
		SST25_Write_Enable();
		SPI_FLASH_CSN_0;
		SPI2_Write_Byte(COM_BP);//0x02
		SPI2_Write_Byte((addr&0xFF0000)>>16);
		SPI2_Write_Byte((addr&0xFF00)>>8);
		SPI2_Write_Byte(addr&0xFF);
		SPI2_Write_Byte(pBuf[length-1]);//д�����һ���ֽ�
		SPI_FLASH_CSN_1;
		while(SST25_Read_Status()&0x01);//�ȴ��������
		SST25_Write_Disable();
	}
}

void SST25_Read_Data(u32 addr,u8 *pBuf,u16 length)//������,�������25M
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

void SST25_FRead_Data(u32 addr,u8 *pBuf,u16 length)//������,�������80M
{
	u16 i;
	
	SPI_FLASH_CSN_0;
	SPI2_Write_Byte(COM_FREAD);//0x0B
	SPI2_Write_Byte((addr&0xFF0000)>>16);
	SPI2_Write_Byte((addr&0xFF00)>>8);
	SPI2_Write_Byte(addr&0xFF);
	SPI2_Write_Byte(0xFF);//�ղ���
	for(i=0;i<length;i++)
	{
		pBuf[i] = SPI2_Read_Byte();
	}
	SPI_FLASH_CSN_1;
}

//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void SPI_Flash_Write_NoCheck(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//��������ҳ
	while(1)
	{	   
		SST25_Write_Data(WriteAddr,pBuffer,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	}	    
} 

//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)  		   
void SPI_Flash_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=WriteAddr/4096;//������ַ 0~511
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		SST25_Read_Data(secpos*4096,SPI_FLASH_BUF,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			SST25_Sector_Erase(secpos*4096);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			SPI_Flash_Write_NoCheck(secpos*4096,SPI_FLASH_BUF,4096);//д����������  

		}else SPI_Flash_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	} 	 
}

