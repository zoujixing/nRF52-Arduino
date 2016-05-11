
#ifndef TONE_H_
#define TONE_H_

#include "device_common.h"
#include "wiring_constants.h"

#define TONE_USED_GPIOTE_NUM          7
#define TONE_USED_PPI_CHANNAL         19
#define TONE_USED_TIMER               NRF_TIMER1
#define TONE_USED_TIMER_IRQn          TIMER1_IRQn
#define TONE_USED_TIMER_IRQHandler    TIMER1_IRQHandler

/*
 * @brief Output a tone
 *
 * @param[in] pin       Pin of output tone.
 * @param[in] freq      Frequency of tone.
 * @param[in] duration  Duration in seconds.
 */
void tone(uint8_t pin, uint16_t freq, uint32_t duration);

/*
 * @brief Output a tone persistently.
 *
 * @param[in] pin       Pin of output tone.
 * @param[in] freq      Frequency of tone.
 */
void tone(uint8_t pin, uint16_t freq);

/*
 * @brief Stop a tone.
 *
 * @param[in] pin       Pin of output tone.
 */
void noTone(uint8_t pin);

#endif
