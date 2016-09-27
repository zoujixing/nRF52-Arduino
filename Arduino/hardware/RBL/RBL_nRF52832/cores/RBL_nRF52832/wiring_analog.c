
#include <math.h>
#include "wiring_analog.h"

#define PWM_POLARITY_FALLING_EDGE  0x8000 //HIGH
#define PWM_POLARITY_RISING_EDGE   0x0000 //LOW
#define PWM_POLARITY_MODE          PWM_POLARITY_FALLING_EDGE

#define PWM_MAX_CHANNELS           8
#define PWM_COUNTERTOP_VALUE       1600 //10KHz
/****************************************************
*                 Local Variables
****************************************************/
// analogRead variables
static int16_t  saadc_value;
static uint8_t  readResolution  = DEFAULT_READ_RESOLUTION;
static uint8_t  writeResolution = DEFAULT_WRITE_RESOLUTION;
static uint32_t saadReference   = SAADC_CH_CONFIG_REFSEL_VDD1_4;

// analogWrite variables
static uint8_t  pwm0_is_init = 0;
static uint8_t  pwm1_is_init = 0;
static uint8_t  pwm_ch_pin[2][4] = { {0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF}};
static uint16_t pwm_seq[2][4]    = { {0x00|PWM_POLARITY_MODE, 0x00|PWM_POLARITY_MODE, 0x00|PWM_POLARITY_MODE, 0x00|PWM_POLARITY_MODE},
                                     {0x00|PWM_POLARITY_MODE, 0x00|PWM_POLARITY_MODE, 0x00|PWM_POLARITY_MODE, 0x00|PWM_POLARITY_MODE} };

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
            saadReference = SAADC_CH_CONFIG_REFSEL_VDD1_4;
            break;
    }
}

uint32_t analogRead(uint8_t pin)
{
    uint8_t  nrf_pin;
    uint32_t saadcResolution;
    uint32_t resolution;
    uint32_t config_gain = SAADC_CH_CONFIG_GAIN_Gain1_4;
    uint32_t pselp = SAADC_CH_PSELP_PSELP_NC;

    nrf_pin = Pin_nRF51822_to_Arduino(pin);
    switch( nrf_pin ) {
        case 2:
          pselp = SAADC_CH_PSELP_PSELP_AnalogInput0;
          break;
        case 3:
          pselp = SAADC_CH_PSELP_PSELP_AnalogInput1;
          break;
        case 4:
          pselp = SAADC_CH_PSELP_PSELP_AnalogInput2;
          break;
        case 5:
          pselp = SAADC_CH_PSELP_PSELP_AnalogInput3;
          break;
        case 28:
          pselp = SAADC_CH_PSELP_PSELP_AnalogInput4;
          break;
        case 29:
          pselp = SAADC_CH_PSELP_PSELP_AnalogInput5;
          break;
        case 30:
          pselp = SAADC_CH_PSELP_PSELP_AnalogInput6;
          break;
        case 31:
          pselp = SAADC_CH_PSELP_PSELP_AnalogInput7;
          break;
        default :
          log_error("ANALOG : Wrong PSELP \r\n");
          return 0;
    }

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
    log_info("ANALOG : Resolution is %d \r\n", resolution);

    NRF_SAADC->RESOLUTION  = saadcResolution;
    // Bypass oversampling
    NRF_SAADC->OVERSAMPLE  = (SAADC_OVERSAMPLE_OVERSAMPLE_Bypass << SAADC_OVERSAMPLE_OVERSAMPLE_Pos);
    // Disable all interrupts
    NRF_SAADC->INTENCLR = 0xFFFFFFFF;
    // Clear event flag
    NRF_SAADC->EVENTS_END = 0;
    NRF_SAADC->EVENTS_DONE = 0;
    // Enable NRF_SAADC.
    NRF_SAADC->ENABLE = (SAADC_ENABLE_ENABLE_Enabled << SAADC_ENABLE_ENABLE_Pos);
    for (int i = 0; i < 8; i++) {
        NRF_SAADC->CH[i].PSELN = SAADC_CH_PSELP_PSELP_NC;
        NRF_SAADC->CH[i].PSELP = SAADC_CH_PSELP_PSELP_NC;
    }
    // Input range = (+- 0.6 V or +-VDD/4)/Gain
    // RESULT = [V(P) – V(N) ] * GAIN/REFERENCE * 2^(RESOLUTION - m)
    // If CONFIG.MODE=SE,m=0 or if CONFIG.MODE=Diff,m=1.
    if(saadReference == SAADC_CH_CONFIG_REFSEL_Internal) {
        log_info("ANALOG : Reference is Internal 0.6V, Input valtage is 0~3.6V\r\n");
        config_gain = SAADC_CH_CONFIG_GAIN_Gain1_6;
    }
    else if(saadReference == SAADC_CH_CONFIG_REFSEL_VDD1_4) {
        log_info("ANALOG : Reference is VDD \r\n");
        config_gain = SAADC_CH_CONFIG_GAIN_Gain1_4;
    }
    NRF_SAADC->CH[0].CONFIG = ( (SAADC_CH_CONFIG_RESP_Bypass << SAADC_CH_CONFIG_RESP_Pos) | //Positive channel resistor control:Bypass resistor ladder.
                                (SAADC_CH_CONFIG_RESP_Bypass << SAADC_CH_CONFIG_RESN_Pos) | //Negative channel resistor control:Bypass resistor ladder.
                                (config_gain  << SAADC_CH_CONFIG_GAIN_Pos) | //Gain control
                                (saadReference << SAADC_CH_CONFIG_REFSEL_Pos) | //Reference
                                (SAADC_CH_CONFIG_TACQ_10us << SAADC_CH_CONFIG_TACQ_Pos) | //Acquisition time:ADC uses to sample the input voltage.
                                (SAADC_CH_CONFIG_MODE_SE << SAADC_CH_CONFIG_MODE_Pos) |  //Single ended, PSELN will be ignored, negative input to ADC shorted to GND.
                                (SAADC_CH_CONFIG_BURST_Disabled << SAADC_CH_CONFIG_BURST_Pos) //Disable burst mode(normal operation).
                               );
    NRF_SAADC->CH[0].PSELN = SAADC_CH_PSELP_PSELP_NC;
    NRF_SAADC->CH[0].PSELP = pselp;
    // Set DMA RAM.
    saadc_value = 0;
    NRF_SAADC->RESULT.PTR = (uint32_t)&saadc_value;
    NRF_SAADC->RESULT.MAXCNT = 1; //Only one sample.
    // Start SAADC.
    log_info("ANALOG : Tasks start \r\n");
    NRF_SAADC->TASKS_START = 1;
    while(!NRF_SAADC->EVENTS_STARTED);
    NRF_SAADC->EVENTS_STARTED = 0;
    // Start sample.
    NRF_SAADC->TASKS_SAMPLE = 1;
    while(!NRF_SAADC->EVENTS_END);
    NRF_SAADC->EVENTS_END = 0;
    // Stop SAADC.
    NRF_SAADC->TASKS_STOP = 1;
    while(!NRF_SAADC->EVENTS_STOPPED);
    NRF_SAADC->EVENTS_STOPPED = 0;

    log_info("ANALOG : Sample saadc_value %d \r\n", saadc_value);
    if(saadc_value < 0)
        saadc_value = 0;
    // Disable SAADC.
    NRF_SAADC->ENABLE = (SAADC_ENABLE_ENABLE_Disabled << SAADC_ENABLE_ENABLE_Pos);

    return conversionResolution(saadc_value, resolution, readResolution);
}

void pwm_init(void)
{
    pwm0_is_init = 0;
    pwm1_is_init = 0;
    memset(pwm_ch_pin, 0x00, sizeof(pwm_ch_pin));
    memset(pwm_seq, 0x00|PWM_POLARITY_MODE, sizeof(pwm_seq)/sizeof(uint16_t));
}

static uint8_t pwm_enable(NRF_PWM_Type *pwm)
{
    if( (pwm != NRF_PWM0) && (pwm != NRF_PWM1) )
        return 1;

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
    pwm->PRESCALER  = (PWM_PRESCALER_PRESCALER_DIV_1 << PWM_PRESCALER_PRESCALER_Pos);
    // Enable pwm
    pwm->ENABLE     = (PWM_ENABLE_ENABLE_Enabled << PWM_ENABLE_ENABLE_Pos);
    // PWM period = 1600 * 1/16000000 Hz = 100us = 0.1ms(10KHz)
    pwm->COUNTERTOP = (PWM_COUNTERTOP_VALUE << PWM_COUNTERTOP_COUNTERTOP_Pos);
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
    if(pwm == NRF_PWM0)
        pwm->SEQ[0].PTR = (((uint32_t)(&pwm_seq[0][0])) << PWM_SEQ_PTR_PTR_Pos);
    else
        pwm->SEQ[0].PTR = (((uint32_t)(&pwm_seq[1][0])) << PWM_SEQ_PTR_PTR_Pos);
    pwm->SEQ[0].CNT = (4 << PWM_LOOP_CNT_Pos);
    //
    pwm->SEQ[0].REFRESH  = 1;
    pwm->SEQ[0].ENDDELAY = 0;

    return 0;
}

void analogWrite(uint8_t pin, uint32_t value)
{
    uint8_t  index, pwm_num, pwm_ch_num;
    uint8_t  nrf_pin;
    uint16_t temp_value, update_value, temp;

    nrf_pin = Pin_nRF51822_to_Arduino(pin);
    if(nrf_pin >= 31)
        return;
    // PWM compare max value is PWM_COUNTERTOP_VALUE
    temp = (pow(2, DEFAULT_WRITE_RESOLUTION) - 1);
    temp_value = conversionResolution(value, writeResolution, DEFAULT_WRITE_RESOLUTION);
    update_value = PWM_COUNTERTOP_VALUE*temp_value/temp;
    log_info("ANALOG : xxxx 0x%x \r\n", temp);
    log_info("ANALOG : nrf_pin %d \r\n", nrf_pin);
    log_info("ANALOG : update_value 0x%x \r\n", update_value);
    for(index=0; index<PWM_MAX_CHANNELS; index++) {
        pwm_num = index/4;
        pwm_ch_num = index%4;
        if(nrf_pin == pwm_ch_pin[pwm_num][pwm_ch_num]) {
            log_info("ANALOG : Find a exist pin \r\n");
            pwm_seq[pwm_num][pwm_ch_num] = update_value|PWM_POLARITY_MODE;
            if(pwm_num ==0) {
                NRF_PWM0->TASKS_SEQSTART[0] = 1;
                while(NRF_PWM0->EVENTS_SEQSTARTED[0] == 0);
                NRF_PWM0->EVENTS_SEQSTARTED[0] = 0;
            }
            else {
                NRF_PWM1->TASKS_SEQSTART[0] = 1;
                while(NRF_PWM1->EVENTS_SEQSTARTED[0] == 0);
                NRF_PWM1->EVENTS_SEQSTARTED[0] = 0;
            }
            return;
        }
    }
    //No exist, need to initialize
    for(index=0; index<PWM_MAX_CHANNELS; index++) {
        pwm_num = index/4;
        pwm_ch_num = index%4;
        if(pwm_ch_pin[pwm_num][pwm_ch_num] == 0xFF) {
            log_info("ANALOG : Find a free channel \r\n");
            pwm_ch_pin[pwm_num][pwm_ch_num] = nrf_pin;
            pwm_seq[pwm_num][pwm_ch_num] = update_value|PWM_POLARITY_MODE;
            if(pwm_num==0) {
                log_info("ANALOG : PWM0 channel %d \r\n", pwm_ch_num);
                if(!pwm0_is_init) {
                    log_info("ANALOG : Init PWM0 \r\n");
                    pwm_enable(NRF_PWM0);
                    pwm0_is_init = 1;
                }
                NRF_PWM0->PSEL.OUT[pwm_ch_num] = ((nrf_pin << PWM_PSEL_OUT_PIN_Pos) |
                                                  (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos));
                NRF_PWM0->TASKS_SEQSTART[0] = 1;
                while(NRF_PWM0->EVENTS_SEQSTARTED[0] == 0);
                NRF_PWM0->EVENTS_SEQSTARTED[0] = 0;
            }
            else {
                log_info("ANALOG : PWM1 channel %d \r\n", pwm_ch_num);
                if(!pwm1_is_init) {
                    log_info("ANALOG : Init PWM1 \r\n");
                    pwm_enable(NRF_PWM1);
                    pwm1_is_init = 1;
                }
                NRF_PWM1->PSEL.OUT[pwm_ch_num] = ((nrf_pin << PWM_PSEL_OUT_PIN_Pos) |
                                                  (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos));
                NRF_PWM1->TASKS_SEQSTART[0] = 1;
                while(NRF_PWM1->EVENTS_SEQSTARTED[0] == 0);
                NRF_PWM1->EVENTS_SEQSTARTED[0] = 0;
            }
            return;
        }
    }
    log_error("ANALOG : More than 8 channels \r\n");
}
