#include "cfg.h"
#include "ff.h"
#include "Page.h"
#include "Tx.h"
#include <string.h>
#include "flash.h"

CSTR LoadDefCfg[]={"使用默认配置...","Default Cfg..."};
CSTR SaveDeviceCfg[]={"保存遥控器配置...","Save Device Cfg..."};
CSTR CanotOpenTXBIN[]={"无法打开TX.BIN!","Canot Open TX.BIN!"};
CSTR CanotWriteTXBIN[]={"无法写入TX.BIN!","Canot Write TX.BIN!"};
CSTR LoadDefModelCfg[]={"使用默认模型配置...","Default Model Cfg..."};
CSTR SaveModelCfg[]={"保存模型参数...","Save Model Cfg..."};
CSTR CanotOpenModelFile[]={"无法打开模型文件!","Canot Open Model!"};
CSTR CanotWriteModelFile[]={"无法写入模型文件","Canot Write Model!"};
CSTR ModelCopied[]={"模型已复制","Model Copied"};

//遥控器配置参数
TXSYS TxSys;
const TXSYS *TxSysBak = (const TXSYS *)PARAM_TXSYS_ADDRESS;
const TXSYS TxSysDef={
	0,	//u8 StkCalied
	0,	//u32 RfId;
	"000000",//char RfIdStr[7];
	1,	//s16 ModelNo;
	2,  //s16 TrimStep;
	0,	//s16 StkType; 
	{	//s16 StkCali[4][3];
		{1700,2048,1700},
		{1700,2048,1700},
		{1700,2048,1700},
		{1700,2048,1700},		
	},
	50,	//s16 StkDa;
	1,	//s16 Vibrator;
	1,	//s16 Music;
	8,	//s16 KeyBeep;
	20,	//s16 KeyTone;
	8,	//s16 Light;
	4,	//s16 Contrast;
	3,	//s16 LightOff;
	0,	//s16 BatTYpe;
	74,	//s16 BatWarn;
	ENGLISH,//s16 Language;
	0,	//u16 BatTime;
	0,	//u16 BatVoltRec;
	0,	//u32 crc
};

//模型参数
MODEL Model;
MODEL ModelBuf;
const MODEL *ModelBak = (MODEL *)PARAM_MODEL_BASE_ADDRESS;
const MODEL ModelDef={
	"PLANE",	//char 	Name[MODELNL+1];
	"MODEL",	//char 	Icon[ICONNL+1];
	0,			//s16		Type;
	{0,0,0,0},	//s8		Trim[4];
	2,			//s16		Protocol=RCB-OS10;
	//0,			//u32		RfId;++++++++++++++++++++++++++++++
	//"000000",	//char		RfIdStr[8];++++++++++++++++++++++++++++++++
	7,			//s16		RfPwr;=100mW
	{0,1,2,3,4,5,6,7,8,9},	//s16		ChMap[10]
	{0,1,2,3,4,5,6,7,8,9},	//s16		TrChMap[10]
	{0,0,-90,0},			//TIMER Timer
	{0,0,0,0,0,0,0,0,0,0},	//s16	Rev[10];
	{						//s16	Epa[10][2];
		{100,+100},
		{100,+100},
		{100,+100},
		{100,+100},
		{100,+100},
		{100,+100},
		{100,+100},
		{100,+100},
		{100,+100},
		{100,+100},
	},
	{0,0,0,0,0,0,0,0,0,0},	//s16	Neu[10];
	-20,				//s16	ThrHold;
	{0,100,-100,100,100},	//DELTAMIX Delta;
	{0,100,-100,100,100},	//VTAILMIX Vtail;
	{0,100,-100,100,100},	//FLAPAIL;
	{0,0,0,0},				//YAWMIX
	{0,0,0},				//ThrMix
	{0,100,-100,100,100},	//AILRUDMIX
	{						//StkCurve
		{						
			{0,125,250,375,500,625,750,875,1000,100,0,0},
			{0,125,250,375,500,625,750,875,1000,100,0,0},
			{0,125,250,375,500,625,750,875,1000,100,0,0},
		},                                     
		{                                      
			{0,125,250,375,500,625,750,875,1000,100,0,0},
			{0,125,250,375,500,625,750,875,1000,100,0,0},
			{0,125,250,375,500,625,750,875,1000,100,0,0},
		},                                     
		{                                      
			{0,125,250,375,500,625,750,875,1000,100,0,0},
			{0,125,250,375,500,625,750,875,1000,100,0,0},
			{0,125,250,375,500,625,750,875,1000,100,0,0},
		},
	},
	{		//ThrCurve[3][CURVE_DS]
		{0,125,250,375,500,625,750,875,1000,100,0,0},
		{0,125,250,375,500,625,750,875,1000,100,0,0},
		{0,125,250,375,500,625,750,875,1000,100,0,0},
	},                                     
	{		//PitCurve[4][CURVE_DS]                     
		{0,125,250,375,500,625,750,875,1000,100,0,0},
		{0,125,250,375,500,625,750,875,1000,100,0,0},
		{0,125,250,375,500,625,750,875,1000,100,0,0},
		{0,125,250,375,500,625,750,875,1000,100,0,0},
	},
	{50,50,50,50},		//Gyro[4]
	0,					//SwashType
	{50,50,50},			//Swash
	{-100,0,100},		//AUXVALUE Gear;
	{-100,0,100},		//AUXVALUE Flap;
	{					//AUXVALUE Aux[6];
		{-100,0,100},
		{-100,0,100},
		{-100,0,100},
		{-100,0,100},
		{-100,0,100},
		{-100,0,100}
	},
	{0,0,0,0,0,0,0,0,0,0},	//Delay[10]
	{0,0,0,1,4,2,3,0},	//s16 SwDef[8];
	0,					//s16 PpmIn
	0,					//u8  StillSeed
	0,					//s16 RxBatType
	37,					//s16 RxBatWarn
	{0,3},				//s16 VirtualSwStk[2];
	{0,0},				//s16 VirtualSwVal[2];
	0,					//CRC校验值
};

//计算CRC16，crc是前段数据CRC16的值，用于分段计算，首段的crc=0;
uint16_t math_crc16(uint16_t crc,const void * data,uint16_t len)
{
    uint8_t h_crc;
    
    const static uint16_t crc_tab[16] =
    {
        0x0000 , 0x1021 , 0x2042 , 0x3063 , 0x4084 , 0x50A5 , 0x60C6 , 0x70E7 ,
        0x8108 , 0x9129 , 0xA14A , 0xB16B , 0xC18C , 0xD1AD , 0xE1CE , 0xF1EF
    };
    const uint8_t * ptr = (const uint8_t *)data;

    while(len --)
    {
        h_crc = (uint8_t)(crc >> 12);
        crc <<= 4;
        crc ^= crc_tab[h_crc ^ ((*ptr) >> 4)];
       
        h_crc = crc >> 12;
        crc <<= 4;
        crc ^= crc_tab[h_crc ^ ((*ptr) & 0x0F)];
      
        ptr ++;
    }
    return crc;
}

int32_t TXSYS_checkValid(const TXSYS *param)
{
    if(math_crc16(0,param,sizeof(TXSYS))!= 0)
        return -1;
    return 0;
}

void TXSYS_refreshCRC(TXSYS *param)
{
    uint16_t crc16_ccitt;
    uint8_t *crc_var = (uint8_t *)&(param->crc);
    crc_var[0] = 0;
    crc_var[1] = 0;
    crc16_ccitt = math_crc16(0,param,sizeof(TXSYS)-2);
    crc_var[2] = ((uint8_t *)&crc16_ccitt)[1];
    crc_var[3] = ((uint8_t *)&crc16_ccitt)[0];
}

void LoadCfg(void)
{
	if(TXSYS_checkValid(TxSysBak)==0)	TxSys = *TxSysBak;
    else	TxSys = TxSysDef;
}

void SaveCfg(void)
{	
	uint16_t i;
    int32_t same = 1;

    TXSYS_refreshCRC(&TxSys);
   
    for(i=0;i<sizeof(TXSYS);i++)
    {
        if(((const uint8_t *)TxSysBak)[i] != ((uint8_t *)&TxSys)[i])
        {
            same = 0;
            break;
        }
    }
    if(same)//参数相同，不需要重新写入
    {
        return;
    }
    else
    {
        STMFLASH_Write(PARAM_TXSYS_ADDRESS,(uint16_t *)&TxSys,sizeof(TXSYS)/2);
    }
}

int32_t MODEL_checkValid(const MODEL *param)
{
    if(math_crc16(0,param,sizeof(MODEL))!= 0)
        return -1;
    return 0;
}

void MODEL_refreshCRC(MODEL *param)
{
    uint16_t crc16_ccitt;
    uint8_t *crc_var = (uint8_t *)&(param->crc);
    crc_var[0] = 0;
    crc_var[1] = 0;
    crc16_ccitt = math_crc16(0,param,sizeof(MODEL)-2);
    crc_var[2] = ((uint8_t *)&crc16_ccitt)[1];
    crc_var[3] = ((uint8_t *)&crc16_ccitt)[0];
}

void ModelLoad(u16 id,u8 txopen)
{
	u32 ParamModAddr = PARAM_MODEL_BASE_ADDRESS+(id-1)*sizeof(MODEL);
	ModelBak = (const MODEL *)ParamModAddr;
	
	if(MODEL_checkValid(ModelBak)==0)	Model = *ModelBak;
	else	Model = ModelDef;
	
	if(txopen)
	{
		//加载发射模块
		TxLoad(Model.Protocol);
	}
}

void ModelSave(u16 id)
{
	uint16_t i;
    int32_t same = 1;
	u32 ParamModAddr = PARAM_MODEL_BASE_ADDRESS+(id-1)*sizeof(MODEL);
	ModelBak = (const MODEL *)ParamModAddr;
	
	MODEL_refreshCRC(&Model);
  
    for(i=0;i<sizeof(MODEL);i++)
    {
        if(((const uint8_t *)ModelBak)[i] != ((uint8_t *)&Model)[i])
        {
            same = 0;
            break;
        }
    }
    if(same)//参数相同，不需要重新写入
    {
        return;
    }
    else
    {
        STMFLASH_Write(ParamModAddr,(uint16_t *)&Model,sizeof(MODEL)/2);
    }
}

void ModelCopy(u8 from,u8 to)
{
	u32 ParamModAddr = PARAM_MODEL_BASE_ADDRESS+(from-1)*sizeof(MODEL);
	ModelBak = (const MODEL *)ParamModAddr;
	ModelBuf = *ModelBak;
	
	ParamModAddr = PARAM_MODEL_BASE_ADDRESS+(to-1)*sizeof(MODEL);
	STMFLASH_Write(ParamModAddr,(uint16_t *)&ModelBuf,sizeof(MODEL)/2);
	
	ParamModAddr = PARAM_MODEL_BASE_ADDRESS+(TxSys.ModelNo-1)*sizeof(MODEL);
	ModelBak = (const MODEL *)ParamModAddr;
	
	PageAlertModel(ModelCopied[TxSys.Language],300);
}

//生成模型初始化配置文件
void ModelFormat(void)
{
	u16 i;
	u32 ParamModAddr;
	
	Model = ModelDef;
	ModelBuf = ModelDef;
	for(i=1;i<=MODCFG_NUM;i++)
	{
		ModelBuf.Name[5]=i/10+'0';
		ModelBuf.Name[6]=i%10+'0';
		ModelBuf.Name[7]=0;//PLANExx
		PageAlertModel(ModelBuf.Name,50);
		
		MODEL_refreshCRC(&ModelBuf);
		ParamModAddr = PARAM_MODEL_BASE_ADDRESS+(i-1)*sizeof(MODEL);
		STMFLASH_Write(ParamModAddr,(uint16_t *)&ModelBuf,sizeof(MODEL)/2);
	}
	
	Model.Name[5]=TxSys.ModelNo/10+'0';
	Model.Name[6]=TxSys.ModelNo%10+'0';
	Model.Name[7]=0;
}
