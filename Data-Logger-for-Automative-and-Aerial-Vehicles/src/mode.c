/**
 * @file    :   mode.c
 * @brief   :   An abstraction for mode functions
 *
 *              This source file provides mode functions which are used to
 *              initialize the mode gpio and get the current mode of the system
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
#include <stdbool.h>

#include <inc/buzzer.h>
#include <inc/mode.h>

/* Initialize mode to gyroscope */
mode_t mode = GYROSCOPE;

void mode_gpio_init(void)
{
    /* Configuring P1.4 as an input and enabling interrupts */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);
    MAP_Interrupt_enableInterrupt(INT_PORT1);

    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();
}

/*********************************************************************************
 * @function:   PORT1_IRQHandler
 *
 * @brief   :   ISR for Port 1
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void PORT1_IRQHandler(void)
{
    uint32_t status;
    static uint8_t counter = 0;
    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    /* Toggling the output on the LED */
    if(status & GPIO_PIN4)
    {
        counter++;

        /*Enable the buzzer after 2 changes of state so that accelerometer can be
         * tested properly without the buzzer beeping continuously
         * */
        if(counter == 2)
            buzzer_state_change(true);
        if(mode == ACCELEROMETER)
            mode = GYROSCOPE;
        else if(mode == GYROSCOPE)
            mode = ACCELEROMETER;

    }

}

mode_t get_mode(void)
{
    return mode;
}


