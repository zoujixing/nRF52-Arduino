
#ifndef UART_H_
#define UART_H_

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

/*
 * @brief Regist uart Rx Handler.
 *
 * @param[in] handler
 */
void UART0_registRxHandler(UART0_IRQ_Handler_t handler);

/*
 * @brief Init uart
 *
 * @param[in] nrf_tx_pin  
 * @param[in] nrf_rx_pin
 * @param[in] baud_rate
 */
void UART0_init(uint8_t nrf_tx_pin, uint8_t nrf_rx_pin, uint32_t baud_rate);

/*
 * @brief DeInit uart
 */
void UART0_uninit(void);

/*
 * @brief Send one byte
 *
 * @param[in] dat
 */
void UART0_send(uint8_t dat);

/*
 * @brief Read uart rx state
 *
 * @return uart rx state
 */
uint32_t UART0_readRxState(void);

/*
 * @brief Clear uart rx state
 */
void     UART0_clearRxState(void);

/*
 * @brief Read uart rx data
 *
 * @return Data in RXD
 */
uint8_t  UART0_readRxData(void);

#ifdef __cplusplus
} 
#endif

#endif
