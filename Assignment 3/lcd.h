/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: events.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090215  MoH   Module created.
*
*****************************************************************************/

#ifndef LCD_H_
#define LCD_H_

/***************************** Include files *******************************/
//****************************   Defines   *********************************/
// Pins
#define LCD_RS                  0x04            // PD2
#define LCD_E                   0x08            // PD3

// Hardware
#define LCD_DISPLAY_WIDTH       0x10            // How many characters in width

// Display modes
#define LCD_FUNCTIONRESET       0x03            // Function reset
#define LCD_ENABLE_4BIT         0x02            // Enable 4bit mode
#define LCD_FUNCTION_SET        0x28            // 4 bit mode, 1/16, 5x8 font
#define LCD_DISPLAY_OFF         0x08            // Display off
#define LCD_DISPLAY_ON_BLINK    0x0F            // Display on with blinking cursor
#define LCD_ENTRYMODE           0x06            // Entry mode 
#define LCD_HOME                0x02            // Home
#define LCD_CLEAR_DISPLAY       0x01            // Clear display
#define LCD_DISPLAY_SHIFT       0x1C            // Right shift all characters
#define LCD_BREAKLINE           0xA8            // Change cursor to start of second line


/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
void lcd_init(void);                            // Initializing LCD display
void lcd_print(INT8U string_type[]);            // Prints a string on the display
void lcd_shift(INT8U value);                    // Shift all characters on the display to the right. (may add left shift in future edurations)
void lcd_setcursor(INT8U width, BOOLEAN height);// Set cursor to specific position, determent by width and height of the display.
void lcd_clear(void);                           // Clears the display


/****************************** End Of Module *******************************/
#endif /*LCD_H_*/
