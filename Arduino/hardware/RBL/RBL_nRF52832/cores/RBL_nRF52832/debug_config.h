
#ifndef DEBUG_CONFIG_H_
#define DEBUG_CONFIG_H_

#include "debug.h"

#ifdef __cplusplus
extern "C" {
#endif

// Enable or Disable log
#define LOG_INFOR_ENABLE  1
#define LOG_ERROR_ENABLE  1

#if LOG_INFOR_ENABLE
#define log_info(format, ...)  debug_print(LOG_LEVEL_INFO, format,  ## __VA_ARGS__)
#else
#define log_info(format, ...)
#endif

#if LOG_ERROR_ENABLE
#define log_error(format, ...) debug_print(LOG_LEVEL_ERROR, format,  ## __VA_ARGS__)
#else 
#define log_error(format, ...)
#endif

#ifdef __cplusplus
}
#endif

#endif
