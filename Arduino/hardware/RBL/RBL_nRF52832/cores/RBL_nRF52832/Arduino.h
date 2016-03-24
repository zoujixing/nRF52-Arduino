
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

#include "nrf.h"
#include "nrf51_to_nrf52.h"
#include "nrf52.h"
#include "nrf52_bitfields.h"
#include "nrf52_name_change.h"
#include "compiler_abstraction.h"
#include "system_nrf52.h"

#include "pin_transform.h"
#include "wiring_constants.h"
#include "nrf_clock.h"
#include "delay.h"

#include "wiring_digital.h"

void yield(void);

//sketch 
extern void setup( void ) ;
extern void loop( void ) ;

#ifdef __cplusplus
} 
#endif 



#endif 
