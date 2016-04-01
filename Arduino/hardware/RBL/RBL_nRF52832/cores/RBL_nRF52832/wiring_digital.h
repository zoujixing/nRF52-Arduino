
#ifndef WIRING_DIGITAL_H_
#define WIRING_DIGITAL_H_

#include <stdint.h>
#include <string.h>
#include "device_common.h"

#include "pins_arduino.h"
#include "pin_transform.h"
#include "wiring_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

void pinMode(uint8_t pin, uint8_t mode);
 
void digitalWrite(uint8_t pin, uint32_t value);

int digitalRead(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif /* _WIRING_DIGITAL_ */
