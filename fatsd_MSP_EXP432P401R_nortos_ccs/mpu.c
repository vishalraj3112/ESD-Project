/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== blink.c ========
 */
#include "msp432p401r.h"
#include <stdbool.h>
#include <stdio.h>
#include  <math.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>   //usleep()
/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/ADC.h>
#include <ti/drivers/I2C.h>

/* Display Driver Header files */
#include <ti/display/Display.h>
#include <ti/display/DisplayUart.h>

/* Driver configuration */
#include "ti_drivers_config.h"
#include "mpu.h"
#include "lcd.h"

// I2C configuration
I2C_Params mpu6050Params;
I2C_Handle mpu6050Handle;
I2C_Transaction i2cTranscation = {0};

#define MPU6050_ADDRESS 0x68

void MPU6050_Reset();
void MPU6050_ReadData(int16_t accelerometer[3], int16_t gyro[3], int16_t *temp);
void process_raw_values(int16_t accelerometer[3], int16_t gyro[3]);

//SCL - P6.5
//SDA - P6.4
//UART config - 115k2, 8N2.

//***Value processing code***
//Declaring some global variables
int gyro_x, gyro_y, gyro_z;
long acc_x, acc_y, acc_z, acc_total_vector;
int temperature;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
long loop_timer;
int lcd_loop_counter;
float angle_pitch, angle_roll,angle_yaw;
int angle_pitch_buffer, angle_roll_buffer;
bool set_gyro_angles;
float angle_roll_acc, angle_pitch_acc;
float angle_pitch_output, angle_roll_output;
uint8_t angle_pitch_int;
//***Value processing code***

//Display_Params  uartParams;
//Display_Handle  uartHandle;
int16_t accelerometer[3], gyro[3], temp;

/*
 *  ======== mainThread ========
 */
void mpu6050(void)
{

    /* Call driver init functions */
    GPIO_init();
    //Display_init();
    I2C_init();
    printf("Hello World!\r\n");

    //Initialize Display params
    //Display_Params_init(&uartParams);
    //uartHandle = Display_open(Display_Type_UART, &uartParams);

    I2C_Params_init(&mpu6050Params);
    mpu6050Params.bitRate = I2C_400kHz;
    mpu6050Handle = I2C_open(CONFIG_I2C_0, &mpu6050Params);

    MPU6050_Reset();

    lcd_set_cursor(0,0);
    lcd_print("Gyro angles:");
    lcd_set_cursor(12,0);
    while(1)
    {
        MPU6050_ReadData(accelerometer, gyro, &temp);

        process_raw_values(accelerometer,gyro);

//        Display_print3(uartHandle, 0, 0, "Accelerometer   X_OUT= %d   Y_OUT= %d   Z_OUT= %d\r\n",accelerometer[0], accelerometer[1], accelerometer[2]);
//        Display_print3(uartHandle, 1, 0, "Gyro   X_OUT= %d   Y_OUT= %d   Z_OUT= %d\r\n",gyro[0], gyro[1], gyro[2]);
//        Display_print1(uartHandle, 2, 0, "Temperature= %2f \r\n", (temp/340.0) + 36.53);
        Display_print2(display, 3, 0,"Pitch : %f^ , Roll : %f^",angle_pitch_output,angle_roll_output);
        angle_pitch_int = (int)angle_pitch_output;
        //lcd_print(&angle_pitch_int);
        lcd_print("Angl");
        delay_ms(1000);
        lcd_set_cursor(12,0);
        // printf("Pitch : %f^ , Roll : %f^\r\n",angle_pitch_output,angle_roll_output);
        //usleep(500000);
        usleep(500);
    }
    //return 0;
}


void MPU6050_Reset()//Init function
{
    int16_t accelerometer[3], gyro[3], temp;
    int i,j=0;
    //Use P2.0 for calibration indication
    P2->DIR |= BIT0;

    uint8_t writeData[] = {0x6B, 0x00};//Internal 8Mhz clock

    i2cTranscation.slaveAddress = MPU6050_ADDRESS;
    i2cTranscation.writeBuf = writeData;
    i2cTranscation.writeCount = 2;

    I2C_transfer(mpu6050Handle, &i2cTranscation);

    //Configure accelerometer for +-8g
    //writeData[] = {0x1C,0x10};
    writeData[0] = 0x1C;
    writeData[1] = 0x10;

    i2cTranscation.slaveAddress = MPU6050_ADDRESS;
    i2cTranscation.writeBuf = writeData;
    i2cTranscation.writeCount = 2;

    I2C_transfer(mpu6050Handle, &i2cTranscation);

    //Selecting full scale range as 500 degree per second
    //writeData[] = {0x1B,0x08};
    writeData[0] = 0x1B;
    writeData[1] = 0x08;

    i2cTranscation.slaveAddress = MPU6050_ADDRESS;
    i2cTranscation.writeBuf = writeData;
    i2cTranscation.writeCount = 2;

    I2C_transfer(mpu6050Handle, &i2cTranscation);

    P2->OUT |= BIT0;//to indicate start of calibration

    Display_print0(display, 0, 0," MPU-6050 IMU V1.0\r\n");
    Display_print0(display, 1, 0,"Calibrating gyro\r\n");
    //printf("MPU-6050 IMU V1.0\r\n");
    //printf("Calibrating gyro\r\n");

    for (i = 0; i < 2000 ; i++ ){
        if(i % 125 == 0){
            Display_print0(display, 2, j,".");
            //printf(".");
            j++;
        }
        MPU6050_ReadData(accelerometer, gyro, &temp);
        gyro_x_cal += gyro[0];
        gyro_y_cal += gyro[1];
        gyro_z_cal += gyro[2];
        //delay(3);//us
    }
    printf("\r\n");
    gyro_x_cal /= 2000;
    gyro_y_cal /= 2000;
    gyro_z_cal /= 2000;

    //Display_print0(uartHandle, 6, 0,"Pitch:\r\n");
    //Display_print0(uartHandle, 7, 0,"Roll:\r\n");

    P2->OUT ^= BIT0;//turn the led off

}

void MPU6050_ReadData(int16_t accelerometer[3], int16_t gyro[3], int16_t *temp)
{
    uint8_t readData[6];
    uint8_t writeData;
    // reading the accelerometer data
    writeData = 0x3B;
    i2cTranscation.slaveAddress = MPU6050_ADDRESS;
    i2cTranscation.writeBuf = &writeData;
    i2cTranscation.writeCount = 1;
    i2cTranscation.readBuf = readData;
    i2cTranscation.readCount = 6;
    I2C_transfer(mpu6050Handle, &i2cTranscation);

    accelerometer[0] = ((readData[0] << 8) | readData[1]);
    accelerometer[1] = ((readData[2] << 8) | readData[3]);
    accelerometer[2] = ((readData[4] << 8) | readData[5]);

    //Gyro data
    writeData = 0x43;
    i2cTranscation.slaveAddress = MPU6050_ADDRESS;
    i2cTranscation.writeBuf = &writeData;
    i2cTranscation.writeCount = 1;
    i2cTranscation.readBuf = readData;
    i2cTranscation.readCount = 6;
    I2C_transfer(mpu6050Handle, &i2cTranscation);

    gyro[0] = ((readData[0] << 8) | readData[1]);
    gyro[1] = ((readData[2] << 8) | readData[3]);
    gyro[2] = ((readData[4] << 8) | readData[5]);

    // read the temperture data
    writeData = 0x41;
    i2cTranscation.slaveAddress = MPU6050_ADDRESS;
    i2cTranscation.writeBuf = &writeData;
    i2cTranscation.writeCount = 1;
    i2cTranscation.readBuf = readData;
    i2cTranscation.readCount = 2;
    I2C_transfer(mpu6050Handle, &i2cTranscation);

    *temp = ((readData[0] << 8) | readData[1]);

}

void process_raw_values(int16_t accelerometer[3], int16_t gyro[3])
{
    //Z-axis,Yaw axis angle only found through gyro
    //reading gyro values
    gyro_x = gyro[0];
    gyro_y = gyro[1];
    gyro_z = gyro[2];

    //reading acceleromter values
    acc_x = accelerometer[0];
    acc_y = accelerometer[1];
    acc_z = accelerometer[2];

    gyro_x -= gyro_x_cal;
    gyro_y -= gyro_y_cal;
    gyro_z -= gyro_z_cal;

   //Gyro angle calculations
   //0.0000611 = 1 / (250Hz / 65.5)
   angle_pitch += gyro_x * 0.0000611;
   angle_roll += gyro_y * 0.0000611;
   angle_yaw += gyro_z * 0000611;

   //0.000001066 = 0.0000611 * (3.142(PI) / 180degr)
   angle_pitch += angle_roll * sin(gyro_z * 0.000001066);
   angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);

   //Accelerometer angle calculations
   acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z));
   angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296;
   angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;

   angle_pitch_acc -= 0.0; //for calibration
   angle_roll_acc -= 0.0; //for calibration

   if(set_gyro_angles){                                                 //If the IMU is already started
       angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
       angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        //Correct the drift of the gyro roll angle with the accelerometer roll angle
     }
     else{                                                                //At first start
       angle_pitch = angle_pitch_acc;                                     //Set the gyro pitch angle equal to the accelerometer pitch angle
       angle_roll = angle_roll_acc;                                       //Set the gyro roll angle equal to the accelerometer roll angle
       set_gyro_angles = true;                                            //Set the IMU started flag
     }

   //To dampen the pitch and roll angles a complementary filter is used
   angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1;   //Take 90% of the output pitch value and add 10% of the raw pitch value
   angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1;

}
