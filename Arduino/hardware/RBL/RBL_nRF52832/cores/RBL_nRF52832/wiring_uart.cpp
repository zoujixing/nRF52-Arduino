
#include "wiring_uart.h"
#include "ring_buffer.h"
#include "uart.h"

/****************************************************
*                 Local Variables
****************************************************/

static Ring_Buffer uart_rx_buf;
static rx_callback_t uart_rx_handler = NULL;

/****************************************************
*                 Function Definitions
****************************************************/
void uart_irq_handler(void)
{
    if(UART0_readRxState())
    {
        UART0_clearRxState();
        // Store byte.
        uart_rx_buf.store(UART0_readRxData());
        // When receive a byte, call rx_handler.
        if(uart_rx_handler)
            uart_rx_handler();
    }
}

UARTClass::UARTClass(void)
{
    uart_rx_buf.clear();
    UART0_registRxHandler(NULL);
    uart_rx_handler = NULL;
}

void UARTClass::begin(uint32_t baudRate)
{
    begin(baudRate, DERAULT_TX_PIN, DEFAULT_RX_PIN);
}

void UARTClass::begin(uint32_t baudRate, uint8_t tx_pin, uint8_t rx_pin)
{
    uint8_t nrf_tx_pin;
    uint8_t nrf_rx_pin;

    nrf_tx_pin = Pin_nRF51822_to_Arduino(tx_pin);
    nrf_rx_pin = Pin_nRF51822_to_Arduino(rx_pin);

    uart_rx_buf.clear();
    UART0_registRxHandler(uart_irq_handler);
    UART0_init(nrf_tx_pin, nrf_rx_pin, baudRate);
    debug_open();
}

void UARTClass::end(void)
{
    UART0_uninit();
    UART0_registRxHandler(NULL);
    uart_rx_buf.clear();
    debug_close();

    uart_rx_handler = NULL;
}

int UARTClass::available(void)
{
    return uart_rx_buf.available();
}

int UARTClass::read(void)
{
    return uart_rx_buf.read();
}

int UARTClass::peek(void)
{
    return uart_rx_buf.peek();
}

void UARTClass::flush(void)
{

}

size_t UARTClass::write(uint8_t dat)
{
    UART0_send(dat);
    return 1;
}

void UARTClass::attach(rx_callback_t handler)
{
    uart_rx_handler = handler;
}

UARTClass Serial;
