
#ifndef PIN_TRANSFORM_H_
#define PIN_TRANSFORM_H_

#include "device_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Pin mapping
 *
 * @param[in] pin   Arduino pin
 * 
 * @return nRF52 pin
 */
uint8_t Pin_nRF51822_to_Arduino(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif
