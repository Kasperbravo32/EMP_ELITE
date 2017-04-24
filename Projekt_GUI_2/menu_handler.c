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
#include <string.h>
#include "setup.h"
#include "tm4c123gh6pm.h"
#include "lcd.h"
#include "switches.h"
#include "set_color.h"
#include "emp_type.h"
#include "menu_handler.h"
#include "timers.h"
#include "adcudma.h"


// ----------------------------------------
//              Variables
// ----------------------------------------
//testeleste


static int current_state             = MAIN;
static int current_gender            = MALE;
static int current_age               = 20;
static int pulse_trigger             = 0;
static int age_chars                 = 0;
static int arrow_pos                 = 1;
static int SAB_flag                  = 0;

static int state_update_LED          = 0;

static long int pulse_avg       = 0;
static long int pulse_avg_arr[4];
static long int pulse_tick_arr[4];

static long int pulse_ticks          = 0;
static long int ADC0_result          = 0;
static long int ADC1_result          = 0;

static double pulse_threshold        = 2650;

static char current_pulse_status[20];
static char pulse_stat_1[20]         = "Very Low Pulse!";
static char pulse_stat_2[20]         = "Low Pulse";
static char pulse_stat_3[20]         = "O.K.";
static char pulse_stat_4[20]         = "High Pulse";
static char pulse_stat_5[20]         = "Very High Pulse!";
static char pulse_stat_SAB[20]       = "Skipped a beat!";
static char pulse_str[5]             = "0000";
static char pulse_status[20]         = "Very Low Pulse!";


static char age_char_1               = ' ';
static char age_char_2               = ' ';
static char age_char_3               = ' ';

const static char menu_strs[4][15]   = {"Pulse" , "FFT" , "Details" , "Settings"};

static INT8U  pause_dummy            = 0;
static INT8U  line_one_char          = 0x7E;
static INT8U  line_two_char          = ' ';
static INT8U  line_three_char        = ' ';
static INT8U  line_four_char         = ' ';
static INT8U  details_line_one_char  = 0x7E;
static INT8U  details_line_two_char  = ' ';

static   INT64U update_state_timer   = TIMER_4000;
static   INT64U LED_timer            = TIMER_1000;
volatile INT64U pause_screen_timer;



// ----------------------------------------
//              Functions
// ----------------------------------------


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
        update_state();
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

        else if (current_state == DETAILS_GENDER)
        {
            if (details_line_one_char == 0x7E)
            {
                details_line_one_char = ' ';
                details_line_two_char = 0x7E;
                lcd_instruct(LCD_LINE_THREE);
                lcd_data(details_line_one_char);
                lcd_instruct(LCD_LINE_FOUR);
                lcd_data(details_line_two_char);
            }
            else
            {
                details_line_one_char = 0x7E;
                details_line_two_char = ' ';
                lcd_instruct(LCD_LINE_THREE);
                lcd_data(details_line_one_char);
                lcd_instruct(LCD_LINE_FOUR);
                lcd_data(details_line_two_char);
            }
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

        else if (current_state == DETAILS_GENDER)
        {
            if (details_line_one_char == 0x7E)
            {
                current_gender = MALE;
                enter_age();
            }
            else
            {
                current_gender = FEMALE;
                enter_age();
            }
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
        else if (current_state == PULS)
        {
            SAB_flag = 0;
        }


        break;

    case SW2_DOUBLE:
        SET_LED(LED_RED | LED_YELLOW);
        LED_timer = TIMER_1000;
        if (current_state == MAIN)
        {
            lcd_enter();
        }

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
 *              Handle keypad input
 * ------------------------------------------*/
void handle_keypad(INT8U value)
{
    /*
    if (current_state == DETAILS_AGE)
    {
        switch(age_chars)
        {
        case 0 :
            if (value == '1')
            {
                lcd_data(value);
                age_chars++;
                break;
            }
        case 1 :
            lcd_data(value);
            age_chars++;
            break;
        case 2 :
            lcd_data(value);
            age_chars++;
            break;
        case 3 :
            return_menu();
            break;

        }
        if (age_char_1 == ' ')
        {
            SET_LED(LED_RED | LED_YELLOW | LED_GREEN);
            switch(value)
            {
                case '1' : age_char_1 = '1'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '2' : age_char_1 = '2'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '3' : age_char_1 = '3'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '4' : age_char_1 = '4'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '5' : age_char_1 = '5'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '6' : age_char_1 = '6'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '7' : age_char_1 = '7'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '8' : age_char_1 = '8'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '9' : age_char_1 = '9'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '0' : age_char_1 = '0'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
            }
            lcd_data(age_char_1);
        }
        else if (age_char_2 == ' ')
        {
            SET_LED(LED_RED | LED_YELLOW | LED_GREEN);
            switch(value)
            {
                case '1' : age_char_2 = '1'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '2' : age_char_2 = '2'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '3' : age_char_2 = '3'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '4' : age_char_2 = '4'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '5' : age_char_2 = '5'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '6' : age_char_2 = '6'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '7' : age_char_2 = '7'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '8' : age_char_2 = '8'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '9' : age_char_2 = '9'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '0' : age_char_2 = '0'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
            }
            lcd_data(age_char_2);
        }
        else if (age_char_3 == ' ')
        {
            SET_LED(LED_RED | LED_YELLOW | LED_GREEN);
            switch(value)
            {
                case '1' : age_char_3 = '1'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '2' : age_char_3 = '2'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '3' : age_char_3 = '3'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '4' : age_char_3 = '4'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '5' : age_char_3 = '5'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '6' : age_char_3 = '6'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '7' : age_char_3 = '7'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '8' : age_char_3 = '8'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '9' : age_char_3 = '9'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
                case '0' : age_char_3 = '0'; SET_LED(LED_RED | LED_YELLOW | LED_GREEN); break;
            }
            lcd_data(age_char_3);
        }
    }
    else
        update_state();
        */
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
        enter_details();
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
    if (current_state != PULS)
    {
        current_state = PULS;
        lcd_instruct(LCD_CLEAR_DISPLAY);
        wait_mil(2);
        lcd_data_string("Pulse : ");
        wait_mil(2);
        lcd_instruct(LCD_LINE_THREE);
        lcd_data_string("Status : ");
        wait_mil(2);
    }

    itostr(pulse_avg);
    lcd_instruct(LCD_LINE_ONE + 8);
    lcd_data_string(pulse_str);

    if (current_pulse_status != pulse_status)
    {
        lcd_instruct(LCD_LINE_FOUR + 4);
        lcd_data_string("                ");
        lcd_instruct(LCD_LINE_FOUR);
        lcd_data_string(pulse_status);
    }

    lcd_instruct(LCD_LINE_ONE);
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
void enter_details()
{
    current_state = DETAILS;
    age_chars = 0;
    enter_gender();
    wait_mil(1);

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

    if (value == SETUP_PB5)
        ADC_LED(ADC1_result);
    else if (value == SETUP_PB4)
        ADC_PULSE(ADC1_result);
}



/* --------------------------------------------
 *               ADC_pulse funktion
 * ------------------------------------------*/
void ADC_PULSE(int value)
{
    if (pulse_trigger == 0 && value > pulse_threshold)
    {
        pulse_trigger = 1;
        pulse_ticks++;

        //pulse_calc = (60.0 / (0.005*pulse_ticks));

        pulse_tick_arr[0] = pulse_tick_arr[1];
        pulse_tick_arr[1] = pulse_tick_arr[2];
        pulse_tick_arr[2] = pulse_ticks;

        pulse_avg_arr[0] = pulse_avg_arr[1];
        pulse_avg_arr[1] = pulse_avg_arr[2];
        pulse_avg_arr[2] = (60.0 / (0.005 * pulse_ticks));

        if (pulse_tick_arr[2] >= 1.8*pulse_tick_arr[1])
        {
            if (pulse_tick_arr[2] >= 1.8*pulse_tick_arr[0])
                SAB_flag = 1;
        }

        pulse_avg = ( pulse_avg_arr[0] + pulse_avg_arr[1] + pulse_avg_arr[2] ) / 3;




        if(SAB_flag == 0)
        {
            if (pulse_avg_arr[2] < 30)
            {
                strcpy(current_pulse_status , pulse_stat_1);
                strcpy(pulse_status , pulse_stat_1);
            }
            else if (pulse_avg_arr[2] > 29 && pulse_avg_arr[2] < 45)
            {
                strcpy(current_pulse_status , pulse_stat_2);
                strcpy(pulse_status , pulse_stat_2);
            }
            else if (pulse_avg_arr[2] > 44 && pulse_avg_arr[2] < 80)
            {
                strcpy(current_pulse_status , pulse_stat_3);
                strcpy(pulse_status , pulse_stat_3);
            }
            else if (pulse_avg_arr[2] > 79 && pulse_avg_arr[2] < 100)
            {
                strcpy(current_pulse_status , pulse_stat_4);
                strcpy(pulse_status , pulse_stat_4);
            }
            else if (pulse_avg_arr[2] > 99)
            {
                strcpy(current_pulse_status , pulse_stat_5);
                strcpy(pulse_status , pulse_stat_5);
            }
            else
            {
                strcpy(current_pulse_status , pulse_stat_3);
                strcpy(pulse_status , pulse_stat_3);
            }
        }
        else if (SAB_flag == 1)
        {
            strcpy(current_pulse_status , pulse_stat_SAB);
            strcpy(pulse_status , pulse_stat_SAB);
        }

        pulse_ticks = 0;

    }
    else if (pulse_trigger == 1 && value > pulse_threshold)
    {
        pulse_ticks++;
    }

    else if (pulse_trigger == 1 && value < pulse_threshold-100)
    {
        pulse_ticks++;
        pulse_trigger = 0;
    }
    else if (pulse_trigger == 0 && value < pulse_threshold-100)
    {
        pulse_ticks++;
    }
}



// --------------------------------------------
//           Convers ints to strings
// --------------------------------------------
void itostr(long int value)
{
    itoa(value , pulse_str , 10);

    if (value < 100 && pulse_str[2] != ' ')
        pulse_str[2] = ' ';
    if (value < 1000 && pulse_str[3] != ' ')
        pulse_str[3] = ' ';
}



// --------------------------------------------
//               Potmeter LED-flash funktion
// --------------------------------------------
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

// --------------------------------------------
//               Potmeter Age
// --------------------------------------------
void ADC_AGE()
{

}

/* --------------------------------------------
 *                  Enter Gender
 * ------------------------------------------*/
void enter_gender()
{
    current_state = DETAILS_GENDER;

    lcd_instruct(LCD_CLEAR_DISPLAY);
    wait_mil(1);
    lcd_data_string("   Gender");
    lcd_instruct(LCD_LINE_THREE);
    lcd_data(details_line_one_char);
    lcd_data_string("Male");
    lcd_instruct(LCD_LINE_FOUR);
    lcd_data(details_line_two_char);
    lcd_data_string("Female");
}



/* --------------------------------------------
 *                  Enter age
 * ------------------------------------------*/
void enter_age()
{
    current_state = DETAILS_AGE;
    lcd_instruct(LCD_CLEAR_DISPLAY);
    wait_mil(1);
    lcd_data_string("   Age");
    lcd_instruct(LCD_LINE_THREE);
    lcd_data_string("   ");

    lcd_data(age_char_1);
    lcd_data(age_char_2);
    lcd_data(age_char_3);
}



/* --------------------------------------------
 *                  Update state
 * ------------------------------------------*/
void update_state()
{
    if (! --update_state_timer)
    {
        if (current_state == MAIN)
        {
            update_state_timer = TIMER_5000;

            if (state_update_LED)
                SET_LED(LED_RED);
        }
        else if (current_state == PULS)
        {
            update_state_timer = TIMER_500;

            if (state_update_LED)
                SET_LED(LED_YELLOW);

            enter_pulse();
        }
        else if (current_state == FFT)
        {
            update_state_timer = TIMER_1500;

            if (state_update_LED)
                SET_LED(LED_GREEN);

            enter_FFT();
        }
        else if (current_state == DETAILS)
        {
            update_state_timer = TIMER_3000;

            if (state_update_LED)
                SET_LED(LED_RED | LED_YELLOW);

            enter_details();
        }
        else if (current_state == DETAILS_AGE)
        {
            update_state_timer = TIMER_1500;

            enter_age();
        }
        else if (current_state == SETTINGS)
        {
            update_state_timer = TIMER_5000;

            if (state_update_LED)
                SET_LED(LED_YELLOW | LED_GREEN);

            enter_settings();
        }
    }
}

