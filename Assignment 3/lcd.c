#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

//******************* Defines *******************//
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

//******************* Function declaration *******************//
void wait_micro(INT8U time);                    // Does nothing for "time" microsecounds
void pulse_enable(void);                        // Pulse the enable pin
void lcd_write8(INT8U value);                   // Write byte command
void lcd_write4(INT8U value);                   // Write 4bit command
void lcd_pinsetup(void);                        // Sets up the display pins
void lcd_write_char(INT8U value);               // Write a single character
void wait(INT8U time);                          // Wait for "time" millisecounds

// Functions to be used outside this scope.
void lcd_init(void);                            // Initializing LCD display
void lcd_print(INT8U string_type[]);            // Prints a string on the display
void lcd_shift(INT8U value);                    // Shift all characters on the display to the right. (may add left shift in future edurations)
void lcd_setcursor(INT8U value);                // Set cursor to specific position, determent by width and height of the display.
void lcd_clear(void);                           // Clears the display

//******************* End of function *******************//

void lcd_clear(void)
{
    lcd_write8(LCD_CLEAR_DISPLAY);
}

//******************* Set cursor *******************//
void lcd_setcursor(INT8U width, BOOLEAN height)
{
    if (height)                                 // If height is 1 add a linebreak
    {
        width = width + 0x28;                   // add linebreak
    }
    if (width >= LCD_DISPLAY_WIDTH)             // Make sure cursor is within borders of the display
    {
        width = 0x00;                           // If not, back to home.
    }
    lcd_write8(width); //Set cursor
}
//******************* End of function *******************//

//******************* Display shift *******************//
// Shifts the display to the right.
void lcd_shift(INT8U value)
{
    while(value)
    {
        lcd_write8(LCD_DISPLAY_SHIFT);          //Display shift
        wait(50);
        value--;
    }
}
//******************* End of function *******************//

//******************* Print string *******************//
// Print a string, if the string is longer than 16 characters add linebreak. Note: that it does not work for the second line
void lcd_print(INT8U string_type[])
{
    volatile int i = 0;                         // Resets i every time the function is entered
    while(string_type[i] != 0)                  // Runs the loop until the string is "empty".
    {
        lcd_write_char(string_type[i]);         // Print the i'th element of the string.
        i++;
        if (i == LCD_DISPLAY_WIDTH)             // If the string makes it to the end of the display add a linebreak.
        {
            lcd_write8(LCD_BREAKLINE);          //break line
        }
        wait_micro(80);
    }
}
//******************* End of function *******************//

//******************* HARDWARE SETUP *******************//
void lcd_pinsetup(void)
{

    int dummy;
    // Enable the GPIO port that is used for the LCD Display.
    SYSCTL_RCGC2_R |= (SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOD | SYSCTL_RCGC2_GPIOF) ;

    // Do a dummy read to insert a few cycles after enabling the peripheral.
    dummy = SYSCTL_RCGC2_R;

    GPIO_PORTC_LOCK_R = 0x4C4F434B;             // Unlock the CR register
    GPIO_PORTD_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_LOCK_R = 0x4C4F434B;

    GPIO_PORTC_CR_R   = 0xFF;                   // Enable overwrite of PUR to FP0
    GPIO_PORTD_CR_R   = 0xFF;
    GPIO_PORTF_CR_R   = 0xFF;

    GPIO_PORTC_DIR_R |= 0xF0;                   // Set port direction for the LCD display (all outputs).
    GPIO_PORTD_DIR_R |= 0x0C;
    GPIO_PORTF_DIR_R |= 0x0E;

    GPIO_PORTC_DEN_R |= 0xF0;                   // Enable the digital funktion of the GPIO pins for the LCD display.
    GPIO_PORTD_DEN_R |= 0x0C;                   // And the same thing for the RS and enable LCD pin
    GPIO_PORTF_DEN_R |= 0x0E;
}
//******************* End of function *******************//

//************ Initializing of the LCD display *************//
void lcd_init(void)
{
    lcd_pinsetup();                             //Setting up pins

    // This part is nessesary beacouse we are working with 3.3V logic and power and not 5V
    // The reset circuit only works when Vcc is above 4.5V
    // Need to implement som timing, cause of the slow access times on the LCD
    GPIO_PORTC_DATA_R &= ~(0xF0);               // Makes sure the datapins is off
    GPIO_PORTD_DATA_R &= ~(0x0C);               // Same for enable and RS

    wait(500);                                  // wait 500ms

    lcd_write4(LCD_FUNCTIONRESET);              //First try
    wait_micro(40000);

    lcd_write4(LCD_FUNCTIONRESET);              //Second try
    wait_micro(400);

    lcd_write4(LCD_FUNCTIONRESET);              //Third and lucky try
    wait_micro(400);

    lcd_write4(LCD_ENABLE_4BIT);                //Enable 4bit mode
    wait_micro(160);

    lcd_write8(LCD_FUNCTION_SET);               //4 bit mode, 1/16, 5x8 font
    wait_micro(160);

    lcd_write8(LCD_DISPLAY_OFF);                // Display off 
    wait_micro(160);

    lcd_write8(LCD_DISPLAY_ON_BLINK);           // Display on with blinking cursor
    wait_micro(160);

    lcd_write8(LCD_ENTRYMODE);                  // Entry mode 
    wait_micro(160);

    lcd_write8(LCD_HOME);                       // Home
    wait_micro(160);
}
//******************* End of function *******************//

//******************* Functional functions *******************//
// Writing characters to the display
void lcd_write_char(INT8U value)
{
    GPIO_PORTD_DATA_R |= LCD_RS;                // Enables the LCD_RS pin to set the LCD in data mode
    wait_micro(20);
    lcd_write8(value);                          // Write char
    wait_micro(20);
    GPIO_PORTD_DATA_R &= ~(LCD_RS);             //Makes sure the RS, is pull down. (optimisation could be to wait by setting the RS low when done with writing string.)
}

// Sends a byte in two halvs.
void lcd_write8(INT8U value)
{
    // I do not call the lcd_write4 because I want the program to be relatively fast.
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Makes sure that the bits are turned off.
    GPIO_PORTC_DATA_R |= ( value );             // Enable the upper nibble.
    pulse_enable();                             // Sends a pulse on the enable pin to make the controller read the data.
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Make sure the datapins is turned off before sending more data.

    // Note: Right shift does not save the value
    GPIO_PORTC_DATA_R |= (value << 4);          // Enable the lower nibble.
    pulse_enable();                             // Sends a pulse on the enable pin to make the controller read the data.
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Make sure the datapins is turned off before sending more data.
}

// If the LCD is in 8bit mode this command sends a 4bit value on the the highbits
void lcd_write4(INT8U value)
{
    GPIO_PORTD_DATA_R &= ~( LCD_RS );
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Makes sure that the bits are turned off.

    GPIO_PORTC_DATA_R |= ( value << 4 );        // Left shift the bits to make them sit on the highbits.
    pulse_enable();                             // Sends a pulse on the enable pin.
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Turns the bits off.
}

// Sends a pulse on the enable bit to "send" the command to the LCD
void pulse_enable(void)
{
    GPIO_PORTD_DATA_R |= LCD_E;
    wait_micro(1);                              // Enable pulse must be more than 450ns
    GPIO_PORTD_DATA_R &= ~(LCD_E);
    wait_micro(1);                              // Commands need > 37us to settle
}

// Thies funktion most be changed to software timers!!
// Waits in microsecounds*time
void wait_micro( INT8U time )
{
    time = 16*time;                             //80Mhz CPU
    while (time--);
}

//This function is only used when it's an estimate
void wait (INT8U time)
{
    time = 16000*time;                          //80Mhz CPU
    while (time--);
}
