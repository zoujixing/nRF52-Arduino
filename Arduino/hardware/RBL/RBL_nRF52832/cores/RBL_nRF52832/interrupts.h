
#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "device_common.h"
#include "wiring_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*interruptCB_t)(void);

/*
 * @brief Add a external interrupt handler.
 *
 * @param[in] pin       Pin of interrupt.
 * @param[in] callback  Callback handler function.
 * @param[in] mode      Mode of interrupt:RISING,FALLING,CHANGE.
 */
void attachInterrupt(uint8_t pin, interruptCB_t callback, uint8_t mode);

/*
 * @brief Delete a external interrupt handler.
 *
 * @param[in] pin       Pin of interrupt.
 */
void detachInterrupt(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif
