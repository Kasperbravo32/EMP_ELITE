/*
 * details_task.c
 *
 *  Created on: 26. apr. 2017
 *      Author: Frederik
 */
// ------------------------------------------
//              Includes
// ------------------------------------------
// ------------------------------------------
//              Includes
// ------------------------------------------
#include "details_task.h"
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
//              Queues
// ------------------------------------------
extern xQueueHandle LCD_handle_q;
extern xQueueHandle switch_q;

// ------------------------------------------
//               Mutex'
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

char number_array[10] = {'0' , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9'};

volatile char male_string[6]   = {"MALE  "};
volatile char female_string[6] = {"FEMALE"};
char *ptr;
volatile char current_age_string[3];
volatile char current_weight_string[3];

long int current_age;
volatile int current_weight;
volatile int current_gender;
int enter_line = 0;
int enter_char = 10;
// ------------------------------------------
//              Functions
// ------------------------------------------
void details_task(void *pvparameters)
{
    int transmit;
    int receive;

    while(1)
    {
        if (state_mutex == DETAILS_TASK)
        {
            vTaskDelay( ACTIVE_TASK_DELAY );
            strcpy(lcd_image_arr[0] , "Enter details below");
            strcpy(lcd_image_arr[1] , "Age   ");
            strcpy(lcd_image_arr[2] , "Weight");
            strcpy(lcd_image_arr[3] , "Gender");

            line_one_char_image   = ' ';
            line_two_char_image   = ' ';
            line_three_char_image = ' ';
            line_four_char_image  = ' ';

            transmit = CHECK_ALL;
            xQueueSend(LCD_handle_q , &transmit , 500);
            vTaskDelay( 2 );

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

               case '0' :
                   if (lcd_image_arr[3][8] != ':')
                   {
                       lcd_image_arr[enter_line][enter_char] = number_array[0];
                       enter_char++;
                       transmit = CHECK_ALL_CHARS;
                       xQueueSend(LCD_handle_q , &transmit , 500);
                       vTaskDelay(ACTIVE_TASK_DELAY);
                   }
                   break;


               case '1' :
                   if (lcd_image_arr[3][8] == ':')
                   {
                       for (int i = 0; i < 6; i++)
                           lcd_image_arr[3][i+10] = male_string[i];
                   }

                   else
                   {
                       lcd_image_arr[enter_line][enter_char] = number_array[1];
                       enter_char++;
                   }

                   transmit = CHECK_ALL_CHARS;
                   xQueueSend(LCD_handle_q , &transmit , 500);
                   break;

               case '2' :
                   if (lcd_image_arr[3][8] == ':')
                   {
                       for (int i = 0; i < 6; i++)
                           lcd_image_arr[3][i+10] = female_string[i];
                   }
                   else
                   {
                       lcd_image_arr[enter_line][enter_char] = number_array[2];
                       enter_char++;
                   }

                   transmit = CHECK_ALL_CHARS;
                   xQueueSend(LCD_handle_q , &transmit , 500);
                   vTaskDelay(ACTIVE_TASK_DELAY);
                   break;

               case '3' :
                   if (lcd_image_arr[3][8] != ':')
                   {
                       lcd_image_arr[enter_line][enter_char] = number_array[3];
                       enter_char++;
                       transmit = CHECK_ALL_CHARS;
                       xQueueSend(LCD_handle_q , &transmit , 500);
                       vTaskDelay(ACTIVE_TASK_DELAY);
                   }
                   break;

               case '4' :
                   if (lcd_image_arr[3][8] != ':')
                   {
                       lcd_image_arr[enter_line][enter_char] = number_array[4];
                       enter_char++;
                       transmit = CHECK_ALL_CHARS;
                       xQueueSend(LCD_handle_q , &transmit , 500);
                       vTaskDelay(ACTIVE_TASK_DELAY);
                   }
                   break;

               case '5' :
                   if (lcd_image_arr[3][8] != ':')
                   {
                       lcd_image_arr[enter_line][enter_char] = number_array[5];
                       enter_char++;
                       transmit = CHECK_ALL_CHARS;
                       xQueueSend(LCD_handle_q , &transmit , 500);
                       vTaskDelay(ACTIVE_TASK_DELAY);
                   }
                   break;

               case '6' :
                   if (lcd_image_arr[3][8] != ':')
                   {
                       lcd_image_arr[enter_line][enter_char] = number_array[6];
                       enter_char++;
                       transmit = CHECK_ALL_CHARS;
                       xQueueSend(LCD_handle_q , &transmit , 500);
                       vTaskDelay(ACTIVE_TASK_DELAY);
                   }
                   break;

               case '7' :
                   if (lcd_image_arr[3][8] != ':')
                   {
                       lcd_image_arr[enter_line][enter_char] = number_array[7];
                       enter_char++;
                       transmit = CHECK_ALL_CHARS;
                       xQueueSend(LCD_handle_q , &transmit , 500);
                       vTaskDelay(ACTIVE_TASK_DELAY);
                   }
                   break;

               case '8' :
                   if (lcd_image_arr[3][8] != ':')
                   {
                       lcd_image_arr[enter_line][enter_char] = number_array[8];
                       enter_char++;
                       transmit = CHECK_ALL_CHARS;
                       xQueueSend(LCD_handle_q , &transmit , 500);
                       vTaskDelay(ACTIVE_TASK_DELAY);
                   }
                   break;

               case '9' :
                   if (lcd_image_arr[3][8] != ':')
                   {
                       lcd_image_arr[enter_line][enter_char] = number_array[9];
                       enter_char++;
                       transmit = CHECK_ALL_CHARS;
                       xQueueSend(LCD_handle_q , &transmit , 500);
                       vTaskDelay(ACTIVE_TASK_DELAY);
                   }
                   break;

               case '#' :
                   move_colon();
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


void move_colon()
{
    int transmit;

    if (lcd_image_arr[3][8] == ':')
    {
        if (lcd_image_arr[3][10] == 'M')
            current_gender = MALE;
        else if (lcd_image_arr[3][10] == 'F')
            current_gender = FEMALE;

        for (int i = 0; i < 3; i++)
            current_age_string[i] = lcd_image_arr[1][i+10];

        for (int i = 0; i < 3; i++)
            current_weight_string[i] = lcd_image_arr[2][i+10];

        current_age = strtol(current_age_string, &ptr, 10);
        current_weight = strtol(current_weight_string, &ptr, 10);

        enter_line = 0;
        enter_char = 10;
        lcd_image_arr[3][8] = ' ';
        state_mutex = MENU_TASK;
        transmit = CLEAR_BUFFERS;
        xQueueSend(LCD_handle_q , &transmit , 500);
        vTaskDelay(2);
    }
    else if (lcd_image_arr[2][8] == ':')
    {
        lcd_image_arr[2][8] = ' ';
        lcd_image_arr[3][8] = ':';
    }
    else if (lcd_image_arr[1][8] == ':')
    {
        lcd_image_arr[1][8] = ' ';
        lcd_image_arr[2][8] = ':';
    }
    else
        lcd_image_arr[1][8] = ':';

    enter_line++;
    enter_char = 10;
    transmit = CHECK_DETAILS_COLON;
    xQueueSend(LCD_handle_q , &transmit , 500);
}
