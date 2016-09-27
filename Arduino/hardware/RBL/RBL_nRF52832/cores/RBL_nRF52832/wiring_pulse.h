
#ifndef WIRING_PULSE_H_
#define WIRING_PULSE_H_

#include "device_common.h"
#include "wiring_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Measure the input pulse
 *
 * @note  Works on pulses from 2-3 microseconds to 3 minutes in length.
 *        But must be called at least a few dozen microsecondsbefore the start of the pulse.
 *
 * @param[in] pin
 * @param[in] state     The type of pulse to measurestate, HIGH or LOW
 * @param[in] timeout
 *
 * @return   Time in ms
 */
uint32_t pulseIn(uint8_t pin, uint32_t state, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif
