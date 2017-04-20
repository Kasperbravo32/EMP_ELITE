/*
 * menu_hangler.c
 *
 *  Created on: 30. mar. 2017
 *      Author: Frederik
 */
// -----------------------------------
//              Includes
// -----------------------------------

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tm4c123gh6pm.h"
#include "lcd.h"
#include "switches.h"
#include "set_color.h"
#include "emp_type.h"
#include "menu_handler.h"
#include "timers.h"
#include "adcudma.h"


// -----------------------------------
//              Variables
// -----------------------------------

static int current_state             = MAIN;
static int pulse_trigger             = 0;
static int pulse_counter_min         = 0;
static int pulse_counter             = 0;
static long int pulse_ticks          = 0;
static int pulse_calc                = 0;

static long int ADC0_result          = 0;
static long int ADC1_result          = 0;

static char pulse_string[]           = "000";

char pulse_stat_1[16] = "Very Low Pulse!";
char pulse_stat_2[16] = "Low Pulse";
char pulse_stat_3[16] = "O.K.";
char pulse_stat_4[16] = "High Pulse";
char pulse_stat_5[16] = "Very High Pulse!";

char pulse_status[16]           = "Very Low Pulse!";
static char char_one                 = '1';
static char char_two                 = '2';
static char char_three               = '3';

const static char menu_strs[4][15]   = {"Pulse" , "FFT" , "Credits" , "Settings"};

static INT8U  arrow_pos              = 1;
static INT8U  pause_dummy            = 0;

static INT8U  line_one_char          = 0x7E;
static INT8U  line_two_char          = ' ';
static INT8U  line_three_char        = ' ';
static INT8U  line_four_char         = ' ';

static   INT64U update_state_timer   = TIMER_4000;
static   INT64U LED_timer            = TIMER_1000;
volatile INT64U pause_screen_timer;
// -----------------------------------
//              Functions
// -----------------------------------


/* --------------------------------------------
 *              Handle clicks
 * ------------------------------------------*/
void handle_click(int value)
{
    switch(value)
    {
    /*
     * Update state loop
     */
    case NO_EVENT :

        if (! --update_state_timer)
        {
            if (current_state == MAIN)
            {
                update_state_timer = TIMER_5000;
                SET_LED(LED_RED);
            }
            else if (current_state == PULS)
            {
                update_state_timer = TIMER_1500;
                SET_LED(LED_YELLOW);
                enter_pulse();
            }
            else if (current_state == FFT)
            {
                update_state_timer = TIMER_1500;
                SET_LED(LED_GREEN);
                enter_FFT();
            }
            else if (current_state == CREDITS)
            {
                update_state_timer = TIMER_5000;
                SET_LED(LED_RED | LED_YELLOW);
                enter_credits();
            }
            else if (current_state == SETTINGS)
            {
                update_state_timer = TIMER_5000;
                SET_LED(LED_YELLOW | LED_GREEN);
                enter_settings();
            }
        }
        break;

    case SW1_SINGLE:
        SET_LED(LED_RED);
        LED_timer = TIMER_1000;
        //pause_screen_timer = TIMER_60000;
        if (current_state == MAIN)
        {
            lcd_move_arrow_down();
            break;
        }
        else if (current_state == PAUSE)
        {
            current_state = MAIN;
            return_menu();
            break;
        }
        break;

    case SW1_DOUBLE:
        SET_LED(LED_YELLOW);
        LED_timer = TIMER_1000;
        //pause_screen_timer = TIMER_60000;
        if (current_state == MAIN)
        {
            lcd_enter();
        }
        break;

    case SW1_LONG:
        SET_LED(LED_GREEN);
        LED_timer = TIMER_1000;
        //pause_screen_timer = TIMER_60000;
        if (current_state != MAIN)
        {
            return_menu();
        }
        break;

    case SW2_SINGLE:
        SET_LED(LED_RED);
        LED_timer = TIMER_1000;
        if (current_state == MAIN)
        {
            lcd_move_arrow_up();
        }
        else if (current_state == PAUSE)
        {
            current_state = MAIN;
            return_menu();
        }


        break;

    case SW2_DOUBLE:
        SET_LED(LED_RED | LED_YELLOW);
        LED_timer = TIMER_1000;
        if (current_state == MAIN)
        {
            lcd_enter();
        }/*
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

        }*/
        break;

    case SW2_LONG:
        SET_LED(LED_RED | LED_YELLOW | LED_GREEN);
        LED_timer = TIMER_1000;
        if (current_state != MAIN)
        {
            return_menu();
        }
        break;

    default:
        break;
    }
}


/* --------------------------------------------
 *              Move cursor down
 * ------------------------------------------*/
void lcd_move_arrow_down()
{
    arrow_pos++;
    if(arrow_pos >= 5)
        arrow_pos = 1;
    switch(arrow_pos)
    {
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


/* --------------------------------------------
 *              Move cursor up
 * ------------------------------------------*/
void lcd_move_arrow_up()
{
    arrow_pos--;
    if(arrow_pos <= 0)
        arrow_pos = 4;
    switch(arrow_pos)
    {
    case 1:
        line_one_char   = 0x7E;
        line_two_char   = ' ';
        line_three_char = ' ';
        line_four_char  = ' ';

        lcd_instruct(LCD_LINE_TWO);
        lcd_data(' ');
        lcd_instruct(LCD_LINE_ONE);
        lcd_data(0x7E);
        break;

    case 2:
        line_one_char   = ' ';
        line_two_char   = 0x7E;
        line_three_char = ' ';
        line_four_char  = ' ';

        lcd_instruct(LCD_LINE_THREE);
        lcd_data(' ');
        lcd_instruct(LCD_LINE_TWO);
        lcd_data(0x7E);
        break;

    case 3:
        line_one_char   = ' ';
        line_two_char   = ' ';
        line_three_char = 0x7E;
        line_four_char  = ' ';

        lcd_instruct(LCD_LINE_FOUR);
        lcd_data(' ');
        lcd_instruct(LCD_LINE_THREE);
        lcd_data(0x7E);
        break;

    case 4:
        line_one_char   = ' ';
        line_two_char   = ' ';
        line_three_char = ' ';
        line_four_char  = 0x7E;

        lcd_instruct(LCD_LINE_ONE);
        lcd_data(' ');
        lcd_instruct(LCD_LINE_FOUR);
        lcd_data(0x7E);
        break;
    }
}


/* --------------------------------------------
 *                  Menu out
 * ------------------------------------------*/
void lcd_menu()

{
    switch(arrow_pos)
    {
    case 1:
        lcd_instruct(LCD_CLEAR_DISPLAY);
        wait_mil(2);

        lcd_data(line_one_char);
        lcd_data_string(menu_strs[0]);
        lcd_instruct(LCD_LINE_TWO);
        lcd_data(line_two_char);
        lcd_data_string(menu_strs[1]);
        lcd_instruct(LCD_LINE_THREE);
        lcd_data(line_three_char);
        lcd_data_string(menu_strs[2]);
        lcd_instruct(LCD_LINE_FOUR);
        lcd_data(line_four_char);
        lcd_data_string(menu_strs[3]);
        break;
    }
}


/* --------------------------------------------
 *           Register enter click
 * ------------------------------------------*/
void lcd_enter()
{
    if (arrow_pos == 1)
    {
        enter_pulse();
    }
    else if (arrow_pos == 2)
    {
        enter_FFT();
    }
    else if (arrow_pos == 3)
    {
        enter_credits();
    }
    else if (arrow_pos == 4)
    {
        enter_settings();
    }
}


/* --------------------------------------------
 *              Return to menu
 * ------------------------------------------*/
void return_menu()
{
    current_state = MAIN;
    lcd_instruct(LCD_CLEAR_DISPLAY);
    wait_mil(2);

    lcd_data(line_one_char);
    lcd_data_string(menu_strs[0]);
    lcd_instruct(LCD_LINE_TWO);
    lcd_data(line_two_char);
    lcd_data_string(menu_strs[1]);
    lcd_instruct(LCD_LINE_THREE);
    lcd_data(line_three_char);
    lcd_data_string(menu_strs[2]);
    lcd_instruct(LCD_LINE_FOUR);
    lcd_data(line_four_char);
    lcd_data_string(menu_strs[3]);

}


/* --------------------------------------------
 *              Pulse-function
 * ------------------------------------------*/
void enter_pulse()
{

    current_state = PULS;
    lcd_instruct(LCD_CLEAR_DISPLAY);
    lcd_data_string("Pulse : ");

    itostr(pulse_calc);
    if (char_one == '0')
        char_one = ' ';
    if (char_two == '0')
        char_two = ' ';

    lcd_data(char_one);
    lcd_data(char_two);
    lcd_data(char_three);

    lcd_instruct(LCD_LINE_THREE);
    lcd_data_string("Status : ");
    lcd_instruct(LCD_LINE_FOUR);
    lcd_data_string(pulse_status);
}


/* --------------------------------------------
 *               FFT-function
 * ------------------------------------------*/
void enter_FFT()
{
    current_state = FFT;
    lcd_instruct(LCD_CLEAR_DISPLAY);
    lcd_data_string("Func : FFT");
}


/* --------------------------------------------
 *               Credits-function
 * ------------------------------------------*/
void enter_credits()
{
    current_state = CREDITS;
    lcd_instruct(LCD_CLEAR_DISPLAY);
    lcd_data_string("Func : Credits");
}


/* --------------------------------------------
 *               Settings-function
 * ------------------------------------------*/
void enter_settings()
{
    current_state = SETTINGS;
    lcd_instruct(LCD_CLEAR_DISPLAY);
    lcd_data_string("Func : Settings");
}


/* --------------------------------------------
 *               pause-function
 * ------------------------------------------*/
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
        pause_dummy = 0;
}



/* --------------------------------------------
 *               ADC1-collect-function
 * ------------------------------------------*/
void ADC_collect(int value)
{
    ADC1_result = ADC1_SSFIFO3_R;   // Gets the data from the queue
    ADC1_ISC_R = (1 << 3);          // Resets interrupt flag, to enable new data to be put into the FIFO

    if (value == SETUP_PB4)
        ADC_PULSE(ADC1_result);
    else if (value == SETUP_PB5)
        ADC_LED(ADC1_result);
}



/* --------------------------------------------
 *               ADC_pulse funktion
 * ------------------------------------------*/
void ADC_PULSE(int value)
{
    if (pulse_trigger == 0 && value > 3100)
    {
        pulse_trigger = 1;
        pulse_ticks++;
        pulse_calc = (60.0 / (0.005*value));

        if (pulse_calc < 30)
            strcpy(pulse_status , pulse_stat_1);
        else if (pulse_calc < 45 && pulse_calc > 29)
            strcpy(pulse_status , pulse_stat_2);
        else if (pulse_calc > 44 && pulse_calc < 80)
            strcpy(pulse_status , pulse_stat_3);
        else if (pulse_calc > 79 && pulse_calc < 100)
            strcpy(pulse_status , pulse_stat_4);
        else if (pulse_calc > 99)
            strcpy(pulse_status , pulse_stat_5);
        else
            strcpy(pulse_status , pulse_stat_3);


        pulse_ticks = 0;
    }
    else if (pulse_trigger == 1 && value > 3100)
    {
        pulse_ticks++;
    }

    else if (pulse_trigger == 1 && value < 3000)
    {
        pulse_ticks++;
        pulse_trigger = 0;
    }
    else if (pulse_trigger == 0 && value < 3000)
    {
        pulse_ticks++;
    }
}



/* --------------------------------------------
*            Convers ints to strings
 * ------------------------------------------*/
void itostr(int value)
{
    char_one   = (value / 100) % 10 + '0';
    char_two   = (value / 10) % 10 + '0';
    char_three = (value / 1) % 10 + '0';
}



/* --------------------------------------------
 *               Potmeter LED-flash funktion
 * ------------------------------------------*/
void ADC_LED(int value)
{
    if (value > 0 && value < 1300) {
        SET_LED(LED_RED);
    }
    else if (value > 1300 && value < 2650) {
        SET_LED(LED_YELLOW);
    }
    else if (value > 2650 && value < 3700) {
        SET_LED(LED_GREEN);
    }
    else if (value > 3700) {
        SET_LED(!LED_RED);
    }

}
