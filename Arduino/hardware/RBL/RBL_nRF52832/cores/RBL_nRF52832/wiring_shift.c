
#include "wiring_shift.h"
#include "wiring_digital.h"


/****************************************************
*                  Function Definitions
****************************************************/

uint32_t shiftIn(uint8_t ulDataPin, uint8_t ulClockPin, uint32_t ulBitOrder)
{
    uint8_t value = 0;
    uint8_t i;

    for (i=0 ; i<8; ++i) {
        digitalWrite(ulClockPin, HIGH);

        if (ulBitOrder == LSBFIRST)
            value |= digitalRead(ulDataPin) << i;
        else
            value |= digitalRead(ulDataPin) << (7 - i);

        digitalWrite(ulClockPin, LOW);
    }

    return value;
}

void shiftOut(uint8_t ulDataPin, uint8_t ulClockPin, uint32_t ulBitOrder, uint32_t ulValue)
{
    uint8_t i ;

    for (i=0; i<8; i++) {
        if(ulBitOrder == LSBFIRST)
            digitalWrite(ulDataPin, !!(ulValue & (1 << i)));
        else
            digitalWrite(ulDataPin, !!(ulValue & (1 << (7 - i))));

        digitalWrite(ulClockPin, HIGH);
        digitalWrite(ulClockPin, LOW);
    }
}
