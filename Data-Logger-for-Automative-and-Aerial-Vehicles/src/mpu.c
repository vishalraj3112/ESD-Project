/**
 * @file    :   mpu.c
 * @brief   :   An I2C driver file for MPU6050
 *
 *             This source codes provides the data from MPU-6050 using I2C to
 *             get three axis gyroscope and calibration values each.
 *
 * @author  :   Vishal Raj
 * @date    :   November 28, 2021
 * @version :   1.0
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   Some implementation referred from http://www.brokking.net/imu.html.
*/


#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <stdbool.h>
#include <stdio.h>
#include  <math.h>
#include <stdint.h>
#include <stddef.h>
#include "lcd.h"
#include "mpu.h"


#define MPU6050_ADDRESS 0x68

void MPU6050_Reset();
void MPU6050_ReadData(int16_t accelerometer[3], int16_t gyro[3], int16_t *temp);
void process_raw_values(int16_t accelerometer[3], int16_t gyro[3]);

//SCL - P6.5
//SDA - P6.4


//***Value processing code***

int gyro_x, gyro_y, gyro_z;
long acc_x, acc_y, acc_z, acc_total_vector;
int temperature;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
long loop_timer;
int lcd_loop_counter;
float angle_pitch, angle_roll,angle_yaw;
int angle_pitch_buffer, angle_roll_buffer;
bool set_gyro_angles;
float angle_roll_acc, angle_pitch_acc, angle_yaw_acc;
float angle_pitch_output, angle_roll_output;
uint8_t angle_pitch_int;
int16_t accelerometer[3], gyro[3], temp;
volatile uint8_t read_idx;
static gAcc_gyro gAccelero_t;

//***Value processing code***

gAcc_gyro get_mpu_values(void)//getter for MPU values
{
    MPU6050_ReadData(accelerometer, gyro, &temp);

    process_raw_values(accelerometer,gyro);
    return gAccelero_t;
}

void mpu6050(void)
{
    MPU6050_Reset();

    int i = 0;
    char s[10];
    lcd_home();

    while(1)
    {
        MPU6050_ReadData(accelerometer, gyro, &temp);

        process_raw_values(accelerometer,gyro);

        printf("Pitch : %f^ , Roll : %f^ Yaw: %d\r\n",gAccelero_t.gGyroVal[0],gAccelero_t.gGyroVal[1],gAccelero_t.gGyroVal[2]);
        printf("Pitch : %dg , Roll : %dg Yaw: %dg ",gAccelero_t.gAcclVal[0],gAccelero_t.gAcclVal[1],gAccelero_t.gAcclVal[2]);
        sprintf(s, "%d", i++);

        delay_ms(100);
        char buff[20];
        angle_pitch_int = (int)angle_pitch_output;
        sprintf(buff, "%d", angle_pitch_int);

    }
}


void MPU6050_Reset()//Init function
{
    int16_t accelerometer[3], gyro[3], temp;
    int i,j=0;

    //Use P2.0 for calibration indication
    P2->DIR |= BIT0;

    //Internal 8Mhz clock
    uint8_t writeData[] = {0x6B, 0x00};

    //Check for last transaction to complete
    while (MAP_I2C_masterIsStopSent(EUSCI_B1_BASE));
    //send bytes from master to slave
    MAP_I2C_masterSendMultiByteStart(EUSCI_B1_BASE,writeData[0]);
    MAP_I2C_masterSendMultiByteFinish(EUSCI_B1_BASE,writeData[1]);

    //Configure accelerometer for +-8g
    writeData[0] = 0x1C;
    writeData[1] = 0x10;

    while (MAP_I2C_masterIsStopSent(EUSCI_B1_BASE));
    //send bytes from master to slave
    MAP_I2C_masterSendMultiByteStart(EUSCI_B1_BASE,writeData[0]);
    MAP_I2C_masterSendMultiByteFinish(EUSCI_B1_BASE,writeData[1]);

    //Selecting full scale range as 500 degree per second
    writeData[0] = 0x1B;
    writeData[1] = 0x08;

    while (MAP_I2C_masterIsStopSent(EUSCI_B1_BASE));
    //send bytes from master to slave
    MAP_I2C_masterSendMultiByteStart(EUSCI_B1_BASE,writeData[0]);
    MAP_I2C_masterSendMultiByteFinish(EUSCI_B1_BASE,writeData[1]);

    P2->OUT |= BIT0;//to indicate start of calibration

    for (i = 0; i < 2000 ; i++ ){
        MPU6050_ReadData(accelerometer, gyro, &temp);
        gyro_x_cal += gyro[0];
        gyro_y_cal += gyro[1];
        gyro_z_cal += gyro[2];
    }

    gyro_x_cal /= 2000;
    gyro_y_cal /= 2000;
    gyro_z_cal /= 2000;


    P2->OUT ^= BIT0;//turn the led off, to indicate end of calibration

}

void MPU6050_ReadData(int16_t accelerometer[3], int16_t gyro[3], int16_t *temp)
{
    uint8_t writeData;//to write register address to read from

    // reading the accelerometer data
    writeData = 0x3B;
    //**send read address
    while (MAP_I2C_masterIsStopSent(EUSCI_B1_BASE));
    MAP_I2C_masterSendMultiByteStart(EUSCI_B1_BASE,writeData);
    MAP_I2C_masterSendMultiByteStop(EUSCI_B1_BASE);

    //**start reading
    read_idx = 0;
    MAP_I2C_masterReceiveStart(EUSCI_B1_BASE);
    MAP_I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);

    while(!read_done);//dont process until readind done

    if(read_done){
        read_done = false;
        accelerometer[0] = ((RXData[0] << 8) | RXData[1]);
        accelerometer[1] = ((RXData[2] << 8) | RXData[3]);
        accelerometer[2] = ((RXData[4] << 8) | RXData[5]);
    }

    //Gyro data
    writeData = 0x43;
    //**send read address
    while (MAP_I2C_masterIsStopSent(EUSCI_B1_BASE));
    MAP_I2C_masterSendMultiByteStart(EUSCI_B1_BASE,writeData);
    MAP_I2C_masterSendMultiByteStop(EUSCI_B1_BASE);

    //**start reading
    read_idx = 0;
    MAP_I2C_masterReceiveStart(EUSCI_B1_BASE);
    MAP_I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);


    while(!read_done);//dont process until reading done

    if(read_done){
        read_done = false;
        gyro[0] = ((RXData[0] << 8) | RXData[1]);
        gyro[1] = ((RXData[2] << 8) | RXData[3]);
        gyro[2] = ((RXData[4] << 8) | RXData[5]);
    }


}

void process_raw_values(int16_t accelerometer[3], int16_t gyro[3])
{
    int16_t accl_x,accl_y,accl_z;

    /*Output of gyro is in degree/sec, hence for FS=1, Gyro sensitivity scale factor = 65.5
     * therefore 1 degree/sec = 65.5 or rawGyroVal/65.5 = degree/sec*/
    //reading gyro raw values
    gyro_x = gyro[0];
    gyro_y = gyro[1];
    gyro_z = gyro[2];

    /*Output of accelerometer for AFS_SEL=2 is 4096/g, i.e 1g=4096, therefore rawAcclVal/4096=9.8m/s^2(1g)*/
    //reading acceleromter values
    acc_x = accelerometer[0];
    acc_y = accelerometer[1];
    acc_z = accelerometer[2];

    //Compensating current values with the calibration values
    gyro_x -= gyro_x_cal;
    gyro_y -= gyro_y_cal;
    gyro_z -= gyro_z_cal;

   //Gyro angle calculations
   angle_pitch  = gyro_x * 0.0152671;
   angle_roll   = gyro_y * 0.0152671;
   angle_yaw = gyro_z * 0.0152671;

   //Calculating acceleration values
   accl_x = (int16_t) (acc_x/4096) * 10;//in m/s^2
   accl_y = (int16_t) (acc_y/4096) * 10;
   accl_z = (int16_t) ((acc_z/4096) + 1) * 10;

   //storing the values
   gAccelero_t.gGyroVal[0] = angle_roll;//x - roll
   gAccelero_t.gGyroVal[1] = angle_pitch;//y - pitch
   gAccelero_t.gGyroVal[2] = angle_yaw;//z - yaw.
   gAccelero_t.gAcclVal[0] = accl_x;//x - roll
   gAccelero_t.gAcclVal[1] = accl_y;//y - pitch
   gAccelero_t.gAcclVal[2] = accl_z;//z - yaw

}
