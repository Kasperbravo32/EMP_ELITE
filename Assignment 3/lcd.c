#include <stdint.h>
#include "tm4c123gh6pm.h"

#define LCD_D4  0x10    //PC4
#define LCD_D5  0x20    //PC5
#define LCD_D6  0x40    //PC6
#define LCD_D7  0x80    //PC7

#define LCD_RS  0x04    //PD2
#define LCD_E   0x08    //PD3

// This function sets up the LCD display for the EMP board
void lcd_init(void)
{
    //******************* HARDWARE SETUP *******************//
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
    // And the same thing for the RS and enable LCD pin
    GPIO_PORTD_DEN_R |= 0x0C;

    //************ Initializing of the LCD display *************//
    // This part is nessesary beacouse we are working with 3.3V logic and power and not 5V
    // The reset circuit only works when Vcc is above 4.5V
    // Need to implement som timing, cause of the slow access times on the LCD

    // wait 500ms
    wait(500);
    //First try
    lcd_write4(0x03); // 0x30 = 0x10 | 0x20
    wait_micro(4500);

    //Second try
    lcd_write4(0x03);                    // 0x30 = 0x10 | 0x20
    wait_micro(4500);

    //Third and lucky try
    lcd_write4(0x03);                    // 0x30 = 0x10 | 0x20
    wait_micro(200);

    //enable 4bit mode
    lcd_write4(0x02);                             // 0x20
    wait_micro(200);

    //4 bit mode, 1/16, 5x8 font
    lcd_write8(0x28);                               // 0x28
    wait_micro(200);

    lcd_write8(0x08);                               // Display off 
    wait_micro(200);

    lcd_write8(0x0C);                               // Display on, blink curson on 
    wait_micro(200);

    lcd_write8(0x06);                               // Entry mode 
    wait_micro(200);

    lcd_write8(0x01);                               // Home
    wait_micro(200);

}

void lcd_write8(int value)
{
    // I do not call the lcd_write4 because I want the program to be relatively fast.
    // Right shift does not save the value
    GPIO_PORTC_DATA_R &= ~( (value >> 4) && 0x0F ); //Tænder for de øvre bits
    pulse_enable();                                 // Sender en puls på enable for at sende bits
    GPIO_PORTC_DATA_R |= ( (value >> 4) && 0x0F );         // Søger for at de bliver slukket igen

    GPIO_PORTC_DATA_R &= ~( value && 0x0F );        //Tænder for de nedre bits
    pulse_enable();                                 // Sender en puls på enable for at sende bits
    GPIO_PORTC_DATA_R |= ( value && 0x0F );         // Søger for at de bliver slukket igen
}

void lcd_write4(int value)
{
    GPIO_PORTC_DATA_R &= ~(value);                  // Tænder bits
    pulse_enable();                                 // Sender en puls på enable for at sende bits
    GPIO_PORTC_DATA_R |= value;                     // Slukker igen
}

void pulse_enable(void)
{
    GPIO_PORTD_DATA_R &= ~(LCD_E);
    wait_micro(1);                                  // enable pulse must be more than 450ns
    GPIO_PORTD_DATA_R |= LCD_E;
    wait_micro(10);                                // commands need > 37us to settle
}

// Waits in microsecounds*time
void wait_micro( int time )
{
    time = time*80                              //80Mhz CPU
    while (time--);
}

//This function is only used when it's an estimate
void wait (int time)
{
    wait_time = one_ms*time;
    while( wait_time-- );
}
//End function
