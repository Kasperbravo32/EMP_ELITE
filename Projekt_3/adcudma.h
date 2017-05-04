/*
 * adcudma.h
 *
 *  Created on: 19. apr. 2017
 *      Author: Frederik
 */

#ifndef ADCUDMA_H_
#define ADCUDMA_H_
// --------------------------------------------
//                  Includes
//---------------------------------------------
#include "emp_type.h"

// --------------------------------------------
//                  Defines
//---------------------------------------------

#define SETUP_PB4       1
#define SETUP_PB5       2

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
 *           SETUP ADC0 FIFO0
 * ------------------------------------------*/
void ADC0_FIFO0_setup();


/* --------------------------------------------
 *                  SETUP �DMA
 * ------------------------------------------*/
void DMA_setup(void);
/*
 * Sets up the �DMA to work with the ADC
 */
#define  DMA_CH27           27       // The number 27 to specify channel 27.
#define  DMA_CH27_MOVESIZE  4        // Number of elements to be moved.
#define  DMA_MAX_QUEUE_SIZE 1024     //

INT32U *p_adc_sso0 = (INT32U *)0x40039048;  // Address to ADC0SS0 pointer.

struct new_dma_queue {                                                  // Struct to hold data structure.
    INT16U  head;                                                      // First item in the list.
    INT16U  tail;                                                      // Last item in the list.
    INT32U  *next_item[DMA_MAX_QUEUE_SIZE];                             // array of pointers to the next item in the list.
    INT32U  items[DMA_MAX_QUEUE_SIZE];                                  // Array with data values.
};

struct new_dma_queue dma_queue;                                         // Creates new dma_queue.

/* --------------------------------------------
 *                  SETUP DMA
 * ------------------------------------------*/
void DMA_setup(void);
    /*
     * Sets up the DMA to work with the ADC
     */

#endif /* ADCUDMA_H_ */
