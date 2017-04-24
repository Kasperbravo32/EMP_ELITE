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

#ifndef SET_COLOR_H_
#define SET_COLOR_H_

/***************************** Include files *******************************/
/*****************************    Defines    *******************************/
// LED COLOURS
#define LED_RED     0x02
#define LED_YELLOW  0x04
#define LED_GREEN   0x08

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
void SET_LED(INT8U value);

int alive_LED(INT16U timer);

int LED_OFF(INT16U timer);
/****************************** End Of Module *******************************/
#endif /*SET_COLOR_H_*/
