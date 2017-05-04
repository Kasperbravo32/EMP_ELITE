/*
 * pulse_task.c
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
//               Queues
// ------------------------------------------
extern xQueueHandle LCD_handle_q;
extern xQueueHandle LCD_display_q;
extern xQueueHandle switch_q;

// ------------------------------------------
//                Mutex'
// ------------------------------------------
volatile extern MUTEX state_mutex;
volatile extern MUTEX ADC_mutex;

// ------------------------------------------
//              Variables
// ------------------------------------------
volatile extern int ADC_status;

volatile extern long int pulse_output;

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

volatile extern char ADC_status_off[3];
volatile extern char ADC_status_on[3];

char pulse_status_dead[18]      = {"u ded m8  "};
char pulse_status_very_low[18]  = {"Very low! "};
char pulse_status_low[18]       = {"Low       "};
char pulse_status_ok[18]        = {"O.K.      "};
char pulse_status_high[18]      = {"High      "};
char pulse_status_very_high[18] = {"Very high!"};


// ------------------------------------------
//              Functions
// ------------------------------------------
void pulse_task(void * pvparameters)
{
    int transmit;
    int receive;

    while(1)
    {
        if (state_mutex == PULSE_TASK)
        {
            vTaskDelay( ACTIVE_TASK_DELAY );
            strcpy(lcd_image_arr[0] , "Pulse  : ");
            strcpy(lcd_image_arr[1] , "ADC    : ");
            strcpy(lcd_image_arr[2] , "Status : ");
            strcpy(lcd_image_arr[3] , "Flags  : ");

            line_one_char_image   = ' ';
            line_two_char_image   = ' ';
            line_three_char_image = ' ';
            line_four_char_image  = ' ';

            transmit = CHECK_ALL;
            xQueueSend(LCD_handle_q , &transmit , 500);

            transmit = CHECK_PULSE;
            xQueueSend(LCD_handle_q , &transmit , 500);

            transmit = CHECK_LINE_THREE;
            xQueueSend(LCD_handle_q , &transmit , 500);


            if (xQueueReceive(switch_q , &receive , 500))
                {
                    switch(receive)
                    {
                    case SW1_SINGLE :
                        if (ADC_status == 1)
                        {
                            ADC_mutex = 0;
                            ADC_status = 0;
                            SET_LED(!LED_RED);
                            for (int i = 0; i < 3; i++)
                                lcd_image_arr[1][10+i] = ADC_status_off[i];
                        }
                        else if (ADC_status == 0)
                        {
                            SET_LED(LED_RED);
                            ADC_mutex = 1;
                            ADC_status = 1;
                            for (int i = 0; i < 3; i++)
                                lcd_image_arr[1][10+i] = ADC_status_on[i];
                        }
                        transmit = CHECK_LINE_TWO;
                        xQueueSend(LCD_handle_q , &transmit , 500);

                        break;

                    case SW1_DOUBLE :

                        break;

                    case SW1_LONG :
                        transmit = CLEAR_BUFFERS;
                        xQueueSend(LCD_handle_q , &transmit , 500);
                        vTaskDelay( 2 );
                        state_mutex = MENU_TASK;
                        break;

                    case SW2_SINGLE :

                        break;

                    case SW2_DOUBLE :

                        break;

                    case SW2_LONG :

                        break;

                    default :

                        break;

                    }
                    receive = 0;
                }

            if (pulse_output <= 1)
            {
                for (int i = 0; i < 10; i++)
                    lcd_image_arr[2][i+10] = pulse_status_dead[i];
            }

            else if (pulse_output <= 35)
            {
                for (int i = 0; i < 10; i++)
                    lcd_image_arr[2][i+10] = pulse_status_very_low[i];
            }

            else if (pulse_output >= 36 && pulse_output <= 55)
            {
                for (int i = 0; i < 10; i++)
                    lcd_image_arr[2][i+10] = pulse_status_low[i];
            }

            else if (pulse_output >= 56 && pulse_output <= 74)
            {
                for (int i = 0; i < 10; i++)
                    lcd_image_arr[2][i+10] = pulse_status_ok[i];
            }


            else if (pulse_output >= 75 && pulse_output <= 89)
            {
                for (int i = 0; i < 10; i++)
                    lcd_image_arr[2][i+10] = pulse_status_high[i];
            }

            else if (pulse_output >= 90)
            {
                for (int i = 0; i < 10; i++)
                    lcd_image_arr[2][i+10] = pulse_status_very_high[i];
            }



            vTaskDelay( ACTIVE_TASK_DELAY );
        }
        else
            vTaskDelay( IDLE_TASK_DELAY );
    }
}
