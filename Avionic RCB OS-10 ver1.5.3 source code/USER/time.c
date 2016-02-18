#include "time.h"

static struct 
{
    volatile uint32_t msPeriod;	// �����ڵ�ʱ��ms��
    uint32_t ticksPerUs;        // ÿus���ڵ�tick����
    uint32_t ticksPerMs;        // ÿms���ڵ�tick����
    uint32_t msPerPeriod;       // ÿ���ڵ�ms����
    uint32_t reload ;
}time;

void Time_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//ѡ��ʱ��HCLK
	time.msPeriod = 0;
   	time.ticksPerUs = SystemCoreClock/1e6;
    time.ticksPerMs = SystemCoreClock/1e3;
    time.msPerPeriod = 1;
    time.reload = time.msPerPeriod*time.ticksPerMs;
	SysTick_Config(time.reload);
}

//SysTick�жϡ�
void SysTick_Handler(void)
{
    time.msPeriod += time.msPerPeriod;
}

// ��ȡ��ǰʱ�䣬us�� 
uint32_t Time_NowUs(void)
{
    return time.msPeriod*1000+(SysTick->LOAD - SysTick->VAL)/time.ticksPerUs;
}

// ��ȡ��ǰʱ�䣬ms��
uint32_t Time_NowMs(void)
{
    return time.msPeriod+(SysTick->LOAD - SysTick->VAL)/time.ticksPerMs;
}

// ��ʱdelay us��delay>=4ʱ��׼ȷ��
void DelayUs(uint32_t nus)
{
    uint32_t target = Time_NowUs()+nus-2;
    while(Time_NowUs()<=target);
}

//��ʱdelay ms��
void DelayMs(uint32_t nms)
{
    DelayUs(nms*1000);
}


