/*
 * FFT_task.c
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
// ------------------------------------------
//              Definitions
// ------------------------------------------
// ------------------------------------------
//                 Queues
// ------------------------------------------
extern xQueueHandle LCD_handle_q;
extern xQueueHandle switch_q;

// ------------------------------------------
//                 Mutex'
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
// ------------------------------------------
//              Functions
// ------------------------------------------
void FFT_task(void *pvparameters)
{
    int transmit;
    int receive;


    while(1)
    {
        if (state_mutex == FFT_TASK)
        {
            vTaskDelay( ACTIVE_TASK_DELAY );

            strcpy(lcd_image_arr[0] , " State");
            strcpy(lcd_image_arr[1] , " FFT  ");
            strcpy(lcd_image_arr[2] , "");
            strcpy(lcd_image_arr[3] , "");

            lcd_image_arr[0][6] = 0xFF;
            lcd_image_arr[0][7] = ' ';
            lcd_image_arr[0][8] = ' ';
            lcd_image_arr[0][9] = ' ';
            lcd_image_arr[0][10] = ' ';
            lcd_image_arr[0][11] = ' ';
            lcd_image_arr[0][12] = ' ';
            lcd_image_arr[0][13] = ' ';
            lcd_image_arr[0][14] = ' ';
            lcd_image_arr[0][15] = 0xFF;
            lcd_image_arr[0][16] = ' ';
            lcd_image_arr[0][17] = ' ';

            lcd_image_arr[1][6] = 0xFF;
            lcd_image_arr[1][7] = 0xFF;
            lcd_image_arr[1][8] = 0xFF;
            lcd_image_arr[1][9] = ' ';
            lcd_image_arr[1][10] = ' ';
            lcd_image_arr[1][11] = ' ';
            lcd_image_arr[1][12] = ' ';
            lcd_image_arr[1][13] = ' ';
            lcd_image_arr[1][14] = ' ';
            lcd_image_arr[1][15] = 0xFF;
            lcd_image_arr[1][16] = ' ';
            lcd_image_arr[1][17] = ' ';

            lcd_image_arr[2][6] = 0xFF;
            lcd_image_arr[2][7] = 0xFF;
            lcd_image_arr[2][8] = 0xFF;
            lcd_image_arr[2][9] = ' ';
            lcd_image_arr[2][10] = 0xFF;
            lcd_image_arr[2][11] = ' ';
            lcd_image_arr[2][12] = ' ';
            lcd_image_arr[2][13] = ' ';
            lcd_image_arr[2][14] = ' ';
            lcd_image_arr[2][15] = 0xFF;
            lcd_image_arr[2][16] = ' ';
            lcd_image_arr[2][17] = ' ';

            lcd_image_arr[3][6] = 0xFF;
            lcd_image_arr[3][7] = 0xFF;
            lcd_image_arr[3][8] = 0xFF;
            lcd_image_arr[3][9] = 0xFF;
            lcd_image_arr[3][10] = 0xFF;
            lcd_image_arr[3][11] = 0xFF;
            lcd_image_arr[3][12] = 0xFF;
            lcd_image_arr[3][13] = 0xFF;
            lcd_image_arr[3][14] = 0xFF;
            lcd_image_arr[3][15] = 0xFF;
            lcd_image_arr[3][16] = 0xFF;
            lcd_image_arr[3][17] = 0xFF;

            line_one_char_image   = ' ';
            line_two_char_image   = ' ';
            line_three_char_image = ' ';
            line_four_char_image  = ' ';

            transmit = CHECK_ALL_CHARS;
            xQueueSend(LCD_handle_q , &transmit , 500);

           if (xQueueReceive(switch_q , &receive , 500))
           {
               switch(receive)
               {
                case SW1_LONG :
                   transmit = CLEAR_BUFFERS;
                   xQueueSend(LCD_handle_q , &transmit , 500);
                   vTaskDelay( 2 );
                   state_mutex = MENU_TASK;
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

