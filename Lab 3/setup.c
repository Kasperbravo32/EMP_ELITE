/*
 * setup.c
 *
 *  Created on: 7. mar. 2017
 *      Author: Frederik and people
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

