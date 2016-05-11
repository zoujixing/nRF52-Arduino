
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

#define PI 				3.1415926535897932384626433832795
#define HALF_PI 		1.5707963267948966192313216916398
#define TWO_PI 			6.283185307179586476925286766559
#define DEG_TO_RAD 		0.017453292519943295769236907684886
#define RAD_TO_DEG 		57.295779513082320876798154814105


#define VARIANT_MCK         		 64000000UL	

#define clockCyclesPerMicrosecond()  ( VARIANT_MCK / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )



#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif // min

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif // max

#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() __enable_irq()
#define noInterrupts() __disable_irq()

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define bit(b) (1UL << (b))

#define countof(x)  (sizeof(x)/sizeof(x[0]))

typedef unsigned int word;
typedef uint8_t boolean;
typedef uint8_t byte;

enum BitOrder {
	LSBFIRST = 0,
	MSBFIRST = 1
};


#ifdef __cplusplus
} 
#endif

#endif
