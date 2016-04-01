
#include "delay.h"

/****************************************************
*                 Local Variables
****************************************************/
static volatile uint64_t overflow_count;

/****************************************************
*                 Function Definitions
****************************************************/ 
void RTC_DELAY_IRQ_Hanler(void)
{
    if(RTC_DELAY->EVENTS_OVRFLW)
    {
        // RTC is 24bit, so overflow counter is (0xFFFFFF+1=0x1000000).
        overflow_count += 0x1000000;
        // 40bits = 1099511627775(ticks) * (1/32768) = 388 days. 
        overflow_count &= 0xFFFFFFFFFF;
        RTC_DELAY->EVENTS_OVRFLW = 0;
    }  
}

void RTC_DELAY_init(void)
{
    RTC_DELAY->TASKS_STOP  = 1;
    RTC_DELAY->TASKS_CLEAR = 1;
    // Set prescaler.
    // f(RTC) = (32768)/(PRESCALER+1) = 32768Hz.
    // Counter = 1/32768s = 30.517us
    // OverFlow = 512s
    RTC_DELAY->PRESCALER = 0;
    // Enable overflow event and interrupter.
    RTC_DELAY->EVTENSET = (RTC_EVTEN_OVRFLW_Enabled<<RTC_EVTEN_OVRFLW_Pos);
    RTC_DELAY->INTENSET = (RTC_INTENSET_OVRFLW_Enabled<<RTC_INTENSET_OVRFLW_Pos);
    // Enable IRQn
    NVIC_EnableIRQ(RTC_DELAY_IRQn);
    //
    overflow_count = 0;
    // Start counter
    RTC_DELAY->TASKS_START = 1;
}

uint32_t micos(void)
{
    return (uint32_t)((overflow_count + RTC_DELAY->COUNTER) * 1000000 >> 15);
}

uint32_t millis(void)
{
    return (uint32_t)((overflow_count + RTC_DELAY->COUNTER) * 1000 >> 15);
}

void delay(uint32_t ms)
{
    uint32_t start_time;
    start_time = millis();
    while((millis() - start_time) < ms)
    {
        //
    }
}

void delayMicrosenconds(uint32_t us)
{
    uint32_t start_time;
    start_time = micros();
    while((micros() - start_time) < us)
    {
        //
    } 
}
