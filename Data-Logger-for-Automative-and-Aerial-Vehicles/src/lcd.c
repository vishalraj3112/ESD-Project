/**
 * @file    :   lcd.c
 * @brief   :   An abstraction for lcd functions
 *
 *              This source file provides the LCD functions and
 *              to write data at specific locations of the LCD.
 *
 * @author  :   Vishal Raj
 * @date    :   November 26, 2021
 * @version :   1.0
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   LCD command sequence,values referred from-
 *              https://github.com/johnrickman/LiquidCrystal_I2C/blob/master/LiquidCrystal_I2C.cpp.
*/

#include <stdbool.h>
#include <stdio.h>
#include  <math.h>
#include <stdint.h>
#include <stddef.h>
#include "lcd.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


#define I2C_WRITE_VAL           0x27
#define I2C_READ_VAL            0x41

#define DATA_LINES              (0x20 | 0x00)
#define ROW_DISP                (0x20 | 0x08)
#define DOTS                    (0x20 | 0x00)
#define DISP_ON                 (0x08 | 0x04)
#define CURSOR_OFF              (0x08 | 0x00)
#define BLINK_OFF               (0x08 | 0x00)
#define CLR_DISP                (0x01)
#define GO_HOME                  0x02
#define CURSOR_INC              (0x04 | 0x02)
#define DISP_NO_SHIFT           (0x04 | 0x00)
#define DELAY                   (1)

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

//lcd variables
uint8_t data_val;
uint8_t backlight_val;
uint8_t display_func,display_mode,display_control;


#define SLAVE_ADDRESS       0x27//0x48
#define NUM_OF_REC_BYTES    1

/* Variables */
const uint8_t TXData_lcd[] = {0x08,0x08};

/* I2C Master Configuration Parameter */
const eUSCI_I2C_MasterConfig i2cConfig_lcd =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        3000000,                                // SMCLK = 3MHz (default)
        EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};


void i2c_init(void)
{

    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
            GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Initializing I2C Master to SMCLK at 100khz with no autostop */
    MAP_I2C_initMaster(EUSCI_B2_BASE, &i2cConfig_lcd);

    /* Specify slave address */
    MAP_I2C_setSlaveAddress(EUSCI_B2_BASE, SLAVE_ADDRESS);

    /* Enable I2C Module to start operations */
    MAP_I2C_enableModule(EUSCI_B2_BASE);
    MAP_Interrupt_enableInterrupt(INT_EUSCIB2);

    // enable RX interrupts
    MAP_I2C_enableInterrupt(EUSCI_B2_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);
}
void lcd_i2c_init(void)
{

    lcd_init();

    lcd_set_cursor(3,0);
    lcd_print("ESD Project");

    lcd_set_cursor(0,1);
    lcd_print("Vishal & Sanish");

}

//Used to position the LCD cursor
void lcd_set_cursor(uint8_t col, uint8_t row)
{
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

    lcd_cmd(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcd_init(void)
{

    backlight_val = LCD_BACKLIGHT;
    display_func = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    display_func |= LCD_2LINE;
    display_func |= LCD_5x10DOTS;

    delay_ms(50);//Test delay

    i2c_write(backlight_val);
    delay_ms(1000);

    //write data in 8 bit mode and shift to 4 bit mode.
    lcd_4bit_write(0x03 << 4);
    delay_ms(5);

    lcd_4bit_write(0x03 << 4);
    delay_ms(5);

    lcd_4bit_write(0x03 << 4);
    delay_ms(5);

    //set to 4 bit.
    lcd_4bit_write(0x02 <<4);

    //set no of lines and font size, etc.
    lcd_cmd(LCD_FUNCTIONSET | display_func);

    //turn cursor on with blinking
    display_control = LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF;//changed
    lcd_display();

    lcd_clear();

    display_mode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    //set entry mode
    lcd_cmd(LCD_ENTRYMODESET | display_mode);

    lcd_home();
}

void lcd_home()
{
    lcd_cmd(LCD_RETURNHOME);
    delay_ms(2);
}
void lcd_display()
{
    display_control |= LCD_DISPLAYON;
    lcd_cmd(LCD_DISPLAYCONTROL | display_control);
}

void lcd_clear()
{
    lcd_cmd(LCD_CLEARDISPLAY);
    delay_ms(2);
}

void i2c_write(uint8_t data)
{
    data = data | backlight_val;

    //Write one byte of data to I2C slave from master
    while (MAP_I2C_masterIsStopSent(EUSCI_B2_BASE));

    MAP_I2C_masterSendSingleByte(EUSCI_B2_BASE,data);

}

void delay_ms(uint32_t delay)
{
    uint32_t i;
    delay = delay*300;

    for(i = 0; i < delay ; i++)
        __asm("  nop");

}

void lcd_cmd(uint8_t data)
{
    lcd_send(data,0);//RS=0;
}

void lcd_print(char data[])
{
    char *str_ptr = data;
    while(*str_ptr)
        lcd_send(*str_ptr++,Rs);
}

void lcd_send(uint8_t data, uint8_t mode)
{
    uint8_t high_nibble = data & 0xF0;
    uint8_t low_nibble = (data << 4) & 0xF0;
    lcd_4bit_write(high_nibble | mode);
    lcd_4bit_write(low_nibble | mode);
}

void lcd_4bit_write(uint8_t data)
{
   i2c_write(data);
   pulse_enable(data);
}

void pulse_enable(uint8_t data)
{
    i2c_write(data | En);
    delay_ms(1);

    i2c_write(data & ~En);
    delay_ms(1);
}

void lcd_clear_home(void)
{
    lcd_cmd(CLR_DISP);
    lcd_cmd(GO_HOME);
}
