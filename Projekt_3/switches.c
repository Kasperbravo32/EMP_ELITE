/*
 * actions.c
 *
 *  Created on: 7. mar. 2017
 *      Author: Frederik
 */
// ------------------------------------------
//              Includes
// ------------------------------------------

#include "switches.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "set_color.h"
#include "glob_def.h"
#include "FreeRTOS.h"
#include "queue.h"

// ------------------------------------------
//              Defines
// ------------------------------------------

#define IDLE          0
#define FIRST_PUSH    1
#define FIRST_RELEASE 2
#define SECOND_PUSH   3
#define LONG_PUSH     4

// ------------------------------------------
//              Variables
// ------------------------------------------
static int sw1_counter = 0;
static int sw2_counter = 0;

extern xQueueHandle switch_q;
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
void determine_switch_1(void *pvParameters)                                                         // Check amount & length of clicks
{
    static  INT8U  sw1_state = IDLE;
            int  sw_event = NO_EVENT;
    while(1)
    {
        switch(sw1_state)
        {
        case IDLE:
            if (sw1_clicked())
            {
                sw1_state    = FIRST_PUSH;
                vTaskDelay(50/5);
            }
            break;

        case FIRST_PUSH:
            sw1_counter++;
            vTaskDelay(50/5);
            if(sw1_counter == 25)
            {
                sw1_state = LONG_PUSH;
                sw_event = SW1_LONG;
            }
            else
            {
                if(! sw1_clicked())
                {
                    sw1_state = FIRST_RELEASE;
                    sw1_counter = 0;
                    vTaskDelay(50/5);
                }
            }
            break;

        case FIRST_RELEASE:
            if(! sw1_clicked())
            {
                sw1_state = IDLE;
                sw_event = SW1_SINGLE;
                vTaskDelay(60/5);
            }
            else
            {
                if(sw1_clicked())
                {
                    sw1_state = SECOND_PUSH;
                    vTaskDelay(50/5);
                }
            }
            break;

        case SECOND_PUSH:
            sw1_counter++;
            vTaskDelay(50/5);
            if(sw1_counter == 25)
            {
                sw1_counter = 0;
                sw1_state = LONG_PUSH;
                sw_event = SW1_LONG;
                vTaskDelay(50/5);
            }
            else
            {
                if(! sw1_clicked())
                {
                    sw1_state = IDLE;
                    sw_event = SW1_DOUBLE;
                    vTaskDelay(50/5);
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
        xQueueSend(switch_q, &sw_event, 500);
        sw_event = NO_EVENT;
    }
}


// -----------------------------------------
//              SWITCH 2 begins
// -----------------------------------------

void determine_switch_2(void *pvparameters)
{
    static  INT8U  sw2_state = IDLE;
            int  sw_event = NO_EVENT;
    while(1)
    {
        switch(sw2_state)
        {
        case IDLE:
            if (sw2_clicked())
            {
                sw2_state    = FIRST_PUSH;
                vTaskDelay(50 / 5);
            }
            break;

        case FIRST_PUSH:
            sw2_counter++;
            vTaskDelay(50/5);
            if(sw2_counter == 30)
            {
                sw2_state = LONG_PUSH;
                sw_event = SW2_LONG;
            }
            else
            {
                if(! sw2_clicked())
                {
                    sw2_state = FIRST_RELEASE;
                    sw2_counter = 0;
                    vTaskDelay(50/5);
                }
            }
            break;

        case FIRST_RELEASE:
            if(! sw2_clicked())
            {
                sw2_state = IDLE;
                sw_event = SW2_SINGLE;
                vTaskDelay(50/5);
            }
            else
            {
                if(sw2_clicked())
                {
                    sw2_state = SECOND_PUSH;
                    vTaskDelay(50/5);
                }
            }
            break;

        case SECOND_PUSH:
            sw2_counter++;
            vTaskDelay(50/5);
            if(sw2_counter == 30)
            {
                sw2_counter = 0;
                sw2_state = LONG_PUSH;
                sw_event = SW2_LONG;
                vTaskDelay(50/5);
            }
            else
            {
                if(! sw2_clicked())
                {
                    sw2_state = IDLE;
                    sw_event = SW2_DOUBLE;
                    vTaskDelay(50/5);
                }
            }
            break;

        case LONG_PUSH:
            if(! sw2_clicked())
                sw2_state = IDLE;
            break;

        default:
            break;
        }
        xQueueSend(switch_q, &sw_event, 500);
        sw_event = NO_EVENT;
    }
}



void read_switch_q(void *p)
{
    int item = 0;
    while(1)
    {
        if(xQueueReceive(switch_q, &item, 500))
        {
            switch(item)
            {
            case SW1_SINGLE :
                SET_LED(LED_RED);
                break;
            case SW2_SINGLE :
                SET_LED(LED_GREEN);
                break;
            default :
                break;
            }
        }
        vTaskDelay( 100 / 5);
    }
}
