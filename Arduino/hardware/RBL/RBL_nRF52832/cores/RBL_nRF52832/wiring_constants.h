
#ifndef WIRING_CONSTANTS_H_
#define WIRING_CONSTANTS_H_

#ifdef __cplusplus
extern "C"{
#endif

#define INPUT           0x0
#define OUTPUT          0x1
#define INPUT_PULLUP    0x2

#define INPUT_NOPULL    0x3
#define INPUT_PULLDOWN  0x4
#define OUTPUT_S0S1     0x5
#define OUTPUT_H0S1     0x6
#define OUTPUT_S0H1     0x7
#define OUTPUT_H0H1     0x8
#define OUTPUT_D0S1     0x9
#define OUTPUT_D0H1     0xA
#define OUTPUT_S0D1     0xB
#define OUTPUT_H0D1     0xC

//#define true          0x01
//#define false         0x00

#define LOW             0
#define HIGH            1
#define CHANGE          2
#define FALLING         3
#define RISING          4


typedef unsigned int word;
typedef uint8_t boolean ;
typedef uint8_t byte ;

#ifdef __cplusplus
} 
#endif

#endif
