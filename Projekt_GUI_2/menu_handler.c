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
#include "tm4c123gh6pm.h"
#include "lcd.h"
#include "switches.h"
#include "set_color.h"
#include "emp_type.h"
#include "menu_handler.h"
#include "timers.h"

// -----------------------------------
//              Variables
// -----------------------------------

volatile INT64U pause_screen_timer;

static int    ADC0_result         = 0;
static int    ADC1_result         = 0;
static int    pulse_triggered     = 0;
static int    pulse_counter_60    = 0;
static int    pulse_counter       = 0;
static int    current_state       = MAIN;

static INT8U  arrow_pos           = 1;
static INT8U  pause_dummy         = 0;
static INT8U  pulse_string[3]     =  "000";
static INT8U  menu_strs[4][15]    = {"Pulse" , "FFT" , "Credits" , "Settings"};
static INT8U  line_one_char       = 0x7E;
static INT8U  line_two_char       = ' ';
static INT8U  line_three_char     = ' ';
static INT8U  line_four_char      = ' ';

static INT64U update_state_timer  = TIMER_4000;
static INT64U LED_timer           = TIMER_1000;

// -----------------------------------
//              Functions
// -----------------------------------

void handle_click(int value)
{
    switch(value)
    {
    case 0:
        if (! --update_state_timer)
        {
            update_state_timer = TIMER_4000;
            if (current_state == MAIN)
            {
                SET_LED(LED_RED);
                break;
            }
            else if (current_state == PULS)
            {
                SET_LED(LED_YELLOW);
                enter_pulse();
                break;
            }
            else if (current_state == FFT)
            {
                SET_LED(LED_GREEN);
                enter_FFT();
                break;
            }
            else if (current_state == CREDITS)
            {
                SET_LED(LED_RED | LED_YELLOW);
                enter_credits();
                break;
            }
            else if (current_state == SETTINGS)
            {
                SET_LED(LED_YELLOW | LED_GREEN);
                enter_settings();
                break;
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
        else if (current_state == PAUSE) {
            current_state = MAIN;
            return_menu();
        }
        break;

    case SW1_DOUBLE:
        SET_LED(LED_YELLOW);
        LED_timer = TIMER_1000;
        //pause_screen_timer = TIMER_60000;
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

    case SW1_LONG:
        SET_LED(LED_GREEN);
        LED_timer = TIMER_1000;
        //pause_screen_timer = TIMER_60000;
        if (current_state != MAIN)
        {
            return_menu();
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

    case SW2_SINGLE:
        SET_LED(LED_RED);
        LED_timer = TIMER_1000;
        if (current_state == MAIN)
        {
            lcd_move_arrow_up();
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
        else if (current_state == PAUSE) {
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

    default:
        break;
    }
}

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

void lcd_move_arrow_up()
{
    arrow_pos--;
    if(arrow_pos <= 0)
        arrow_pos = 4;

    switch(arrow_pos) {
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

void enter_pulse()
{
    current_state = PULS;
    pulse_counter_60 = pulse_counter * 12;
    pulse_counter = 0;
    //sprintf(pulse_string ,"%d" , pulse_counter_60);               // Der er noget galt med cast'en fra int til string. Det må i bøvle med :)
    lcd_instruct(LCD_CLEAR_DISPLAY);
    lcd_data_string("Puls : ");
    lcd_data_string(pulse_string);
}

void enter_FFT()
{
    current_state = FFT;
    lcd_instruct(LCD_CLEAR_DISPLAY);
    lcd_data_string("Func : FFT");
}

void enter_credits()
{
    current_state = CREDITS;
    lcd_instruct(LCD_CLEAR_DISPLAY);
    lcd_data_string("Func : Credits");
}

void enter_settings()
{
    current_state = SETTINGS;
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
        pause_dummy = 0;
}

void ADC_collect()
{
    /*
    ADC0_result = ADC0_SSFIFO3_R;
    ADC0_ISC_R = (1 << 3);
    */
    ADC1_result = ADC1_SSFIFO3_R;   //gets the data from the queue
    ADC1_ISC_R = (1 << 3);          //

    //ADC0();   // Funktion som behandler data fra ADC0. ADC0 er ikke sat op endnu :(
    ADC1();     // Funktion som behandler det data vi får fra ADC1 kaldes. pt er det puls.
}
/*
void ADC0()
{

}
*/
void ADC1()
{
    if (ADC1_result > 3100 && pulse_triggered == 0) {                   // Hvis input er over ~2.5 volt OG der har været en down-slope, -
        pulse_counter++;                                                // for at forhindre flere puls slag på ét slag.
        pulse_triggered = 1;
    }
    else if (ADC1_result < 3100 && pulse_triggered == 1) {              //
        pulse_triggered = 0;
    }
}
