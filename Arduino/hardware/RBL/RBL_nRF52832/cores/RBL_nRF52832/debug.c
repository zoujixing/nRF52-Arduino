
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
    
    if(!debug_flag)
        return;    
    
    uint16_t index, len;
    va_list args;
    
    if( (log_level != LOG_LEVEL_INFO) && (log_level != LOG_LEVEL_ERROR) )
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

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    log_error("Error info \r\n");
    log_error("The error code : %d \r\n", error_code);
    log_error("The line num   : %d \r\n", line_num);
    log_error("The file name  : %s \r\n", p_file_name);
}

void assert_nrf_callback(uint16_t line_num, const uint8_t *file_name)
{
    log_error("Assert info \r\n");
    log_error("The line num   : %d \r\n", line_num);
    log_error("The file name  : %s \r\n", file_name);    
}

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
    log_error("Softdevice Assert info \r\n");
    log_error("The id  : %d \r\n", id);
    log_error("The pc  : %d \r\n", pc);
    log_error("The pc  : %d \r\n", info);
}