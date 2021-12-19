/**
 * @file    :   MSPIO.h
 * @brief   :   An abstraction for MSPIO functions
 *
 *              This header file provides an abstraction of the MSPIO functions which
 *              are used to print characters, integers and strings on to the UART
 *
 * @author  :   Alejandro Martinez
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   This code has been leveraged from the github of Alejandro Martinez
 *              https://github.com/amartinezacosta/MSP432-SD_Card
*/
#ifndef MSPIO_H_
#define MSPIO_H_

#include <stdio.h>
#include <Hardware/UART_Driver.h>

void MSPrintf(uint32_t UART, const char *fs, ...);
int MSPgets(uint32_t UART, char *b, int size);

#endif /* MSPIO_H_ */
