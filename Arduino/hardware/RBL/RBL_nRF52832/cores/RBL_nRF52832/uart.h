
#ifndef UART_H_
#define UART_H_


#include <stdint.h>
#include <string.h>
#include "device_common.h"

#ifdef __cplusplus
extern "C"{
#endif

/****************************************************
*                 Micro Definitions
****************************************************/


/****************************************************
*                 Type Definitions
****************************************************/ 
typedef enum
{
	UART0_NotStart,
	UART0_BeforeFirstTX,
	UART0_AfterFirstTX,
	
}UART0_States_t;

typedef void (*UART0_IRQ_Handler_t)(void);

/****************************************************
*                 Function Declarations
****************************************************/

void UART0_registRxHandler(UART0_IRQ_Handler_t handler);
void UART0_init(uint8_t nrf_tx_pin, uint8_t nrf_rx_pin, uint32_t baud_rate);
void UART0_uninit(void);
void UART0_send(uint8_t dat);

uint32_t UART0_readRxState(void);
void     UART0_clearRxState(void);
uint8_t  UART0_readRxData(void);

#ifdef __cplusplus
} 
#endif

#endif
