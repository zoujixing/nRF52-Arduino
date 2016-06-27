
#ifndef WIRE_H_
#define WIRE_H_

#include "Arduino.h"
#include "WStream.h"
#include "ring_buffer.h"

#define TWIM_ID                       NRF_TWIM1
#define TWIS_ID                       NRF_TWIS1
#define TWI_IRQn                      SPI1_TWI1_IRQn
#define TWI_IRQn_Handler              SPI1_TWI1_IRQHandler


class TwoWire : public WStream
{
public:
    typedef enum {
        UNINITIALIZED,
        MASTER_IDLE,
        MASTER_SEND,
        MASTER_RECV,
        SLAVE_IDLE,
        SLAVE_RECV,
        SLAVE_SEND
    }TwoWireStatus_t;
    
public:
    TwoWire(NRF_TWIM_Type *p_twim, NRF_TWIS_Type *p_twis);
    
    void begin(void);
    void begin(uint8_t scl, uint8_t sda);
    
    void begin(uint8_t addr);
    void begin(uint8_t addr, uint8_t scl, uint8_t sda);
    void end(void);
    
    void setClock(uint32_t frequency);
    
    void beginTransmission(uint8_t addr);
    uint8_t endTransmission(bool stopBit);
    uint8_t endTransmission(void);
    
    uint8_t requestFrom(uint8_t addr, size_t quantity, bool stopBit);
    uint8_t requestFrom(uint8_t addr, size_t quantity);
 
    void onReceive(void(*function)(int));
    void onRequest(void(*function)(void));
    
    void onIRQnCallback(void);
    
    virtual size_t write(uint8_t data);
    virtual size_t write(const uint8_t *p_data, size_t quantity);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    
    using Print::write; 
    
private:
    NRF_TWIM_Type *TWIMx;
    NRF_TWIS_Type *TWISx;
    // 1 master, 0 slave;
    bool twi_mode;
    
    TwoWireStatus_t twi_status;
    
    uint8_t twi_addr;
    
    uint8_t scl_pin;
    uint8_t sda_pin;
    
    Ring_Buffer rx_buffer;
    Ring_Buffer tx_buffer;
    
    // Callback functions for TWI slave
    void (*receiveCallback)(int);
    void (*requestCallback)(void);
};

extern TwoWire Wire;

#endif
