/*
 * main.c
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

#include "setup.h"
#include "systick.h"
#include "timers.h"
#include "set_color.h"

extern INT16S ticks;
int main(void)
{
    INT8U click;
    INT8U output;


    INT8U alive_timer = TIMER_500;

    disable_global_int();
    init_systick();
    setup();
    enable_global_int();

    while(1)

    {


        while( !ticks );

            // The following will be executed every 5mS
            ticks--;

            if( ! --alive_timer )
            {
              alive_timer        = TIMER_500;
              GPIO_PORTD_DATA_R ^= 0x40;
            }

        click  = determine_click();
        output = lcdcase        (click);
                 react          (output);
    }

    return 0;
}
