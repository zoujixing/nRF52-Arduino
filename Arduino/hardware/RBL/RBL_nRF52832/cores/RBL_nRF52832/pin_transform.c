
#include "pin_transform.h"

/****************************************************
*                 Function Definitions
****************************************************/
uint8_t Pin_nRF51822_to_Arduino(uint8_t pin)
{
    return pin;
	// uint8_t nrf_pin=0xFF;
	
	// switch(pin)
	// {	
		// case 0 : nrf_pin = 11;break;	//D0/RXD/MISO
		// case 1 : nrf_pin = 9;break;		//D1/TXD/MOSI
		// case 2 : nrf_pin = 10;break;	//D2/CTS/CS/SDA
		// case 3 : nrf_pin = 8;break;		//D3/RTS/SCK/SCL
		
		// case 4 : nrf_pin = 28;break;	//D4
		// case 5 : nrf_pin = 29;break;	//D5
		// case 6 : nrf_pin = 15;break;	//D6
		// case 7 : nrf_pin = 7;break;		//D7

		// case 13 : nrf_pin = 19;break;	//D13/LED
		
		// case 8  : nrf_pin = 1;break;	//A0
		// case 9  : nrf_pin = 2;break;	//A1
		// case 10 : nrf_pin = 3;break;	//A2
		// case 11 : nrf_pin = 4;break;	//A3
		// case 12 : nrf_pin = 5;break;	//A4
		// case 14 : nrf_pin = 6;break;	//A5

		// default : break;		
	// }

	// return (nrf_pin);
}
