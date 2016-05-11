
#ifndef NRF_CLOCK_H_
#define NRF_CLOCK_H_

#include "device_common.h"

#ifdef __cplusplus
extern "C"{
#endif

/*
 * @brief Enable 32.768KHz crystal oscillator.
 */
void LFCLK_init(void);

/*
 * @brief Enable external high frequency crystal oscillator.
 */
void HFCLK_init(void);

#ifdef __cplusplus
} 
#endif

#endif
