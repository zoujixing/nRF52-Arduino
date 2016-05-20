
#ifndef WIRING_SHIFT_H_
#define WIRING_SHIFT_H_

#include "device_common.h"
#include "wiring_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Shift in a data
 *
 * @param[in] ulDataPin  
 * @param[in] ulClockPin
 * @param[in] ulBitOrder    LSBFIRST or MSBFIRST
 */
uint32_t shiftIn(uint8_t ulDataPin, uint8_t ulClockPin, uint32_t ulBitOrder);

/*
 * @brief Shift out a data
 *
 * @param[in] ulDataPin  
 * @param[in] ulClockPin
 * @param[in] ulBitOrder    LSBFIRST or MSBFIRST
 * @param[in] ulValue         
 */
void shiftOut(uint8_t ulDataPin, uint8_t ulClockPin, uint32_t ulBitOrder, uint32_t ulValue);

#ifdef __cplusplus
}
#endif

#endif
