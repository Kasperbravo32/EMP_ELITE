/*
 * anders.h
 *
 *  Created on: 30. mar. 2017
 *      Author: Frederik
 */

#ifndef MENU_HANDLER_H_
#define MENU_HANDLER_H_
// -----------------------------------
//              Includes
// -----------------------------------
#include "emp_type.h"

// -----------------------------------
//              Constants
// -----------------------------------
#define MAIN            0
#define PULS            1
#define FFT             2
#define DETAILS         3
#define DETAILS_GENDER 31
#define DETAILS_AGE    32
#define SETTINGS        4
#define PAUSE           5

#define MALE            1
#define FEMALE          2

#define UP              0
#define DOWN            1

#define VLP             0
#define  LP             1
#define  OK             2
#define  HP             3
#define VHP             4
#define SABP            5
#define RPS             6
// -----------------------------------
//              Variables
// -----------------------------------


// -----------------------------------
//              Functions
// -----------------------------------

// Handles click from switch 1/2
void handle_click(int value);

//
void handle_keypad(INT8U value);

// function to update current state
void update_state();

//gets the data from the ADC's
void ADC_collect(int value);

//processes data from ADC0
void ADC_LED(int value);

//processes data from ADC1
void ADC_PULSE(int value);

//moves the arrow/cursor down on the menu
void lcd_move_arrow_down();

//moves the arrow/cursor up on the menu
void lcd_move_arrow_up();

// Moves arrow up or down depending on input, and depending on limits given
void lcd_move_arrow(int value, int upper_lim, int lower_lim);

//outputs the main menu on the LCD display
void lcd_menu();

//enters the function that the arrow points to
void lcd_enter();

//enters the pulse function
void enter_pulse();

//enters the Fast Fourier Transform function
void enter_FFT();

//enters the credits function
void enter_details();

//enters the settings function
void enter_settings();

//shows the pause screen on the LCD display
void pause_screen();

//returns to the main menu
void return_menu();

// converts an int value to 3 characters
void itostr(long int value);

// Allows user to enter gender
void enter_gender();

// Allows user to enter age
void enter_age();

// Processes ADC input from potmeter into age
void ADC_AGE();

// Calculates pulse from ticks
int process_ticks(long int value);

// Resets pause screen timer
int pause_screen_func(INT16U timer);


#endif /* MENU_HANDLER_H_ */
