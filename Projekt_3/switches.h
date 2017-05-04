/*
 * actions.h
 *
 *  Created on: 7. mar. 2017
 *      Author: Frederik
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_


// ------------------------------------------
//              Includes
// ------------------------------------------
#include "emp_type.h"

// ------------------------------------------
//              Definitions
// ------------------------------------------
#define NO_EVENT    0

#define SW1_SINGLE 11
#define SW1_DOUBLE 12
#define SW1_LONG   13

#define SW2_SINGLE 21
#define SW2_DOUBLE 22
#define SW2_LONG   23
// ------------------------------------------
//              Variables
// ------------------------------------------


// ------------------------------------------
//              Functions
// ------------------------------------------
void determine_switch_1(void *pvParameters);
void determine_switch_2(void *pvParameters);

void open_input_queue();
void read_switch_q(void *p);


#endif /* SWITCHES_H_ */
