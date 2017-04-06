/*
 * setup.c
 *
 *  Created on: 7. mar. 2017
 *      Author: Frederik
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "setup.h"

void setup(void)
{
    int dummy;
    // Enable the GPIO port that is used for the on-board LED.
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOD | SYSCTL_RCGC2_GPIOF;

    // Do a dummy read to insert a few cycles after enabling the peripheral.
    dummy = SYSCTL_RCGC2_R;

    // Set the direction as output (PF1, PF2 and PF3).
    GPIO_PORTF_DIR_R = 0x0E;
    // Set the direction as output (PD6).
    GPIO_PORTD_DIR_R = 0x40;

    // Enable the GPIO pins for digital function (PF0, PF1, PF2, PF3, PF4).
    GPIO_PORTF_DEN_R = 0x1F;
    // Enable the GPIO pins for digital function (PD6).
    GPIO_PORTD_DEN_R = 0x40;

    // Enable internal pull-up (PF0 and PF4).
    GPIO_PORTF_PUR_R = 0x11;
}


void ADCsetup(int value) {


    /*
     * Setup ADC og Port
     * Port B, Pin 4
     * ADC1
     * PB4 = Input
     * Clock = 16MHz (standard)
     *
     */
    SYSCTL_RCGCADC_R = 0x2;                   // Vælger ADC1 Run-Mode Clock Gating Control
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;     // Sætter Port B til GPIO

    /*
     * Sætter PB4 eller PB5 til input
     */
    if      (value == 1)    GPIO_PORTB_DIR_R &= ~(0x10);        // PB4
    else if (value == 2)    GPIO_PORTB_DIR_R &= ~(0x20);        // PB5
    else if (value == 3)    GPIO_PORTB_DIR_R &= ~(0x30);        // PB4+5
    /*
     * Enabler Alternate Function på PB4 eller PB5
     */
    if      (value == 1)    GPIO_PORTB_AFSEL_R = 0x10;          // PB4
    else if (value == 2)    GPIO_PORTB_AFSEL_R = 0x20;          // PB5
    else if (value == 3)    GPIO_PORTB_AFSEL_R = 0x30;          // PB4+5
    /*
     * Sætter PB4 eller PB5 til digital function
     */
    if      (value == 1)    GPIO_PORTB_DEN_R |= 0x10;           // PB4
    else if (value == 2)    GPIO_PORTB_DEN_R |= 0x20;           // PB5
    else if (value == 3)    GPIO_PORTB_DEN_R |= 0x30;           // PB4+5

    /*
     * Disable analog isolation på 0x10 (PB4) eller 0x20 (PB5)
     */
    if      (value == 1)    GPIO_PORTB_AMSEL_R = 0x10;          // PB4
    else if (value == 2)    GPIO_PORTB_AMSEL_R = 0x20;          // PB5
    else if (value == 3)    GPIO_PORTB_AMSEL_R = 0x30;          // PB4+5
    /*
     * Setup Sample Sequencer
     * Sample Sequencer 3
     * FIFO = 1
     * PB4 = AIN10
     * PB5 = AIN11 (Ikke sat op, men ledig)
     *
     */

    ADC1_ACTSS_R &= ~(1 << 3);                // Disable SS3 før setup
    ADC1_EMUX_R = (0xF << 12);                // Vælg trigger-event (12 == continuous sampling)


    /*
     * Sætter AIN10 (PB4) eller AIN11 (PB5) som analog input til SS3, ADC1
     */

    if      (value == 1)    ADC1_SSMUX3_R = 10;                 // PB4
    else if (value == 2)    ADC1_SSMUX3_R = 11;                 // PB5
    else if (value == 3)    ADC1_SSMUX3_R = 10 | 11;            // PB4+5

    ADC1_SSCTL3_R |= 0x6;                     // Sætter END-bit for ADC1
    ADC1_ACTSS_R |= (1 << 3);                 // Enable SS3
    ADC1_ISC_R = (1 << 3);                    // Clear interrupt flag

}
