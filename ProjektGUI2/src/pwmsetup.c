#include "tm4c123gh6pm."

#define PWM_COMPAREVALUE 0x9C3F     // 40000-1 clocks
#define PWM_CMPAVALUE    752F       // 25%
#define PWM_CMPBVALUE    270F       // 75%

/* PWM Setup
   Following setup on page 1239 in the datasheet. */
SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM0; // enables PWM
for (INT8U i = 0; i < 5; i++);

GPIO_PORTB_AFSEL_R |= ( 1<<6 )  // Enable alternate function PB6

SYSCTL_RCC_R  |= SYSCTL_RCC_PWMDIV_2 + SYSCTL_RCC_USEPWMDIV;

PWM0_CTL_R    |= 0x000;

PWM0_0_GENA_R |= 0x8C;              // See page 1239 in the datasheet

PWM0_0_GENB_R |= 0x80C;             // See page 1239 in the datasheet

PWM0_0_LOAD_R |= PWM_COMPAREVALUE;  // Set compare value

PWM0_0_CMPA_R |= PWM_CMPAVALUE;
PWM0_0_CMPB_R |= PWM_CMPBVALUE;

// Start PWM
PWM0_CTL_R    |= PWM_0_CTL_ENABLE;    // Enable.

PWM0_ENABLE_R |= PWM_ENABLE_PWM0EN;

/* PWM0 Interrupt and Trigger Enable.*/
PWM0_INTEN_R  |= PWM_0_INTEN_TRCMPAU // Trigger for Counter=Comparator A Up.
