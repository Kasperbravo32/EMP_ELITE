/*
 * process.c
 *
 *  Created on: 7. mar. 2017
 *      Author: Frederik
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "states.h"
#include "events.h"



int process_button(int button, int state_2)
{
    switch(button)
    {
// -----------------------------------------
//              SWITCH 1 begins
// -----------------------------------------
        case SW1_SINGLE:
            if (state_2 == TRAFFIC_LIGHT)
                state_2 = NORWEGIAN_NIGHT;
            else if (state_2 == NORWEGIAN_NIGHT)
                state_2 = TRAFFIC_LIGHT;
            else if (state_2 == EMERGENCY)
                state_2 = state_2;
            else
                state_2 = NORWEGIAN_NIGHT;
            break;

        case SW1_DOUBLE:
            if (state_2      == TRAFFIC_LIGHT)
                state_2 = EMERGENCY;
            else if (state_2 == NORWEGIAN_NIGHT)
                state_2 = EMERGENCY;
            else if (state_2 == EMERGENCY)
                state_2 = state_2;
            else
                state_2 = EMERGENCY;
            break;

        case SW1_LONG:
            if (state_2      == EMERGENCY)
                state_2 = TRAFFIC_LIGHT;
            else if (state_2 == TRAFFIC_LIGHT)
                state_2 = state_2;
            else if (state_2 == NORWEGIAN_NIGHT)
                state_2 = state_2;
            break;


// -----------------------------------------
//              SWITCH 2 begins
// -----------------------------------------

        case SW2_SINGLE:
            if(state_2 != EMERGENCY)
                state_2 = NORWEGIAN_NIGHT;

            else if (state_2 == EMERGENCY)
                state_2 = state_2;
            break;

        case SW2_DOUBLE:
            if (state_2 != EMERGENCY)
                state_2 = EMERGENCY;

            else if (state_2 == EMERGENCY);
                state_2 = state_2;
            break;

        case SW2_LONG:
            if (state_2 == NORWEGIAN_NIGHT)
                state_2 = TRAFFIC_LIGHT;
            else if (state_2 == EMERGENCY)
                state_2 = TRAFFIC_LIGHT;
            else
                state_2 = state_2;
            break;

        default:
            break;
    }
    return state_2;
}

