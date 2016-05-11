

#ifndef DEBUG_H_
#define DEBUG_H_

#include "device_common.h"
#include "wiring_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_LEVEL_INFO   1
#define LOG_LEVEL_ERROR  2

/*
 * @brief Open debug.
 */
void debug_open(void);

/*
 * @brief Close debug.
 */
void debug_close(void);

/*
 * @brief Pring log
 *
 * @param[in] log_level LOG_LEVEL_INFO or LOG_LEVEL_ERROR
 * @param[in] *format   
 * @param[in] ...
 */
void debug_print(uint8_t log_level, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
