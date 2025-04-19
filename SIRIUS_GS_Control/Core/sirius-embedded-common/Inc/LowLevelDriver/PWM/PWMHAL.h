#pragma once

#include "PWM.h"

/*
FREQ = CLOCKFREQ/(PSC+1)*(ARR+1)

PSC = CLOCKFREQ/FREQ(ARR+1)-1
*/

extern void PWMHAL_init(PWM* instance);

extern void PWMHAL_setDutyCycle(PWM* instance, uint16_t dutyCycle_CCR);