/*
 * LCD_handler_task.c
 *
 *  Created on: 26. apr. 2017
 *      Author: Frederik
 */
// ------------------------------------------
//              Includes
// ------------------------------------------
#include "FreeRTOS.h"
#include "queue.h"
#include "menu_task.h"
#include "emp_type.h"
#include "set_color.h"
#include "lcd.h"
#include "LCD_handler_task.h"
#include "LCD_display_task.h"
// ------------------------------------------
//              Definitions
// ------------------------------------------
// ------------------------------------------
//                 Queues
// ------------------------------------------
extern xQueueHandle LCD_display_q;
extern xQueueHandle LCD_handle_q;

// ------------------------------------------
//                 Mutex'
// ------------------------------------------
extern MUTEX state_mutex;
// ------------------------------------------
//              Variables
// ------------------------------------------
volatile char lcd_image_arr[4][20];
volatile char lcd_output_arr[4][20];

volatile char line_one_char_image;
volatile char line_two_char_image;
volatile char line_three_char_image;
volatile char line_four_char_image;

volatile char line_one_char_out;
volatile char line_two_char_out;
volatile char line_three_char_out;
volatile char line_four_char_out;

volatile extern char pulse_output_string[4];
volatile extern int  ADC_status;
volatile extern char ADC_status_str[8];

char space_char[2] = {" "};

// ------------------------------------------
//              Functions
// ------------------------------------------
void lcd_handler_task(void *pvparameters)
{
    int receive;
    int transmit;
    while(1)
    {
        if (xQueueReceive(LCD_handle_q , &receive , 500))
        {
            switch(receive)
            {
            case CLEAR_BUFFERS :
                for (int i = 0; i < 4; i++)
                {
                    for (int n = 0; n < 20; n++)
                    {
                        lcd_image_arr[i][n]  = ' ';
                        lcd_output_arr[i][n] = ' ';
                    }
                }

                transmit = SEND_LINE_ONE;
                xQueueSend(LCD_display_q , &transmit , 500);
                vTaskDelay(2);

                transmit = SEND_LINE_TWO;
                xQueueSend(LCD_display_q , &transmit , 500);
                vTaskDelay(2);

                transmit = SEND_LINE_THREE;
                xQueueSend(LCD_display_q , &transmit , 500);
                vTaskDelay(2);

                transmit = SEND_LINE_FOUR;
                xQueueSend(LCD_display_q , &transmit , 500);
                vTaskDelay(2);

                break;

            case CHECK_LINE_ONE :
                compare_lcd_image_char(line_one_char_image , line_two_char_image , line_three_char_image , line_four_char_image);
                for (int i = 0; i < 19; i++)
                    compare_lcd_image_specific(lcd_image_arr[0][i] , 0 , i);
                break;

            case CHECK_LINE_TWO :
                compare_lcd_image_char(line_one_char_image , line_two_char_image , line_three_char_image , line_four_char_image);
                for (int i = 0; i < 19; i++)
                    compare_lcd_image_specific(lcd_image_arr[1][i] , 1 , i);
                break;

            case CHECK_LINE_THREE :
                compare_lcd_image_char(line_one_char_image , line_two_char_image , line_three_char_image , line_four_char_image);
                for (int i = 0; i < 19; i++)
                    compare_lcd_image_specific(lcd_image_arr[2][i] , 2 , i);
                break;

            case CHECK_LINE_FOUR :
                compare_lcd_image_char(line_one_char_image , line_two_char_image , line_three_char_image , line_four_char_image);
                for (int i = 0; i < 19; i++)
                    compare_lcd_image_specific(lcd_image_arr[3][i] , 3 , i);
                break;

            case CHECK_ALL :
                compare_lcd_image_char(line_one_char_image , line_two_char_image , line_three_char_image , line_four_char_image);
                compare_lcd_image_str(lcd_image_arr);
                break;

            case CHECK_CHARS :
                compare_lcd_image_char(line_one_char_image , line_two_char_image , line_three_char_image , line_four_char_image);
                break;

            case CHECK_PULSE :
                for (int i = 0; i < 6; i++)
                    compare_lcd_image_specific(lcd_image_arr[0][i+10], 0 , i+10);
                break;

            case CHECK_DETAILS_COLON :
                for (int i = 0; i < 3; i++)
                    compare_lcd_image_specific(lcd_image_arr[1+i][8] , 1+i , 8);
                break;

            case CHECK_DETAILS_INPUT :
                for (int i = 0; i < 3; i++)
                {
                    for (int n = 0; n < 3; n++)
                    {
                        compare_lcd_image_specific(lcd_image_arr[i+1][n+10] , i+1 , n+10);
                    }
                }
                break;

            case MOVE_DETAILS_COLON :

                for (int i = 0; i < 3; i++)
                    compare_lcd_image_specific(lcd_image_arr[1+i][8] , 1+i , 8);
                break;

            case CHECK_ALL_CHARS :
                compare_all_chars();
                break;

            default :
                break;
            }
        }
        vTaskDelay( ACTIVE_TASK_DELAY );
    }
}

void compare_single_string(char arr[20] , int line)
{
    int transmit;

    if (strcmp(arr , lcd_output_arr[line]))
    {
        strcpy(lcd_output_arr[line] , arr);
        if (line == 0)
            transmit = SEND_LINE_ONE;
        else if (line == 1)
            transmit = SEND_LINE_TWO;
        else if (line == 2)
            transmit = SEND_LINE_THREE;
        else if (line == 3)
            transmit = SEND_LINE_FOUR;

        xQueueSend(LCD_display_q , &transmit , 500);
    }
}

void compare_lcd_image_str(char arr[4][20])
{
    int transmit;

    if ((strcmp(arr[0] , lcd_output_arr[0])))
    {
        strcpy(lcd_output_arr[0] , arr[0]);
        transmit = SEND_LINE_ONE;
        xQueueSend(LCD_display_q , &transmit, 500);
        vTaskDelay(5);
    }

    if ((strcmp(arr[1] , lcd_output_arr[1])))
    {
        strcpy(lcd_output_arr[1] , arr[1]);
        transmit = SEND_LINE_TWO;
        xQueueSend(LCD_display_q , &transmit, 500);
        vTaskDelay(5);
    }

    if ((strcmp(arr[2] , lcd_output_arr[2])))
    {
        strcpy(lcd_output_arr[2] , arr[2]);
        transmit = SEND_LINE_THREE;
        xQueueSend(LCD_display_q , &transmit, 500);
        vTaskDelay(5);
    }

    if ((strcmp(arr[3] , lcd_output_arr[3])))
    {
        strcpy(lcd_output_arr[3] , arr[3]);
        transmit = SEND_LINE_FOUR;
        xQueueSend(LCD_display_q , &transmit, 500);
        vTaskDelay(5);
    }
}



void compare_lcd_image_char(char char_1 , char char_2 , char char_3 , char char_4)
{
    int transmit;

    if (char_1 != line_one_char_out)
    {
        line_one_char_out = char_1;
        transmit = SEND_CHAR_ONE;
        xQueueSend(LCD_display_q , &transmit , 500);
        vTaskDelay(2);
    }
    if (char_2 != line_two_char_out)
    {
        line_two_char_out = char_2;
        transmit = SEND_CHAR_TWO;
        xQueueSend(LCD_display_q , &transmit , 500);
        vTaskDelay(2);
    }
    if (char_3 != line_three_char_out)
    {
        line_three_char_out = char_3;
        transmit = SEND_CHAR_THREE;
        xQueueSend(LCD_display_q , &transmit , 500);
        vTaskDelay(2);
    }
    if (char_4 != line_four_char_out)
    {
        line_four_char_out = char_4;
        transmit = SEND_CHAR_FOUR;
        xQueueSend(LCD_display_q , &transmit , 500);
        vTaskDelay(2);
    }
}


void compare_lcd_image_specific(char chars_1 , int line , int space)
{
    int transmit;
    if (chars_1 != lcd_output_arr[line][space] && chars_1 != '\0')
    {
        lcd_output_arr[line][space] = chars_1;

        transmit = SEND_ONE_CHAR;
        xQueueSend(LCD_display_q , &transmit , 500);
        transmit = line;
        xQueueSend(LCD_display_q , &transmit , 500);
        transmit = space;
        xQueueSend(LCD_display_q , &transmit , 500);


    }

}

void compare_all_chars()
{
    for (int i = 0; i < 4; i++)
    {
        for (int n = 1; n < 20; n++)
        {
            compare_lcd_image_specific(lcd_image_arr[i][n] , i , n);
        }
    }
}
