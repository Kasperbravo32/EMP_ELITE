/*
 * functionalities.c
 *
 *  Created on: 27. apr. 2017
 *      Author: Frederik
 */

// ------------------------------------------
//              Includes
// ------------------------------------------
#include "menu_task.h"
#include "pulse_task.h"
#include "FFT_task.h"
#include "details_task.h"
#include "settings_task.h"
#include "LCD_display_task.h"
#include "LCD_handler_task.h"
#include "lcd.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "functionalities.h"

// ------------------------------------------
//              Definitions
// ------------------------------------------

// ------------------------------------------
//              Queues
// ------------------------------------------
extern xQueueHandle  LCD_display_q;
extern xQueueHandle  LCD_handle_q;


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

volatile extern int  arrow_pos;

// ------------------------------------------
//              Functions
// ------------------------------------------
void lcd_move_arrow(int direction, int upper_lim, int lower_lim)
{
    int transmit;

    if (direction == DOWN)
    {
        arrow_pos++;
        if (arrow_pos > lower_lim)
            arrow_pos = upper_lim;
    }
    else if (direction == UP)
    {
        arrow_pos--;
        if(arrow_pos < upper_lim)
            arrow_pos = lower_lim;
    }

    switch(arrow_pos)
    {
    case 1:
        line_one_char_image   = 0x7E;
        line_two_char_image   = ' ';
        line_three_char_image = ' ';
        line_four_char_image  = ' ';

        break;

    case 2:
        line_one_char_image   = ' ';
        line_two_char_image   = 0x7E;
        line_three_char_image = ' ';
        line_four_char_image  = ' ';
        break;

    case 3:
        line_one_char_image   = ' ';
        line_two_char_image   = ' ';
        line_three_char_image = 0x7E;
        line_four_char_image  = ' ';
        break;

    case 4:
        line_one_char_image   = ' ';
        line_two_char_image   = ' ';
        line_three_char_image = ' ';
        line_four_char_image  = 0x7E;
        break;
    }
}
