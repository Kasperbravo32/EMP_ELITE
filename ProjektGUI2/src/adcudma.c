/*
 * adcudma.c
 *
 *  Created on: 19. apr. 2017
 *      Author: Frederik and Lars
 */

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "adcudma.h"
#include "setup.h"

/*
// Define structure for control table.
#if defined(ewarm)                                                      // To make sure it's compiled correctly there is support for ewarm and ccs compiler types
    #pragma data_alignment=1024                                         // Force declaration of struct to be on 0x10000000000 similar to .org for Atmel MPUs
    typedef struct  {                                                   // Every element in the struct must be 32bits = 4bytes -> gives a total of 16bytes pr channel.
        INT32U   dest;                                                  // Destination pointer
        INT32U   sour;                                                  // Source pointer
        INT32U   ctrl;                                                  // Control word
        INT32U   unused;                                                // Unused space, can be used for other stuff.
    } new_dma_ctrl_table ;                                                                  // This struct can be made shortere so it's only channel 27 data.
#elif defined(ccs)                                                      // Support for other types of compilers
    #pragma DATA_ALIGN(dma_ctrl_table, 1024)
    typedef struct {
        INT32U   dest;
        INT32U   sour;
        INT32U   ctrl;
        INT32U   unused;
    } new_dma_ctrl_table ;
#endif
*/
typedef struct {
    INT32U   *dest;
    INT32U   *sour;
    INT32U   ctrl;
    INT32U   unused;
} new_dma_ctrl_table;

volatile new_dma_ctrl_table dma_prim_ctrl_table[32] __attribute__ ((aligned (1024)));                      // Define primary control table
volatile new_dma_ctrl_table dma_alte_ctrl_table[32];                                                       // Define alternate control table

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
    SYSCTL_RCGCADC_R = 0x1;                                         // Sets ADC0 Run-Mode Clock Gating Control
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;                           // Sets Port B to GPIO

    /*
     * sets PB4 or PB5 as input
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_DIR_R &= ~(0x10);    // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_DIR_R &= ~(0x20);    // PB5

    /*
     * Enables Alternate Function on PB4 or PB5
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_AFSEL_R = 0x10;      // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_AFSEL_R = 0x20;      // PB5

    /*
     * sets PB4 or PB5 to digital function
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_DEN_R |= 0x10;       // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_DEN_R |= 0x20;       // PB5

    /*
     * Disable analog isolation on 0x10 (PB4) or 0x20 (PB5)
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_AMSEL_R = 0x10;      // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_AMSEL_R = 0x20;      // PB5

    /*
     * Setup Sample Sequencer
     * Sample Sequencer 3
     * FIFO = 1
     * PB4 = AIN10
     * PB5 = AIN11 (Ikke sat op, men ledig)
     *
     */
    ADC0_ACTSS_R &= ~(1 << 3);                                      // Disable SS3 for setup
    ADC0_EMUX_R = (0xF << 12);                                      // Sets trigger-event (12 == continuous sampling)

    /*
     * sets AIN10 (PB4) or AIN11 (PB5) as analog input to SS3, ADC1
     */
    if      (value == SETUP_PB4)    ADC0_SSMUX3_R = 10;             // PB4
    else if (value == SETUP_PB5)    ADC0_SSMUX3_R = 11;             // PB5

    ADC0_SSCTL3_R |= 0x6;                                           // Sets END-bit for ADC1
    ADC0_ACTSS_R |= (1 << 3);                                       // Enable SS3
    ADC0_ISC_R = (1 << 3);                                          // Clear interrupt flag
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
    SYSCTL_RCGCADC_R = 0x2;                                         // Sets ADC1 Run-Mode Clock Gating Control
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;                           // Sets Port B to GPIO

    /*
     * Sets PB4 or PB5 as input
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_DIR_R &= ~(0x10);    // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_DIR_R &= ~(0x20);    // PB5

    /*
     * Enables Alternate Function on PB4 or PB5
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_AFSEL_R = 0x10;      // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_AFSEL_R = 0x20;      // PB5

    /*
     * Sets PB4 or PB5 to digital function
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_DEN_R |= 0x10;       // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_DEN_R |= 0x20;       // PB5

    /*
     * Disable analog isolation on 0x10 (PB4) or 0x20 (PB5)
     */
    if      (value == SETUP_PB4)    GPIO_PORTB_AMSEL_R = 0x10;      // PB4
    else if (value == SETUP_PB5)    GPIO_PORTB_AMSEL_R = 0x20;      // PB5

    /*
     * Setup Sample Sequencer
     * Sample Sequencer 3
     * FIFO = 1
     * PB4 = AIN10
     * PB5 = AIN11 (Ikke sat op, men ledig)
     *
     */
    ADC1_ACTSS_R &= ~(1 << 3);                                      // Disable SS3 før setup
    ADC1_EMUX_R = (0xF << 12);                                      // Set trigger-event (12 == continuous sampling)

    /*
     * Sets AIN10 (PB4) or AIN11 (PB5) as analog input to SS3, ADC1
     */
    if      (value == SETUP_PB4)    ADC1_SSMUX3_R = 10;             // PB4
    else if (value == SETUP_PB5)    ADC1_SSMUX3_R = 11;             // PB5

    ADC1_SSCTL3_R |= 0x6;                                           // Sets END-bit for ADC1
    ADC1_ACTSS_R |= (1 << 3);                                       // Enable SS3
    ADC1_ISC_R = (1 << 3);                                          // Clear interrupt flag
}

/* --------------------------------------------
 *                  SETUP uDMA
 *  INPUT:  None.
 *  OUTPUT: None.
 * ------------------------------------------*/

 void dma_queue_makespace( INT16U move )
 {

     if (move > DMA_MAX_QUEUE_SIZE)                                 // Queue overflow.
         move = DMA_MAX_QUEUE_SIZE;                                 // Inserts the max allowed items.

     if ( !move <= 0)
     {
         /*  Move Head  */
         if (DMA_MAX_QUEUE_SIZE - dma_queue.head < move)                  // If head reaches start of array jump to end of array.
             dma_queue.head = move - DMA_MAX_QUEUE_SIZE - dma_queue.head;                                 // move-1 because it makes 4 places free because that DMA_MAX_QUEUE_SIZE is a place in it self.
         else
             dma_queue.head = dma_queue.head + move;                // Move array head.

         /*  Move tail  */
         if (DMA_MAX_QUEUE_SIZE - dma_queue.tail < move)                                   // If tail reaches start of array jump to end of array.
             dma_queue.tail = move - DMA_MAX_QUEUE_SIZE - dma_queue.tail;
         else
             dma_queue.tail = dma_queue.tail + move;                // Move array end.
     }

 }

 void dma_interupt_handler( void )
 {
     static BOOLEAN dma_ch27_ctrl_sw = 0;                                    // Check if it's the primary og alternative control structure.

     switch (UDMA_CHIS_R) {                                                  // The reason for the switchcase is to make room for expansion.
         case (1<<DMA_CH27):
             dma_queue_makespace( DMA_CH27_MOVESIZE );                       // Make room for transer.
             if ( !dma_ch27_ctrl_sw ) {
                 dma_prim_ctrl_table[DMA_CH27].dest = dma_queue.next_item[dma_queue.head - DMA_CH27_MOVESIZE - 1];
                 dma_ch27_ctrl_sw = 1;
             }
             else
             {
                 dma_alte_ctrl_table[DMA_CH27].dest = dma_queue.next_item[dma_queue.head - DMA_CH27_MOVESIZE - 1];
                 dma_ch27_ctrl_sw = 0;
             }
         break;

         default:
         break;
     }

 }

void DMA_setup( void )
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

    // Makes sure that the ADC is able to make a request to the DMA.
    UDMA_REQMASKCLR_R |= (1<<DMA_CH27);

    /*  3. Program the location of the channel control table by writing the base address of the table to the
        DMA Channel Control Base Pointer (DMACTLBASE) register. The base address must be
        aligned on a 1024-byte boundary. */

    /* Data structure define! */

    // Pointer array.
    for (INT16U i = 0; i < DMA_MAX_QUEUE_SIZE-1; i++) {                     // Need to use INT16U because i count higher than 255
        dma_queue.next_item[i] = &dma_queue.items[i+1];                     // Contruct pointer array.
    }
    dma_queue.next_item[DMA_MAX_QUEUE_SIZE-1] = dma_queue.items;            // No "&" in front of "items" when it's an array.

    dma_queue.head = 0;                                                     // Adds first queue element.
    dma_queue.tail = DMA_MAX_QUEUE_SIZE-1;                                  // Last queue element.
    /* End of data structure define. */

    /* Create control tabel. */
    dma_queue_makespace(DMA_CH27_MOVESIZE);
    dma_prim_ctrl_table[DMA_CH27].dest = dma_queue.next_item[dma_queue.head];
    dma_prim_ctrl_table[DMA_CH27].sour = p_adc_sso0;
    dma_prim_ctrl_table[DMA_CH27].ctrl |= UDMA_CHCTL_DSTINC_32 + UDMA_CHCTL_DSTSIZE_32 + UDMA_CHCTL_SRCINC_32 + UDMA_CHCTL_SRCSIZE_32 + UDMA_CHCTL_ARBSIZE_4 + (DMA_CH27_MOVESIZE << 4) + UDMA_CHCTL_XFERMODE_PINGPONG;  // Control settings.

    dma_queue_makespace(DMA_CH27_MOVESIZE);
    dma_alte_ctrl_table[DMA_CH27].dest = dma_queue.next_item[dma_queue.head];
    dma_alte_ctrl_table[DMA_CH27].sour = p_adc_sso0;                        // SS0 8 samples. Needs to be incrementet by 32
    dma_alte_ctrl_table[DMA_CH27].ctrl |= UDMA_CHCTL_DSTINC_32 + UDMA_CHCTL_DSTSIZE_32 + UDMA_CHCTL_SRCINC_32 + UDMA_CHCTL_SRCSIZE_32 + UDMA_CHCTL_ARBSIZE_4 + (DMA_CH27_MOVESIZE << 4) + UDMA_CHCTL_XFERMODE_PINGPONG;  // Control settings.

    // end of control table.

// FEJL!!!!!
//    UDMA_CTLBASE_R = (dma_prim_ctrl_table << 0x8)                                   // Set control table start address.

    /*  Enable the DMA and start transfer!  */
    UDMA_ENASET_R = (1<<DMA_CH27);
}


/* --------------------------------------------
 *     Adds items to uDMA queue.
 *  INPUT:  .
 *  OUTPUT: None.
 * ------------------------------------------*/
void dma_queue_add( INT32U items[] )
{
    static INT16U move;
    move = sizeof(items)/sizeof(INT32U);

    if (move > DMA_MAX_QUEUE_SIZE)                                          // Queue overflow.
        move = DMA_MAX_QUEUE_SIZE;                                          // Inserts the max allowed items.

    dma_queue_makespace( move );

    // Insert items in array.
    for ( INT16U i = 0; i < move; i++ )
    {
        if (dma_queue.head == DMA_MAX_QUEUE_SIZE) {

        }

    }
}
