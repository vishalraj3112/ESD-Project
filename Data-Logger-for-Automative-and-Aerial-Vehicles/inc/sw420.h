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

#ifndef INC_SW420_H_
#define INC_SW420_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/*********************************************************************************
 * @function:   SW420_gpio_init
 *
 * @brief   :   Initializes the SW420 sensor's GPIO pin
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void SW420_gpio_init(void);


/*********************************************************************************
 * @function:   shock_detected
 *
 * @brief   :   Checks if shock was detected by the SW420 sensor
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
bool shock_detected(void);

/*********************************************************************************
 * @function:   shock_detected
 *
 * @brief   :   Clears the shock detected flag
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void clear_shock_detected_flag(void);

#endif /* INC_SW420_H_ */
