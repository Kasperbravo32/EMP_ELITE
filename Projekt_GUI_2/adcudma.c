/*
 * adcudma.c
 *
 *  Created on: 19. apr. 2017
 *      Author: Frederik
 */

#include "adcudma.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "setup.h"

/* --------------------------------------------
 *           SETUP ADC0, PB4 / PB5
 * ------------------------------------------*/

void ADC0_setup(int value)
{
    /*
     * Setup ADC0 and Port B
     * Port B, Pin 4 / Pin 5
     * ADC0
     * Sample Sequencer 3
     * FIFO = 1
     * PB4/5 = Input
     *
    */
    SYSCTL_RCGCADC_R = 0x1;                                             // Sets ADC0 Run-Mode Clock Gating Control
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;                               // Sets Port B to GPIO

    /*
     * sets PB4 or PB5 as input
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_DIR_R &= ~(0x10);        // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_DIR_R &= ~(0x20);        // PB5

    /*
     * Enables Alternate Function on PB4 or PB5
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_AFSEL_R = 0x10;          // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_AFSEL_R = 0x20;          // PB5

    /*
     * sets PB4 or PB5 to digital function
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_DEN_R |= 0x10;           // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_DEN_R |= 0x20;           // PB5

    /*
     * Disable analog isolation on 0x10 (PB4) or 0x20 (PB5)
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
    ADC0_ACTSS_R &= ~(1 << 3);                                          // Disable SS3 for setup
    ADC0_EMUX_R = (0xF << 12);                                          // Sets trigger-event (12 == continuous sampling)

    /*
     * sets AIN10 (PB4) or AIN11 (PB5) as analog input to SS3, ADC1
     */
    if      (value == SETUP_PB4)    ADC0_SSMUX3_R = 10;                 // PB4
    else if (value == SETUP_PB5)    ADC0_SSMUX3_R = 11;                 // PB5

    ADC0_SSCTL3_R |= 0x6;                     // Sets END-bit for ADC1
    ADC0_ACTSS_R |= (1 << 3);                 // Enable SS3
    ADC0_ISC_R = (1 << 3);                    // Clear interrupt flag
}

/* --------------------------------------------
 *           SETUP ADC1, PB4 / PB5
 * ------------------------------------------*/
void ADC1_setup(int value)
{
    /*
     * Setup ADC and Port
     * Port B, Pin 4
     * ADC1
     * PB4 = Input
     *
     */
    SYSCTL_RCGCADC_R = 0x2;                                             // Sets ADC1 Run-Mode Clock Gating Control
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;                               // Sets Port B to GPIO

    /*
     * Sets PB4 or PB5 as input
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_DIR_R &= ~(0x10);        // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_DIR_R &= ~(0x20);        // PB5

    /*
     * Enables Alternate Function on PB4 or PB5
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_AFSEL_R = 0x10;          // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_AFSEL_R = 0x20;          // PB5

    /*
     * Sets PB4 or PB5 to digital function
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_DEN_R |= 0x10;           // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_DEN_R |= 0x20;           // PB5

    /*
     * Disable analog isolation on 0x10 (PB4) or 0x20 (PB5)
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
    ADC1_EMUX_R = (0xF << 12);                                          // Set trigger-event (12 == continuous sampling)

    /*
     * Sets AIN10 (PB4) or AIN11 (PB5) as analog input to SS3, ADC1
     */
    if      (value == SETUP_PB4)    ADC1_SSMUX3_R = 10;                 // PB4
    else if (value == SETUP_PB5)    ADC1_SSMUX3_R = 11;                 // PB5

    ADC1_SSCTL3_R |= 0x6;                     // Sets END-bit for ADC1
    ADC1_ACTSS_R |= (1 << 3);                 // Enable SS3
    ADC1_ISC_R = (1 << 3);                    // Clear interrupt flag
}

/* --------------------------------------------
 *                  SETUP uDMA
 * ------------------------------------------*/
void DMA_setup(void)
{
    /*
     * Everything is set up using the Initialization and configuration guide in
     * "tm4c123gh6pm.pdf", page 599 - 603, + some additional pages for Register configurations
     * and specific device settings
    */

    // ---------------------- Module Initialization ---------------------- */

    // 1. Enable the uDMA clock using the RCGCDMA register (see page 341).
    SYSCTL_RCGCDMA_R |= SYSCTL_RCGCDMA_R0;
    for (int i = 5; i > 0; i--);            // Do nothing to give the peripheral time to start.

    // 3. Enable the uDMA controller by setting the MASTEREN bit of the DMA Configuration (DMACFG) register.
    UDMA_CFG_R |= UDMA_CFG_MASTEN;

    /*  3. Program the location of the channel control table by writing the base address of the table to the
        DMA Channel Control Base Pointer (DMACTLBASE) register. The base address must be
        aligned on a 1024-byte boundary. */
    INT4U  udma_control[1024]; // Array to save control structure in for the uDMA


    //udma_control[435] = 0;  // Unused.
    #if defined(ewarm)
    #pragma data_alignment=1024
    uint8_t ui8ControlTable[1024];
    #elif defined(ccs)
    #pragma DATA_ALIGN(ui8ControlTable, 1024)
    uint8_t ui8ControlTable[1024];
    #else
    uint8_t ui8ControlTable[1024] __attribute__ ((aligned(1024)));
    #endif

    UDMA_CHMAP3_R |= ui8ControlTable;

    UDMA_ALTCLR_R |= (1<<27);

    UDMA_USEBURSTCLR_R |= (1<<DMA_CH27);

    // Channel 27 setup.
    // Start item = 27*4 = 108
    udma_control[432] = 0;  // Source End Pointer.
    udma_control[433] = 0;  // Destination End Pointer.
    udma_control[434] = 0;  // Control Word.


    //UDMA_CTLBASE_R |= (ARRAY << 10);          Create array somewhere, to be fed enough



    /* ---------------------- Configure the Channel Attributes ---------------------- */

    /*  Channel 30 is optimized for software-initiated transfer (ours), so that channel will be used.
        1. Program bit 30 of the DMA Channel Priority Set (DMAPRIOSET) or DMA Channel Priority
        Clear (DMAPRIOCLR) registers to set the channel to High priority or Default priority. */
    UDMA_PRIOCLR_R = 0xFFFF;                // Clears priority of ALL channels, as a safety
    wait2_mil(1);
    UDMA_PRIOSET_R = 0x4000;                // Sets priority of channel 30 to high, above all others
    /*
     *
    2. Set bit 30 of the DMA Channel Primary Alternate Clear (DMAALTCLR) register to select the µ
    primary channel control structure for this transfer.*/

    UDMA_ALTCLR_R = 0x4000;

    /*
    3. Set bit 30 of the DMA Channel Useburst Clear (DMAUSEBURSTCLR) register to allow the      µ
    μDMA controller to respond to single and burst requests.*/

    UDMA_USEBURSTCLR_R = 0x4000;

    /*
    4. Set bit 30 of the DMA Channel Request Mask Clear (DMAREQMASKCLR) register to allow       µ
    the μDMA controller to recognize requests for this channel.*/

    UDMA_REQMASKCLR_R = 0x4000;


    /* Configure the Channel Control Structure ----------------------------------------------------- */

    /*
    1. Program the source end pointer at offset 0x1E0 to the address of the source buffer + 0x3FC.*/


    /*
    2. Program the destination end pointer at offset 0x1E4 to the address of the destination buffer +
    0x3FC.*/



    /* Start the Transfer -------------------------------------------------------------------------- */

    /*
    1. Enable the channel by setting bit 30 of the DMA Channel Enable Set (DMAENASET) register.     µ
    */

    UDMA_ENASET_R = 0x4000;

    /*
    2. Issue a transfer request by setting bit 30 of the DMA Channel Software Request (DMASWREQ)    µ
    register.*/

    UDMA_SWREQ_R = 0x4000;


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
