
#ifndef WIRING_DIGITAL_H_
#define WIRING_DIGITAL_H_

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
