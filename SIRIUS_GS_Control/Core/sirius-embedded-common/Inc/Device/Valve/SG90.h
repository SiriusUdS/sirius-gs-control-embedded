#pragma once

#include "Valve.h"

#define SG90_OPERATING_FREQUENCY_HZ 50

#define SG90_PSC 59
#define SG90_ARR 32031

// 0 is 0%
// 32031 is 100 %
// SG90 is from 5 to 10 %
#define SG90_PWM_DUTY_CYCLE_MIN_CCR 1601
#define SG90_PWM_DUTY_CYCLE_MAX_CCR 3204

// MARC
#define SG90_ELAPSED_DELAY_MS 1
#define SG90_ELAPSED_STEP 10

extern void SG90_init(Valve* instance);

extern void SG90_gatherData(Valve* instance);

extern void SG90_setDutyCycle(Valve* instance, uint32_t dutyCycle_pct);

extern void SG90_tick(Valve* instance);