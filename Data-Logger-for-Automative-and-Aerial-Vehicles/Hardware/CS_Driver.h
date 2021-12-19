/**
 * @file    :   CS_Driver.h
 * @brief   :   An abstraction for CS_Driver functions
 *
 *              This header file provides an abstraction to the CS_Driver function
 *              which is used to initialize the Clock source for the MSP432
 *
 * @author  :   Alejandro Martinez
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   This code has been leveraged from the github of Alejandro Martinez
 *              https://github.com/amartinezacosta/MSP432-SD_Card
*/

#ifndef HARDWARE_CS_DRIVER_H_
#define HARDWARE_CS_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/cs.h>
#include <ti/devices/msp432p4xx/driverlib/pcm.h>
#include <ti/devices/msp432p4xx/driverlib/flash.h>
#include <ti/devices/msp432p4xx/driverlib/wdt_a.h>


/*********************************************************************************
 * @function:   CS_Init
 *
 * @brief   :   Initializes the clock source
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void CS_Init(void);

#endif /* HARDWARE_CS_DRIVER_H_ */
