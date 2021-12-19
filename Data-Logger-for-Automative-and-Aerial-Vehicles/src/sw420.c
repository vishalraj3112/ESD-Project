/**
 * @file    :   sw420.c
 * @brief   :   An abstraction for RTC functions
 *
 *              This header file provides abstraction of vibration sensor functions
 *              which are used to initialize it and track shocks detected
 *
 * @author  :   Sanish Kharade
 * @date    :   November 28, 2021
 * @version :   1.0
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   MSP432 Reference Manual
*/

#include <stdbool.h>

#include <inc/sw420.h>

bool shock_tracker = false;

void SW420_gpio_init(void)
{
    /* Configuring P2.5 as an input and enabling interrupts */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN5);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P2, GPIO_PIN5);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN5);
    MAP_Interrupt_enableInterrupt(INT_PORT2);

    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();
}

/*********************************************************************************
 * @function:   PORT2_IRQHandler
 *
 * @brief   :   ISR for Port 2
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void PORT2_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P2, status);

    /* Toggling the output on the LED */
    if(status & GPIO_PIN5)
    {
        shock_tracker = true;
    }

}



bool shock_detected(void)
{
    return shock_tracker;
}
void clear_shock_detected_flag(void)
{
    shock_tracker = false;
}

