
#ifndef Arduino_h
#define Arduino_h

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// some libraries and sketches depend on this
// AVR stuff, assuming Arduino.h or WProgram.h
// automatically includes it..
#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C"{
#endif 

#include "device_common.h"

#include "nrf_clock.h"
#include "delay.h"

#include "wiring_constants.h"
#include "wiring_digital.h"
#include "wiring_analog.h"
#include "wiring_shift.h"
#include "wiring_pulse.h"
#include "interrupts.h"

#include "debug_config.h"

void yield(void);

//sketch 
extern void setup( void );
extern void loop( void );

#ifdef __cplusplus
} 
#endif 

#include "wiring_uart.h"
#include "tone.h"


#endif 
