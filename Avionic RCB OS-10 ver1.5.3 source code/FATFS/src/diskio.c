#include "Ext_Flash.h"
#include "spi.h"
#include "diskio.h"	 		   

#define EX_FLASH	0  //SPI Flash,���Ϊ0 

#define FLASH_SECTOR_SIZE 	512			   			    
#define FLASH_BLOCK_SIZE  	8     //ÿ��BLOCK��8������
u16	    FLASH_SECTOR_COUNT=2048*2;//2M�ֽ�

//��ʼ������
DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{	
	u8 res=0;	    
	switch(drv)
	{
		case EX_FLASH://SPI_Flash
  			break;
		default:
			res=1; 
	}		 
	if(res)return  STA_NOINIT;
	else return 0; //��ʼ���ɹ�
}   
//��ô���״̬
DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{		   
    return 0;
}
 //������
 //drv:���̱��0~9
 //*buff:���ݽ��ջ����׵�ַ
 //sector:������ַ
 //count:��Ҫ��ȡ��������
DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	u8 res=0; 
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
	switch(drv)
	{
		case EX_FLASH://SPI_Flash
			for(;count>0;count--)
			{
				SST25_FRead_Data(sector*FLASH_SECTOR_SIZE,buff,FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
   //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res==0x00)return RES_OK;	 
    else return RES_ERROR;	   
}  
 //д����
 //drv:���̱��0~9
 //*buff:���������׵�ַ
 //sector:������ַ
 //count:��Ҫд���������	    
#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	        /* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	u8 res=0;  
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
	switch(drv)
	{
		case EX_FLASH://SPI_Flash
			for(;count>0;count--)
			{										    
				SPI_Flash_Write(sector*FLASH_SECTOR_SIZE,(u8*)buff,FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
    //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res == 0x00)return RES_OK;	 
    else return RES_ERROR;		 
}
#endif /* _READONLY */

//����������Ļ��
 //drv:���̱��0~9
 //ctrl:���ƴ���
 //*buff:����/���ջ�����ָ��
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{	
	DRESULT res;						  			     
	if(drv==EX_FLASH)//SPI_Flash
	{
	    switch(ctrl)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = FLASH_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = FLASH_BLOCK_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = FLASH_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}else res=RES_ERROR;//�����Ĳ�֧��
    return res;
}   
//���ʱ��
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */    
DWORD get_fattime (void)
{				 
	return 0x44C90000;
}	
