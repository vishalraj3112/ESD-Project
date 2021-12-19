/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
/******************************************************************************* */

/*
 * Project  - Data Logger for Automotive and Aerial Vehicles
 * Members  - Sanish Kharade and Vishal Raj
 *            University of Colorado Boulder
 * Course   - Embedded System Design
 *
 * Commemnts    - All function descriptions are present in header files.
 *
 * */

/**
 * @file    :   main.c
 *
 * @brief   :   Main file
 *
 * @author  :   Sanish Kharade
 * @date    :   November 28, 2021
 * @version :   1.0
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   -
*/

/* Standard Defines */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* DriverLib Defines */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <Hardware/SPI_Driver.h>
#include <Hardware/GPIO_Driver.h>
#include <Hardware/CS_Driver.h>
#include <Hardware/TIMERA_Driver.h>
#include <fatfs/ff.h>
#include <fatfs/diskio.h>
#include <Devices/MSPIO.h>


#include <inc/my_file_func.h>
#include <inc/rtc.h>
#include <inc/sd_card.h>
#include <inc/sw420.h>
#include <inc/lcd.h>
#include <inc/buzzer.h>
#include <inc/lcd.h>
#include <inc/mpu.h>
#include <inc/mode.h>


/* Slave Address for I2C Slave */
#define SLAVE_ADDRESS       0x68


/* Variables */
const uint8_t TXData[] = {0x04};
uint8_t RXData[NUM_OF_REC_BYTES];
static volatile uint32_t xferIndex;
static volatile bool stopSent;
volatile bool read_done = false;

/* I2C Master Configuration Parameter */
const eUSCI_I2C_MasterConfig i2cConfig =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        3000000,                                // SMCLK = 3MHz
        EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};


/*Huge thanks to bluehash @ https://github.com/bluehash/MSP432Launchpad/tree/master/MSP432-Launchpad-FatFS-SDCard*/

/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 115200 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */

eUSCI_UART_ConfigV1 UART0Config =
{
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     13,
     0,
     37,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
};

/* SPI Configuration Parameter. These are the configuration parameters to
 * make the eUSCI B SPI module to operate with a 500KHz clock.*/
eUSCI_SPI_MasterConfig SPI0MasterConfig =
{
     EUSCI_B_SPI_CLOCKSOURCE_SMCLK,
     3000000,
     500000,
     EUSCI_B_SPI_MSB_FIRST,
     EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,
     EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH,
     EUSCI_B_SPI_3PIN
};

/* Timer_A UpMode Configuration Parameters */
Timer_A_UpModeConfig upConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_64,          // SMCLK/1 = 3MHz
        30000,                                  // 1 ms tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};

FATFS FS;
DIR DI;
FILINFO FI, FILEINFO;

/*********************************************************************************
 * @function:   main
 *
 * @brief   :   Main entry point to the application
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void main(void)
{
    WDT_A_holdTimer();

    DWORD str=0;
    str = get_fattime();
    RTC_C_Calendar start_time =
    {
         (str & 0x0000001F)*2,
         (str & 0x000007E0)>>5,
         (str & 0x0000F800)>>11,
         3,
         (str & 0x001F0000)>>16,
         (str & 0x01E00000)>>21,
         ((str & 0xFE000000)>>21)/16 + 1980 // epoch has been set as 1980
         //>>25 was not allowed for year field. hence >>21 and divide by 16
    };

    CS_Init();
    RTC_init(start_time);
    
    /*Initialize all hardware required for the SD Card*/
    SPI_Init(EUSCI_B0_BASE, SPI0MasterConfig);
    UART_Init(EUSCI_A0_BASE, UART0Config);
    GPIO_Init(GPIO_PORT_P5, GPIO_PIN0);
    TIMERA_Init(TIMER_A1_BASE, UP_MODE, &upConfig, disk_timerproc);

    Interrupt_enableMaster();

    char startup_buff[100];
    sprintf(startup_buff, "Startup - Date-> %02d-%02d-%04d, Time-> %02d:%02d:%02d",   start_time.month, start_time.dayOfmonth, start_time.year,
                                                                    start_time.hours, start_time.minutes, start_time.seconds);
    MSPrintf(EUSCI_A0_BASE, startup_buff);

    FRESULT r;

    /* Mount the SD Card into the Fatfs file system*/
    r = f_mount(&FS, "0", 1);
    if(r != FR_OK)
    {
        MSPrintf(EUSCI_A0_BASE, "Error mounting SD Card, check your connections\r\n");
        while(1);
    }

    /* Open the root directory on the SD Card*/
    r = f_opendir(&DI, "/");
    if(r != FR_OK)
    {
        MSPrintf(EUSCI_A0_BASE, "Could not open root directory\r\n");
        while(1);
    }

    /*Read everything inside the root directory*/
    do
    {
        /*Read a directory/file*/
        r = f_readdir(&DI, &FI);
        if(r != FR_OK)
        {
           MSPrintf(EUSCI_A0_BASE, "Error reading file/directory\r\n");
           while(1);
        }

        /*Print the file to the serial terminal*/
        MSPrintf(EUSCI_A0_BASE, "%c%c%c%c%c %s\r\n",
               (FI.fattrib & AM_DIR) ? 'D' : '-',
               (FI.fattrib & AM_RDO) ? 'R' : '-',
               (FI.fattrib & AM_HID) ? 'H' : '-',
               (FI.fattrib & AM_SYS) ? 'S' : '-',
               (FI.fattrib & AM_ARC) ? 'A' : '-',
               ((char*)FI.fname));

    }while(FI.fname[0]);


    RTC_C_Calendar time;
    char read_buf[70]={0};
    //f_getcwd(read_buf, sizeof(read_buf));
    MSPrintf(EUSCI_A0_BASE, read_buf);

    SW420_gpio_init();
    mode_gpio_init();

    //////////////////////////////////////MPU6050/////////////////////////////////////

    /* Select Port 6 for I2C - Set Pin 4, 5 to input Primary Module Function,
     *   (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL).
     */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
            GPIO_PIN4 + GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);
    stopSent = false;
    memset(RXData, 0x00, NUM_OF_REC_BYTES);

    /* Initializing I2C Master to SMCLK at 100khz with no autostop */
    MAP_I2C_initMaster(EUSCI_B1_BASE, &i2cConfig);

    /* Specify slave address */
    MAP_I2C_setSlaveAddress(EUSCI_B1_BASE, SLAVE_ADDRESS);

    /* Enable I2C Module to start operations */
    MAP_I2C_enableModule(EUSCI_B1_BASE);
    MAP_Interrupt_enableInterrupt(INT_EUSCIB1);

    //////////////////////////////////////MPU6050/////////////////////////////////////

    gAcc_gyro sensor;

    i2c_init();

    lcd_i2c_init();
    MPU6050_Reset();

    while(1)
    {
        if(one_second_elapsed() == true)
        {

            time = RTC_get_current_time();

            sensor = get_mpu_values();
            update_SD_card(time,sensor);
            clear_one_second_elapsed_flag();
            clear_shock_detected_flag();

        }
    }

}
/*******************************************************************************
 * eUSCIB0 ISR. The repeated start and transmit/receive operations happen
 * within this ISR.
 *******************************************************************************/
void EUSCIB1_IRQHandler(void)
{
    uint_fast16_t status;

    status = MAP_I2C_getEnabledInterruptStatus(EUSCI_B1_BASE);

    /* Receives bytes into the receive buffer. If we have received all bytes,
     * send a STOP condition */
    if (status & EUSCI_B_I2C_RECEIVE_INTERRUPT0)
    {
        if (read_idx == NUM_OF_REC_BYTES - 2)
        {
            MAP_I2C_disableInterrupt(EUSCI_B1_BASE,
                    EUSCI_B_I2C_RECEIVE_INTERRUPT0);
            MAP_I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_STOP_INTERRUPT);//dont disable I2c

            /*
             * Switch order so that stop is being set during reception of last
             * byte read byte so that next byte can be read.
             */
            MAP_I2C_masterReceiveMultiByteStop(EUSCI_B1_BASE);
            RXData[read_idx++] = MAP_I2C_masterReceiveMultiByteNext(
                    EUSCI_B1_BASE);
            if(read_idx == NUM_OF_REC_BYTES -1)//last byte received
                read_done = true;
        }
        else
        {
            RXData[read_idx++] = MAP_I2C_masterReceiveMultiByteNext(
            EUSCI_B1_BASE);
        }
    }
    else if (status & EUSCI_B_I2C_STOP_INTERRUPT)
    {
        RXData[read_idx++] = MAP_I2C_masterReceiveMultiByteNext(
                EUSCI_B1_BASE);
        MAP_Interrupt_disableSleepOnIsrExit(); //dont sleep on exit
        MAP_I2C_disableInterrupt(EUSCI_B1_BASE,EUSCI_B_I2C_STOP_INTERRUPT);//dont disable I2C!
    }
}
