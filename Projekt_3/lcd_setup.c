/*
 * lcd_setup.c
 *
 *  Created on: 27. apr. 2017
 *      Author: Frederik
 */

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "Task.h"
#include "queue.h"
#include "semphr.h"
#include "emp_type.h"
#include "lcd_setup.h"
/*****************************    Defines    *******************************/
#define TIM_1_USEC              16
#define TIM_100_USEC            1600
#define TIM_200_USEC            3200
#define TIM_5_MSEC              80000
#define TIM_40_MSEC             640000

#define POWERUP                 1
#define READY                   2

#define LCD_READY               1
#define LCD_ESC_RECIEVED        2
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
       INT16U lcd_pulse         = 0;
       INT16U lcd_stringtimer   = 0;
       INT16U lcd_inittimer     = TIM_1_USEC;       // 16 = 1 USEC
       INT32U lcd_powerup_timer = TIM_40_MSEC;      // 640000 = 40 MSEC
/*****************************   Functions   *******************************/

//***************************************************************************
void lcd_init2()
{
// Power-up delay
    while(lcd_powerup_timer)
        {
            lcd_powerup_timer--;
        }

// Set up the RS and E bits for the 4bit write function (Make sure the bits are low)
    GPIO_PORTD_DATA_R &= ~(0b00001100); // PD2 = LCD_RS ---  PD3 = LCD_E

// Reset the LCD controller
    lcd_4bitwrite(LCD_RESET); // First part of reset sequence
    lcd_powerup_timer = TIM_5_MSEC; // Minimum is 4.1 MSEC
    while(lcd_powerup_timer)
        {
            lcd_powerup_timer--;
        }

    lcd_4bitwrite(LCD_RESET); // Second part of reset sequence
    lcd_powerup_timer = TIM_200_USEC; // Minimum is 100us
    while(lcd_powerup_timer)
        {
            lcd_powerup_timer--;
        }

    lcd_4bitwrite(LCD_RESET); // Third part of reset sequence
    lcd_powerup_timer = TIM_200_USEC; // This is not included in the data sheet
    while(lcd_powerup_timer)
        {
            lcd_powerup_timer--;
        }

// Set LCD to 4-bit mode:
    lcd_4bitwrite(LCD_SET4BIT); // Set 4-bit mode
    lcd_powerup_timer = TIM_100_USEC; // Minimum 40us delay
    while(lcd_powerup_timer)
        {
            lcd_powerup_timer--;
        }

    lcd_write_instruction(LCD_SET4BIT); // set mode, lines, and font
    lcd_powerup_timer = TIM_100_USEC; // Minimum 40us delay
    while(lcd_powerup_timer)
        {
            lcd_powerup_timer--;
        }

    lcd_write_instruction(LCD_OFF); // Turn display off
    lcd_powerup_timer = TIM_100_USEC; // Minimum 40 us delay
    while(lcd_powerup_timer)
        {
            lcd_powerup_timer--;
        }

    lcd_write_instruction(LCD_CLEAR); // Clear LCD RAM
    lcd_powerup_timer = TIM_5_MSEC; // Minimum delay 1.64 MSEC
    while(lcd_powerup_timer)
        {
            lcd_powerup_timer--;
        }

    lcd_write_instruction(LCD_ENTRYMODE); // Set desired shift characteristics
    lcd_powerup_timer = TIM_100_USEC; // Minimum delay 40us
    while(lcd_powerup_timer)
        {
            lcd_powerup_timer--;
        }

    lcd_write_instruction(LCD_ON); // Turn LCD ON
    lcd_powerup_timer = TIM_100_USEC; // Minimum delay 40us
    while(lcd_powerup_timer)
        {
            lcd_powerup_timer--;
        }

    lcd_write_instruction(LCD_HOME); // Set cursor to first line first character
    lcd_powerup_timer = TIM_100_USEC; // Minimum delay 40us
    while(lcd_powerup_timer)
        {
            lcd_powerup_timer--;
        }
}


//***********************************************************************************************
void lcd_write_instruction( INT8U theinstruction )
{
    // Select the instruction register by setting RS-bit low (PD2) -- And set the E-bit low (PD3)
    GPIO_PORTD_DATA_R &= ~(0b00001100);
    lcd_4bitwrite(theinstruction); // Write the 4 high bits of the instruction
    lcd_4bitwrite(theinstruction << 4); // Write the 4 low bits if the instruction
}
//***********************************************************************************************



//***********************************************************************************************
void lcd_4bitwrite( INT8U thebyte )
{
    GPIO_PORTC_DATA_R = thebyte;

    GPIO_PORTD_DATA_R |= 0b00001000; // Set enable-bit high (PD3)
    lcd_pulse = TIM_1_USEC;          // The E-Bit must be pulsed for 1us to write the 4-bit data
    while(lcd_pulse--);


    GPIO_PORTD_DATA_R &= ~(0b00001000); // Set enable-bit low (PD3)
    lcd_pulse = TIM_1_USEC;             // The E-Bit must be turned off at the end of the pulse and a 1us delay is called
    while(lcd_pulse--);                    // for the "Data hold time"
}
//***********************************************************************************************

//***********************************************************************************************
void lcd_write_character( INT8U thedata )
{
    // Select the information register by setting RS-bit (PD2)
    GPIO_PORTD_DATA_R |= 0b00000100;
    // Set the E-bit low (PD3)
    GPIO_PORTD_DATA_R &= ~(0b00001000);
    lcd_4bitwrite(thedata);
    lcd_4bitwrite(thedata << 4);
}
//***********************************************************************************************

//***********************************************************************************************
void lcd_write_string(INT8U thestring[])
{
    volatile int i = 0;                             // character counter*/
    while (thestring[i] != 0)
    {
        lcd_write_character(thestring[i]);
        i++;
        lcd_stringtimer = TIM_100_USEC;             // 40 uS delay (min)
        while(lcd_stringtimer)
            {
                lcd_stringtimer--;
            }
    }
}
//***********************************************************************************************

//***********************************************************************************************
INT8U lcd_write_queue( INT8U Ch )
{
  //  return( queue_put( Q_OUTPUT, Ch ) );
}

//***********************************************************************************************

//***********************************************************************************************
void lcd_task( INT8U task_no )
{
    /*
          INT8U ch;

static    INT8U LCD_state = LCD_READY;

    switch( LCD_state )
    {
    case LCD_READY:
        if( ( ch = queue_get( Q_OUTPUT ) ) ) // Takes element from queue
        {
            switch( ch )
            {
            case ESC:
                LCD_state = LCD_ESC_RECIEVED;
                break;

            default:
                lcd_write_character( ch );
                break;
            }
        }
        break;

    case LCD_ESC_RECIEVED: // If ESC was received, next char is instruction
        if( ( ch = queue_get( Q_OUTPUT ) ) )
        {
            lcd_write_instruction( ch );
        }
        LCD_state = LCD_READY;
        break;
    }
 */
}



// ****************************** End of Module *************************

