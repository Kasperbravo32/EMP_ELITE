/*
 * menu_task.c
 *
 *  Created on: 26. apr. 2017
 *      Author: Frederik
 */
// ------------------------------------------
//              Includes
// ------------------------------------------
#include "FreeRTOS.h"
#include "queue.h"
#include "lcd.h"
#include "emp_type.h"
#include "menu_task.h"
#include "set_color.h"
#include "LCD_handler_task.h"
#include "LCD_display_task.h"
#include "switches.h"
#include "functionalities.h"
// ------------------------------------------
//              Definitions
// ------------------------------------------

// ------------------------------------------
//                Queues
// ------------------------------------------
extern xQueueHandle LCD_handle_q;
extern xQueueHandle switch_q;

// ------------------------------------------
//                Mutex'
// ------------------------------------------
volatile extern MUTEX state_mutex;

// ------------------------------------------
//              Variables
// ------------------------------------------


volatile extern char line_one_char_image;
volatile extern char line_two_char_image;
volatile extern char line_three_char_image;
volatile extern char line_four_char_image;

volatile extern char line_one_char_out;
volatile extern char line_two_char_out;
volatile extern char line_three_char_out;
volatile extern char line_four_char_out;

volatile extern char lcd_image_arr[4][20];
volatile extern char lcd_output_arr[4][20];

volatile int arrow_pos = 1;
// ------------------------------------------
//              Functions
// ------------------------------------------
void menu_task(void * pvparameters)
{
    int transmit;
    int receive;

    while(1)
    {
        if (state_mutex == MENU_TASK)
        {
            vTaskDelay( 2 );
            strcpy(lcd_image_arr[0] , "Pulse");
            strcpy(lcd_image_arr[1] , "FFT");
            strcpy(lcd_image_arr[2] , "Details");
            strcpy(lcd_image_arr[3] , "System info");


            if (arrow_pos == 1)
            {
                line_one_char_image   = 0x7E;
                line_two_char_image   = ' ';
                line_three_char_image = ' ';
                line_four_char_image  = ' ';
            }
            else if (arrow_pos == 2)
            {
                line_one_char_image   = ' ';
                line_two_char_image   = 0x7E;
                line_three_char_image = ' ';
                line_four_char_image  = ' ';
            }
            else if (arrow_pos == 3)
            {
                line_one_char_image   = ' ';
                line_two_char_image   = ' ';
                line_three_char_image = 0x7E;
                line_four_char_image  = ' ';
            }
            else if (arrow_pos == 4)
            {
                line_one_char_image   = ' ';
                line_two_char_image   = ' ';
                line_three_char_image = ' ';
                line_four_char_image  = 0x7E;
            }

            transmit = CHECK_ALL;
            xQueueSend(LCD_handle_q , &transmit , 500);

            if (xQueueReceive(switch_q , &receive , 500))
            {
                switch(receive)
                {
                case SW1_SINGLE :
                    lcd_move_arrow(DOWN , 1 , 4);
                    break;

                case SW1_DOUBLE :

                    transmit = CLEAR_BUFFERS;
                    xQueueSend(LCD_handle_q , &transmit , 500);
                    vTaskDelay( 2 );

                    if (arrow_pos == 1)
                        state_mutex = PULSE_TASK;
                    else if (arrow_pos == 2)
                        state_mutex = FFT_TASK;
                    else if (arrow_pos == 3)
                        state_mutex = DETAILS_TASK;
                    else if (arrow_pos == 4)
                        state_mutex = SETTINGS_TASK;

                    break;

                case SW2_SINGLE :
                    lcd_move_arrow(UP , 1 , 4);

                    break;

                default :
                    break;
                }
                receive = 0;
            }



            vTaskDelay( ACTIVE_TASK_DELAY );
        }
        else
            vTaskDelay( IDLE_TASK_DELAY );
    }
}


