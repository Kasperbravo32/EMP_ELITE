/*
 *      Created on: 26. April. 2017
 *      Author: Frederik Snedevind
 *
 *      Group :
 *
 *      Frederik Snedevind
 *      Kasper Banke Jørgensen
 *      Lars Nørgaard Larsen
 *      Philip Grundsø
 *      Casper Kronborg Pedersen
 *      Axmed Cabdirashid
 *
 *      4. Semester Spring 2017
 *      University of Southern Denmark
 *      Teknisk Fakultet
 *
 *      Project - Pulse Analyzer
 */
// ----------------------------------------------------------------------------------------------------------
//
//                                                Libraries
//
// ----------------------------------------------------------------------------------------------------------
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "systick_frt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"
#include "setup.h"
#include "emp_type.h"
#include "glob_def.h"
#include "adcudma.h"
#include "switches.h"
#include "set_color.h"
#include "status_led.h"
#include "queue.h"
#include "lcd.h"
#include "lcd_setup.h"
#include "menu_task.h"
#include "pulse_task.h"
#include "FFT_task.h"
#include "details_task.h"
#include "settings_task.h"
#include "ADC_task.h"
#include "LCD_handler_task.h"
#include "LCD_display_task.h"
#include "keypad.h"

// ----------------------------------------------------------------------------------------------------------
//
//                                                Defines
//
// ----------------------------------------------------------------------------------------------------------
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3


// ----------------------------------------------------------------------------------------------------------
//
//                                                Queues
//
// ----------------------------------------------------------------------------------------------------------
xQueueHandle switch_q      = NULL;               // Queue for input from switches + keyboard
xQueueHandle LCD_display_q = NULL;               // Queue for LCD to display
xQueueHandle LCD_handle_q  = NULL;               // Queue for LCD Handler to compare, and for tasks to put into

// ----------------------------------------------------------------------------------------------------------
//
//                                                Variables
//
// ----------------------------------------------------------------------------------------------------------
volatile int  ADC_status = 0;

volatile char ADC_status_str[8];

// ----------------------------------------------------------------------------------------------------------
//
//                                                 Mutex'
//
// ----------------------------------------------------------------------------------------------------------
volatile MUTEX state_mutex = MENU_TASK;

// ----------------------------------------------------------------------------------------------------------
//
//                                                Functions
//
// ----------------------------------------------------------------------------------------------------------
int putChar()
{
  return(0);
}

//char menu_strs[4][15]   = {"Pulse" , "FFT" , "Details" , "Settings"};

// ----------------------------------------------------------------------------------------------------------
//
//                                                  Main
//
// ----------------------------------------------------------------------------------------------------------
int main(void) {

    portBASE_TYPE return_value = pdTRUE;

    setupHardware();
    SET_LED(!LED_RED);
    //lcd_instruct(LCD_CLEAR_DISPLAY);
    /*
     * Sets up the GPIO for :
     *                       Switch 1 & Switch 2
     *                       Matrix Keyboard
     *                       LCD
     *
     * Sets up systick timer and interrupts
     *
     * Sets up ADC1 with chosen input port (default SETUP_PB4 , Port B pin 4
     *
     * Initializes LCD
     *
     * Sets up µDMA
     */

/* ------------------------------------------------------------------------------------------------------------
 *                                              Start Tasks
 * --------------------------------------------------------------------------------------------------------- */
    return_value &= xTaskCreate( status_led_task   , (signed portCHAR * ) "Status_led"       , USERTASK_STACK_SIZE , NULL , LOW_PRIO , NULL); // Task for alive LED
    return_value &= xTaskCreate( determine_switch_1, (signed portCHAR * ) "Switch_1_task"    , USERTASK_STACK_SIZE , NULL , LOW_PRIO , NULL); // Task for switch 1 functionality
    return_value &= xTaskCreate( determine_switch_2, (signed portCHAR * ) "Switch_2_task"    , USERTASK_STACK_SIZE , NULL , LOW_PRIO , NULL); // Task for switch 1 functionality
    return_value &= xTaskCreate( keypad_task       , (signed portCHAR * ) "Keypad_task"      , 128 , NULL , LOW_PRIO , NULL); // Task for keypad functionality
    return_value &= xTaskCreate( lcd_handler_task  , (signed portCHAR * ) "LCD_handler_task" , 128 , NULL , LOW_PRIO , NULL);
    return_value &= xTaskCreate( lcd_display_task  , (signed portCHAR * ) "LCD_display_task" , 64  , NULL , LOW_PRIO , NULL);

    return_value &= xTaskCreate( menu_task         , (signed portCHAR * ) "Menu_task"        , 64  , NULL , LOW_PRIO , NULL);
    return_value &= xTaskCreate( pulse_task        , (signed portCHAR * ) "Pulse_task"       , 128 , NULL , LOW_PRIO , NULL);
    return_value &= xTaskCreate( FFT_task          , (signed portCHAR * ) "FFT_task"         , 64  , NULL , LOW_PRIO , NULL);
    return_value &= xTaskCreate( details_task      , (signed portCHAR * ) "Details_task"     , 128 , NULL , LOW_PRIO , NULL);
    return_value &= xTaskCreate( settings_task     , (signed portCHAR * ) "Settings_task"    , 192 , NULL , LOW_PRIO , NULL);

    return_value &= xTaskCreate( ADC1_task         , (signed portCHAR * ) "ADC1_task"        , 32  , NULL , LOW_PRIO , NULL);


/* ------------------------------------------------------------------------------------------------------------
 *                                              Declare Queues
 * --------------------------------------------------------------------------------------------------------- */
    switch_q      = xQueueCreate(8 , sizeof(int));               // Queue for switches and keyboard
    LCD_display_q = xQueueCreate(8 , sizeof(int));               // Queue for LCD to display
    LCD_handle_q  = xQueueCreate(8 , sizeof(int));               // Queue for LCD Handler to compare, and for functions to put into

/* ------------------------------------------------------------------------------------------------------------
 *                                              Check for memory shortage
 * --------------------------------------------------------------------------------------------------------- */
  if( return_value != pdTRUE ) // Check if tasks couldn't be started<
  {
      GPIO_PORTD_DATA_R &= 0xBF; // Turn on status LED
      while(1);                  // could not create one or more tasks.
  }


/* ------------------------------------------------------------------------------------------------------------
 *                                              Start Scheduler
 * --------------------------------------------------------------------------------------------------------- */
      vTaskStartScheduler();

      // Will only get here, if there was insufficient memory.
      // -----------------------------------------------------
      return 1;
}
