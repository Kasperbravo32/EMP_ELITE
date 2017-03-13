/*
 * actions.c
 *
 *  Created on: 7. mar. 2017
 *      Author: Frederik
 */
// ------------------------------------------
//              Includes
// ------------------------------------------

#include "timers.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "events.h"
#include "actions.h"
// ------------------------------------------
//              Defines
// ------------------------------------------

#define IDLE          0
#define FIRST_PUSH    1
#define FIRST_RELEASE 2
#define SECOND_PUSH   3
#define LONG_PUSH     4
// Jeg ændrer lige noget

// ------------------------------------------
//              Functions
// ------------------------------------------


INT8U sw1_clicked()                                                                 // Switch 1 clicked
{
    return (! (GPIO_PORTF_DATA_R & 0b10000));
}

INT8U sw2_clicked()                                                                 // Switch 2 clicked
{
    return (! (GPIO_PORTF_DATA_R &0b00001));
}

// ------------------------------------------
//              Recognize Clicks
// ------------------------------------------

INT8U determine_click()                                                         // Check amount & length of clicks
{
    static  INT8U  sw1_state = IDLE;
    static  INT16U button_timer;
            INT8U  sw1_event = NO_EVENT;

    switch(sw1_state)
    {
    case IDLE:
        if (sw1_clicked())
        {
            sw1_state    = FIRST_PUSH;
            button_timer = TIMER_2000;
        }
        break;

    case FIRST_PUSH:
        if(! --button_timer)
        {
            sw1_state = LONG_PUSH;
            sw1_event = SW1_LONG;
        }
        else
        {
            if(! sw1_clicked())
            {
                sw1_state = FIRST_RELEASE;
                button_timer = TIMER_100;
            }
        }
        break;

    case FIRST_RELEASE:
        if(! --button_timer)
        {
            sw1_state = IDLE;
            sw1_event = SW1_SINGLE;
        }
        else
        {
            if(sw1_clicked())
            {
                sw1_state = SECOND_PUSH;
                button_timer = TIMER_2000;
            }
        }
        break;

    case SECOND_PUSH:
        if(! --button_timer)
        {
            sw1_state = LONG_PUSH;
            sw1_event = SW1_LONG;
        }
        else
        {
            if(! sw1_clicked())
            {
                sw1_state = IDLE;
                sw1_event = SW1_DOUBLE;
            }
        }
        break;

    case LONG_PUSH:
        if(! sw1_clicked())
            sw1_state = IDLE;
        break;

    default:
        break;
    }
    return(sw1_event);
}
