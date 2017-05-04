/*
 * settings_task.c
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
#include "details_task.h"
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
//                  Mutex'
// ------------------------------------------
volatile extern MUTEX state_mutex;

// ------------------------------------------
//                 Variables
// ------------------------------------------
volatile extern int  ADC_status;
volatile extern char ADC_status_off[3];
volatile extern char ADC_status_on[3];

volatile extern char active_adc;
volatile extern char active_ss;

volatile extern char input_pin[3];

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
volatile extern char male_string[6];
volatile extern char female_string[6];

volatile extern char current_age_string[3];
volatile extern char current_weight_string[3];

volatile extern int current_gender;
// ------------------------------------------
//              Functions
// ------------------------------------------
void settings_task(void *pvparameters)
{
    int transmit;
    int receive;

    while(1)
    {
        if (state_mutex == SETTINGS_TASK)
        {
            transmit = CLEAR_BUFFERS;
            xQueueSend(LCD_handle_q , &transmit , 500);
            vTaskDelay(2);

            line_one_char_image   = ' ';
            line_two_char_image   = ' ';
            line_three_char_image = ' ';
            line_four_char_image  = ' ';

            vTaskDelay( ACTIVE_TASK_DELAY );
            strcpy(lcd_image_arr[0] , "Information");
            strcpy(lcd_image_arr[1] , "Gender : ");
            strcpy(lcd_image_arr[2] , "Age    : ");
            strcpy(lcd_image_arr[3] , "Weight : ");

            if (current_gender == MALE)
            {
                for (int i = 0; i < 6; i++)
                    {
                        lcd_image_arr[1][i+12] = male_string[i];
                    }
            }
            else if (current_gender == FEMALE)
            {
                for (int i = 0; i < 6; i++)
                {
                    lcd_image_arr[1][i+12] = female_string[i];
                }
            }

            for (int i = 0; i < 3; i++)
            {
                lcd_image_arr[2][i+12] = current_age_string[i];
                lcd_image_arr[3][i+12] = current_weight_string[i];
            }
            SET_LED(LED_RED);
            transmit = CHECK_LINE_ONE;
            xQueueSend(LCD_handle_q , &transmit , 500);
            transmit = CHECK_LINE_TWO;
            xQueueSend(LCD_handle_q , &transmit , 500);
            transmit = CHECK_LINE_THREE;
            xQueueSend(LCD_handle_q , &transmit , 500);
            transmit = CHECK_LINE_FOUR;
            xQueueSend(LCD_handle_q , &transmit , 500);
            vTaskDelay(3000 / 5);


            transmit = CLEAR_BUFFERS;
            xQueueSend(LCD_handle_q , &transmit , 500);
            vTaskDelay(2);

            line_one_char_image   = ' ';
            line_two_char_image   = ' ';
            line_three_char_image = ' ';
            line_four_char_image  = ' ';

            strcpy(lcd_image_arr[0] , "Information");
            strcpy(lcd_image_arr[1] , "ADC       : ");
            strcpy(lcd_image_arr[2] , "Sam. Seq. : ");
            strcpy(lcd_image_arr[3] , "Input pin : ");

            if (ADC_status == 1)
            {
                for (int i = 0; i < 3; i++)
                {
                    lcd_image_arr[1][i+12] = ADC_status_on[i];
                }
            }

            else if (ADC_status == 0)
            {
                for (int i = 0; i < 3; i++)
                {
                    lcd_image_arr[1][i+12] = ADC_status_off[i];
                }
            }

            lcd_image_arr[2][12] = active_ss;

            for (int i = 0; i < 4; i++)
            {
                lcd_image_arr[3][i+12] = input_pin[i];
            }

            transmit = CHECK_LINE_ONE;
            xQueueSend(LCD_handle_q , &transmit , 500);
            transmit = CHECK_LINE_TWO;
            xQueueSend(LCD_handle_q , &transmit , 500);
            transmit = CHECK_LINE_THREE;
            xQueueSend(LCD_handle_q , &transmit , 500);
            transmit = CHECK_LINE_FOUR;
            xQueueSend(LCD_handle_q , &transmit , 500);
            SET_LED(!LED_RED);
            vTaskDelay(3000 / 5);



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

               case '#' :
                   transmit = CLEAR_BUFFERS;
                   xQueueSend(LCD_handle_q , &transmit , 500);
                   vTaskDelay( 2 );
                   state_mutex = MENU_TASK;

               default :
                   break;

               }
               receive = 0;
           }
           vTaskDelay(3000 / 5);

            vTaskDelay( ACTIVE_TASK_DELAY );
        }
        else
            vTaskDelay( IDLE_TASK_DELAY );
    }
}

