/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : mass_mal.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Medium Access Layer interface
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"  	 
#include "Ext_Flash.h"
#include "mass_mal.h"
																			   
u32 Mass_Memory_Size[2];
u32 Mass_Block_Size[2];
u32 Mass_Block_Count[2];
																				   
/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
u16 MAL_Init(u8 lun)
{
  u16 status = MAL_OK;  
  switch (lun)
  {
    case 0:			    
      break;			   
    case 1:					  
      break;		  
    default:
      return MAL_FAIL;
  }
  return status;
}
		 
/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : 0,OK
                   1,FAIL
*******************************************************************************/
u16 MAL_Write(u8 lun, u32 Memory_Offset, u32 *Writebuff, u16 Transfer_Length)
{	  
	u8 STA;
	switch (lun)
	{
	case 0:				  
		STA=0;
		SPI_Flash_Write(Memory_Offset,(u8*)Writebuff,Transfer_Length); 		 
		break;							  
	case 1:													   
		break; 
	default:
		return MAL_FAIL;
	}
	if(STA!=0)return MAL_FAIL;
	return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : 0,OK
                   1,FAIL
*******************************************************************************/
u16 MAL_Read(u8 lun, u32 Memory_Offset, u32 *Readbuff, u16 Transfer_Length)
{
	u8 STA;
	switch (lun)
	{
	case 0:		    
		STA=0;
		SST25_FRead_Data(Memory_Offset,(u8*)Readbuff,Transfer_Length);
		break;			    
	case 1:	 
		break;	  
	default:
		return MAL_FAIL;
	}
	if(STA!=0)return MAL_FAIL;
	return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 MAL_GetStatus (u8 lun)
{
    switch(lun)
    {
    case 0:
        return MAL_OK;
    case 1:
        return MAL_OK;
    case 2:
        return MAL_FAIL;
    default:
        return MAL_FAIL;
    }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
