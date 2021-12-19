/**
 * @file    :   buzzer.c
 * @brief   :   An abstraction for buzzer functions
 *
 *              This source file provides buzzer functions
 *              which are used to initialize and play the buzzer
 *
 * @author  :   Vishal Raj
 * @date    :   November 28, 2021
 * @version :   1.0
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   MSP432 Reference Manual
*/

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>

#include <inc/buzzer.h>

bool enable_buzzer = false;

void play_buzzer(int buzz_count)
{
    if(enable_buzzer ==  true)
    {
        volatile int i,j;

        /* Configure P2.4 as output */
        MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);

        /* Turn on the buzzer */
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);

        for(i = 0; i < buzz_count ; i++)
        {
            /* ON Time */
            for(j = 0; j < ON_TIME; j++);

            /* Turn off the buzzer */
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);

            /* OFF Time */
            for(j = 0; j < OFF_TIME; j++);

            /* Turn on the buzzer */
            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
        }

        /* Turn off the buzzer */
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);
    }
}

void buzzer_state_change(bool state)
{
    enable_buzzer = state;
}

