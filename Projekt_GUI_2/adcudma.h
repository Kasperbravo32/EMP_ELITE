/*
 * adcudma.h
 *
 *  Created on: 19. apr. 2017
 *      Author: Frederik
 */

#ifndef ADCUDMA_H_
#define ADCUDMA_H_

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
 *                  SETUP �DMA
 * ------------------------------------------*/
void DMA_setup(void);
/*
 * Sets up the �DMA to work with the ADC
 */
 #define DMA_CH27 27

#endif /* ADCUDMA_H_ */
