/*
 * setup.h
 *
 *  Created on: 7. mar. 2017
 *      Author: Frederik
 */

#ifndef SETUP_H_
#define SETUP_H_
// --------------------------------------------
//                  Defines
// --------------------------------------------

#define SETUP_PB4       1
#define SETUP_PB5       2

// --------------------------------------------
//                  Functions
// --------------------------------------------

/* --------------------------------------------
 *                  SETUP PF&PD
 * ------------------------------------------*/
void gpio_setup(void);
/* S�tter PF 1, 2, 3 som output
 * S�tter PD 6 som output
 *
 * S�t PF 0 : 4 til digital function
 * S�t PD6 til digital function
 * S�tter internal pull-up ON p� PF0 og PF4
 */


/* --------------------------------------------
 *               SETUP LCD
 * ------------------------------------------*/
void LCD_setup();
/*
 * Sets up GPIO ports for the LCD
 */


/* --------------------------------------------
 *                  Wait
 * ------------------------------------------*/
void wait2_mil (int time);
/*
 * waits 'time' milliseconds, roughly. used between register operations
 */
#endif /* SETUP_H_ */
