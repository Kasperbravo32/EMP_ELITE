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
void setup(void);
/* Sætter PF 1, 2, 3 som output
 * Sætter PD 6 som output
 *
 * Sæt PF 0 : 4 til digital function
 * Sæt PD6 til digital function
 * Sætter internal pull-up ON på PF0 og PF4
 */




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

#endif /* SETUP_H_ */
