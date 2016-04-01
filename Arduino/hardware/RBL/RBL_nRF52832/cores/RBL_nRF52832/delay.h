
#ifndef DELAY_H_
#define DELAY_H_


#include <stdint.h>
#include <string.h>
#include "device_common.h"

#ifdef __cplusplus
extern "C"{
#endif

/****************************************************
*                 Micro Definitions
****************************************************/
#define RTC_DELAY             NRF_RTC2
#define RTC_DELAY_IRQn        RTC2_IRQn
#define RTC_DELAY_IRQ_Hanler  RTC2_IRQHandler

/****************************************************
*                 Function Declarations
****************************************************/

void RTC_DELAY_init(void);

uint32_t millis(void);
uint32_t micros(void);
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);


#ifdef __cplusplus
} 
#endif

#endif
