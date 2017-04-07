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
void setup(void);
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
void lcd_pinsetup();
/*
 * Sets up GPIO ports for the LCD
 */


/* --------------------------------------------
 *           SETUP ADC, PB4 / PB5
 * ------------------------------------------*/
void ADCsetup(int value);
/*
 * Setup ADC og Port
 * Port B, Pin 4
 * ADC1
 * PB4 = Input
 * Clock = 16MHz (standard)
 *
 * Setup Sample Sequencer
 * Sample Sequencer 3
 * FIFO = 1
 * PB4 = AIN10
 * PB5 = AIN11 (Ikke sat op, men ledig)
 */

/* --------------------------------------------
 *                  SETUP �DMA
 * ------------------------------------------*/
void DMAsetup();
/*
 * Sets up the �DMA to work with the ADC
 */
#endif /* SETUP_H_ */
