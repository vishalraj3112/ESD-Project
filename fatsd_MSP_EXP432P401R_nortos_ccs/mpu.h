/*
 * mpu.h
 *
 *  Created on: 20-Nov-2021
 *      Author: vishalraj
 */

#ifndef MPU_H_
#define MPU_H_

#include <stdint.h>

/* Display Driver Header files */
#include <ti/display/Display.h>
#include <ti/display/DisplayUart.h>

//externs
extern Display_Handle display;

typedef struct AcclVal gAccelero_s;

void mpu6050(void);
gAccelero_s get_mpu_values(void);//get MPU values in format: gyro: x y z, accelerometer: x y z.



#endif /* MPU_H_ */
