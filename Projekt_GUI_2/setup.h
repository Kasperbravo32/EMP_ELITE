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
/* Sætter PF 1, 2, 3 som output
 * Sætter PD 6 som output
 *
 * Sæt PF 0 : 4 til digital function
 * Sæt PD6 til digital function
 * Sætter internal pull-up ON på PF0 og PF4
 */


/* --------------------------------------------
 *               SETUP LCD
 * ------------------------------------------*/
void LCD_setup();
/*
 * Sets up GPIO ports for the LCD
 */



/* --------------------------------------------
 *           SETUP ADC0, PB4 / PB5
 * ------------------------------------------*/
void ADC0_setup(int value);
/*
 * Setup ADC og Port
 * Port B, Pin 4
 * ADC0
 * PB4 = Input
 *
 * Setup Sample Sequencer
 * Sample Sequencer 3
 * FIFO = 1
 * PB4 = AIN10
 * PB5 = AIN11
 */



/* --------------------------------------------
 *           SETUP ADC1, PB4 / PB5
 * ------------------------------------------*/
void ADC1_setup(int value);
/*
 * Setup ADC og Port
 * Port B, Pin 4
 * ADC1
 * PB4 = Input
 *
 * Setup Sample Sequencer
 * Sample Sequencer 3
 * FIFO = 1
 * PB4 = AIN10
 * PB5 = AIN11
 */

/* --------------------------------------------
 *                  SETUP µDMA
 * ------------------------------------------*/
void DMA_setup();
/*
 * Sets up the µDMA to work with the ADC
 */





/* --------------------------------------------
 *                  Wait
 * ------------------------------------------*/
void wait2_mil (int time);
/*
 * waits 'time' milliseconds, roughly. used between register operations
 */
#endif /* SETUP_H_ */
