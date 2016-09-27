
#define ARDUINO_MAIN

#include "Arduino.h"

/*
 * \brief Main entry point of Arduino application
 */


int main(void)
{
    LFCLK_init();
    HFCLK_init();
    RTC_DELAY_init();

    setup();

    for (;;)
    {
        loop();
    }

    return 0;
}
