/**
 * @file    :   sd_card.c
 * @brief   :   An abstraction for RTC functions
 *
 *              This source file provides abstraction of SD Card function
 *              which is used to update the SD Card
 *
 * @author  :   Sanish Kharade
 * @date    :   November 28, 2021
 * @version :   1.0
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   -
*/

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <fatfs/ff.h>
#include <fatfs/diskio.h>
#include <Devices/MSPIO.h>

#include <inc/sd_card.h>
#include <inc/my_file_func.h>

#include <inc/sw420.h>
#include <inc/lcd.h>
#include <inc/buzzer.h>
#include <inc/mode.h>

/* Enum for time */
enum time_e
{
    YEAR,
    MONTH,
    DATE,
    HOUR,
    MINUTE,
    SECOND
};

void update_SD_card(RTC_C_Calendar ntime, gAcc_gyro sensor)
{
    static RTC_C_Calendar old_time = {0,0,0,0,0,0,0};
    FIL fp;
    //,fp1;

    /* Create buffers */
    char final_path[100] = {0};
    char write_buffer[200] = {0};
    char path[10][5] = {0};
    char read_buffer[20];

    /* Set values of RTC for the path */
    sprintf(path[YEAR],"%04d", ntime.year);
    sprintf(path[MONTH],"%02d", ntime.month);
    sprintf(path[DATE],"%02d", ntime.dayOfmonth);
    sprintf(path[HOUR],"%02d", ntime.hours);
    sprintf(path[MINUTE],"%02d", ntime.minutes);
    sprintf(path[SECOND],"%02d", ntime.seconds);

    /* If year changed, create a new directory in the correct path */
    if(ntime.year != old_time.year)
    {
        sprintf(final_path, "/");

        f_chdir(final_path);
        f_mkdir(path[YEAR]);
        f_chdir(path[YEAR]);

        f_getcwd(read_buffer, sizeof(read_buffer));
        MSPrintf(EUSCI_A0_BASE, read_buffer);

        memset(final_path, 0, sizeof(final_path));

    }
    /* If month changed, create a new directory in the correct path */
    if(ntime.month != old_time.month)
    {
        sprintf(final_path, "/%s/",path[YEAR]);

        f_chdir(final_path);
        f_mkdir(path[MONTH]);
        f_chdir(path[MONTH]);

        f_getcwd(read_buffer, sizeof(read_buffer));
        MSPrintf(EUSCI_A0_BASE, read_buffer);

        memset(final_path, 0, sizeof(final_path));

    }
    /* If date changed, create a new directory in the correct path */
    if(ntime.dayOfmonth != old_time.dayOfmonth)
    {

        sprintf(final_path, "/%s/%s/",path[YEAR], path[MONTH]);

        f_chdir(final_path);
        f_mkdir(path[DATE]);
        f_chdir(path[DATE]);

        f_getcwd(read_buffer, sizeof(read_buffer));
        MSPrintf(EUSCI_A0_BASE, read_buffer);

        memset(final_path, 0, sizeof(final_path));

    }
    /* If hour changed, create a new directory in the correct path */
    if(ntime.hours != old_time.hours)
    {
        sprintf(final_path, "/%s/%s/%s",path[YEAR], path[MONTH],path[DATE]);

        f_chdir(final_path);
        f_mkdir(path[HOUR]);
        f_chdir(path[HOUR]);

        f_getcwd(read_buffer, sizeof(read_buffer));
        MSPrintf(EUSCI_A0_BASE, read_buffer);

        memset(final_path, 0, sizeof(final_path));

    }

    char filename[10]={0};
    char date[20]={0};
    char timestamp[20]={0};
    char sensor_data[200] = {0};
    char sensor_data_disp[20] = {0};

    memset(read_buffer, 0, sizeof(read_buffer));

    sprintf(date, "Date-> %s-%s-%s", path[MONTH], path[DATE], path[YEAR]);
    sprintf(timestamp, "Time->%s:%s:%s ", path[HOUR], path[MINUTE], path[SECOND]);

    /* Print the time on the LCD */
    lcd_home();
    lcd_print(timestamp);

    /* Check the mode and print correct sensor's data */
    if(get_mode() == ACCELEROMETER)
    {
        lcd_print("A");
        lcd_set_cursor(0,1);
        sprintf(sensor_data_disp,"X%02dg Y%02dg Z%02dg  ",sensor.gAcclVal[0], sensor.gAcclVal[1], sensor.gAcclVal[2]);
        lcd_print(sensor_data_disp);
    }
    else
    {
        lcd_print("G");
        lcd_set_cursor(0,1);
        sprintf(sensor_data_disp,"X%02d Y%02d Z%02d     ",(int16_t)sensor.gGyroVal[0],(int16_t) sensor.gGyroVal[1],(int16_t) sensor.gGyroVal[2]);
        lcd_print(sensor_data_disp);
    }

    /* If shock was detectedpplay the buzzer and add a note along with sensor data */
    if(shock_detected() == true)
    {
        play_buzzer(BUZZ_CNT);
        sprintf(sensor_data,  "Acceleration(m/s^2)      : X = %d\t  , Y = %d\t , Z = %d\n"
                              "Gyroscope(degrees/sec)   : X = %f\t  , Y = %f, Z = %f\n"
                              "Shock Detected\n\r",
                              sensor.gAcclVal[0], sensor.gAcclVal[1], sensor.gAcclVal[2],
                              sensor.gGyroVal[0], sensor.gGyroVal[1], sensor.gGyroVal[2]);
    }
    else
    {
        sprintf(sensor_data,  "Acceleration(m/s^2)      : X = %d\t  , Y = %d\t , Z = %d\n"
                              "Gyroscope(degrees/sec)   : X = %f\t  , Y = %f, Z = %f\n\r",
                              sensor.gAcclVal[0], sensor.gAcclVal[1], sensor.gAcclVal[2],
                              sensor.gGyroVal[0], sensor.gGyroVal[1], sensor.gGyroVal[2]);
    }

    sprintf(write_buffer, "%s, %s \n%s", date, timestamp, sensor_data);

    sprintf(filename, "%s.txt", path[MINUTE]);
    MSPrintf(EUSCI_A0_BASE, write_buffer);

    my_file_append(fp, filename, write_buffer, strlen(write_buffer));

    /* Update old time */
    old_time = ntime;
    memset(final_path, 0, sizeof(final_path));

}


