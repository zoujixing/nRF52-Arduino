
#ifndef SPI_H_
#define SPI_H_

#include "Arduino.h"

#define SPI_USED  NRF_SPI0

#define SPI_FREQUENCY_125K 0x02000000UL
#define SPI_FREQUENCY_250K 0x04000000UL
#define SPI_FREQUENCY_500K 0x08000000UL
#define SPI_FREQUENCY_1M   0x10000000UL
#define SPI_FREQUENCY_2M   0x20000000UL
#define SPI_FREQUENCY_4M   0x40000000UL
#define SPI_FREQUENCY_8M   0x80000000UL

#define SPI_125K 0
#define SPI_250K 1
#define SPI_500K 2
#define SPI_1M   3
#define SPI_2M   4
#define SPI_4M   5
#define SPI_8M   6

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

class SPIClass
{
public:
    SPIClass(NRF_SPI_Type *p_spi);

    /**
     * @brief SPI begin
     */
    void begin(void);

    /**
     * @brief SPI begin
     *
     * @param[in]  sck
     * @param[in]  mosi
     * @param[in]  miso
     */
    void begin(uint32_t sck, uint32_t mosi, uint32_t miso);

    /**
     * @brief SPI transfer one byte
     *
     * @param[in]  data  Data to be sending
     *
     * @return Data received from device
     */
    uint8_t transfer(uint8_t data);

    /**
     * @brief Diable SPIx
     */
    void end(void);

    /**
     * @brief Set SPIx mode
     *
     * @param[in]  mode    SPI_MODE0 : CPOL_ActiveHigh, CPHA_Leading
     *                     SPI_MODE1 : CPOL_ActiveHigh, CPHA_Trailing
     *                     SPI_MODE2 : CPOL_ActiveLow,  CPHA_Leading
     *                     SPI_MODE3 : CPOL_ActiveLow,  CPHA_Trailing
     */
    void setSPIMode(uint8_t mode);

    /**
     * @brief Set frequency
     *
     * @param[in]  speed  0~6 is Corresponding to SPI_125K, SPI_250K, SPI_500K, SPI_1M, SPI_2M, SPI_4M, SPI_8M
     */
    void setFrequency(uint8_t speed);

    /**
     * @brief Set BIT order
     *
     * @param[in]  bit  MSBFIRST or LSBFIRST
     */
    void setORDER(BitOrder bit);

    /**
     * @brief Set sck phase
     *
     * @param[in]  trailing  0 Leading   : Sample on leading edge of clock, shift serial data on trailing edge
     *                       1 Traniling : Sample on trailing edge of clock, shift serial data on leading edge
     */
    void setCPHA(bool trailing);

    /**
     * @brief Set sck polarity
     *
     * @param[in]  active   0 ActiveHigh : Active high
     *                      1 ActiveLow  : Active high
     */
    void setCPOL(bool active);

private:
    NRF_SPI_Type *SPIx;

    uint32_t sck_pin;
    uint32_t mosi_pin;
    uint32_t miso_pin;
};

extern SPIClass SPI;

#endif
