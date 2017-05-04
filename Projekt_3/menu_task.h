/*
 * menu_task.h
 *
 *  Created on: 26. apr. 2017
 *      Author: Frederik
 */

#ifndef MENU_TASK_H_
#define MENU_TASK_H_


// ------------------------------------------
//              Includes
// ------------------------------------------


// ------------------------------------------
//              Definitions
// ------------------------------------------
#define MENU_IN            501
#define MENU_OUT           502

#define MENU_TASK            1
#define PULSE_TASK           2
#define FFT_TASK             3
#define DETAILS_TASK         4
#define SETTINGS_TASK        5


typedef int MUTEX;

// ------------------------------------------
//              Variables
// ------------------------------------------


// ------------------------------------------
//              Functions
// ------------------------------------------


void menu_task(void * pvparameters);



#endif /* MENU_TASK_H_ */
