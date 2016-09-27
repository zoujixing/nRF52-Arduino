
#include "interrupts.h"

//Used GPIOTE channals : 0~4
#define INTERRUPTS_MAX_NUMBER 5

/****************************************************
*                 Local Variables
****************************************************/

static uint8_t enable = 0;
static uint8_t interrupt_pin[INTERRUPTS_MAX_NUMBER];
static interruptCB_t interrupt_callback[INTERRUPTS_MAX_NUMBER];

/****************************************************
*                 Function Definitions
****************************************************/
static void init(void)
{
    uint8_t index;

    memset(interrupt_pin, 0xFF, INTERRUPTS_MAX_NUMBER);
    for(index=0; index<INTERRUPTS_MAX_NUMBER; index++) {
        interrupt_callback[index] = NULL;
    }
    enable = 1;
    log_info("INTERRUPT : Init Interrupts \r\n");
}

void attachInterrupt(uint8_t pin, interruptCB_t callback, uint8_t mode)
{
    uint8_t nrf_pin;
    uint32_t polarity;

    nrf_pin = Pin_nRF51822_to_Arduino(pin);
    if(nrf_pin >= 31)
        return;
    // Init once.
    if(!enable)
        init();

    if(mode == RISING)
        polarity = GPIOTE_CONFIG_POLARITY_LoToHi;
    else if(mode == FALLING)
        polarity = GPIOTE_CONFIG_POLARITY_HiToLo;
    else if(mode == CHANGE)
        polarity = GPIOTE_CONFIG_POLARITY_Toggle;
    else
        return;

    uint8_t ch;
    for(ch=0; ch<INTERRUPTS_MAX_NUMBER; ch++) {
        if((interrupt_pin[ch] == 0xFF) || (interrupt_pin[ch] == nrf_pin)) {
            // Save nrf_pin and callback.
            interrupt_pin[ch] = nrf_pin;
            interrupt_callback[ch] = callback;
            log_info("INTERRUPT : The GPIOTE channel is %d \r\n", ch);

            // Config GPIOTE.
            NRF_GPIOTE->CONFIG[ch] &= ~(GPIOTE_CONFIG_PSEL_Msk | GPIOTE_CONFIG_MODE_Msk | GPIOTE_CONFIG_POLARITY_Msk);
            NRF_GPIOTE->CONFIG[ch] = ( (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos) |
                                        (nrf_pin << GPIOTE_CONFIG_PSEL_Pos) |
                                        (polarity << GPIOTE_CONFIG_POLARITY_Pos)
                                     );
            log_info("INTERRUPT : Enable IRQn \r\n");
            // Enable interrupter for current GPIOTE channel.
            NRF_GPIOTE->INTENSET = (1 << ch);
            // Enable GPIOTE IRQn.
            NVIC_SetPriority(GPIOTE_IRQn, 1);
            NVIC_EnableIRQ(GPIOTE_IRQn);
            break;
        }
    }
    if(ch >= INTERRUPTS_MAX_NUMBER)
        log_info("INTERRUPT : No free channel \r\n");
}

void detachInterrupt(uint8_t pin)
{
    uint8_t nrf_pin;

    nrf_pin = Pin_nRF51822_to_Arduino(pin);
    if(nrf_pin >= 31)
        return;

    for(uint8_t ch=0; ch<INTERRUPTS_MAX_NUMBER; ch++) {
        if(interrupt_pin[ch] == nrf_pin) {
            interrupt_pin[ch] = 0xFF;
            interrupt_callback[ch]  = NULL;
            // Clean configuration.
            NRF_GPIOTE->CONFIG[ch] &= ~(GPIOTE_CONFIG_PSEL_Msk | GPIOTE_CONFIG_MODE_Msk | GPIOTE_CONFIG_POLARITY_Msk);
            NRF_GPIOTE->INTENCLR = (1 << ch);

            break;
        }
    }
}

void GPIOTE_IRQHandler(void)
{
    log_info("INTERRUPT : IRQn handler \r\n");
    for(uint8_t ch=0; ch<INTERRUPTS_MAX_NUMBER; ch++) {
        // Check event channal
        if((NRF_GPIOTE->EVENTS_IN[ch] == 1) && (NRF_GPIOTE->INTENSET & (1<<ch))) {
            // Check whether callback is exist
            if((interrupt_pin[ch] != 0xFF) && (interrupt_callback[ch] != NULL) ) {
                interrupt_callback[ch]();
            }
            // Clean event flag
            NRF_GPIOTE->EVENTS_IN[ch] = 0;
        }
    }
}
