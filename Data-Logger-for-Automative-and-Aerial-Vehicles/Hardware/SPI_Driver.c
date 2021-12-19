/**
 * @file    :   SPI_Driver.c
 * @brief   :   An abstraction for SPI_Driver functions
 *
 *              This header file provides the SPI_Driver function which is used to
 *              initialize, write and read to the SPI peripheral of the MSP432
 *
 * @author  :   Alejandro Martinez
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   This code has been leveraged from the github of Alejandro Martinez
 *              https://github.com/amartinezacosta/MSP432-SD_Card
*/

#include "SPI_Driver.h"

void SPI_Init(uint32_t SPI, eUSCI_SPI_MasterConfig SPIConfig)
{
    switch(SPI)
    {
    case EUSCI_B0_BASE:
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

        MAP_SPI_initMaster(SPI, &SPIConfig);
        MAP_SPI_enableModule(SPI);
        break;
    case EUSCI_B1_BASE:
        break;
    default:
        break;
    }
}

void SPI_Write(uint32_t SPI, uint8_t *Data, uint32_t Size)
{
    uint32_t i;
    for(i = 0; i < Size; i++)
    {
        MAP_SPI_transmitData(SPI, Data[i]);
    }
}

void SPI_Read(uint32_t SPI, uint8_t *Data, uint32_t Size)
{
    uint32_t i;
    for(i = 0; i < Size; i++)
    {
        Data[i] = MAP_SPI_receiveData(SPI);
    }
}

