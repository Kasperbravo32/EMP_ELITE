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
#include "states.h"
#include "events.h"
#include "actions.h"
#include "timers.h"
#include "set_color.h"
#include "state_run.h"

/*****************************    Defines    ********************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U LED_COUNTER  = 0;
/*****************************   Functions   *******************************/

void RUN_STATE(int value)
{

    static INT16U RUN_STATE_TIMER;

    // Normal or traffic light mode
    if(value == TRAFFIC_LIGHT)
    {
        if(! --RUN_STATE_TIMER)
        {
            switch(LED_COUNTER) {

                case RED_LIGHT_ON:
                    SET_LED(LED_RED);
                    LED_COUNTER++;
                    RUN_STATE_TIMER = TIMER_500;
                break;

                case YELLOW_RED_LIGHT_ON:
                    SET_LED(LED_RED & LED_YELLOW);
                    LED_COUNTER++;
                    RUN_STATE_TIMER = TIMER_500;
                break;

                case GREEN_LIGHT_ON:
                    SET_LED(LED_GREEN);
                    LED_COUNTER++;
                    RUN_STATE_TIMER = TIMER_500;
                break;

                case YELLOW_LIGHT_ON:
                    SET_LED(LED_YELLOW);
                    LED_COUNTER = 0;
                    RUN_STATE_TIMER = TIMER_500;
                break;

                default:
                    LED_COUNTER = 0;
                    RUN_STATE_TIMER = TIMER_500;
                break;
             }
        }
    }
    // NORWEGIAN_NIGHT
    if(value == NORWEGIAN_NIGHT)
    {
        if(! --RUN_STATE_TIMER)
        {
            if(LED_COUNTER)
            {
                SET_LED(0);
                LED_COUNTER = 0;
            }
            else
            {
                SET_LED(LED_YELLOW);
                LED_COUNTER = 1;
            }

            RUN_STATE_TIMER = TIMER_500;
        }
    }

    //EMERGENCY
    if(value == EMERGENCY)
    {
        SET_LED(LED_RED);
    }

}

/****************************** End Of Module *******************************/
