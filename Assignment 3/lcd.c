#include <stdint.h>
#include "tm4c123gh6pm.h"

#define LCD_D4                  0x10    // PC4
#define LCD_D5                  0x20    // PC5
#define LCD_D6                  0x40    // PC6
#define LCD_D7                  0x80    // PC7

#define LCD_RS                  0x04    // PD2
#define LCD_E                   0x08    // PD3

#define LCD_CLEAR_DISPLAY       0x01    // Clear display
#define LCD_DISPLAY_OFF         0x08    // Display off
#define LCD_DISPLAY_ON_BLINK    0x0C    // Display on with blinking cursor
#define LCD_FUNCTIONRESET       0x03    // Function reset
#define LCD_ENABLE_4BIT         0x02    // Enable 4bit mode
#define LCD_FUNCTION_SET        0x28    // 4 bit mode, 1/16, 5x8 font
#define LCD_ENTRYMODE           0x06    // Entry mode 
#define LCD_HOME                0x02    // Home
//******************* Function declaration *******************//
void wait_micro( int time );
void lcd_write8(int value);
void lcd_write4(int value);
void pulse_enable(void);
void init_LCD(void);
void wait(int time);

// This function sets up the LCD display for the EMP board
int main(void)
{

    lcd_pinsetup();     //Setting up pins

    lcd_init();         //Initializing LCD

    //Clear display
    GPIO_PORTD_DATA_R &= ~(LCD_RS);
    lcd_write8(LCD_CLEAR_DISPLAY);
    wait_micro(10);

    while(1);

    return(0);
}
//******************* End of function *******************//

//******************* HARDWARE SETUP *******************//
void lcd_pinsetup(void)
{

    int dummy;
    // Enable the GPIO port that is used for the LCD Display.
    SYSCTL_RCGC2_R |= (SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOD) ;

    // Do a dummy read to insert a few cycles after enabling the peripheral.
    dummy = SYSCTL_RCGC2_R;

    GPIO_PORTC_LOCK_R = 0x4C4F434B;  // Unlock the CR register
    GPIO_PORTD_LOCK_R = 0x4C4F434B;  // Unlock the CR register

    GPIO_PORTC_CR_R   = 0xFF;        // Enable overwrite of PUR to FP0
    GPIO_PORTD_CR_R   = 0xFF;        // Enable overwrite of PUR to FP0

    // Set port direction for the LCD display (all outputs).
    GPIO_PORTC_DIR_R |= 0xF0;
    GPIO_PORTD_DIR_R |= 0x0C;

    // Enable the digital funktion of the GPIO pins for the LCD display.
    GPIO_PORTC_DEN_R |= 0xF0;
    GPIO_PORTD_DEN_R |= 0x0C;
    // And the same thing for the RS and enable LCD pin

}
//******************* End of function *******************//

//************ Initializing of the LCD display *************//
void lcd_init(void)
{
    // This part is nessesary beacouse we are working with 3.3V logic and power and not 5V
    // The reset circuit only works when Vcc is above 4.5V
    // Need to implement som timing, cause of the slow access times on the LCD
    GPIO_PORTC_DATA_R &= ~(0xF0);       //slukker alle data porte
    GPIO_PORTD_DATA_R &= ~(0x0C);       //slukker RS og Enable

    wait(5000);                         // wait 500ms

    lcd_write4(LCD_FUNCTIONRESET);      //First try
    wait_micro(2000);

    lcd_write4(LCD_FUNCTIONRESET);      //Second try
    wait_micro(400);

    lcd_write4(LCD_FUNCTIONRESET);      //Third and lucky try
    wait_micro(400);

    lcd_write4(LCD_ENABLE_4BIT);        //Enable 4bit mode
    wait_micro(4500);

    lcd_write8(LCD_FUNCTION_SET);       //4 bit mode, 1/16, 5x8 font
    wait_micro(4500);

    lcd_write8(LCD_DISPLAY_OFF);        // Display off 
    wait_micro(4500);

    lcd_write8(LCD_DISPLAY_ON_BLINK);   // Display on with blinking cursor
    wait_micro(4500);

    lcd_write8(LCD_ENTRYMODE);          // Entry mode 
    wait_micro(4500);

    lcd_write8(0x02);                   // Home
    wait_micro(4500);
}
//******************* End of function *******************//

// Writing characters to the display
void lcd_write(int value)
{
    // Enables the LCD_RS pin to set the LCD in data mode
    GPIO_PORTD_DATA_R |= LCD_RS;
    wait(2);
    lcd_write8(value);
    wait(2);
    GPIO_PORTD_DATA_R &= ~(LCD_RS);
}
//******************* End of function *******************//

// Sends a byte in two halvs.
void lcd_write8(int value)
{
    GPIO_PORTC_DATA_R |= ( value && 0xF0 );     //Tænder for de nedre bits
    pulse_enable();                             // Sender en puls på enable for at sende bits
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Søger for at de bliver slukket igen


    GPIO_PORTC_DATA_R |= (value << 4);          //Tænder for de øvre bits
    pulse_enable();                             // Sender en puls på enable for at sende bits
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Søger for at de bliver slukket igen
    // I do not call the lcd_write4 because I want the program to be relatively fast.
    // Right shift does not save the value
}
//******************* End of function *******************//

// If the LCD is in 8bit mode this command sends a 4bit value on the the highbits
void lcd_write4(int value)
{
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Søger for at de bliver slukket igen
    GPIO_PORTC_DATA_R |= (value << 4) ;         // Tænder bits
    pulse_enable();                             // Sender en puls på enable for at sende bits
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Søger for at de bliver slukket igen
}
//******************* End of function *******************//

// Sends a pulse on the enable bit to "send" the command to the LCD
void pulse_enable(void)
{
    GPIO_PORTD_DATA_R |= LCD_E;
    wait_micro(1);                              // enable pulse must be more than 450ns
    GPIO_PORTD_DATA_R &= ~(LCD_E);
    wait_micro(1);                              // commands need > 37us to settle
}
//******************* End of function *******************//

// Waits in microsecounds*time
void wait_micro( int time )
{                           //80Mhz CPU
    while (time--);
}
//******************* End of function *******************//

//This function is only used when it's an estimate
void wait (int time)
{
    time = 2000*time;
    while (time--);
}
//******************* End of function *******************//
