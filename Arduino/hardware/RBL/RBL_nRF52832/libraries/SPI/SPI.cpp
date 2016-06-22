
#include "SPI.h"

SPIClass::SPIClass(NRF_SPI_Type *p_spi)
{
    SPIx = p_spi;
}

void SPIClass::setCPOL(bool active)
{
    if(active)
        SPIx->CONFIG |= (SPI_CONFIG_CPOL_ActiveLow << SPI_CONFIG_CPOL_Pos);
    else
        SPIx->CONFIG |= (SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos);
}

void SPIClass::setCPHA(bool trailing)
{
    if(trailing)
        SPIx->CONFIG |= (SPI_CONFIG_CPHA_Trailing << SPI_CONFIG_CPHA_Pos);
    else
        SPIx->CONFIG |= (SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos);
}

void SPIClass::setORDER(BitOrder bit)
{
    if(bit == MSBFIRST)
        SPIx->CONFIG |= (SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos);
    else
        SPIx->CONFIG |= (SPI_CONFIG_ORDER_LsbFirst << SPI_CONFIG_ORDER_Pos);
}

void SPIClass::setSPIMode(uint8_t mode)
{
    if (SPI_MODE0 == mode) {
        setCPOL(0);
        setCPHA(0);
    }
    else if (mode == SPI_MODE1) {
        setCPOL(0);
        setCPHA(1);
    }
    else if (mode == SPI_MODE2) {
        setCPOL(1);
        setCPHA(0);
    }
    else if (mode == SPI_MODE3) {
        setCPOL(1);
        setCPHA(1);
    }
    else {
        setCPOL(0);
        setCPHA(0);
    }
}

void SPIClass::setFrequency(uint8_t speed)
{
    if (speed == 0)
        SPIx->FREQUENCY = SPI_FREQUENCY_125K;
    else if (speed == 1)
        SPIx->FREQUENCY = SPI_FREQUENCY_250K;
    else if (speed == 2)
        SPIx->FREQUENCY = SPI_FREQUENCY_500K;
    else if (speed == 3)
        SPIx->FREQUENCY = SPI_FREQUENCY_1M;
    else if (speed == 4)
        SPIx->FREQUENCY = SPI_FREQUENCY_2M;
    else if (speed == 5)
        SPIx->FREQUENCY = SPI_FREQUENCY_4M;
    else if (speed == 6)
        SPIx->FREQUENCY = SPI_FREQUENCY_8M;
    else
        SPIx->FREQUENCY = SPI_FREQUENCY_4M;
}

void SPIClass::begin(void)
{
    begin(DEFAULT_SCK, DEFAULT_MOSI, DEFAULT_MISO);
}

void SPIClass::begin(uint32_t sck, uint32_t mosi, uint32_t miso)
{
    uint8_t nrf_sck;
    uint8_t nrf_mosi;
    uint8_t nrf_miso;

    nrf_sck  = Pin_nRF51822_to_Arduino(sck);
    nrf_mosi = Pin_nRF51822_to_Arduino(mosi);
    nrf_miso = Pin_nRF51822_to_Arduino(miso);

    if(nrf_sck>=31 || nrf_mosi>=31 || nrf_miso>=31)
        return;

    NRF_GPIO->PIN_CNF[nrf_sck] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
                                 (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                 (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                 (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                 (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);

    NRF_GPIO->PIN_CNF[nrf_mosi] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
                                  (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                  (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                  (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                  (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);

    NRF_GPIO->PIN_CNF[nrf_miso] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
                                  (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                  (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                  (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                  (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
    // Config pin select
    SPIx->PSEL.SCK  = nrf_sck;
    SPIx->PSEL.MOSI = nrf_mosi;
    SPIx->PSEL.MISO = nrf_miso;
    // Set SPIx speed 2MHz, Mode0, MSBFIRST
    setFrequency(SPI_2M);
    setSPIMode(SPI_MODE0);
    setORDER(MSBFIRST);
    // Enable SPIx
    SPIx->EVENTS_READY = 0;
    SPIx->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);
}

uint8_t SPIClass::transfer(uint8_t data)
{
    // Wait SPIx TX finish
    while( SPIx->EVENTS_READY != 0U );
    // Put data to TXD
    SPIx->TXD = (uint32_t)data;
    // Wait SPIx TX finish
    while(SPIx->EVENTS_READY == 0);
    // Get data from RXD
    data = (uint8_t)SPIx->RXD;
    // Clear event flag
    SPIx->EVENTS_READY = 0;

    return data;
}

void SPIClass::end(void)
{
    SPIx->ENABLE = (SPI_ENABLE_ENABLE_Disabled << SPI_ENABLE_ENABLE_Pos);
}

SPIClass SPI(SPI_USED);
