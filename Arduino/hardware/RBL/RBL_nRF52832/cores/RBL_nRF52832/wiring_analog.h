
#ifndef WIRING_ANALOG_H_
#define WIRING_ANALOG_H_

#include <stdint.h>
#include <string.h>
#include "device_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_READ_RESOLUTION  10
#define DEFAULT_WRITE_RESOLUTION 8

//#ifdef NRF52
typedef enum{
    REFSEL_INTERNAL,
    REFSEL_VDD1_4
}eAnalogReference_t;
//#endif

/*
 * @brief Init pwm sequence and some flags. It's not must.
 */
void pwm_init(void);

/*
 * @brief Set the resolution of analogRead return values. Default is 10 bits (range from 0 to 1023).
 *
 * @param[in] resolution 
 */
void analogReadResolution(uint8_t resolution);

/*
 * @brief Set the resolution of analogWrite parameters. Default is 8 bits (range from 0 to 255).
 *
 * @param[in] resolution 
 */
void analogWriteResolution(uint8_t resolution);

/*
 * @brief Configures the reference voltage used for analog input.
 *
 * @note  Default is REFSEL_INTERNAL.
 *
 * @param[in] type 
 */
void analogReference(eAnalogReference_t type);

/*
 * @brief Reads the value from the specified analog pin.
 *
 * @param[in] pin 
 */
uint32_t analogRead(uint8_t pin);

/*
 * @brief Writes an analog value (PWM wave) to a pin.
 *
 * @param[in] pin 
 * @param[in] value 
 */
void analogWrite(uint8_t pin, uint32_t value);




#ifdef __cplusplus
}
#endif

#endif
