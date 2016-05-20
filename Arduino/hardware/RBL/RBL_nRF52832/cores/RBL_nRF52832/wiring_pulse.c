
#include "wiring_pulse.h"

// See pulse_asm.S
extern unsigned long countPulseASM(const volatile uint32_t *port, uint32_t bit, uint32_t stateMask, unsigned long maxloops);

/****************************************************
*                  Function Definitions
****************************************************/
// unsigned long countPulseASM(const volatile uint32_t *port, uint32_t bit, uint32_t stateMask, unsigned long maxloops)
// {
    // unsigned long width = 0;

    ////wait for any previous pulse to end
    // while ((*port & bit) == stateMask)
        // if (--maxloops == 0)
            // return 0;

    ////wait for the pulse to start
    // while ((*port & bit) != stateMask)
        // if (--maxloops == 0)
            // return 0;

    ////wait for the pulse to stop
    // while ((*port & bit) == stateMask) {
        // if (++width == maxloops)
            // return 0;
    // }
    // return width;
// }


uint32_t pulseIn(uint8_t pin, uint32_t state, uint32_t timeout)
{
    uint8_t  nrf_pin;
    
    nrf_pin = Pin_nRF51822_to_Arduino(pin);
    if(nrf_pin >= 31)
        return 0;
    log_info(" Pulse : Start \r\n");
    uint32_t bit = 1 << nrf_pin;
    uint32_t stateMask = state ? bit : 0;
    
    // convert the timeout from microseconds to a number of times through
    // the initial loop; it takes (roughly) 13 clock cycles per iteration.
    uint32_t maxloops = microsecondsToClockCycles(timeout) / 13;
    uint32_t width = countPulseASM(&(NRF_GPIO->IN), bit, stateMask, maxloops);
    log_info(" Pulse : maxloops %d \r\n", maxloops);
    log_info(" Pulse : width %d \r\n", width);
    
    // convert the reading to microseconds. The loop has been determined
    // to be 13 clock cycles long and have about 16 clocks between the edge
    // and the start of the loop. There will be some error introduced by
    // the interrupt handlers.
    if (width)
        return clockCyclesToMicroseconds(width * 13 + 16);
    else
        return 0;    
}
