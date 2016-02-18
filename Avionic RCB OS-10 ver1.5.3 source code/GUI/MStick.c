#include "Page.h"
#include "cfg.h"
#include "adc.h"

///////////////////////////////////////////////////////////////////////////////////////
//
//  ADC���ݽṹ
//
u16 StickRaw[STK_NUM];
s16 StickCal[STK_NUM];
s16 StickValue[STK_NUM];
s16 StickTrim[STK_NUM];

	//ҡ��ģʽӳ��
const u8 StickMap[][4]={
	{0,1,2,3},//�ձ�
	{0,2,1,3},//����
	{3,1,2,0},//���ձ�
	{3,2,1,0},//������
};
	
void StickHandler(void)
{
	u8 i,idx;
	s32 v;
	
	//ȡ��ԭʼ����
	StickRaw[0]=ADC_AvgValue[AD_LH];
	StickRaw[1]=ADC_AvgValue[AD_RV];
	StickRaw[2]=ADC_AvgValue[AD_LV];
	StickRaw[3]=ADC_TRV*2-ADC_AvgValue[AD_RH];
	
	for(i=0;i<STK_NUM;i++)
	{
		v = StickRaw[i]-TxSys.StkCali[i][1];
		
		//��������
		if(v<0)
		{
			if(v<-TxSys.StkDa)	v+=TxSys.StkDa;
			else				v=0;
		}  
		else
		{
			if(v>TxSys.StkDa)	v-=TxSys.StkDa;
			else				v=0;
		}  
		
		v *= STK_TRV;		
		StickCal[i] = v/(TxSys.StkCali[i][v>0?2:0]-TxSys.StkDa);
		
		//����ͨ��ӳ��
		idx = StickMap[TxSys.StkType][i];
		StickValue[idx] = StickCal[i];
		StickTrim[idx] = Model.Trim[i]*TxSys.TrimStep;
	}
}
