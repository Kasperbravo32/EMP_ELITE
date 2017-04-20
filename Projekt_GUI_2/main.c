/*
 * main.c
 */
// --------------------------------------
//              Includes
// --------------------------------------
#include <stdint.h>
#include "emp_type.h"
#include "switches.h"
#include "set_color.h"
#include "systick.h"
#include "lcd.h"
#include "menu_handler.h"
#include "timers.h"
#include "setup.h"
#include "tm4c123gh6pm.h"
#include "rtcs.h"
#include "adcudma.h"

// --------------------------------------
//              Functions
// --------------------------------------

// --------------------------------------
//              Variables
// --------------------------------------

volatile INT64U LED_timer          = TIMER_1000;
static   INT16U alive_timer        = TIMER_500;
static   INT8U  event              = NO_EVENT;
static   INT64U pause_screen_timer = TIMER_1000;
static   INT8U  pause_screen_on    = 0;
static   int    adc_pin            = SETUP_PB5;


// --------------------------------------
//                Main
// --------------------------------------

extern INT16S ticks;

int main(void)
{

    disable_global_int();                   // Disable interrupts for setup of systick
    init_systick();                         // Initialize systick
    gpio_setup();                           // Setup general purpose ports for switch + led use
    LCD_setup();                            // Setup LCD
    lcd_init();                             // Initialize lcd
    ADC1_setup(adc_pin);                    // Setup ADC1 with PB4 / PB5
    DMA_setup();                            // Setup UDMA

    enable_global_int();                    // Enable global int after setup
    lcd_menu();                             // Out the menu on screen for first time

    while(1)                                // Start Superloop
    {
        while(!ticks);
        ticks--;

        if(! --alive_timer)
        {
            alive_timer = TIMER_500;
            GPIO_PORTD_DATA_R ^= 0x40;
        }

        if(! -- LED_timer)
        {
            LED_timer = TIMER_500;
            SET_LED(!(LED_RED | LED_YELLOW | LED_GREEN));
        }

        if(! --pause_screen_timer && pause_screen_on == 1)
        {
            pause_screen();
            pause_screen_timer = TIMER_5000;
        }

        event  = determine_click();
                 handle_click(event);
                 ADC_collect(adc_pin);
    }
}
