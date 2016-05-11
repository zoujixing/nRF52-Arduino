
#include "wiring_analog.h"

/****************************************************
*                 Local Variables
****************************************************/
// analogRead variables
static uint8_t  readResolution  = DEFAULT_READ_RESOLUTION;
static uint8_t  writeResolution = DEFAULT_WRITE_RESOLUTION;
static uint32_t saadReference   = SAADC_CH_CONFIG_REFSEL_Internal;

// analogWrite variables
//static uint8_t  pwm0_ch_pin[4] = {0xFF, 0xFF, 0xFF, 0xFF};
//static uint8_t  pwm1_ch_pin[4] = {0xFF, 0xFF, 0xFF, 0xFF};

//static uint16_t pwm0_seq[4] = {0, 0, 0, 0};
//static uint16_t pwm1_seq[4] = {0, 0, 0, 0};


/****************************************************
*                 Function Definitions
****************************************************/ 
static inline uint32_t conversionResolution(uint32_t value, uint32_t from, uint32_t to)
{
    if (from == to)
        return value;
    if (from > to)
        return value >> (from-to);
    else
        return value << (to-from);    
}

void analogReadResolution(uint8_t resolution)
{
    readResolution = resolution;
}

void analogWriteResolution(uint8_t resolution)
{
    writeResolution = resolution;
}

void analogReference(eAnalogReference_t type)
{
    switch(type) {
        case REFSEL_INTERNAL:
            saadReference = SAADC_CH_CONFIG_REFSEL_Internal;
            break;
        case REFSEL_VDD1_4:
            saadReference = SAADC_CH_CONFIG_REFSEL_VDD1_4;
            break;
        default:
            saadReference = SAADC_CH_CONFIG_REFSEL_Internal;
            break;
    }
}

uint32_t analogRead(uint8_t pin)
{
    uint8_t  nrf_pin;
    uint32_t saadcResolution;
    uint32_t resolution;
    int32_t  value;
    
    nrf_pin = Pin_nRF51822_to_Arduino(pin);
    if(nrf_pin > 7)
        return 0;
    
    log_info("ANALOG : Analog read  \r\n");
    if(readResolution <= 8) {
        resolution = 8;
        saadcResolution = SAADC_RESOLUTION_VAL_8bit;
    }
    else if(readResolution <= 10) {
        resolution = 10;
        saadcResolution = SAADC_RESOLUTION_VAL_10bit;
    }
    else if(readResolution <= 12) {
        resolution = 12;
        saadcResolution = SAADC_RESOLUTION_VAL_12bit;
    }   
    else {
        resolution = 14;
        saadcResolution = SAADC_RESOLUTION_VAL_14bit;
    }
    log_info("ANALOG : Resolution is %d \r\n", &resolution);
    
    NRF_SAADC->RESOLUTION  = saadcResolution;
    
    NRF_SAADC->CH[nrf_pin].CONFIG = ( (SAADC_CH_CONFIG_RESP_Bypass << SAADC_CH_CONFIG_RESP_Pos) | //Positive channel resistor control:Bypass resistor ladder.
                                      (SAADC_CH_CONFIG_RESN_Bypass << SAADC_CH_CONFIG_RESN_Pos) | //Negative channel resistor control:Bypass resistor ladder.
                                      (SAADC_CH_CONFIG_GAIN_Gain1  << SAADC_CH_CONFIG_GAIN_Pos) | //Gain control
                                      (saadReference << SAADC_CH_CONFIG_REFSEL_Pos) | //Reference
                                      (SAADC_CH_CONFIG_TACQ_3us << SAADC_CH_CONFIG_TACQ_Pos) | //Acquisition time:ADC uses to sample the input voltage.
                                      (SAADC_CH_CONFIG_MODE_SE << SAADC_CH_CONFIG_MODE_Pos) |  //Single ended, PSELN will be ignored, negative input to ADC shorted to GND.
                                      (SAADC_CH_CONFIG_BURST_Disabled << SAADC_CH_CONFIG_BURST_Pos) //Disable burst mode(normal operation).
                                    );
    NRF_SAADC->CH[nrf_pin].PSELN = nrf_pin;
    NRF_SAADC->CH[nrf_pin].PSELP = nrf_pin;    
    // Enable NRF_SAADC.
    log_info("ANALOG : Enable SAADC \r\n");
    NRF_SAADC->ENABLE = (SAADC_ENABLE_ENABLE_Enabled << SAADC_ENABLE_ENABLE_Pos);
    // Set DMA RAM.
    NRF_SAADC->RESULT.PTR = (uint32_t)&value;
    NRF_SAADC->RESULT.MAXCNT = 1; //Only one sample.
    // Start SAADC.
    log_info("ANALOG : Tasks start \r\n");
    NRF_SAADC->TASKS_START = 1;
    // Wait EVENTS_STARTED = 1.
    while(!NRF_SAADC->EVENTS_STARTED);
    NRF_SAADC->EVENTS_STARTED = 0;
    // Start sample.
    log_info("ANALOG : Start sample \r\n");
    NRF_SAADC->TASKS_SAMPLE = 1;
    while(!NRF_SAADC->EVENTS_END);
    NRF_SAADC->EVENTS_END = 0;
    // Stop SAADC.
    log_info("ANALOG : Tasks stop \r\n");
    NRF_SAADC->TASKS_STOP = 1;
    while(!NRF_SAADC->EVENTS_STOPPED);
    NRF_SAADC->EVENTS_STOPPED = 0;
    
    if(value < 0)
        value = 0;
    // Disable SAADC.
    log_info("ANALOG : Disable SAADC \r\n");
    NRF_SAADC->ENABLE = (SAADC_ENABLE_ENABLE_Disabled << SAADC_ENABLE_ENABLE_Pos);
    
    return conversionResolution(value, resolution, readResolution);
}

void analogWrite(uint8_t pin, uint32_t value)
{
    //uint8_t  nrf_pin;
    
    //nrf_pin = Pin_nRF51822_to_Arduino(pin);
    
}




