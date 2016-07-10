
#include "Servo.h"

#define SERVO_PWMx                 NRF_PWM2

#define PWM_POLARITY_FALLING_EDGE  0x8000 //HIGH
#define PWM_POLARITY_RISING_EDGE   0x0000 //LOW
#define PWM_POLARITY_MODE          PWM_POLARITY_FALLING_EDGE

#define PWM_CHANNEL0_ENABLE        0x01
#define PWM_CHANNEL1_ENABLE        0x02
#define PWM_CHANNEL2_ENABLE        0x04
#define PWM_CHANNEL3_ENABLE        0x08
#define PWM_CHANNEL_ALL_ENABLE     0x0F

#define MIN_TICKS                  125
#define MAX_TICKS                  625

#define usToTicks(_us)            (map(_us, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, MIN_TICKS, MAX_TICKS))      // converts microseconds to tick 
#define ticksToUs(_ticks)         (map(_ticks, MIN_TICKS, MAX_TICKS, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH))   // converts from ticks back to microseconds

static uint8_t pwm_is_enable      = 0;
static uint8_t pwm_channel_status = 0;
static uint8_t ServoCount         = 0;  // The total number of attached servos                     
static servo_t servos[MAX_SERVOS];      // static array of servo structures               

static uint16_t pwm_seq[MAX_SERVOS] = {0x00|PWM_POLARITY_MODE, 0x00|PWM_POLARITY_MODE,
                                       0x00|PWM_POLARITY_MODE, 0x00|PWM_POLARITY_MODE};

                                       
static uint8_t pwm_init(NRF_PWM_Type *pwm)
{ 
    // Disable all pin select
    pwm->PSEL.OUT[0] = (PWM_PSEL_OUT_CONNECT_Disconnected << PWM_PSEL_OUT_CONNECT_Pos);
    pwm->PSEL.OUT[1] = (PWM_PSEL_OUT_CONNECT_Disconnected << PWM_PSEL_OUT_CONNECT_Pos);
    pwm->PSEL.OUT[2] = (PWM_PSEL_OUT_CONNECT_Disconnected << PWM_PSEL_OUT_CONNECT_Pos);
    pwm->PSEL.OUT[3] = (PWM_PSEL_OUT_CONNECT_Disconnected << PWM_PSEL_OUT_CONNECT_Pos);
    // Up counter - edge aligned PWM duty-cycle
    pwm->MODE = (PWM_MODE_UPDOWN_Up << PWM_MODE_UPDOWN_Pos);
    // DIV_1 0 Divide by 1 (16MHz)
    // DIV_2 1 Divide by 2 ( 8MHz)
    // DIV_4 2 Divide by 4 ( 4MHz)
    // DIV_8 3 Divide by 8 ( 2MHz)
    // DIV_16 4 Divide by 16 ( 1MHz)
    // DIV_32 5 Divide by 32 ( 500kHz)
    // DIV_64 6 Divide by 64 ( 250kHz)
    // DIV_128 7 Divide by 128 ( 125kHz)
    pwm->PRESCALER  = (PWM_PRESCALER_PRESCALER_DIV_64 << PWM_PRESCALER_PRESCALER_Pos);
    // Enable pwm
    pwm->ENABLE     = (PWM_ENABLE_ENABLE_Enabled << PWM_ENABLE_ENABLE_Pos);
    // PWM period = 5000 * 1/250000 Hz = 20ms(50Hz)
    // Servo 125(0.5ms) ~ 625(2.5ms)
    pwm->COUNTERTOP = (5000 << PWM_COUNTERTOP_COUNTERTOP_Pos);
    pwm->LOOP       = (PWM_LOOP_CNT_Disabled << PWM_LOOP_CNT_Pos);
    // 1st half word (16-bit) in ch.0; 2nd in ch.1; ...; 4th in ch.3
    // SEQ[n].REFRESH is used to determine loading internal compare registers
    pwm->DECODER    = ( (PWM_DECODER_LOAD_Individual << PWM_DECODER_LOAD_Pos) |
                        (PWM_DECODER_MODE_RefreshCount << PWM_DECODER_MODE_Pos) );                         
    // Clear event flags
    pwm->EVENTS_LOOPSDONE = 0;
    pwm->EVENTS_SEQEND[0] = 0;
    pwm->EVENTS_SEQEND[1] = 0;
    pwm->EVENTS_STOPPED   = 0;
    //   
    pwm->SEQ[0].PTR = (((uint32_t)(pwm_seq)) << PWM_SEQ_PTR_PTR_Pos);
    pwm->SEQ[0].CNT = (4 << PWM_LOOP_CNT_Pos);
    //
    pwm->SEQ[0].REFRESH  = 1;
    pwm->SEQ[0].ENDDELAY = 0;
    
    return 0;    
}

static uint8_t pwm_add(NRF_PWM_Type *pwm, uint8_t ch, uint8_t pin, uint16_t value)
{
    if(ch >= 4)
        return 1;
    // Config pin select
    pwm->PSEL.OUT[ch] = ((pin << PWM_PSEL_OUT_PIN_Pos) |
                         (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos));
    // Set pwm compare value
    pwm_seq[ch] = value|PWM_POLARITY_MODE;
    return 0;
}

static uint8_t pwm_update(uint8_t ch, uint16_t value)
{
    if(ch >= 4)
        return 1;
    // Set pwm compare value
    pwm_seq[ch] = value|PWM_POLARITY_MODE;
    return 0;
}

static uint8_t pwm_delete(NRF_PWM_Type *pwm, uint8_t ch)
{
    if(ch >= 4)
        return 1;
    // Release pin select
    pwm->PSEL.OUT[ch] = (PWM_PSEL_OUT_CONNECT_Disconnected << PWM_PSEL_OUT_CONNECT_Pos);
    pwm_seq[ch] = 0x00|PWM_POLARITY_MODE;   
    return 0;
}

static uint8_t pwm_taskRestart(NRF_PWM_Type *pwm)
{
    // Restart task to update channel pulse
    pwm->TASKS_SEQSTART[0] = 1;
    nrf_delay_us(300);
    //while(pwm->EVENTS_SEQSTARTED[0] == 0);
    //pwm->EVENTS_SEQSTARTED[0] = 0;
    return 0;
}

static uint8_t pwm_deInit(NRF_PWM_Type *pwm)
{
    pwm->ENABLE = (PWM_ENABLE_ENABLE_Disabled << PWM_ENABLE_ENABLE_Pos);
    return 0;
}

Servo::Servo(void)
{
    if(ServoCount < MAX_SERVOS) {
        // assign a servo index to this instance
		this->servoIndex = ServoCount++;
        servos[this->servoIndex].ticks = map(DEFAULT_PULSE_WIDTH, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, MIN_TICKS, MAX_TICKS);
    }
    else {
        this->servoIndex = INVALID_SERVO;
    }
}

uint8_t Servo::attach(uint8_t pin)
{
    return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(uint8_t pin, uint16_t min, uint16_t max)
{
    if(this->servoIndex < MAX_SERVOS) {
        // Set servo pin to output and save
        pinMode(pin, OUTPUT);
        servos[this->servoIndex].Pin.nbr = pin;
        
        this->min = min;
        this->max = max;
        // If no init pwm module, init first
        if(!pwm_is_enable) {
            //log_info("Servo : Init pwm module \r\n");
            pwm_init(SERVO_PWMx);
            pwm_is_enable = 1;
        }
        // Config pin select and compare ticks
        uint8_t nrf_pin = Pin_nRF51822_to_Arduino(pin);
        pwm_add(SERVO_PWMx, this->servoIndex, nrf_pin, servos[this->servoIndex].ticks);
        pwm_channel_status |= (1 << this->servoIndex);
        // Restart task to update pulse
        //log_info("Servo : restart task \r\n");
        pwm_taskRestart(SERVO_PWMx);
        servos[this->servoIndex].Pin.isActive = true;
    }
    return this->servoIndex; 
}

void Servo::detach(void)
{
    servos[this->servoIndex].Pin.isActive = false;
    // Remove from pwm
    pwm_delete(SERVO_PWMx, this->servoIndex);
    // If all servo is detached, disable pwm
    pwm_channel_status &= ~(1 << this->servoIndex);
    if(pwm_channel_status == 0x00 && pwm_is_enable) {
        //log_info("Servo : deInit pwm \r\n");
        pwm_deInit(SERVO_PWMx);
        pwm_is_enable = 0;
    }  
}

void Servo::write(uint16_t value)
{
    if(value < MIN_PULSE_WIDTH) {
        if(value < 0)
            value = 0;
        else if(value > 180)
            value = 180;
        // Change angle to pulse time        
        //log_info("Servo : angle to pulse time \r\n");
        //log_info("Servo : this->min %d \r\n", this->min);
        //log_info("Servo : this->max %d \r\n", this->max);
        //log_info("Servo : angle %d \r\n", value);
        value = map(value, 0, 180, this->min, this->max);
        //log_info("Servo : pulse time %d \r\n", value);
    }
    writeMicroseconds(value);
}

void Servo::writeMicroseconds(uint16_t value)
{
    byte channel = this->servoIndex;
    if(channel < MAX_SERVOS) {
        if(value < this->min)
            value = this->min;
        else if(value > this->max)
            value = this->max;
        // Convert to ticks after compensating for interrupt overhead
        value = map(value, this->min, this->max, MIN_TICKS, MAX_TICKS);
        //log_info("Servo : ticks %d \r\n", value);
        // Save and refresh
        servos[channel].ticks = value;
        pwm_update(channel, value);
        pwm_taskRestart(SERVO_PWMx);
    }
}

int Servo::read(void)
{
    // return the value as degrees
    return map(readMicroseconds()+1, this->min, this->max, 0, 180);    
}

int Servo::readMicroseconds(void)
{
    unsigned int pulsewidth;
    if(this->servoIndex != INVALID_SERVO) 
        pulsewidth = map(servos[this->servoIndex].ticks, MIN_TICKS, MAX_TICKS, this->min, this->max);
    else
        pulsewidth = 0;
    
    return pulsewidth;
}

bool Servo::attached(void)
{
    return servos[this->servoIndex].Pin.isActive;
}

