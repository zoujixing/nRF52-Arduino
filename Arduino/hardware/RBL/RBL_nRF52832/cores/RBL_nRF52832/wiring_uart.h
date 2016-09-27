
#ifndef WIRING_UART_H_
#define WIRING_UART_H_

#include <cstddef>
#include "device_common.h"

#include "HardwareSerial.h"

typedef void (*rx_callback_t)(void);

/****************************************************
*                 Type Definitions
****************************************************/

class UARTClass : public HardwareSerial
{
public:
    UARTClass(void);
    void begin(uint32_t baudRate);
    void begin(uint32_t baudRate, uint8_t tx_pin, uint8_t rx_pin);
    void end(void);
    int  available(void);
    int  peek(void);
    int  read(void);
    void flush(void);
    size_t write(uint8_t dat);

    void attach(rx_callback_t handler);

    using Print::write;

    operator bool() {
        return true;
    };
};

extern UARTClass Serial;

#endif
