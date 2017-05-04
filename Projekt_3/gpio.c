/*
 * gpio.c
 *
 *  Created on: 24. apr. 2017
 *      Author: kronb
 */
/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: gpio.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "gpio.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_gpio(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{
  int dummy;

  // Enable the GPIO port that is used for the on-board LED.
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA
          | SYSCTL_RCGC2_GPIOC
          | SYSCTL_RCGC2_GPIOD
          | SYSCTL_RCGC2_GPIOE
          | SYSCTL_RCGC2_GPIOF;

  SYSCTL_RCGC1_R |= 0x00000001;
  // Do a dummy read to insert a few cycles after enabling the peripheral.
  dummy = SYSCTL_RCGC2_R;

  __asm__("nop");
  __asm__("nop");
  __asm__("nop");

  GPIO_PORTF_LOCK_R = 0x4C4F434B;  // Unlock the CR register
  GPIO_PORTD_LOCK_R = 0x4C4F434B;  // Unlock the CR register
  GPIO_PORTC_LOCK_R = 0x4C4F434B;  // Unlock the CR register
  GPIO_PORTA_LOCK_R = 0x4C4F434B;  // Unlock the CR register
  GPIO_PORTE_LOCK_R = 0x4C4F434B;  // Unlock the CR register
  GPIO_PORTF_CR_R   = 0xFF;        // Enable overwrite of PUR to FP0

  // Set the direction as output (PF1, PF2 and PF3).
  GPIO_PORTF_DIR_R |= 0x0E;
  // Set the direction as output (PD2, PD3, PD6). --- PD2 = LCD_RS --- PD3 = LCD_E
  GPIO_PORTD_DIR_R |= 0x4C;

  // Enable the GPIO pins for digital function (PF0, PF1, PF2, PF3, PF4).
  GPIO_PORTF_DEN_R |= 0x1F;
  // Enable the GPIO pins for digital function (PD2, PD3, PD6). --- PD2 = LCD_RS --- PD3 = LCD_E
  GPIO_PORTD_DEN_R |= 0x4C;

  // Enable internal pull-up (PF0 and PF4).
  GPIO_PORTF_PUR_R |= 0x11;


  // Set up I/O for LCD *******************************************
  // Set the direction as output (PC4, PC5, PC6, PC7)
  GPIO_PORTC_DIR_R |= 0xF0;
  // Enable the GPIO pins for digital function (PC4, PC5, PC6, PC7)
  GPIO_PORTC_DEN_R |= 0xF0;

  //***************************************************************
  // Set up I/O for KEY
  // Enable the GPIO pins for digital function (PA4, PA3, PA2)
  //                                            (X1, X2, X3)
  GPIO_PORTA_DEN_R |= 0x1C;
  // Set the direction as output (PA4, PA3, PA2)
  GPIO_PORTA_DIR_R |= 0x1C; //0b00011100
  // Enable the GPIO pins for digital function (PE3, PE2, PE1, PE0)
  //                                              (Y1, Y2, Y3, Y4)
  GPIO_PORTE_DEN_R |= 0x0F;
}

/****************************** End Of Module *******************************/
