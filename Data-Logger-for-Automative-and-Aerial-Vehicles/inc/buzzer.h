/**
 * @file    :   buzzer.h
 * @brief   :   An abstraction for buzzer functions
 *
 *              This header file provides an abstraction of buzzer functions
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

#ifndef BUZZER_H_
#define BUZZER_H_

#include <stdbool.h>

/* Define MACROS for buzzer timings */
#define ON_TIME         (150000)
#define OFF_TIME        (100000)
#define BUZZ_CNT        (2)


/*********************************************************************************
 * @function:   play_buzzer
 *
 * @brief   :   Beeps the buzzer count times
 *
 * @param   :   buzz_count   - No of beeps of buzzer.
 *
 * @return  :   void
 *
**********************************************************************************/
void play_buzzer(int buzz_count);


/*********************************************************************************
 * @function:   buzzer_state_change
 *
 * @brief   :   Changes the state of the buzzer
 *
 * @param   :   state   - true  - turn ON the buzzer
 *                      - flase - turn OFF the buzzer
 *
 * @return  :   void
 *
**********************************************************************************/
void buzzer_state_change(bool state);


#endif /* BUZZER_H_ */
