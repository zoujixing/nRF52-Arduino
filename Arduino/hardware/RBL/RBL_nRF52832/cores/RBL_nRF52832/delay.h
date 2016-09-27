
#ifndef DELAY_H_
#define DELAY_H_

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

/*
 * @brief Init RTCx, call in main.cpp
 */
void RTC_DELAY_init(void);

/*
 * @brief Get the time(in ms) after system begin
 *
 * @return Time in ms
 */
uint32_t millis(void);

/*
 * @brief Get the time(in us) after system begin
 *
 * @return Time in us
 */
uint32_t micros(void);

/*
 * @brief Delay in ms
 *
 * @param[in] ms      Time in ms.
 */
void delay(uint32_t ms);

/*
 * @brief Delay in us
 *
 * @param[in] us      Time in us.
 */
void delayMicroseconds(uint32_t us);

/*
 * @brief Delay in us. Used in some app libraries.
 *
 * @param[in] us      Time in us.
 */
static void __INLINE nrf_delay_us(uint32_t volatile number_of_us) __attribute__((always_inline));
static void __INLINE nrf_delay_us(uint32_t volatile number_of_us)
{
register uint32_t delay __ASM ("r0") = number_of_us;
__ASM volatile (
    "1:\n"
    " SUBS %0, %0, #1\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " NOP\n"
    " BNE 1b\n"
    : "+r" (delay));
}

#ifdef __cplusplus
}
#endif

#endif
