/**
 * @file    :   sd_card.h
 * @brief   :   An abstraction for RTC functions
 *
 *              This header file provides abstraction of SD Card function
 *              which is used to update the SD Card
 *
 * @author  :   Sanish Kharade
 * @date    :   November 28, 2021
 * @version :   1.0
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   http://elm-chan.org/fsw/ff/00index_e.html
*/

#ifndef INC_SD_CARD_H_
#define INC_SD_CARD_H_

#include <inc/mpu.h>
#include <inc/rtc.h>

/*********************************************************************************
 * @function:   update_SD_card
 *
 * @brief   :   updates the SD card with the timestamp and sensor reading
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void update_SD_card(RTC_C_Calendar ntime, gAcc_gyro sensor);


#endif /* INC_SD_CARD_H_ */
