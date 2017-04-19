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
void gpio_setup(void)
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
void LCD_setup()
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


void wait2_mil(int time)
{
    time = 16000*time;
    while (time--);
}
