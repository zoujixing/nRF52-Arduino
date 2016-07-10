
#ifndef SERVO_H_
#define SERVO_H_

#include "Arduino.h"

#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL    20000     // minumim time to refresh servos in microseconds 

#define MAX_SERVOS              4
#define INVALID_SERVO         255     // flag indicating an invalid servo index

/**
 * @brief Pin of Servo structures type
 */
typedef struct  {
  uint8_t nbr        :6 ;             // a pin number from 0 to 63
  uint8_t isActive   :1 ;             // true if this channel is enabled, pin not pulsed if false 
} ServoPin_t;

/**
 * @brief Servo parameter structures type
 */
typedef struct {
  ServoPin_t Pin;
  uint16_t ticks;
} servo_t;

class Servo
{
public:
    /**
     * @brief Servo constructor
     */
    Servo(void);
    
    /**
     * @brief Attach the given pin to the a free channel
     *
     * @params[in]  pin
     *
     * @return   0 fail or channel number
     */
    uint8_t attach(uint8_t pin);
    
    /**
     * @brief Attach the given pin to the a free channel, also sets min and max values for writes
     *
     * @params[in]  pin
     * @params[in]  min
     * @params[in]  max
     *
     * @return   0 fail or channel number
     */
    uint8_t attach(uint8_t pin, uint16_t min, uint16_t max);
    
    /**
     * @brief Detach servo pin and channel
     */
    void detach(void);
    
    /**
     * @brief Set a pulse of servo
     *
     * @params[in]  value  If value is < 200 its treated as an angle, otherwise as pulse width in microseconds
     *                     Range:0~180 or MIN_PULSE_WIDTH~MAX_PULSE_WIDTH
     */
    void write(uint16_t value);
    
    /**
     * @brief Set a pulse(in microseconds) of servo
     *
     * @params[in]  value  In microseconds, range:MIN_PULSE_WIDTH~MAX_PULSE_WIDTH
     */    
    void writeMicroseconds(uint16_t value);
    
    /**
     * @brief Read the pulse of servo as an angle between 0 and 180 degrees
     *
     * @return The current angle
     */        
    int read(void);
    
    /**
     * @brief Read the pulse of servo in microseconds
     *
     * @return The current angle
     */       
    int readMicroseconds(void);
    
    /**
     * @brief Check whether current servo is attached
     *
     * @return true or false
     */        
    bool attached(void);

private:
    uint8_t servoIndex;
    uint16_t min;
    uint16_t max;
};

#endif
