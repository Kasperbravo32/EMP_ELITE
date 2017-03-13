/*
 * main.c
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

#include "setup.h"
#include "systick.h"
#include "timers.h"

#include "events.h"
#include "actions.h"
#include "states.h"
#include "processes.h"
#include "state_run.h"
#include "set_color.h"

#define SW1 0b10000
#define SW2 0b00001
// Hallo
extern INT16S ticks;

int main(void)
{
    INT8U state = TRAFFIC_LIGHT;

    int event;

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

        event = determine_click();
        state = process_click(event,state);

        RUN_STATE(state);
    }

    return 0;
}
