
#include "uart.h"

/****************************************************
*                 Local Variables
****************************************************/

static UART0_IRQ_Handler_t UART0_handler = NULL;

static UART0_States_t UART0_State = UART0_NotStart;

/****************************************************
*                 Function Definitions
****************************************************/

void UART0_registRxHandler(UART0_IRQ_Handler_t handler)
{
    UART0_handler = handler;
}

void UART0_init(uint8_t nrf_tx_pin, uint8_t nrf_rx_pin, uint32_t baud_rate)
{
    if(UART0_State != UART0_NotStart)
        return;

    // Configurate TXD/RXD pin.
    NRF_GPIO->PIN_CNF[nrf_tx_pin] = ((GPIO_PIN_CNF_SENSE_Disabled<<GPIO_PIN_CNF_SENSE_Pos)   |
                                     (GPIO_PIN_CNF_DRIVE_S0S1<<GPIO_PIN_CNF_DRIVE_Pos)       |
                                     (GPIO_PIN_CNF_PULL_Disabled<<GPIO_PIN_CNF_PULL_Pos)     |
                                     (GPIO_PIN_CNF_INPUT_Disconnect<<GPIO_PIN_CNF_INPUT_Pos) |
                                     (GPIO_PIN_CNF_DIR_Output<<GPIO_PIN_CNF_DIR_Pos) );

    NRF_GPIO->PIN_CNF[nrf_rx_pin] = ((GPIO_PIN_CNF_SENSE_Disabled<<GPIO_PIN_CNF_SENSE_Pos)   |
                                     (GPIO_PIN_CNF_DRIVE_S0S1<<GPIO_PIN_CNF_DRIVE_Pos)       |
                                     (GPIO_PIN_CNF_PULL_Disabled<<GPIO_PIN_CNF_PULL_Pos)     |
                                     (GPIO_PIN_CNF_INPUT_Disconnect<<GPIO_PIN_CNF_INPUT_Pos) |
                                     (GPIO_PIN_CNF_DIR_Input<<GPIO_PIN_CNF_DIR_Pos) );

    NRF_UART0->PSELTXD = (uint32_t)nrf_tx_pin;
    NRF_UART0->PSELRXD = (uint32_t)nrf_rx_pin;
    NRF_UART0->PSELRTS = 0xFFFFFFFF;
    NRF_UART0->PSELCTS = 0xFFFFFFFF;

    uint32_t baud;
    switch(baud_rate)
    {
        case 1200: baud = UART_BAUDRATE_BAUDRATE_Baud1200; break;
        case 2400: baud = UART_BAUDRATE_BAUDRATE_Baud2400; break;
        case 4800: baud = UART_BAUDRATE_BAUDRATE_Baud4800; break;
        case 9600: baud = UART_BAUDRATE_BAUDRATE_Baud9600; break;
        case 14400: baud = UART_BAUDRATE_BAUDRATE_Baud14400; break;
        case 19200: baud = UART_BAUDRATE_BAUDRATE_Baud19200; break;
        case 28800: baud = UART_BAUDRATE_BAUDRATE_Baud28800; break;
        case 38400: baud = UART_BAUDRATE_BAUDRATE_Baud38400; break;
        case 57600: baud = UART_BAUDRATE_BAUDRATE_Baud57600; break;
        case 76800: baud = UART_BAUDRATE_BAUDRATE_Baud76800; break;
        case 115200: baud = UART_BAUDRATE_BAUDRATE_Baud115200; break;
        case 230400: baud = UART_BAUDRATE_BAUDRATE_Baud230400; break;
        case 250000: baud = UART_BAUDRATE_BAUDRATE_Baud250000; break;
        case 460800: baud = UART_BAUDRATE_BAUDRATE_Baud460800; break;
        case 921600: baud = UART_BAUDRATE_BAUDRATE_Baud921600; break;
        case 1000000: baud = UART_BAUDRATE_BAUDRATE_Baud1M; break;
        default: baud = UART_BAUDRATE_BAUDRATE_Baud115200;break;
    }
    NRF_UART0->BAUDRATE = (baud<<UART_BAUDRATE_BAUDRATE_Pos);
    NRF_UART0->CONFIG = ((UART_CONFIG_PARITY_Excluded << UART_CONFIG_PARITY_Pos) | (UART_CONFIG_HWFC_Disabled << UART_CONFIG_HWFC_Pos));

    NRF_UART0->INTENCLR = 0xFFFFFFFF;
    NRF_UART0->INTENSET = (UART_INTENSET_RXDRDY_Enabled << UART_INTENSET_RXDRDY_Pos);

    NVIC_SetPriority(UART0_IRQn, 3);
    NVIC_ClearPendingIRQ(UART0_IRQn);
    NVIC_EnableIRQ(UART0_IRQn);

    NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);

    NRF_UART0->EVENTS_RXDRDY    = 0;
    //NRF_UART0->EVENTS_TXDRDY    = 0;
    NRF_UART0->TASKS_STARTTX    = 1;
    NRF_UART0->TASKS_STARTRX    = 1;

    UART0_State = UART0_BeforeFirstTX;
}

void UART0_uninit(void)
{
    if(UART0_State == UART0_NotStart)
        return;

    NVIC_DisableIRQ(UART0_IRQn);
    // wait for any date to be sent
    if( UART0_State == UART0_AfterFirstTX )
        while(!NRF_UART0->EVENTS_TXDRDY);

    //must clear PSELTXD and PSELRXD
    NRF_UART0->PSELTXD = 0xFFFFFFFF;
    NRF_UART0->PSELRXD = 0xFFFFFFFF;
    NRF_UART0->TASKS_STOPTX = 1;
    NRF_UART0->TASKS_STOPRX = 1;
    //Disable UART and clear event
    NRF_UART0->ENABLE       = (UART_ENABLE_ENABLE_Disabled << UART_ENABLE_ENABLE_Pos);
    NRF_UART0->INTENCLR = 0xffffffffUL;

    UART0_State = UART0_NotStart;
}

void UART0_send(uint8_t dat)
{
    if(UART0_State == UART0_AfterFirstTX)
    {   // Wait for sending is ready.
        while(!NRF_UART0->EVENTS_TXDRDY);
    }
    NRF_UART0->EVENTS_TXDRDY = 0;
    NRF_UART0->TXD = dat;

    if (UART0_State == UART0_BeforeFirstTX)
        UART0_State = UART0_AfterFirstTX;
}

uint32_t UART0_readRxState(void)
{
    return NRF_UART0->EVENTS_RXDRDY;
}

void UART0_clearRxState(void)
{
    NRF_UART0->EVENTS_RXDRDY = 0;
}

uint8_t UART0_readRxData(void)
{
    return NRF_UART0->RXD;
}

void UART0_IRQHandler(void)
{
    if(UART0_handler!=NULL)
        UART0_handler();
}
