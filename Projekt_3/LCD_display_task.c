/*
 * LCD_display_task.c
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
#include "menu_task.h"
#include "set_color.h"
#include "LCD_handler_task.h"
#include "LCD_display_task.h"
// ------------------------------------------
//              Definitions
// ------------------------------------------
// ------------------------------------------
//                 Queues
// ------------------------------------------
extern xQueueHandle  LCD_display_q;
extern xQueueHandle  LCD_handle_q;

// ------------------------------------------
//                 Mutex'
// ------------------------------------------

// ------------------------------------------
//               Variables
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
volatile extern char ADC_status_str[8];

// ------------------------------------------
//              Functions
// ------------------------------------------

void lcd_display_task(void *pvparameters)
{
    int receive;
    int transmit;

    int line;
    int space;

    while(1)
    {
        if (xQueueReceive(LCD_display_q , &receive , 500))
        {
            switch(receive)
            {
            case SEND_LINE_ONE :
                lcd_instruct(LCD_LINE_ONE + 1);
                lcd_data_string(lcd_output_arr[0]);
                break;

            case SEND_LINE_TWO :
                lcd_instruct(LCD_LINE_TWO + 1);
                lcd_data_string(lcd_output_arr[1]);
                break;

            case SEND_LINE_THREE :
                lcd_instruct(LCD_LINE_THREE + 1);
                lcd_data_string(lcd_output_arr[2]);
                break;

            case SEND_LINE_FOUR :
                lcd_instruct(LCD_LINE_FOUR + 1);
                lcd_data_string(lcd_output_arr[3]);
                break;


            case SEND_CHAR_ONE :
                lcd_instruct(LCD_LINE_ONE);
                lcd_data(line_one_char_out);
                break;

            case SEND_CHAR_TWO :
                lcd_instruct(LCD_LINE_TWO);
                lcd_data(line_two_char_out);
                break;

            case SEND_CHAR_THREE :
                lcd_instruct(LCD_LINE_THREE);
                lcd_data(line_three_char_out);
                break;

            case SEND_CHAR_FOUR :
                lcd_instruct(LCD_LINE_FOUR);
                lcd_data(line_four_char_out);
                break;

            case SEND_ONE_CHAR :

                xQueueReceive(LCD_display_q , &line , 500);
                xQueueReceive(LCD_display_q , &space , 500);
                if (line == 0)
                    lcd_instruct(LCD_LINE_ONE + space);
                else if (line == 1)
                    lcd_instruct(LCD_LINE_TWO + space);
                else if (line == 2)
                    lcd_instruct(LCD_LINE_THREE + space);
                else if (line == 3)
                    lcd_instruct(LCD_LINE_FOUR + space);
                lcd_data(lcd_output_arr[line][space]);
                break;

            default :
                break;
            }
        }
        vTaskDelay( ACTIVE_TASK_DELAY );
    }
}
