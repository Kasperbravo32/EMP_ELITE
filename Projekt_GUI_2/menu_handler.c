/*
 * anders.c
 *
 *  Created on: 30. mar. 2017
 *      Author: Frederik
 */
// -----------------------------------
//              Includes
// -----------------------------------

#include <stdint.h>
#include "lcd.h"
#include "switches.h"
#include "set_color.h"
#include "emp_type.h"
#include "menu_handler.h"
#include "timers.h"

// -----------------------------------
//              Variables
// -----------------------------------

static INT8U  current_state = MAIN;
static INT8U  arrow_pos = 1;
static INT8U LED_timer;
static INT8U pause_dummy = 0;
INT8U strs[4][15] = {"Puls" , "FFT" , "Credits" , "Settings"};
volatile INT64U pause_screen_timer;

static INT8U line_one_char   = 0x7E;
static INT8U line_two_char   = ' ';
static INT8U line_three_char = ' ';
static INT8U line_four_char  = ' ';

// -----------------------------------
//              Functions
// -----------------------------------

int cases(int value)
{
    switch(value)
    {
    case 0:
        return 0;
        break;

    case SW1_SINGLE:
        SET_LED(LED_RED);
        LED_timer = TIMER_1000;
        pause_screen_timer = TIMER_60000;
        if (current_state == MAIN)
        {
            lcd_move_arrow();
            return arrow_pos;
        }
        else if (current_state == PULS)
        {


        }
        else if (current_state == FFT)
        {


        }
        else if (current_state == CREDITS)
        {


        }
        else if (current_state == SETTINGS)
        {


        }
        else if (current_state == PAUSE) {
            current_state = MAIN;
            return_main();
        }
        break;

    case SW1_DOUBLE:
        SET_LED(LED_YELLOW);
        LED_timer = TIMER_1000;
        pause_screen_timer = TIMER_60000;
        if (current_state == MAIN)
        {
            lcd_enter();

        }
        else if (current_state == PULS)
        {


        }
        else if (current_state == FFT)
        {


        }
        else if (current_state == CREDITS)
        {


        }
        else if (current_state == SETTINGS)
        {


        }
        break;

    case SW1_LONG:
        SET_LED(LED_GREEN);
        LED_timer = TIMER_1000;
        pause_screen_timer = TIMER_60000;
        if (current_state != MAIN)
        {
            return_main();
        }

        else if (current_state == PULS)
        {


        }
        else if (current_state == FFT)
        {


        }
        else if (current_state == CREDITS)
        {


        }
        else if (current_state == SETTINGS)
        {


        }
        break;

    case SW2_SINGLE:
        SET_LED(LED_RED);
        LED_timer = TIMER_1000;
        lcd_move_arrow();
        return arrow_pos;

        break;

    case SW2_DOUBLE:
        SET_LED(LED_RED | LED_YELLOW);
        LED_timer = TIMER_1000;
        lcd_move_arrow();
        return arrow_pos;
        break;

    case SW2_LONG:
        SET_LED(LED_RED | LED_YELLOW | LED_GREEN);
        LED_timer = TIMER_1000;
        lcd_move_arrow();
        return arrow_pos;
        break;

    default:
        return 0;
        break;
    }
}

void lcd_move_arrow()
{
    arrow_pos++;
    if(arrow_pos >= 5)
        arrow_pos = 1;

    switch(arrow_pos) {
    case 1:

        line_one_char   = 0x7E;
        line_two_char   = ' ';
        line_three_char = ' ';
        line_four_char  = ' ';

        lcd_instruct(LCD_LINE_FOUR);
        lcd_data(' ');
        lcd_instruct(LCD_LINE_ONE);
        lcd_data(0x7E);
        break;

    case 2:

        line_one_char   = ' ';
        line_two_char   = 0x7E;
        line_three_char = ' ';
        line_four_char  = ' ';

        lcd_instruct(LCD_LINE_ONE);
        lcd_data(' ');
        lcd_instruct(LCD_LINE_TWO);
        lcd_data(0x7E);
        break;

    case 3:

        line_one_char   = ' ';
        line_two_char   = ' ';
        line_three_char = 0x7E;
        line_four_char  = ' ';

        lcd_instruct(LCD_LINE_TWO);
        lcd_data(' ');
        lcd_instruct(LCD_LINE_THREE);
        lcd_data(0x7E);
        break;

    case 4:

        line_one_char   = ' ';
        line_two_char   = ' ';
        line_three_char = ' ';
        line_four_char  = 0x7E;

        lcd_instruct(LCD_LINE_THREE);
        lcd_data(' ');
        lcd_instruct(LCD_LINE_FOUR);
        lcd_data(0x7E);
        break;
    }
}

void lcd_output()
{
    switch(arrow_pos)
    {
    case 1:
        lcd_instruct(LCD_CLEAR_DISPLAY);
        wait_mil(2);

        lcd_data(line_one_char);
        lcd_data_string(strs[0]);
        lcd_instruct(LCD_LINE_TWO);
        lcd_data(line_two_char);
        lcd_data_string(strs[1]);
        lcd_instruct(LCD_LINE_THREE);
        lcd_data(line_three_char);
        lcd_data_string(strs[2]);
        lcd_instruct(LCD_LINE_FOUR);
        lcd_data(line_four_char);
        lcd_data_string(strs[3]);
        break;
    }
}

void lcd_enter()
{
    if (arrow_pos == 1)
    {
        current_state = PULS;
        enter_puls();
    }
    else if (arrow_pos == 2)
    {
        current_state = FFT;
        enter_FFT();
    }
    else if (arrow_pos == 3)
    {
        current_state = CREDITS;
        enter_credits();
    }
    else if (arrow_pos == 4)
    {
        current_state = SETTINGS;
        enter_settings();
    }
}

void return_main()
{
    current_state = MAIN;
    lcd_instruct(LCD_CLEAR_DISPLAY);
    wait_mil(2);

    lcd_data(line_one_char);
    lcd_data_string(strs[0]);
    lcd_instruct(LCD_LINE_TWO);
    lcd_data(line_two_char);
    lcd_data_string(strs[1]);
    lcd_instruct(LCD_LINE_THREE);
    lcd_data(line_three_char);
    lcd_data_string(strs[2]);
    lcd_instruct(LCD_LINE_FOUR);
    lcd_data(line_four_char);
    lcd_data_string(strs[3]);

}

void enter_puls()
{
    lcd_instruct(LCD_CLEAR_DISPLAY);
    lcd_data_string("Func :");
    lcd_data_string(" Puls");
}

void enter_FFT()
{
    lcd_instruct(LCD_CLEAR_DISPLAY);
    lcd_data_string("Func : FFT");
}

void enter_credits()
{
    lcd_instruct(LCD_CLEAR_DISPLAY);
    lcd_data_string("Func : Credits");
}

void enter_settings()
{
    lcd_instruct(LCD_CLEAR_DISPLAY);
    lcd_data_string("Func : Settings");
}

void pause_screen()
{
    pause_dummy++;
    current_state = PAUSE;
    if (pause_dummy == 1)
    {
        lcd_instruct(LCD_CLEAR_DISPLAY);
        wait_mil(2);
        lcd_data_string("BANDIT");
    }
    else if (pause_dummy == 2)
    {
        lcd_instruct(LCD_CLEAR_DISPLAY);
        wait_mil(2);
        lcd_instruct(NEXT_LINE);
        wait_mil(2);
        lcd_data_string("PAUSE");
    }
    else if (pause_dummy == 3)
    {
        lcd_instruct(LCD_CLEAR_DISPLAY);
        wait_mil(2);
        lcd_instruct(NEXT_LINE);
        wait_mil(2);
        lcd_data_string("huehuehuehue");
    }
    if (pause_dummy >= 4)
        pause_dummy = 1;
}
