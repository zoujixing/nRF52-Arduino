

#ifndef DEBUG_H_
#define DEBUG_H_

#include "device_common.h"
#include "wiring_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_LEVEL_INFO   1
#define LOG_LEVEL_ERROR  2

/** @defgroup NRF_ERRORS_BASE Error Codes Base number definitions
 * @{ */
#define NRF_ERROR_BASE_NUM      (0x0)       ///< Global error base
#define NRF_ERROR_SDM_BASE_NUM  (0x1000)    ///< SDM error base
#define NRF_ERROR_SOC_BASE_NUM  (0x2000)    ///< SoC error base
#define NRF_ERROR_STK_BASE_NUM  (0x3000)    ///< STK error base
/** @} */

#define NRF_SUCCESS                           (NRF_ERROR_BASE_NUM + 0)  ///< Successful command
#define NRF_ERROR_SVC_HANDLER_MISSING         (NRF_ERROR_BASE_NUM + 1)  ///< SVC handler is missing
#define NRF_ERROR_SOFTDEVICE_NOT_ENABLED      (NRF_ERROR_BASE_NUM + 2)  ///< SoftDevice has not been enabled
#define NRF_ERROR_INTERNAL                    (NRF_ERROR_BASE_NUM + 3)  ///< Internal Error
#define NRF_ERROR_NO_MEM                      (NRF_ERROR_BASE_NUM + 4)  ///< No Memory for operation
#define NRF_ERROR_NOT_FOUND                   (NRF_ERROR_BASE_NUM + 5)  ///< Not found
#define NRF_ERROR_NOT_SUPPORTED               (NRF_ERROR_BASE_NUM + 6)  ///< Not supported
#define NRF_ERROR_INVALID_PARAM               (NRF_ERROR_BASE_NUM + 7)  ///< Invalid Parameter
#define NRF_ERROR_INVALID_STATE               (NRF_ERROR_BASE_NUM + 8)  ///< Invalid state, operation disallowed in this state
#define NRF_ERROR_INVALID_LENGTH              (NRF_ERROR_BASE_NUM + 9)  ///< Invalid Length
#define NRF_ERROR_INVALID_FLAGS               (NRF_ERROR_BASE_NUM + 10) ///< Invalid Flags
#define NRF_ERROR_INVALID_DATA                (NRF_ERROR_BASE_NUM + 11) ///< Invalid Data
#define NRF_ERROR_DATA_SIZE                   (NRF_ERROR_BASE_NUM + 12) ///< Invalid Data size
#define NRF_ERROR_TIMEOUT                     (NRF_ERROR_BASE_NUM + 13) ///< Operation timed out
#define NRF_ERROR_NULL                        (NRF_ERROR_BASE_NUM + 14) ///< Null Pointer
#define NRF_ERROR_FORBIDDEN                   (NRF_ERROR_BASE_NUM + 15) ///< Forbidden Operation
#define NRF_ERROR_INVALID_ADDR                (NRF_ERROR_BASE_NUM + 16) ///< Bad Memory Address
#define NRF_ERROR_BUSY                        (NRF_ERROR_BASE_NUM + 17) ///< Busy
#define NRF_ERROR_CONN_COUNT                  (NRF_ERROR_BASE_NUM + 18) ///< Maximum connection count exceeded.
#define NRF_ERROR_RESOURCES                   (NRF_ERROR_BASE_NUM + 19) ///< Not enough resources for operation

/**@brief Structure containing info about an error of the type @ref NRF_FAULT_ID_SDK_ERROR.
 */
typedef struct
{
    uint16_t        line_num;    /**< The line number where the error occurred. */
    uint8_t const * p_file_name; /**< The file in which the error occurred. */
    uint32_t        err_code;    /**< The error code representing the error that occurred. */
} error_info_t;

/**
 * @brief Open debug.
 */
void debug_open(void);

/**
 * @brief Close debug.
 */
void debug_close(void);

/**
 * @brief Pring log
 *
 * @param[in] log_level LOG_LEVEL_INFO or LOG_LEVEL_ERROR
 * @param[in] *format   
 * @param[in] ...
 */
void debug_print(uint8_t log_level, const char *format, ...);

/**
 * @brief Pring error information
 *
 * @param[in] error_code 
 * @param[in] line_num   
 * @param[in] *p_file_name
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);

/** 
 * @brief Function for handling assertions.
 *
 * @param line_num The line number where the assertion is called
 * @param file_name Pointer to the file name
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t *file_name);

/**@brief       Callback function for asserts in the SoftDevice.
 *
 * @param[in] id    Fault identifier. See @ref NRF_FAULT_IDS.
 * @param[in] pc    The program counter of the instruction that triggered the fault.
 * @param[in] info  Optional additional information regarding the fault. Refer to each fault
 *                  identifier for details.
 */
void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info);

#ifdef __cplusplus
}
#endif

#endif
