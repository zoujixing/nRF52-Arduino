
#include "Wire.h"
    
TwoWire::TwoWire(NRF_TWIM_Type *p_twim, NRF_TWIS_Type *p_twis)
{
    TWIMx = p_twim;
    TWISx = p_twis;
    twi_status = UNINITIALIZED;
}

void TwoWire::begin(void)
{   
    // For master mode
    begin(SCL1, SDA1);
}

void TwoWire::begin(uint8_t scl, uint8_t sda)
{
    // For master mode
    uint8_t nrf_scl;
    uint8_t nrf_sda;

    nrf_scl = Pin_nRF51822_to_Arduino(scl);
    nrf_sda = Pin_nRF51822_to_Arduino(sda);
    // Check pin
    if(nrf_scl>31 || nrf_sda>31)
        return;
    // Set twi mode to master
    twi_mode = 1;
    //log_info("TWI : Master mode \r\n");
    // Save pin
    this->scl_pin = nrf_scl;
    this->sda_pin = nrf_sda;
    // Config pin
    NRF_GPIO->PIN_CNF[nrf_scl] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
                                 (GPIO_PIN_CNF_DRIVE_S0D1     << GPIO_PIN_CNF_DRIVE_Pos) |
                                 (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos) |
                                 (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos) |
                                 (GPIO_PIN_CNF_DIR_Input      << GPIO_PIN_CNF_DIR_Pos);

    NRF_GPIO->PIN_CNF[nrf_sda] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
                                 (GPIO_PIN_CNF_DRIVE_S0D1     << GPIO_PIN_CNF_DRIVE_Pos) |
                                 (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos) |
                                 (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos) |
                                 (GPIO_PIN_CNF_DIR_Input      << GPIO_PIN_CNF_DIR_Pos);
    // Config pin as TWI
    TWIMx->PSEL.SCL = nrf_scl;
    TWIMx->PSEL.SDA = nrf_sda;
    // Config default frequency to 100KHz
    TWIMx->FREQUENCY = TWI_FREQUENCY_FREQUENCY_K100;
    // Enable IRQ, set priority to APP_IRQ_PRIORITY_HIGH
    //NVIC_ClearPendingIRQ(TWI_IRQn);
    //NVIC_SetPriority(TWI_IRQn, 2);
    //NVIC_EnableIRQ(TWI_IRQn);    
    // Enable TWIM
    TWIMx->ENABLE = (TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos);
    //log_info("TWI : Enable TWIMx \r\n");
    // Change status
    twi_status = MASTER_IDLE;
}

void TwoWire::begin(uint8_t addr)
{
    // For slave mode
    begin(addr, SCL1, SDA1);
}

void TwoWire::begin(uint8_t addr, uint8_t scl, uint8_t sda)
{
    // For slave mode
    uint8_t nrf_scl;
    uint8_t nrf_sda;

    nrf_scl = Pin_nRF51822_to_Arduino(scl);
    nrf_sda = Pin_nRF51822_to_Arduino(sda);
    // Check pin
    if(nrf_scl>31 || nrf_sda>31)
        return;
    // Set twi mode to master
    twi_mode = 0;
    // Save pin
    this->scl_pin = nrf_scl;
    this->sda_pin = nrf_sda;
    // Config pin
    NRF_GPIO->PIN_CNF[nrf_scl] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
                                 (GPIO_PIN_CNF_DRIVE_S0D1     << GPIO_PIN_CNF_DRIVE_Pos) |
                                 (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos) |
                                 (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos) |
                                 (GPIO_PIN_CNF_DIR_Input      << GPIO_PIN_CNF_DIR_Pos);

    NRF_GPIO->PIN_CNF[nrf_sda] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
                                 (GPIO_PIN_CNF_DRIVE_S0D1     << GPIO_PIN_CNF_DRIVE_Pos) |
                                 (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos) |
                                 (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos) |
                                 (GPIO_PIN_CNF_DIR_Input      << GPIO_PIN_CNF_DIR_Pos);
    // Config pin as TWI
    TWISx->PSEL.SCL = nrf_scl;
    TWISx->PSEL.SDA = nrf_sda;
    // Config slave address
    TWISx->ADDRESS[0] = addr;
    TWISx->CONFIG = (TWIS_CONFIG_ADDRESS0_Enabled << TWIS_CONFIG_ADDRESS0_Pos);
    // Config ORC
    TWISx->ORC = 0xFF;
    // Config interrupts
    TWISx->INTENSET = (TWIS_INTENCLR_STOPPED_Enabled << TWIS_INTENCLR_STOPPED_Pos) | 
                      (TWIS_INTENCLR_ERROR_Enabled << TWIS_INTENCLR_ERROR_Pos) |
                      (TWIS_INTENCLR_WRITE_Enabled << TWIS_INTENCLR_WRITE_Pos) |
                      (TWIS_INTENCLR_READ_Enabled << TWIS_INTENCLR_READ_Pos);
    // Enable IRQ, set priority to APP_IRQ_PRIORITY_HIGH
    NVIC_ClearPendingIRQ(TWI_IRQn);
    NVIC_SetPriority(TWI_IRQn, 2);
    NVIC_EnableIRQ(TWI_IRQn);
    // Enable TWIM
    TWISx->ENABLE = (TWIS_ENABLE_ENABLE_Enabled << TWIS_ENABLE_ENABLE_Pos);    
    // Change status
    twi_status = SLAVE_IDLE;
}

void TwoWire::end(void)
{
    if(twi_mode)
        TWIMx->ENABLE = (TWIM_ENABLE_ENABLE_Disabled << TWIM_ENABLE_ENABLE_Pos);
    else
        TWISx->ENABLE = (TWIS_ENABLE_ENABLE_Disabled << TWIS_ENABLE_ENABLE_Pos);
}

void TwoWire::setClock(uint32_t frequency)
{
    // No need to set frequency at slave mode
    if(!twi_mode) 
        return;
    
    if(frequency <= 100000)
        TWIMx->FREQUENCY = TWI_FREQUENCY_FREQUENCY_K100;
    else if(frequency <= 250000)
        TWIMx->FREQUENCY = TWI_FREQUENCY_FREQUENCY_K250;
    else 
        TWIMx->FREQUENCY = TWI_FREQUENCY_FREQUENCY_K400;   
}

void TwoWire::beginTransmission(uint8_t addr)
{
    // Save transmission addr
    this->twi_addr = addr;
    // Clean tx_buffer
    tx_buffer.clear();
    // Change status
    twi_status = MASTER_SEND;
}

uint8_t TwoWire::endTransmission(bool stopBit)
{
    // Config transmission address
    TWIMx->ADDRESS = this->twi_addr;
    //log_info("TWI : Tx addr %d  \r\n", this->twi_addr);
    // Resume twi bus
    TWIMx->TASKS_RESUME = 1;
    // Config eDMA
    TWIMx->TXD.PTR = (uint32_t)(tx_buffer.ring_buffer);
    TWIMx->TXD.MAXCNT = tx_buffer.available();
    // Start tx
    //log_info("TWI : Start tx \r\n");
    TWIMx->TASKS_STARTTX = 1;
    // Wait for tx is ready
    while(!TWIMx->EVENTS_TXSTARTED && !TWIMx->EVENTS_ERROR);
    TWIMx->EVENTS_TXSTARTED = 0;
    // Wait for tx is over
    while(!TWIMx->EVENTS_LASTTX && !TWIMx->EVENTS_ERROR);
    TWIMx->EVENTS_LASTTX = 0;
    // Whether send stop bit, or suspend bus
    if(stopBit || TWIMx->EVENTS_ERROR) {
        TWIMx->TASKS_STOP = 1;
        while(!TWIMx->EVENTS_STOPPED);
        TWIMx->EVENTS_STOPPED = 0;
    }
    else {
        TWIMx->TASKS_SUSPEND = 1;
        while(!TWIMx->EVENTS_SUSPENDED);
        TWIMx->EVENTS_SUSPENDED = 0;           
    }
    // Change status to IDLE
    twi_status = MASTER_IDLE;
    // Check the error code
    if(TWIMx->EVENTS_ERROR) {
        TWIMx->EVENTS_ERROR = 0;
        
        uint32_t error_code = TWIMx->ERRORSRC;        
        if(error_code & TWIM_ERRORSRC_ANACK_Msk) {
            // NACK received after sending the address
            //log_info("TWI : NACK received after sending the address \r\n");
            return 1; 
        }
        else if(error_code & TWIM_ERRORSRC_DNACK_Msk) {
            // NACK received after sending a data byte
            //log_info("TWI : NACK received after sending a data byte \r\n");
            return 2;
        }
        else {
            // Other error
            //log_info("TWI : Other error \r\n");
            return 3;
        }     
    }
    // Send success
    //log_info("TWI : Send success \r\n");
    return 0;
}

uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);    
}

uint8_t TwoWire::requestFrom(uint8_t addr, size_t quantity, bool stopBit)
{
    if(quantity == 0)
        return 0;
    // Make sure no more than max size
    if(quantity > sizeof(rx_buffer.ring_buffer))
        quantity = sizeof(rx_buffer.ring_buffer);
    
    // Clear rx buffer
    rx_buffer.clear();
    // Set remote device addr
    TWIMx->ADDRESS = addr;
    // Resume twi bus
    TWIMx->TASKS_RESUME = 1;
    // Config eDMA
    TWIMx->RXD.PTR = (uint32_t)(this->rx_buffer.ring_buffer);
    TWIMx->RXD.MAXCNT = quantity;
    // Start task
    TWIMx->EVENTS_RXSTARTED = 0;
    TWIMx->EVENTS_ERROR     = 0;
    TWIMx->TASKS_STARTRX    = 1;
    // Wait tast is started
    while(!TWIMx->EVENTS_RXSTARTED && !TWIMx->EVENTS_ERROR);
    TWIMx->EVENTS_RXSTARTED = 0;
    // Wait last byte is received
    while(!TWIMx->EVENTS_LASTRX && !TWIMx->EVENTS_ERROR);
    TWIMx->EVENTS_LASTRX = 0;
    // Whether send stop bit, or suspend bus
    if(stopBit || TWIMx->EVENTS_ERROR) {
        TWIMx->TASKS_STOP = 1;
        while(!TWIMx->EVENTS_STOPPED);
        TWIMx->EVENTS_STOPPED = 0;
    }
    else {
        TWIMx->TASKS_SUSPEND = 1;
        while(!TWIMx->EVENTS_SUSPENDED);
        TWIMx->EVENTS_SUSPENDED = 0;        
    }
    // Clear error flag, if need
    if(TWIMx->EVENTS_ERROR)
        TWIMx->EVENTS_ERROR = 0;
    // Get the rx size
    rx_buffer.head = TWIMx->RXD.AMOUNT;
    
    return rx_buffer.head;
}

uint8_t TwoWire::requestFrom(uint8_t addr, size_t quantity)
{
    return requestFrom(addr, quantity, true);   
}

size_t TwoWire::write(uint8_t data)
{
    if((twi_status == MASTER_SEND) && (tx_buffer.isFull()))
        return 0;
    // store byte
    tx_buffer.store(data);
    
    return 1;
}

size_t TwoWire::write(const uint8_t *p_data, size_t quantity)
{
    for(uint8_t i=0; i<quantity; i++){
        // If store fail, return the number of being stored
        if(!write(p_data[i]))
            return i;
    }
    // All data have been stored
    return quantity;
}

int TwoWire::available(void)
{
    return rx_buffer.available();
}

int TwoWire::read(void)
{
    return rx_buffer.read();
}

int TwoWire::peek(void)
{
    return rx_buffer.peek();
}

void TwoWire::flush(void)
{
    // Nothing
}



void TwoWire::onReceive(void(*function)(int))
{
    if(function != NULL) {
        // Change status
        twi_status = SLAVE_RECV;
        receiveCallback = function;
    }
}

void TwoWire::onRequest(void(*function)(void))
{
    if(function != NULL) {
        // Change status
        twi_status = SLAVE_SEND;
        requestCallback = function;
    } 
}

void TwoWire::onIRQnCallback(void)
{
    //log_info("TWI : TWI handler \r\n");
    if(TWISx->EVENTS_WRITE) {
        TWISx->EVENTS_WRITE = 0;
        // Clear rx buffer
        rx_buffer.clear();
        // Config eDMA
        TWISx->RXD.PTR = (uint32_t)(rx_buffer.ring_buffer);
        TWISx->RXD.MAXCNT = sizeof(rx_buffer.ring_buffer);
        // Preppare to rx
        TWISx->TASKS_PREPARERX = 1;
    }
    
    if(TWISx->EVENTS_READ) {
        TWISx->EVENTS_READ = 0;
        // Clear tx buffer
        tx_buffer.clear();
        // Call request callback function, to write data.
        if(requestCallback) 
            requestCallback();
        // Config eDMA
        TWISx->TXD.PTR = (uint32_t)(tx_buffer.ring_buffer);
        TWISx->TXD.MAXCNT = tx_buffer.available();
        // Prepare to tx
        TWISx->TASKS_PREPARETX = 1;
    }
    
    if(TWISx->EVENTS_STOPPED) {
        TWISx->EVENTS_STOPPED = 0;
        if(twi_status == SLAVE_RECV) {
            // Get received number
            rx_buffer.head = TWISx->RXD.AMOUNT;
            if(receiveCallback)
                receiveCallback(rx_buffer.head);
        }
    }
    
    if(TWISx->EVENTS_ERROR) {
        TWISx->EVENTS_ERROR = 0;
        
        TWISx->TASKS_STOP = 1;
    }    
}

TwoWire Wire(NRF_TWIM1, NRF_TWIS1);

#ifdef __cplusplus
extern "C"{
#endif

void TWI_IRQn_Handler(void)
{
    Wire.onIRQnCallback();
}

#ifdef __cplusplus
}
#endif
