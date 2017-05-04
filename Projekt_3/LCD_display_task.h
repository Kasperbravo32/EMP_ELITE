/*
 * LCD_display_task.h
 *
 *  Created on: 26. apr. 2017
 *      Author: Frederik
 */

#ifndef LCD_DISPLAY_TASK_H_
#define LCD_DISPLAY_TASK_H_


// ------------------------------------------
//              Includes
// ------------------------------------------


// ------------------------------------------
//              Definitions
// ------------------------------------------
#define LINE_ONE_CHAR         0
#define LINE_TWO_CHAR        20
#define LINE_THREE_CHAR      40
#define LINE_FOUR_CHAR       60

#define SEND_CHAR_ONE       201
#define SEND_CHAR_TWO       202
#define SEND_CHAR_THREE     203
#define SEND_CHAR_FOUR      204
#define SEND_ADC_STATUS     205
#define SEND_ONE_CHAR       206
#define RESEND_ADC_STATUS   207
#define SEND_GRAND_PULSE    208

#define MOVE_DETAILS_COLON  209

#define CHECK_ALL           211
#define CHECK_CHARS         212
#define CHECK_ADC_STATUS    213
#define CHECK_PULSE         214
#define CHECK_DETAILS_COLON 215
#define CHECK_LINE_ONE      216
#define CHECK_LINE_TWO      217
#define CHECK_LINE_THREE    218
#define CHECK_LINE_FOUR     219
#define CHECK_DETAILS_INPUT 220
#define CHECK_ALL_CHARS     221

#define SEND_LINE_ONE       91
#define SEND_LINE_TWO       92
#define SEND_LINE_THREE     93
#define SEND_LINE_FOUR      94

#define CLEAR_BUFFERS       250
// ------------------------------------------
//              Variables
// ------------------------------------------


// ------------------------------------------
//              Functions
// ------------------------------------------
void lcd_display_task (void * pvparameters);


#endif /* LCD_DISPLAY_TASK_H_ */
