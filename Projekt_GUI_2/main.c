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

// --------------------------------------
//              Functions
// --------------------------------------

// --------------------------------------
//              Variables
// --------------------------------------

volatile INT8U  LED_timer;
static   INT16U alive_timer        = TIMER_500;
static   INT8U  event              = NO_EVENT;
static   INT8U  output             = NO_EVENT;
static   INT64U pause_screen_timer = TIMER_1000;
static   INT8U  pause_screen_on    = 0;

// --------------------------------------
//                Main
// --------------------------------------

extern INT16S ticks;

int main(void)
{
    disable_global_int();
    init_systick();
    setup();
    lcd_pinsetup();
    enable_global_int();
    lcd_GUI_startup();

    schedule();
    while(1)
    {
        while(!ticks);
        ticks--;
        if(! --alive_timer)
        {
            alive_timer = TIMER_500;
            GPIO_PORTD_DATA_R ^= 0x40;
        }
        if(! -- LED_timer)
            SET_LED(!(LED_RED | LED_YELLOW | LED_GREEN));
        if(! --pause_screen_timer && pause_screen_on == 1)
        {
            pause_screen();
            pause_screen_timer = TIMER_5000;
        }
        event  = determine_click();
        output = cases(event);
    }
}
