/**
 * @file    :   mode.h
 * @brief   :   An abstraction for mode functions
 *
 *              This header file provides abstraction of mode functions which are used to
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

#ifndef INC_MODE_H_
#define INC_MODE_H_

//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


/* Enum for state of the LCD display */
typedef enum
{
    ACCELEROMETER,
    GYROSCOPE
}mode_t;



/*********************************************************************************
 * @function:   mode_gpio_init
 *
 * @brief   :   Initializes the GPIO pin used to set the mode of LCD display
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void mode_gpio_init(void);

/*********************************************************************************
 * @function:   get_mode
 *
 * @brief   :   Returns the current mode
 *
 * @param   :   mode_t - current mode
 *                  ACCELEROMETER   - accelerometer values are displayed on the LCD
 *                  GYROSCOPE       - gyroscope values are displayed on the LCD
 *
 * @return  :   void
 *
**********************************************************************************/
mode_t get_mode(void);


#endif /* INC_MODE_H_ */
