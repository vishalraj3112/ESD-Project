/**
 * @file    :   UART_Driver.c
 * @brief   :   An abstraction for SPI_Driver functions
 *
 *              This header file provides the UART_Driver functions which are used to
 *              initialize, write and read to the UART peripheral of the MSP432
 *
 * @author  :   Alejandro Martinez
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   This code has been leveraged from the github of Alejandro Martinez
 *              https://github.com/amartinezacosta/MSP432-SD_Card
*/

#ifndef HARDWARE_UART_DRIVER_H_
#define HARDWARE_UART_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>
#include <ti/devices/msp432p4xx/driverlib/uart.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

#define UARTA0_BUFFERSIZE 128
#define UARTA2_BUFFERSIZE 128

/*********************************************************************************
 * @function:   UART_Init
 *
 * @brief   :   Initializes the SPI peripheral
 *
 * @param   :   UART         - instance of UART
 *              UARTConfig   - UART Handle
 *
 * @return  :   void
 *
**********************************************************************************/
void UART_Init(uint32_t UART, eUSCI_UART_ConfigV1 UARTConfig);

/*********************************************************************************
 * @function:   UART_Write
 *
 * @brief   :   Function to write data to the SPI peripheral
 *
 * @param   :   UART    - instance of SPI
 *              Data    - pointer to the data
 *              Size    - size of data to be written
 *
 * @return  :   void
 *
**********************************************************************************/
void UART_Write(uint32_t UART, uint8_t *Data, uint32_t Size);

/*********************************************************************************
 * @function:   UART_Read
 *
 * @brief   :   Function to read data from the SPI peripheral
 *
 * @param   :   UART    - instance of UART
 *              Data    - pointer to the buffer
 *              Size    - size of data to be read
 *
 * @return  :   void
 *
**********************************************************************************/
uint32_t UART_Read(uint32_t UART, uint8_t *Data, uint32_t Size);

#endif /* HARDWARE_UART_DRIVER_H_ */
