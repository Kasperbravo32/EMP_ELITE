/*****************************************************************************
*
* MODULENAME.: SET_LED.c
*
* PROJECT....: Assignment 2
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log: ************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "set_color.h"
#include "timers.h"
/*****************************    Defines    ********************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void SET_LED(INT8U value)
{
    if ( value & LED_RED)
        GPIO_PORTF_DATA_R &= ~( LED_RED ); // T�nder LED'er
    else
        GPIO_PORTF_DATA_R |= LED_RED;   // Turn's off all LED's
    if ( value & LED_YELLOW)
        GPIO_PORTF_DATA_R &= ~( LED_YELLOW ); // T�nder LED'er
    else
        GPIO_PORTF_DATA_R |= LED_YELLOW;    // Turn's off all LED's
    if ( value & LED_GREEN)
        GPIO_PORTF_DATA_R &= ~( LED_GREEN ); // T�nder LED'er
    else
        GPIO_PORTF_DATA_R |= LED_GREEN; // Turn's off all LED's
}


int alive_LED(INT16U timer)
{
    if(! --timer)
    {
        timer = TIMER_500;
        GPIO_PORTD_DATA_R ^= 0x40;
        return timer;
    }
    else
        return timer;
}

int LED_OFF(INT16U timer)
{
    if (! --timer)
    {
        timer = TIMER_500;
        SET_LED(!(LED_RED | LED_YELLOW | LED_GREEN));
        return timer;
    }
    else
        return timer;
}
/****************************** End Of Module *******************************/
