/*
 * ADC_task.c
 *
 *  Created on: 26. apr. 2017
 *      Author: Frederik
 */
// ------------------------------------------
//              Includes
// ------------------------------------------
#include <stdint.h>
#include <string.h>
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
//              Queues
// ------------------------------------------
extern xQueueHandle LCD_handle_q;
extern xQueueHandle switch_q;

// ------------------------------------------
//                Mutex'
// ------------------------------------------
volatile extern MUTEX state_mutex;
volatile        MUTEX ADC_mutex = 0;

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

volatile char ADC_status_off[3] = {"OFF"};
volatile char ADC_status_on[3]  = {"ON "};
volatile int  ADC_status;
volatile long int  pulse_output;
volatile long int  ADC1_result;
volatile char pulse_output_string[6];
volatile char pulse_status_string[20];

volatile char active_adc = '1';
volatile char active_ss  = '0';
volatile char input_pin[3] = {"PB4"};

// ------------------------------------------
//              Functions
// ------------------------------------------

void ADC1_task(void * pvparameters)
{
    int transmit;
    int receive;

    while(1)
    {
        if (ADC_mutex == 1 && state_mutex == PULSE_TASK)
        {
            ADC1_result = ADC1_SSFIFO3_R;
            pulse_output = ((ADC1_result/4096.) * 300);

            /*
             * Input fra µDMA mm. skal herind!
             * Pulsdetektering ___ :
             *  tag seneste input i µDMA array og kør puls algoritme på det, fra tidligere projekt
             */

            itoa(pulse_output , pulse_output_string , 10);

            if (pulse_output < 100000)
                pulse_output_string[5] = ' ';
            if (pulse_output < 10000)
                pulse_output_string[4] = ' ';
            if (pulse_output < 1000)
                pulse_output_string[3] = ' ';
            if (pulse_output < 100)
                pulse_output_string[2] = ' ';
            if (pulse_output < 10)
                pulse_output_string[1] = ' ';

            for (int i = 0; i < 6; i++)
                lcd_image_arr[0][i+10] = pulse_output_string[i];

            transmit = CHECK_PULSE;
            //xQueueSend(LCD_handle_q , &transmit , 500);

            vTaskDelay( ACTIVE_TASK_DELAY );
        }
        else
            vTaskDelay( IDLE_TASK_DELAY );
    }
}
