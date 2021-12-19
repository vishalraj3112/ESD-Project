/**
 * @file    :   mpu.h
 * @brief   :   Hearder for LCD driver
 *
 *             This header file declares LCD functions needed
 *             to write data to the LCD.
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

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

/*********************************************************************************
 * @function:   i2c_init
 *
 * @brief   :   Initializes the I2C for the LCD
 *              using EUSCIB2   :   SDA - pin 3.6
 *                                  SCL - pin 3.7
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void i2c_init(void);


/*********************************************************************************
 * @function:   lcd_i2c_init
 *
 * @brief   :   Initializes the I2C for LCD
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void lcd_i2c_init(void);    //redundant function

/*********************************************************************************
 * @function:   delay_ms
 *
 * @brief   :   Creates a delay in milliseconds using a hard spin loop
 *
 * @param   :   delay   - delay in  milliseconds
 *
 * @return  :   void
 *
**********************************************************************************/
void delay_ms(uint32_t delay);

/*********************************************************************************
 * @function:   lcd_init
 *
 * @brief   :   Initializes the LCD
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void lcd_init(void);

/*********************************************************************************
 * @function:   i2c_write
 *
 * @brief   :   Writes data to the I2C of LCD
 *
 * @param   :   data    - byte of data to be written
 *
 * @return  :   void
 *
**********************************************************************************/
void i2c_write(uint8_t data);

/*********************************************************************************
 * @function:   lcd_cmd
 *
 * @brief   :   Send a command to the LCD
 *
 * @param   :   data    - command to be sent
 *
 * @return  :   void
 *
**********************************************************************************/
void lcd_cmd(uint8_t data);

/*********************************************************************************
 * @function:   lcd_4bit_write
 *
 * @brief   :   writes 4 bits to the LCD
 *
 * @param   :   data    - data to be written
 *
 * @return  :   void
 *
**********************************************************************************/
void lcd_4bit_write(uint8_t data);

/*********************************************************************************
 * @function:   lcd_clear_home
 *
 * @brief   :   clears the LCD and returns to the start
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void lcd_clear_home(void);

/*********************************************************************************
 * @function:   lcd_home
 *
 * @brief   :   returns to the start
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void lcd_home();

/*********************************************************************************
 * @function:   lcd_display
 *
 * @brief   :   turns on the LCD display
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void lcd_display();

/*********************************************************************************
 * @function:   lcd_clear
 *
 * @brief   :   clears the LCD screen
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void lcd_clear();

/*********************************************************************************
 * @function:   lcd_send
 *
 * @brief   :   clears the LCD screen
 *
 * @param   :   data - data to be sent
 *              mode - mode of the LCD
 *
 * @return  :   void
 *
**********************************************************************************/
void lcd_send(uint8_t data, uint8_t mode);

/*********************************************************************************
 * @function:   pulse_enable
 *
 * @brief   :   enables the pulse (blinking), to enable the LCD
 *
 * @param   :   data - data to be sent
 *
 * @return  :   void
 *
**********************************************************************************/
void pulse_enable(uint8_t data);

/*********************************************************************************
 * @function:   lcd_set_cursor
 *
 * @brief   :   sets the LCD cursor
 *
 * @param   :   col - column value
 *              row - row value
 *
 * @return  :   void
 *
**********************************************************************************/
void lcd_set_cursor(uint8_t col, uint8_t row);

/*********************************************************************************
 * @function:   lcd_print
 *
 * @brief   :   prints a string on to the LCD
 *
 * @param   :   data    - string to be printed
 *
 * @return  :   void
 *
**********************************************************************************/
void lcd_print(char data[]);


#endif /* LCD_H_ */
