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
        GPIO_PORTF_DATA_R &= ~( LED_RED ); // Tænder LED'er
    else
        GPIO_PORTF_DATA_R |= LED_RED;   // Turn's off all LED's
    if ( value & LED_YELLOW)
        GPIO_PORTF_DATA_R &= ~( LED_YELLOW ); // Tænder LED'er
    else
        GPIO_PORTF_DATA_R |= LED_YELLOW;    // Turn's off all LED's
    if ( value & LED_GREEN)
        GPIO_PORTF_DATA_R &= ~( LED_GREEN ); // Tænder LED'er
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






























/*
void calculate_puls();


if (pulse_trigger == 0 && value > pulse_threshold)
{
    pulse_trigger = 1;
    pulse_ticks++;

    wide_pulse_ticks++;
    wide_pulse_check(wide_pulse_ticks);
    wide_pulse_ticks = 0;

    calculate_puls(pulse_ticks);
    pulse_ticks = 0;
}

else if (pulse_trigger == 1 && value > pulse_threshold)
{
    pulse_ticks++;
    wide_pulse_ticks++;
}

else if (pulse_trigger == 1 && value < pulse_threshold - 100)
{
    pulse_ticks++;
    pulse_trigger = 0;
}

else if (pulse_trigger == 0 && value < pulse_threshold - 100)
{
    pulse_ticks++;
}


void calculate_puls(int value)
{
    pulse = (60 / (0.005 * pulse_ticks));
}



pulse_tick_arr[0] = pulse_tick_arr[1];
pulse_tick_arr[1] = pulse_tick_arr[2];
pulse_tick_arr[2] = pulse_ticks;

if (pulse_tick_arr[2] >= 1.8*pulse_tick_arr[1])
    SAB_flag = 1;


*/


