/**
 * @file    :   TIMERA_Driver.h
 * @brief   :   An abstraction for TIMERA_Driver functions
 *
 *              This header file provides an abstraction to the TIMERA_Driver function
 *              which is used to initialize the TimerA peripheral of the MSP432
 *
 * @author  :   Alejandro Martinez
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   This code has been leveraged from the github of Alejandro Martinez
 *              https://github.com/amartinezacosta/MSP432-SD_Card
*/

#ifndef HARDWARE_TIMERA_DRIVER_H_
#define HARDWARE_TIMERA_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/timer_a.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>

#define CONTINUOS_MODE  0
#define UP_MODE         1
#define UPDOWN_MODE     2
#define CAPTURE_MODE    3
#define COMPARE_MODE    4
#define PWM_MODE        5

/*********************************************************************************
 * @function:   TIMERA_Init
 *
 * @brief   :   Initializes the TIMERA peripheral
 *
 * @param   :   TIMER       - instance of timer
 *              Mode        - mode
 *              Config      - configuration
 *              TIMER_CB    - Timer CB
 *
 * @return  :   void
 *
**********************************************************************************/
void TIMERA_Init(uint32_t TIMER, uint32_t Mode, void *Config, void(*TIMER_CB)(void));


#endif /* HARDWARE_TIMERA_DRIVER_H_ */
