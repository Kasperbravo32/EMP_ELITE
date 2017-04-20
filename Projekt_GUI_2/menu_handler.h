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
#define MAIN        0
#define PULS        1
#define FFT         2
#define CREDITS     3
#define SETTINGS    4
#define PAUSE       5


// -----------------------------------
//              Variables
// -----------------------------------


// -----------------------------------
//              Functions
// -----------------------------------

//
void handle_click(int value);

//gets the data from the ADC's
void ADC_collect();

//processes data from ADC0
void ADC_LED(int value);

//processes data from ADC1
void ADC_PULSE(int value);

//moves the arrow/cursor down on the menu
void lcd_move_arrow_down();

//moves the arrow/cursor up on the menu
void lcd_move_arrow_up();

//outputs the main menu on the LCD display
void lcd_menu();

//enters the function that the arrow points to
void lcd_enter();

//enters the pulse function
void enter_pulse();

//enters the Fast Fourier Transform function
void enter_FFT();

//enters the credits function
void enter_credits();

//enters the settings function
void enter_settings();

//shows the pause screen on the LCD display
void pause_screen();

//returns to the main menu
void return_menu();

// converts an int value to 3 characters
void itostr(int value);

// Calculates pulse from ticks
int process_ticks(long int value);


#endif /* MENU_HANDLER_H_ */
