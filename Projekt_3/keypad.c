/*
 * keypad.c
 *
 *  Created on: 2. maj 2017
 *      Author: Frederik
 */
// ------------------------------------------
//              Includes
// ------------------------------------------
// ------------------------------------------
//              Includes
// ------------------------------------------
#include <stdint.h>
#include "lcd.h"
#include "emp_type.h"
#include "menu_task.h"
#include "glob_def.h"
#include "tm4c123gh6pm.h"
#include "set_color.h"
#include "LCD_handler_task.h"
#include "LCD_display_task.h"
#include "switches.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
// ------------------------------------------
//              Queues
// ------------------------------------------
extern xQueueHandle switch_q;

// ------------------------------------------
//              Definitions
// ------------------------------------------
#define KEY_IDLE        0
#define KEY_SCANX1      1
#define KEY_SCANX2      2
#define KEY_SCANX3      3
#define KEY_SEND        4

#define X1              0b00010000
#define X2              0b00001000
#define X3              0b00000100

#define X1X2X3          0b00011100

#define Y1              0b00001000
#define Y2              0b00000100
#define Y3              0b00000010
#define Y4              0b00000001
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
int key = 0;
INT8U key_state = KEY_SCANX1;
BOOLEAN click = FALSE;

extern xSemaphoreHandle keypad_sem;
/*****************************   Functions   *******************************/
void keypad_task(void *pvparameters)
{
    while(1)
    {
            switch( key_state )
            {
            case KEY_SCANX1: // (PA4)
                vTaskDelay( 10 );
                key_state = KEY_SCANX2;
                GPIO_PORTA_DATA_R &= ~(X1X2X3);
                GPIO_PORTA_DATA_R |= X1;
                if( GPIO_PORTE_DATA_R & Y1 )
                {
                    key = '1';
                    click = TRUE;
                    key_state = KEY_SEND;
                }
                else if( GPIO_PORTE_DATA_R & Y2 )
                {
                    key = '4';
                    click = TRUE;
                    key_state = KEY_SEND;
                }
                else if( GPIO_PORTE_DATA_R & Y3 )
                {
                    key = '7';
                    click = TRUE;
                    key_state = KEY_SEND;
                }
                else if (GPIO_PORTE_DATA_R & Y4 )
                {
                    key = '*';
                    click = TRUE;
                    key_state = KEY_SEND;
                }
                break;

            case KEY_SCANX2:
                vTaskDelay( 10 );
                key_state = KEY_SCANX3;
                GPIO_PORTA_DATA_R &= ~(X1X2X3);
                GPIO_PORTA_DATA_R |= X2;
                if( GPIO_PORTE_DATA_R & Y1 )
                {
                    key = '2';
                    click = TRUE;
                    key_state = KEY_SEND;
                }
                else if( GPIO_PORTE_DATA_R & Y2 )
                {
                    key = '5';
                    click = TRUE;
                    key_state = KEY_SEND;
                }
                else if( GPIO_PORTE_DATA_R & Y3 )
                {
                    key = '8';
                    click = TRUE;
                    key_state = KEY_SEND;
                }
                else if (GPIO_PORTE_DATA_R & Y4 )
                {
                    key = '0';
                    click = TRUE;
                    key_state = KEY_SEND;
                }
                break;

            case KEY_SCANX3:
                vTaskDelay( 10 );
                key_state = KEY_SCANX1;
                GPIO_PORTA_DATA_R &= ~(X1X2X3);
                GPIO_PORTA_DATA_R |= X3;
                if( GPIO_PORTE_DATA_R & Y1 )
                {
                    key = '3';
                    click = TRUE;
                    key_state = KEY_SEND;
                }
                else if( GPIO_PORTE_DATA_R & Y2 )
                {
                    key = '6';
                    click = TRUE;
                    key_state = KEY_SEND;
                }
                else if( GPIO_PORTE_DATA_R & Y3 )
                {
                    key = '9';
                    click = TRUE;
                    key_state = KEY_SEND;
                }
                else if (GPIO_PORTE_DATA_R & Y4 )
                {
                    key = '#';
                    click = TRUE;
                    key_state = KEY_SEND;
                }
                break;

            case KEY_SEND:
                vTaskDelay( 10 );
                if( click )
                {
                    if( !((GPIO_PORTE_DATA_R & 0x0F ) ) ) // Check for button release
                    {
                        if( switch_q != 0)
                        {
                            if( xQueueSend( switch_q, ( void * ) &key, 500 ) )
                            {
                            key = 0;
                            key_state = KEY_SCANX1;
                            }
                        }
                        click = FALSE;
                    }
                }
                break;
            }
        }
 }


/****************************** End Of Module *******************************/

