/*
 * keypad.h
 *
 *  Created on: 2. maj 2017
 *      Author: Frederik
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

// ------------------------------------------
//              Includes
// ------------------------------------------
// ------------------------------------------
//              Includes
// ------------------------------------------
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
//              Functions
// ------------------------------------------
void keypad_task(void *pvparameters);

#endif /* KEYPAD_H_ */
