
#include "tone.h"
#include "app_util_platform.h"

#include "nrf_soc.h"
#include "nrf_sdm.h"
/****************************************************
*                 Local Variables
****************************************************/

static uint8_t  finish_flag;
static uint8_t  tone_pin;
static uint32_t inter_count;

/****************************************************
*                 Function Definitions
****************************************************/
void tone(uint8_t pin, uint16_t freq, uint32_t duration)
{
    uint8_t nrf_pin;
    uint32_t compare, prescaler;

    nrf_pin = Pin_nRF51822_to_Arduino(pin);
    if(nrf_pin >= 31)
        return;

    log_info("TONE : Start a tone \r\n");
    // Find appropriate values for PRESCALER and COMPARE registers
    uint8_t index;
    for (index=0; index<= 9; index++)
    {
        prescaler = index;
        compare = 16000000UL / freq;
        compare = compare >> (prescaler+1);
        compare = compare - 1;
        if ((compare >= 2) && (compare <= 65535))
            break;
    }
    log_info("TONE : The prescaler is %d \r\n", prescaler);
    log_info("TONE : The compare is %d \r\n", compare);
    // Check duration
    if(duration > 0) {
        finish_flag = 1;
        inter_count = ((freq * duration) / 1000) * 2;
    }
    else {
        finish_flag = 0;
        inter_count = 0xFFFFFFFF;
    }
    // Config GPIOTE task out.
    NRF_GPIOTE->CONFIG[TONE_USED_GPIOTE_NUM] &= ~( GPIOTE_CONFIG_MODE_Msk | GPIOTE_CONFIG_POLARITY_Msk);
    NRF_GPIOTE->CONFIG[TONE_USED_GPIOTE_NUM] = ( (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos) |
                                                 (nrf_pin << GPIOTE_CONFIG_PSEL_Pos) |
                                                 (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos) |  //Task toggle
                                                 (GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos) //Inital value LOW
                                               );
#if defined(SOFTDEVICE_PRESENT)
    // Check whether softdevice is enbale.
    uint8_t  softdevice_enabled;
    uint32_t error_code;
    sd_softdevice_is_enabled(&softdevice_enabled);
    if(softdevice_enabled == 0) {
        log_info("TONE : Softdevice is disable, config PPI \r\n");
        NRF_PPI->CH[TONE_USED_PPI_CHANNAL].EEP = (uint32_t)(&TONE_USED_TIMER->EVENTS_COMPARE[0]);
        NRF_PPI->CH[TONE_USED_PPI_CHANNAL].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[TONE_USED_GPIOTE_NUM]);
        NRF_PPI->CHEN |= (1 << TONE_USED_PPI_CHANNAL);
    }
    else {
        log_info("TONE : Softdevice is enable, config PPI \r\n");
        error_code = sd_ppi_channel_assign(TONE_USED_PPI_CHANNAL, &TONE_USED_TIMER->EVENTS_COMPARE[0], &NRF_GPIOTE->TASKS_OUT[TONE_USED_GPIOTE_NUM]);
        APP_ERROR_CHECK(error_code);
        error_code = sd_ppi_channel_enable_set(1 << TONE_USED_PPI_CHANNAL);
        APP_ERROR_CHECK(error_code);
    }
#else
    log_info("TONE : Softdevice is not used, config PPI \r\n");
    NRF_PPI->CH[TONE_USED_PPI_CHANNAL].EEP = (uint32_t)(&TONE_USED_TIMER->EVENTS_COMPARE[0]);
    NRF_PPI->CH[TONE_USED_PPI_CHANNAL].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[TONE_USED_GPIOTE_NUM]);
    NRF_PPI->CHEN |= (1 << TONE_USED_PPI_CHANNAL);
#endif

    log_info("TONE : Init TIMIERx \r\n");
    // Configure TIMERx
    TONE_USED_TIMER->TASKS_STOP = 1;
    TONE_USED_TIMER->TASKS_CLEAR = 1;

    TONE_USED_TIMER->MODE = TIMER_MODE_MODE_Timer;
    TONE_USED_TIMER->PRESCALER = prescaler;
    TONE_USED_TIMER->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
    TONE_USED_TIMER->SHORTS = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);

    TONE_USED_TIMER->CC[0] = (uint16_t)(compare);
    TONE_USED_TIMER->EVENTS_COMPARE[0] = 0;

    TONE_USED_TIMER->INTENCLR = 0xFFFFFFFF;
    TONE_USED_TIMER->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos);
    // Enable IRQn
    NVIC_SetPriority(TONE_USED_TIMER_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_ClearPendingIRQ(TONE_USED_TIMER_IRQn);
    NVIC_EnableIRQ(TONE_USED_TIMER_IRQn);
    // Start TIMER
    log_info("TONE : Start TIMIERx \r\n");
    TONE_USED_TIMER->TASKS_START = 1;
}

void tone(uint8_t pin, uint16_t freq)
{
    tone(pin, freq, 0);
}

void noTone(uint8_t pin)
{
    uint8_t nrf_pin;

    nrf_pin = Pin_nRF51822_to_Arduino(pin);
    if(nrf_pin >= 31)
        return;

    if(nrf_pin != tone_pin)
        return;
    // Stop TIMER
    TONE_USED_TIMER->TASKS_STOP = 1;
    NVIC_DisableIRQ(TONE_USED_TIMER_IRQn);

#if defined(SOFTDEVICE_PRESENT)
    uint8_t softdevice_enabled;
    sd_softdevice_is_enabled(&softdevice_enabled);
    if(softdevice_enabled == 0) {
        NRF_PPI->CHEN &= (1 << TONE_USED_PPI_CHANNAL);
    }
    else {
        sd_ppi_channel_enable_clr(1 << TONE_USED_PPI_CHANNAL);
    }
#else
    NRF_PPI->CHEN &= (1 << TONE_USED_PPI_CHANNAL);
#endif
    // Disable GPIOTE
    NRF_GPIOTE->CONFIG[TONE_USED_GPIOTE_NUM] &= ~( GPIOTE_CONFIG_MODE_Msk | GPIOTE_CONFIG_POLARITY_Msk);
    // Reset variables
    tone_pin    = 0xFF;
    finish_flag = 0;
    inter_count = 0;
}

#ifdef __cplusplus
extern "C"{
#endif

void TONE_USED_TIMER_IRQHandler(void)
{
    //log_info("TONE : TIMERx IRQ \r\n");
    TONE_USED_TIMER->EVENTS_COMPARE[0] = 0;
    if(finish_flag == 1) {
        if(inter_count)
            inter_count--;
        else
            noTone(tone_pin);
    }
}

#ifdef __cplusplus
}
#endif
