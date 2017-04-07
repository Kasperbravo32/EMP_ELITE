/*
 * lcd.c
 *
 *  Created on: 28. mar. 2017
 *      Author: Frederik
 */
// ------------------------------------------
//                 Functions
// ------------------------------------------
#include <stdint.h>
#include "lcd.h"
#include "emp_type.h"
#include "tm4c123gh6pm.h"


void lcd_init(void)
{
    // This part is nessesary beacouse we are working with 3.3V logic and power and not 5V
    // The reset circuit only works when Vcc is above 4.5V
    // Need to implement som timing, cause of the slow access times on the LCD
    GPIO_PORTC_DATA_R &= ~(0xF0);       //slukker alle data porte
    GPIO_PORTD_DATA_R &= ~(0x0C);       //slukker RS og Enable

    wait_mil(500);                          // wait 500ms

    lcd_write4(LCD_FUNCTIONRESET);          //First try
    wait_micro(40000);

    lcd_write4(LCD_FUNCTIONRESET);          //Second try
    wait_micro(400);

    lcd_write4(LCD_FUNCTIONRESET);          //Third and lucky try
    wait_micro(400);

    lcd_write4(LCD_ENABLE_4BIT);            //Enable 4bit mode
    wait_micro(160);

    //lcd_write8(0x2C);
    /*//*/lcd_write8(LCD_FUNCTION_SET);     //4 bit mode, 1/16, 5x8 font
    wait_micro(160);

    lcd_write8(LCD_DISPLAY_OFF);            // Display off
    wait_micro(160);

    lcd_write8(LCD_DISPLAY_ON);             // Display on with blinking cursor
    wait_micro(160);

    lcd_write8(LCD_ENTRYMODE);              // Entry mode
    wait_micro(160);

    lcd_write8(LCD_HOME);                   // Home
    wait_micro(160);

}

void lcd_write(int value)
{
    // Enables the LCD_RS pin to set the LCD in data mode
    GPIO_PORTD_DATA_R |= LCD_RS;
    wait_micro(200);
    lcd_write8(value);
    wait_micro(20);
    GPIO_PORTD_DATA_R &= ~(LCD_RS);
}

void lcd_write4(int value)
{
    GPIO_PORTD_DATA_R &= ~(LCD_RS);             // Turn RS-Bit to instruction-mode (0)

    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Makes sure that the bits are turned off.

    GPIO_PORTC_DATA_R |= (value << 4);          // Left shift the bits to make them sit on the highbits.
    pulse_enable();                             // Sends a pulse.
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Turns the bits off.
}

void lcd_write8(int value)
{
    GPIO_PORTC_DATA_R &= ~(0xF0);               // Makes sure that the bits are turned off.
    GPIO_PORTC_DATA_R |= ( value );             //Tænder for de øvre bits
    pulse_enable();                             // Sender en puls på enable for at sende bits
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Søger for at de bliver slukket igen

    GPIO_PORTC_DATA_R |= (value << 4);          //Tænder for de øvre bits
    pulse_enable();                             // Sender en puls på enable for at sende bits
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Søger for at de bliver slukket igen
    // I do not call the lcd_write4 because I want the program to be relatively fast.
    // Right shift does not save the value
}

void pulse_enable(void)
{
    E_FLIP(1);
    E_FLIP(0);
}

void wait_micro( int time )
{
    time = 16*time;
    while (time--);
}

void wait_mil (int time)
{
    time = 16000*time;
    while (time--);
}

void lcd_instruct(int value)
{
    E_FLIP(0);
    wait_micro(100);
    RS_FLIP(INSTRUCT);
    wait_micro(100);

    GPIO_PORTC_DATA_R |= ( value );             // Data to first 4 bits
    pulse_enable();                             // Pulses enable
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Turns of upper 4 bits

    GPIO_PORTC_DATA_R |= (value << 4);          //Tænder for de øvre bits
    pulse_enable();                             // Sender en puls på enable for at sende bits
    GPIO_PORTC_DATA_R &= ~( 0xF0 );             // Søger for at de bliver slukket igen
    RS_FLIP(DATA);
    wait_mil(1);
}

void lcd_data(int value)
{
    E_FLIP(0);
    RS_FLIP(DATA);

    GPIO_PORTC_DATA_R &= ~(0xF0);
    GPIO_PORTC_DATA_R |= value;
    pulse_enable();

    GPIO_PORTC_DATA_R &= ~( 0xF0 );
    GPIO_PORTC_DATA_R |= (value << 4);
    pulse_enable();

    GPIO_PORTC_DATA_R &= ~(0xF0);
    wait_micro(100);
}

// Function to flip RS-bit. input == 1, sets RS to data mode
// input == 2 sets RS to instruction mode
int RS_FLIP(int value)
{
    if (value == 1)
        (GPIO_PORTD_DATA_R |= LCD_RS);
    else if (value == 0)
        (GPIO_PORTD_DATA_R &= ~(LCD_RS));
    wait_micro(100);
}

int E_FLIP (int value)
{
    if (value == 1)
        (GPIO_PORTD_DATA_R |= LCD_E);
    else if (value == 0)
        (GPIO_PORTD_DATA_R &= ~(LCD_E));
    wait_micro(1);
}

void lcd_data_string(char str[])
{
    volatile int i = 0;
    while (str[i] != 0)
    {
        lcd_data(str[i]);
        i++;
        wait_micro(40);
    }
}
