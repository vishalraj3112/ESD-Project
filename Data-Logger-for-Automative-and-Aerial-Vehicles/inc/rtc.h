/**
 * @file    :   rtc.h
 * @brief   :   An abstraction for RTC functions
 *
 *              This header file provides abstraction of RTC functions which are
 *              used to initialize and keep track of time
 *
 * @author  :   Sanish Kharade
 * @date    :   November 28, 2021
 * @version :   1.0
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   MSP432 Reference Manual
*/

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>

/*********************************************************************************
 * @function:   RTC_init
 *
 * @brief   :   Initializes the RTC
 *
 * @param   :   start_time - start time for the RTC
 *
 * @return  :   void
 *
**********************************************************************************/
void RTC_init(RTC_C_Calendar start_time);

/*********************************************************************************
 * @function:   RTC_get_current_time
 *
 * @brief   :   Returns the current time as managed by the RTC
 *
 * @param   :   void
 *
 * @return  :   RTC_C_Calendar - current RTC calendar time
 *
**********************************************************************************/
RTC_C_Calendar RTC_get_current_time(void);

/*********************************************************************************
 * @function:   one_second_elapsed
 *
 * @brief   :   Checks if one second has elapsed
 *
 * @param   :   void
 *
 * @return  :   true    - if one second has elapsed
 *              false   - if one second has not elapsed
 *
**********************************************************************************/
bool one_second_elapsed(void);

/*********************************************************************************
 * @function:   clear_one_second_elapsed_flag
 *
 * @brief   :   Clears the one second elapsed flag
 *
 * @param   :   void
 *
 * @return  :   void
 *
**********************************************************************************/
void clear_one_second_elapsed_flag(void);


#endif /* INC_RTC_H_ */
