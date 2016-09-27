
#include "nrf_clock.h"

/****************************************************
*                 Local Variables
****************************************************/

/****************************************************
*                 Function Definitions
****************************************************/
void LFCLK_init(void)
{
    //LFCLK Init
    NRF_CLOCK->LFCLKSRC             = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED  = 0;
    NRF_CLOCK->TASKS_LFCLKSTART     = 1;
    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
        //Do nothing.
    }
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
}

void HFCLK_init(void)
{
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;
    while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
        //do nothing
    }
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
}