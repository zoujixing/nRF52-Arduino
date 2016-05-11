
#include <stdio.h>
#include <stdarg.h> 
#include "debug.h"
#include "uart.h"

/****************************************************
*                 Local Variables
****************************************************/

static uint8_t debug_flag = 0;

/****************************************************
*                 Function Definitions
****************************************************/
void debug_open(void)
{
    debug_flag = 1;
}

void debug_close(void)
{
    debug_flag = 0;
}

void debug_print(uint8_t log_level, const char *format, ...)
{   
    static uint8_t log_buf[250];
    
    uint16_t index, len;
    va_list args;
    
    if(!debug_flag)
        return;
    
    if( (log_level != LOG_LEVEL_INFO) && (log_level != LOG_LEVEL_ERROR))
        return;
    
    va_start(args, format);
    len = vsnprintf((char*)log_buf, sizeof(log_buf) - 1, format, args);
    log_buf[len] = 0x00; /* Make sure its zero terminated */
    
    for(index=0; index<len; index++)
    {
        UART0_send(log_buf[index]);
    }   
    va_end(args);
}
