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



/* --------------------------------------------
 *                  SETUP PF&PD
 * ------------------------------------------*/
void setup(void)
{
    int dummy;
    // Enable the GPIO port that is used for the on-board LED.
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOD | SYSCTL_RCGC2_GPIOF;


    // Do a dummy read to insert a few cycles after enabling the peripheral.
    dummy = SYSCTL_RCGC2_R;

    //Unlock the CR Register
    GPIO_PORTF_LOCK_R = 0x4C4F434B;

    // Enable overwrite of PUR to FP0
    GPIO_PORTF_CR_R = 0xFF;

    // Set the direction as output (PF1, PF2 and PF3).
    GPIO_PORTF_DIR_R = 0b00001110;
    // Set the direction as output (PD6).
    GPIO_PORTD_DIR_R = 0b01000000;

    // Enable the GPIO pins for digital function (PF0, PF1, PF2, PF3, PF4).
    GPIO_PORTF_DEN_R = 0b00011111;
    // Enable the GPIO pins for digital function (PD6).
    GPIO_PORTD_DEN_R = 0b01000000;

    // Enable internal pull-up (PF0 and PF4).
    GPIO_PORTF_PUR_R = 0b00010001;
}

/* --------------------------------------------
 *                   SETUP LCD
 * ------------------------------------------*/
void lcd_pinsetup()
{
    int dummy;
    // Enable the GPIO port that is used for the LCD Display.
    SYSCTL_RCGC2_R |= (SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOD) ;

    // Do a dummy read to insert a few cycles after enabling the peripheral.
    dummy = SYSCTL_RCGC2_R;

    GPIO_PORTC_LOCK_R = 0x4C4F434B;  // Unlock the CR register
    GPIO_PORTD_LOCK_R = 0x4C4F434B;  // Unlock the CR register

    GPIO_PORTC_CR_R   = 0xFF;        // Enable overwrite of PUR to FP0
    GPIO_PORTD_CR_R   = 0xFF;        // Enable overwrite of PUR to FP0

    // Set port direction for the LCD display (all outputs).
    GPIO_PORTC_DIR_R |= 0xF0;
    GPIO_PORTD_DIR_R |= 0x0C;

    // Enable the digital function of the GPIO pins for the LCD display.
    GPIO_PORTC_DEN_R |= 0xF0;
    GPIO_PORTD_DEN_R |= 0x0C;
    // And the same thing for the RS and enable LCD pin
}

/* --------------------------------------------
 *           SETUP ADC, PB4 / PB5
 * ------------------------------------------*/
void ADCsetup(int value) {


    /*
     * Setup ADC og Port
     * Port B, Pin 4
     * ADC1
     * PB4 = Input
     * Clock = 16MHz (standard)
     *
     */
    SYSCTL_RCGCADC_R = 0x2;                                             // Vælger ADC1 Run-Mode Clock Gating Control
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;                               // Sætter Port B til GPIO

    /*
     * Sætter PB4 eller PB5 til input
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_DIR_R &= ~(0x10);        // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_DIR_R &= ~(0x20);        // PB5

    /*
     * Enabler Alternate Function på PB4 eller PB5
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_AFSEL_R = 0x10;          // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_AFSEL_R = 0x20;          // PB5

    /*
     * Sætter PB4 eller PB5 til digital function
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_DEN_R |= 0x10;           // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_DEN_R |= 0x20;           // PB5

    /*
     * Disable analog isolation på 0x10 (PB4) eller 0x20 (PB5)
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_AMSEL_R = 0x10;          // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_AMSEL_R = 0x20;          // PB5

    /*
     * Setup Sample Sequencer
     * Sample Sequencer 3
     * FIFO = 1
     * PB4 = AIN10
     * PB5 = AIN11 (Ikke sat op, men ledig)
     *
     */
    ADC1_ACTSS_R &= ~(1 << 3);                                          // Disable SS3 før setup
    ADC1_EMUX_R = (0xF << 12);                                          // Vælg trigger-event (12 == continuous sampling)

    /*
     * Sætter AIN10 (PB4) eller AIN11 (PB5) som analog input til SS3, ADC1
     */
    if      (value == SETUP_PB4)    ADC1_SSMUX3_R = 10;                 // PB4
    else if (value == SETUP_PB5)    ADC1_SSMUX3_R = 11;                 // PB5

    ADC1_SSCTL3_R |= 0x6;                     // Sætter END-bit for ADC1
    ADC1_ACTSS_R |= (1 << 3);                 // Enable SS3
    ADC1_ISC_R = (1 << 3);                    // Clear interrupt flag

}


/* --------------------------------------------
 *                  SETUP µDMA
 * ------------------------------------------*/
void DMAsetup() {

    /*
     * Everything is set up using the Initialization and configuration guide in
     * "tm4c123gh6pm.pdf", page 599 - 603, + some additional pages for Register configurations
     * and specific device settings
    */
    /* Module Initialization ----------------------------------------------------------------------- */

    /*
    1. Enable the μDMA clock using the RCGCDMA register (see page 341).
    */
    SYSCTL_RCGCDMA_R |= 0x1;
    /*
    2. Enable the μDMA controller by setting the MASTEREN bit of the DMA Configuration (DMACFG)
    register.
    */
    UDMA_CFG_R |= 0x1;

    /*
    3. Program the location of the channel control table by writing the base address of the table to the
    DMA Channel Control Base Pointer (DMACTLBASE) register. The base address must be
    aligned on a 1024-byte boundary.
    */

    //UDMA_CTLBASE_R |= (ARRAY << 10);

    /* Configure the Channel Attributes ------------------------------------------------------------ */

    /*
    1. Program bit 30 of the DMA Channel Priority Set (DMAPRIOSET) or DMA Channel Priority
    Clear (DMAPRIOCLR) registers to set the channel to High priority or Default priority. */

    /*
    2. Set bit 30 of the DMA Channel Primary Alternate Clear (DMAALTCLR) register to select the
    primary channel control structure for this transfer.*/

    /*
    3. Set bit 30 of the DMA Channel Useburst Clear (DMAUSEBURSTCLR) register to allow the
    μDMA controller to respond to single and burst requests.*/

    /*
    4. Set bit 30 of the DMA Channel Request Mask Clear (DMAREQMASKCLR) register to allow
    the μDMA controller to recognize requests for this channel.*/

    /* Configure the Channel Control Structure ----------------------------------------------------- */

    /*
    1. Program the source end pointer at offset 0x1E0 to the address of the source buffer + 0x3FC.*/

    /*
    2. Program the destination end pointer at offset 0x1E4 to the address of the destination buffer +
    0x3FC.*/


    /* Start the Transfer -------------------------------------------------------------------------- */

    /*
    1. Enable the channel by setting bit 30 of the DMA Channel Enable Set (DMAENASET) register.*/

    /*
    2. Issue a transfer request by setting bit 30 of the DMA Channel Software Request (DMASWREQ)
    register.*/


    /* Configure the Channel Attributes ------------------------------------------------------------ */

    /*
    1. Configure bit 7 of the DMA Channel Priority Set (DMAPRIOSET) or DMA Channel Priority
    Clear (DMAPRIOCLR) registers to set the channel to High priority or Default priority.*/

    /*
    2. Set bit 7 of the DMA Channel Primary Alternate Clear (DMAALTCLR) register to select the
    primary channel control structure for this transfer.*/

    /*
    3. Set bit 7 of the DMA Channel Useburst Clear (DMAUSEBURSTCLR) register to allow the
    μDMA controller to respond to single and burst requests.*/

    /*
    4. Set bit 7 of the DMA Channel Request Mask Clear (DMAREQMASKCLR) register to allow
    the μDMA controller to recognize requests for this channel.*/

}
