#include "time.h"

static struct 
{
    volatile uint32_t msPeriod;	// 整周期的时间ms。
    uint32_t ticksPerUs;        // 每us等于的tick数。
    uint32_t ticksPerMs;        // 每ms等于的tick数。
    uint32_t msPerPeriod;       // 每周期的ms数。
    uint32_t reload ;
}time;

void Time_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//选择时钟HCLK
	time.msPeriod = 0;
   	time.ticksPerUs = SystemCoreClock/1e6;
    time.ticksPerMs = SystemCoreClock/1e3;
    time.msPerPeriod = 1;
    time.reload = time.msPerPeriod*time.ticksPerMs;
	SysTick_Config(time.reload);
}

//SysTick中断。
void SysTick_Handler(void)
{
    time.msPeriod += time.msPerPeriod;
}

// 获取当前时间，us。 
uint32_t Time_NowUs(void)
{
    return time.msPeriod*1000+(SysTick->LOAD - SysTick->VAL)/time.ticksPerUs;
}

// 获取当前时间，ms。
uint32_t Time_NowMs(void)
{
    return time.msPeriod+(SysTick->LOAD - SysTick->VAL)/time.ticksPerMs;
}

// 延时delay us，delay>=4时才准确。
void DelayUs(uint32_t nus)
{
    uint32_t target = Time_NowUs()+nus-2;
    while(Time_NowUs()<=target);
}

//延时delay ms。
void DelayMs(uint32_t nms)
{
    DelayUs(nms*1000);
}


