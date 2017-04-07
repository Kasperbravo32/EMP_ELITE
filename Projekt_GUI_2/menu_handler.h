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

int cases(int value);

int ADC_collect();

//int ADC0();

int ADC1();

void lcd_move_arrow();

void lcd_output();

void lcd_enter();

void enter_pulse();

void enter_FFT();

void enter_credits();

void enter_settings();

void pause_screen();

void return_main();


#endif /* MENU_HANDLER_H_ */
