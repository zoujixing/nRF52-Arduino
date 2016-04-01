
#ifndef NRF_CLOCK_H_
#define NRF_CLOCK_H_

#include <stdint.h>
#include <string.h>
#include "device_common.h"

#ifdef __cplusplus
extern "C"{
#endif

void LFCLK_init(void);
void HFCLK_init(void);

#ifdef __cplusplus
} 
#endif

#endif
