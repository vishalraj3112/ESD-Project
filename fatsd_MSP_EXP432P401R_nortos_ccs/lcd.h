/*
 * lcd.h
 *
 *  Created on: 28-Nov-2021
 *      Author: vishalraj
 */

#ifndef LCD_H_
#define LCD_H_


void lcd_i2c_init(void);
void delay_ms(uint32_t delay);
void lcd_init(void);
void i2c_write(uint8_t data);
void lcd_cmd(uint8_t data);
void lcd_4bit_write(uint8_t data);
void lcd_clear_home(void);
void lcd_home();
void lcd_display();
void lcd_clear();
void lcd_send(uint8_t data, uint8_t mode);
void pulse_enable(uint8_t data);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(uint8_t data[]);


#endif /* LCD_H_ */
