/**
 * @file    :   SPI_Driver.h
 * @brief   :   An abstraction for SPI_Driver functions
 *
 *              This header file provides an abstraction to the SPI_Driver function which is
 *              used to initialize, write and read to the SPI peripheral of the MSP432
 *
 * @author  :   Alejandro Martinez
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   This code has been leveraged from the github of Alejandro Martinez
 *              https://github.com/amartinezacosta/MSP432-SD_Card
*/

#ifndef HARDWARE_SPI_DRIVER_H_
#define HARDWARE_SPI_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/spi.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

/*********************************************************************************
 * @function:   SPI_Init
 *
 * @brief   :   Initializes the SPI peripheral
 *
 * @param   :   SPI         - instance of SPI
 *              SPIConfig   - SPI Handle
 *
 * @return  :   void
 *
**********************************************************************************/
void SPI_Init(uint32_t SPI, eUSCI_SPI_MasterConfig SPIConfig);

/*********************************************************************************
 * @function:   SPI_Write
 *
 * @brief   :   Function to write data to the SPI peripheral
 *
 * @param   :   SPI     - instance of SPI
 *              Data    - pointer to the data
 *              Size    - size of data to be written
 *
 * @return  :   void
 *
**********************************************************************************/
void SPI_Write(uint32_t SPI, uint8_t *Data, uint32_t Size);

/*********************************************************************************
 * @function:   SPI_Read
 *
 * @brief   :   Function to read data from the SPI peripheral
 *
 * @param   :   SPI     - instance of SPI
 *              Data    - pointer to the buffer
 *              Size    - size of data to be read
 *
 * @return  :   void
 *
**********************************************************************************/
void SPI_Read(uint32_t SPI, uint8_t *Data, uint32_t Size);

#endif /* HARDWARE_SPI_DRIVER_H_ */
