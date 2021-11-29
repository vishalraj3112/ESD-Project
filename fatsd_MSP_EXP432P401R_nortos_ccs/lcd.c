/*
 * lcd.c
 *
 *  Created on: 28-Nov-2021
 *      Author: vishalraj
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
#include "lcd.h"

//LCD SCL 2.3
//LCD SDA 2.4

#define I2C_WRITE_VAL           0x38
#define I2C_READ_VAL            0x41

#define DATA_LINES              (0x20 | 0x00)
#define ROW_DISP                (0x20 | 0x08)
#define DOTS                    (0x20 | 0x00)
#define DISP_ON                 (0x08 | 0x04)
#define CURSOR_OFF              (0x08 | 0x00)
#define BLINK_OFF               (0x08 | 0x00)
#define CLR_DISP                (0x01)
#define CURSOR_INC              (0x04 | 0x02)
#define DISP_NO_SHIFT           (0x04 | 0x00)
#define DELAY                   (1)

// I2C configuration
I2C_Params lcd_i2c_params;
I2C_Handle lcd_i2c_handle;
I2C_Transaction lcd_i2cTranscation = {0};
uint8_t data_val;

static void delay_ms(uint32_t delay);
static void lcd_init(void);
static void i2c_write(uint8_t data);
static void lcd_cmd(uint8_t data);
static void lcd_4bit_write(uint8_t data);

void lcd_i2c_init(void)
{
    /* Call driver init functions */
    GPIO_init();
    //Display_init();
    I2C_init();
    printf("LCD i2c init!\r\n");

    I2C_Params_init(&lcd_i2c_params);
    lcd_i2c_params.bitRate = I2C_100kHz;
    lcd_i2c_handle = I2C_open(CONFIG_I2C_1, &lcd_i2c_params);

    lcd_init();
}


void lcd_init(void)
{
    data_val = 0x08;

    i2c_write(data_val);
    delay_ms(1);
    //1
    data_val = 0x30;
    i2c_write(data_val);

    data_val = 0x80;
    i2c_write(data_val);
    delay_ms(1);
    data_val &= 0xF7;
    i2c_write(data_val);
    delay_ms(1);
    //2
    data_val = 0x30;
    i2c_write(data_val);

    data_val = 0x80;
    i2c_write(data_val);
    delay_ms(1);
    data_val &= 0xF7;
    i2c_write(data_val);
    delay_ms(1);

    //3
    data_val = 0x30;
    i2c_write(data_val);

    data_val = 0x80;
    i2c_write(data_val);
    delay_ms(1);
    data_val &= 0xF7;
    i2c_write(data_val);
    delay_ms(1);

    data_val = 0x20;
    i2c_write(data_val);
    data_val = 0x80;
    i2c_write(data_val);
    delay_ms(1);
    data_val &= 0xF7;
    i2c_write(data_val);
    delay_ms(1);

    lcd_cmd(DATA_LINES | ROW_DISP | DOTS);
    lcd_cmd(DISP_ON | CURSOR_OFF | BLINK_OFF);
    lcd_cmd(CLR_DISP);
    lcd_cmd(CURSOR_INC | DISP_NO_SHIFT);

}

void i2c_write(uint8_t data)
{

    lcd_i2cTranscation.slaveAddress = I2C_WRITE_VAL;
    lcd_i2cTranscation.writeBuf = &data;
    lcd_i2cTranscation.writeCount = 1;

    I2C_transfer(lcd_i2c_handle, &lcd_i2cTranscation);
}

void delay_ms(uint32_t delay)
{
    uint32_t i;
    delay = delay*3000;

    for(i = 0; i < delay ; i++)
        __asm("  nop");

}

void lcd_cmd(uint8_t data)
{
    data_val &= 0xFB;//RS=0
    i2c_write(data_val);
    lcd_4bit_write(data);
}

void lcd_4bit_write(uint8_t data)
{
    uint8_t temp = 0x00;

    //write upper 4 bits
    temp = (data & 0xF0);//take upper 4 bits of data
    data_val &= 0x0F;//take lower 4 bits of data value
    data_val |= temp;// data(upper 4 bits) | data_val(lower 4 bits)
    i2c_write(data_val);//write to PCF

    data_val |= 0x08;//select DCB function.
    i2c_write(data_val);//write to PCF
    delay_ms(DELAY); //replace with busy wait
    data_val &= 0xF7;//clear DCB
    i2c_write(data_val);//write to PCF.
    delay_ms(DELAY);//replace with busy wait.

    //write lower 4 bits
    temp = (data & 0x0F);//take lower 4 bits of data
    temp <<= 0x04;
    data_val &= 0x0F;
    data_val |= temp;   // data(upper 4 bits) | data_val(lower 4 bits)
    i2c_write(data_val); //write to PCF

    data_val |= 0x08; //set and clear DCF
    i2c_write(data_val);
    delay_ms(DELAY);
    data_val &= 0xF7;
    i2c_write(data_val);
    delay_ms(DELAY);
}
