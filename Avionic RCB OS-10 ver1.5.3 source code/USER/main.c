/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "time.h"
#include "spi.h"
#include "lcd.h"
#include "Ext_Flash.h"
#include "ff.h"
#include "mass_mal.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "usb_istr.h"
#include "page.h"
#include "adc.h"
#include "beep.h"
#include "key.h"
#include "rf_spi.h"
#include "cc2500.h"
#include "cfg.h"
#include "ppm_decode.h"
#include "Tx.h"

CSTR StickNotCal[]={"Ò¡¸ËÎ´Ð£×¼!","calibrate sticks"};
CSTR CloseIdleMode[]={"Çë¹Ø±ÕÌØ¼¼Ä£Ê½!","idle-up warning"};
CSTR ThrNotReset[]={"ÓÍÃÅÒ¡¸ËÎ´¸´Î»!","throttle warning"};

//ÏµÍ³ÖÐ¶Ï¹ÜÀí
void NVIC_Configuration(void)
{ 
	/* 2 bit for pre-emption priority, 2 bits for subpriority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/*
		ÖÐ¶ÏÔ´£º
				1£ºÏµÍ³¶¨Ê±Æ÷ÖÐ¶Ï 					0----0º
				2£ºPPMÊäÈëÍâ²¿ÖÐ¶Ï¡¢Rx½ÓÊÕÍâ²¿ÖÐ¶Ï 	0----1
				3£ºTx·¢Éä¶¨Ê±ÖÐ¶Ï	  				1----0
				4: Tx´¦Àí¶¨Ê±ÖÐ¶Ï					2----0
	*/
}

void GPIO_Comfiguration(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//½«PA15¡¢PB3¡¢PB4ÖØÓ³ÉäÎªÆÕÍ¨IO
}

u8 ParamLoaded;

void PowerManager(void)
{
	static u16 last_tx_v = 0;
	static u16 power_div = 0;
	
	if(power_div++<10)	return;//5*10=50ms
	power_div = 0;
	
	TxBatVol = ADC_AvgValue[AD_BAT]*0.4937;//µ¥Î»0.01V
	
	if(TxBatVol==0)	return;
	if(ParamLoaded==0)	return;
	
	if(last_tx_v/TxBatVol>=2)
	{
		DelayMs(80);
		__disable_irq();//¹Ø±Õ×ÜÖÐ¶Ï
		Beep_Set_Vibration(0);//¹Ø±ÕÕð¶¯µç»ú
		
		ModelSave(TxSys.ModelNo);
		SaveCfg(); 
		while(1);
	}
	else
	{
		last_tx_v = TxBatVol;
	}
}

//ÏµÍ³´¦Àí
void SysProc(void)
{
	KeyScanHandler(); 
	BeepHandler(); 
	BatteryHandler();			
	TimerProc();
	TrimProc();
	PpmInCtl(Model.PpmIn);
	RxK8taUnpacking();
	LCDHandler();
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	FATFS fs;//Âß¼­´ÅÅÌ¹¤×÷Çø
	FRESULT res;
	u16 i;
	char *msg = 0;
	
	NVIC_Configuration();
	GPIO_Comfiguration();
	ADC1_DMA_Configuration();
	Time_Init();//±ØÐëÔÚLCDÖ®Ç°³õÊ¼»¯
	DelayMs(500);
	Key_Configuration();
	Beep_Cofiguration();
	SPI2_Configuration();//³õÊ¼»¯SST25¡¢LCDµÄ½Ó¿Ú
	SST25_Init();
	LCD_Init();
	RF_SPI_Configuration();
	TxTimerCfg();
	TxProcTimerInit();
	PpmInCfg();
	
	res = f_mount(0,&fs);//¹ÒÔØSPI Flash
	Mass_Memory_Size[0]=2*1024*1024;//FLASH ´óÐ¡Îª2M×Ö½Ú
    Mass_Block_Size[0] =512;//ÒòÎªÎÒÃÇÔÚInitÀïÃæÉèÖÃÁËSD¿¨µÄ²Ù×÷×Ö½ÚÎª512¸ö,ËùÒÔÕâÀïÒ»¶¨ÊÇ512¸ö×Ö½Ú.
    Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
	
	if((res!=FR_OK)||(Key_ScanOnce()==KEY_EXT))//ÎÄ¼þÏµÍ³³ö´í»ò°´×¡EXT¿ª»ú
	{
		LCD_Clr_All();
		LcdDrawUsbLogo(0,4);
		LCD_Refresh_Frame();
		LCD_Refresh_Frame();
		LCD_Refresh_Frame();
		LCD_Refresh_Frame();
		LCD_SetBL(16);
		USB_Interrupts_Config();    
		Set_USBClock();   
		USB_Init();	
		
		while(1);
	}
	 
	LoadCfg();//¼ÓÔØÒ£¿ØÆ÷²ÎÊý
	ModelLoad(TxSys.ModelNo,0);//¼ÓÔØÄ£ÐÍÊý¾Ý,²»³õÊ¼»¯Í¨ÐÅ¹¦ÄÜ
	ParamLoaded = 1;//²ÎÊýÒÑ¼ÓÔØÍê±Ï
	BeepMusic(MusicStartup);
	LCD_Disp_BMP(0,0,"res/logo.bmp");
	for(i=0;i<2000;i++)
	{
		SysProc();
	}
	//¶¨Ê±Æ÷¸´Î»
	TxTimer.Reset=1;

	//Ò¡¸ËÐ£×¼¼ì²â
	if(TxSys.StkCalied==0)	PageAlertModel(StickNotCal[TxSys.Language],0);
	
	if(SwTstZYSState()==ZYS_LEFT)
	{
		if(!(TxSys.StkType%2))//0/2
		{
			TxSys.StkType = 1;//Ä£Ê½2
		}
	}
	else
	{
		if(TxSys.StkType%2)//1/3
		{
			TxSys.StkType = 0;//Ä£Ê½1
		}
	}
	   	
	//¼ì²âÓÍÃÅºÍ¿ª¹Ø
	do{
		msg = 0;
		if((FlyMode==FM_IDL1)||(FlyMode==FM_IDL2))	msg = (char *)CloseIdleMode[TxSys.Language];
		else if(TxSys.StkCalied==1 && StickValue[2]>THRSAFE)	msg = (char *)ThrNotReset[TxSys.Language];
		PageAlertModel(msg,100);
	}while(msg);
	
	//ÖØÐÂ¼ÓÔØÄ£ÐÍÊý¾Ý,ÎªÁËÊ¼»¯Í¨ÐÅ¹¦ÄÜ
	ModelLoad(TxSys.ModelNo,1);
	
	PageSet(PageMain,PV_INIT);

	while(1)
	{
		SysProc();
		PageStack[PageStackIdx](PV_RUN);
	}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
