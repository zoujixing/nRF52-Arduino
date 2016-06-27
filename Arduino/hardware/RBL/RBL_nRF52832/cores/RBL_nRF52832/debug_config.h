
#ifndef DEBUG_CONFIG_H_
#define DEBUG_CONFIG_H_

#include "debug.h"

#ifdef __cplusplus
extern "C" {
#endif

// Enable or Disable log
#define LOG_USER_ENABLE   1
#define LOG_INFOR_ENABLE  1
#define LOG_ERROR_ENABLE  1

#if LOG_USER_ENABLE
#define log_user(format, ...)  debug_print(LOG_LEVEL_INFO, format,  ## __VA_ARGS__)
#else
#define log_user(format, ...)
#endif

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

/**@brief Macro for calling error handler function.
 *
 * @param[in] ERR_CODE Error code supplied to the error handler.
 */
#define APP_ERROR_HANDLER(ERR_CODE)                                    \
    do                                                                 \
    {                                                                  \
        app_error_handler((ERR_CODE), __LINE__, (uint8_t*) __FILE__);  \
    } while (0)

/**@brief Macro for calling error handler function if supplied error code any other than NRF_SUCCESS.
 *
 * @param[in] ERR_CODE Error code supplied to the error handler.
 */
#define APP_ERROR_CHECK(ERR_CODE)                           \
    do                                                      \
    {                                                       \
        const uint32_t LOCAL_ERR_CODE = (ERR_CODE);         \
        if (LOCAL_ERR_CODE != NRF_SUCCESS)                  \
        {                                                   \
            APP_ERROR_HANDLER(LOCAL_ERR_CODE);              \
        }                                                   \
    } while (0)

/**@brief Macro for calling error handler function if supplied boolean value is false.
 *
 * @param[in] BOOLEAN_VALUE Boolean value to be evaluated.
 */
#define APP_ERROR_CHECK_BOOL(BOOLEAN_VALUE)                   \
    do                                                        \
    {                                                         \
        const uint32_t LOCAL_BOOLEAN_VALUE = (BOOLEAN_VALUE); \
        if (!LOCAL_BOOLEAN_VALUE)                             \
        {                                                     \
            APP_ERROR_HANDLER(0);                             \
        }                                                     \
    } while (0)

/** @brief Function for checking intended for production code.
 *
 * Check passes if "expr" evaluates to true. */
#define ASSERT(expr) \
if (expr)                                                                     \
{                                                                             \
}                                                                             \
else                                                                          \
{                                                                             \
    assert_nrf_callback((uint16_t)__LINE__, (uint8_t *)__FILE__);             \
}
        
#ifdef __cplusplus
}
#endif

#endif
