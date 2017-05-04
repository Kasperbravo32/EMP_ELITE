/*
 * lcd_setup.h
 *
 *  Created on: 27. apr. 2017
 *      Author: Frederik
 */


#ifndef LCD_SETUP_H_
#define LCD_SETUP_H_

// ************************** Defines ********************************
#define LCD_RESET       0b00110000 // Reset the LCD
#define LCD_SET4BIT     0b00101000 // 4-bit data. 2-line display, 5x7 font
#define LCD_CLEAR       0b00000001 // Replace all characters with ASCII 'space'
#define LCD_ENTRYMODE   0b00000110 // Shift cursor from left to right on read/write
#define LCD_OFF         0b00001000 // Turn display off
#define LCD_ON          0b00001111 // Display on, Cursor On, Blink Cursor/Character
#define LCD_HOME        0b00000010 // Shift cursor to first position on first line
#define LCD_SETCURSOR   0b10000000 // Set cursor position


// Special ASCII characters
// ------------------------

#define LF      0x0A
#define FF      0x0C
#define CR      0x0D

#define ESC     0x1B



// ************************* Functions ********************************
void lcd_init2();

void lcd_4bitwrite( INT8U thebyte );

void lcd_write_instruction( INT8U theinstruction );

void lcd_write_character( INT8U thedata );

void lcd_write_string( INT8U thestring[] );

INT8U lcd_write_queue( INT8U Ch );

void lcd_task( INT8U task_no );

#endif /* LCD_SETUP_H_ */
