/*
 * lcd.h
 *
 *  Created on: 28. mar. 2017
 *      Author: Frederik
 */
#ifndef LCD_H_
#define LCD_H_
// ------------------------------------------
//                 Includes
// ------------------------------------------

//#include <stdint.h>
#include "emp_type.h"
#include "tm4c123gh6pm.h"

// ------------------------------------------
//                 Defines
// ------------------------------------------

#define LCD_D4                  0x10    // PC4
#define LCD_D5                  0x20    // PC5
#define LCD_D6                  0x40    // PC6
#define LCD_D7                  0x80    // PC7

#define LCD_RS                  0x04    // PD2
#define LCD_E                   0x08    // PD3

#define LCD_FUNCTIONRESET       0x03    // Function reset
#define LCD_ENABLE_4BIT         0x02    // Enable 4bit mode
#define LCD_FUNCTION_SET        0x28    // 4 bit mode, 1/16, 5x8 font
#define LCD_DISPLAY_OFF         0x08    // Display off
#define LCD_DISPLAY_ON          0x0C    // Display, cursor blink off
#define LCD_DISPLAY_ON_BLINK    0x0F    // Display on with blinking cursor
#define LCD_ENTRYMODE           0x06    // Entry mode
#define LCD_HOME                0x02    // Home
#define LCD_CLEAR_DISPLAY       0x01    // Clear display
#define SET_CURSOR              0x80    // Set DDRAM Adress
#define INSTRUCT                0       // RS bit instruction mode
#define DATA                    1       // RS bit DATA mode
#define NEXT_LINE               0xC0    // Toggles start of next line

// ------------------------------------------
//                 Functions
// ------------------------------------------

void lcd_pinsetup(void);

void lcd_init(void);

void lcd_write(int value);

void lcd_write4(int value);

void lcd_write8(int value);

void pulse_enable(void);

void wait_micro( int time );

void wait_mil (int time);

void set_cursor(int location);

void lcd_writedata(int value);

void lcd_data(int value);

void lcd_instuct(int value);

//int lcd_data_string(char str[])

int RS_flip(int value);

int E_FLIP(int value);

#endif /* LCD_H_ */
