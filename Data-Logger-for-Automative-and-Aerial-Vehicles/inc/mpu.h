/**
 * @file    :   mpu.h
 * @brief   :   Hearder for MPU6050 driver
 *
 *             This header file declares MPU6050 functions needed
 *             to read and write data from the sensor.
 *
 * @author  :   Vishal Raj
 * @date    :   November 24, 2021
 * @version :   1.0
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   Some implementation referred from http://www.brokking.net/imu.html.
*/

#ifndef MPU_H_
#define MPU_H_

#include <stdint.h>

#define NUM_OF_REC_BYTES    6

struct mpu_values{
    float gGyroVal[3];//order x y z axis
    int16_t gAcclVal[3];//order x y z axis
};

//externs
extern volatile bool read_done;
extern uint8_t RXData[NUM_OF_REC_BYTES];
extern volatile uint8_t read_idx;

typedef struct mpu_values gAcc_gyro;

/*********************************************************************************
 * @function:   MPU6050_Reset
 *
 * @brief   :   Initializes the MPU6050 sensor
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void MPU6050_Reset(void);//add void

/*********************************************************************************
 * @function:   mpu6050
 *
 * @brief   :   A test function for MPU6050 to test the module independently.
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void mpu6050(void);

/*********************************************************************************
 * @function:   get_mpu_values
 *
 * @brief   :   Reads values from MPU6050 and returns them in a structure
 *
 * @param   :   None
 *
 * @return  :    gAcc_gyro - structure containing 2 arrays of 3 elements each
 *                           for accelerometer and gyroscope readings
 *
**********************************************************************************/
gAcc_gyro get_mpu_values(void);


#endif /* MPU_H_ */
