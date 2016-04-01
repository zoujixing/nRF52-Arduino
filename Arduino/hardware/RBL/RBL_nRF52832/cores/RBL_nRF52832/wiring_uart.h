
#ifndef WIRING_UART_H_
#define WIRING_UART_H_

#include <stdint.h>
#include <string.h>
#include "device_common.h"

/****************************************************
*                 Type Definitions
****************************************************/ 

class UARTClass //: public HardwareSerial
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
    //using Print::write; // pull in write(str) and write(buf, size) from Print  

    void attach(void (*rx_callback)(void));    
};



#endif
