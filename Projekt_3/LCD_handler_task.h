/*
 * LCD_handler_task.h
 *
 *  Created on: 26. apr. 2017
 *      Author: Frederik
 */

#ifndef LCD_HANDLER_TASK_H_
#define LCD_HANDLER_TASK_H_

// ------------------------------------------
//              Includes
// ------------------------------------------


// ------------------------------------------
//              Definitions
// ------------------------------------------
#define ACTIVE_TASK_DELAY        2
#define IDLE_TASK_DELAY         20


// ------------------------------------------
//              Variables
// ------------------------------------------


// ------------------------------------------
//              Functions
// ------------------------------------------
void lcd_handler_task(void *pvparameters);

void compare_lcd_image_str(char arr[4][20]);

void compare_lcd_image_char(char char_1 , char char_2 , char char_3 , char char_4);

void compare_lcd_image_specific(char chars_1 , int line , int space);

void compare_single_string(char arr[20] , int line);

void compare_all_chars();


#endif /* LCD_HANDLER_TASK_H_ */
