
#ifndef WIRING_DIGITAL_H_
#define WIRING_DIGITAL_H_

#include "device_common.h"
#include "wiring_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Set pin mode.
 *
 * @param[in] pin
 * @param[in] mode  Pin mode:INPUT,OUTPUT,INPUT_PULLUP,INPUT_PULLDOWN.
 */
void pinMode(uint8_t pin, uint8_t mode);

/*
 * @brief Write digital pin.
 *
 * @param[in] pin
 * @param[in] value Pin status:HIGH or LOW.
 */
void digitalWrite(uint8_t pin, uint32_t value);

/*
 * @brief Read digital pin.
 *
 * @param[in] pin
 *
 * @return  Pin status:HIGH or LOW. If -1, pin error.
 */
int digitalRead(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif
