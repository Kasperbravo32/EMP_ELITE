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

void lcd_move_arrow();

void lcd_output();

void lcd_enter();

void enter_puls();

void enter_FFT();

void enter_credits();

void enter_settings();

void pause_screen();

#endif /* MENU_HANDLER_H_ */
