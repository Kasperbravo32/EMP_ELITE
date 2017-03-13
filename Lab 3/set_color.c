/*****************************************************************************
*
* MODULENAME.: SET_LED.c
*
* PROJECT....: Assignment 2
* This is a change
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log: ************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "set_color.h"
/*****************************    Defines    ********************************/
#
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

/****************************** End Of Module *******************************/
