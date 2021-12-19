/**
 * @file    :   GPIO_Driver.h
 * @brief   :   An abstraction for GPIO_Driver functions
 *
 *              This header file provides an abstraction to the GPIO_Driver functions which are
 *              used to initialize, and change state of the GPIO pins
 *
 * @author  :   Alejandro Martinez
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   This code has been leveraged from the github of Alejandro Martinez
 *              https://github.com/amartinezacosta/MSP432-SD_Card
*/

#ifndef HARDWARE_GPIO_DRIVER_H_
#define HARDWARE_GPIO_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

/*********************************************************************************
 * @function:   GPIO_Init
 *
 * @brief   :   Initializes the GPIO pin
 *
 * @param   :   Port    - port number
 *              Pins    - pin number
 *
 * @return  :   void
 *
**********************************************************************************/
void GPIO_Init(uint32_t Port, uint16_t Pins);

/*********************************************************************************
 * @function:   GPIO_High
 *
 * @brief   :   Sets output high on the GPIO pin
 *
 * @param   :   Port    - port number
 *              Pins    - pin number
 *
 * @return  :   void
 *
**********************************************************************************/
void GPIO_High(uint32_t Port, uint16_t Pins);

/*********************************************************************************
 * @function:   GPIO_Low
 *
 * @brief   :   Sets output low on the GPIO pin
 *
 * @param   :   Port    - port number
 *              Pins    - pin number
 *
 * @return  :   void
 *
**********************************************************************************/
void GPIO_Low(uint32_t Port, uint16_t Pins);

#endif /* HARDWARE_GPIO_DRIVER_H_ */
