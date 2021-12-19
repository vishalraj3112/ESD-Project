/**
 * @file    :   GPIO_Driver.h
 * @brief   :   An abstraction for GPIO_Driver functions
 *
 *              This source file provides the GPIO_Driver functions which are
 *              used to initialize, and change state of the GPIO pins
 *
 * @author  :   Alejandro Martinez
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   This code has been leveraged from the github of Alejandro Martinez
 *              https://github.com/amartinezacosta/MSP432-SD_Card
*/
#include "GPIO_Driver.h"


void GPIO_Init(uint32_t Port, uint16_t Pins)
{
    MAP_GPIO_setAsOutputPin(Port, Pins);
}

void GPIO_High(uint32_t Port, uint16_t Pins)
{
    MAP_GPIO_setOutputHighOnPin(Port, Pins);
}

void GPIO_Low(uint32_t Port, uint16_t Pins)
{
    MAP_GPIO_setOutputLowOnPin(Port, Pins);
}
