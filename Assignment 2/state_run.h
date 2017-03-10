/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: events.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090215  MoH   Module created.
*
*****************************************************************************/

#ifndef STATE_RUN_H_
#define STATE_RUN_H_

/***************************** Include files *******************************/
#include "emp_type.h"
#include "set_color.h"
/*****************************    Defines    *******************************/
#define LED_RED              0x02
#define LED_YELLOW           0x04
#define LED_GREEN            0x08

#define RED_LIGHT_ON         0
#define YELLOW_RED_LIGHT_ON  1
#define GREEN_LIGHT_ON       2
#define YELLOW_LIGHT_ON      3

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
int RUN_STATE(int value);
/****************************** End Of Module *******************************/
#endif /*STATE_RUN_H_*/
