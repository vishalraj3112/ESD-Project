/*
 * buzzer.h
 *
 *  Created on: 03-Dec-2021
 *      Author: vishalraj
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#define ON_TIME         (20000)
#define OFF_TIME        (30000)
#define BUZZ_CNT        (2)

//function to play buzzer
void play_buzzer(int on_time, int off_time, int buzz_count);

#endif /* BUZZER_H_ */
