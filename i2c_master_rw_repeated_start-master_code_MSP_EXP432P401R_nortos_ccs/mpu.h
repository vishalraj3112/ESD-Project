/*
 * mpu.h
 *
 *  Created on: 20-Nov-2021
 *      Author: vishalraj
 */

#ifndef MPU_H_
#define MPU_H_

#include <stdint.h>

#define NUM_OF_REC_BYTES    6


//externs
extern volatile bool read_done;
extern uint8_t RXData[NUM_OF_REC_BYTES];
extern volatile uint8_t read_idx;

typedef struct AcclVal gAccelero_s;

void mpu6050(void);
gAccelero_s get_mpu_values(void);//get MPU values in format: gyro: x y z, accelerometer: x y z.



#endif /* MPU_H_ */
