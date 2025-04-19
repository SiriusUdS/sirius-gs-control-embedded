#pragma once

#include "Valve.h"

#define HBL388_OPERATING_FREQUENCY_HZ 333

#define HBL388_PSC 3
#define HBL388_ARR 54059

#define HBL388_PWM_DUTY_CYCLE_MIN_CCR 0
#define HBL388_PWM_DUTY_CYCLE_MAX_CCR HBL388_ARR

#define HBL388_PWM_OPENED_DUTY_CYCLE_PCT 43
#define HBL388_PWM_CLOSED_DUTY_CYCLE_PCT 73

//#define HBL388_PWM_OPENED_DUTY_CYCLE_PCT 50
//#define HBL388_PWM_CLOSED_DUTY_CYCLE_PCT 60

#define HBL388_OPENED_DUTY_CYCLE_CCR (HBL388_ARR * HBL388_PWM_OPENED_DUTY_CYCLE_PCT) / 100
#define HBL388_CLOSED_DUTY_CYCLE_CCR (HBL388_ARR * HBL388_PWM_CLOSED_DUTY_CYCLE_PCT) / 100

#define HBL388_ELAPSED_DELAY_MS 1
#define HBL388_STEP 10

extern void HBL388_init(Valve* instance);

extern void HBL388_setIdle(Valve* instance);

extern void HBL388_close(Valve* instance, uint32_t timestamp_ms);

extern void HBL388_open(Valve* instance, uint32_t timestamp_ms);

extern void HBL388_tick(Valve* instance, uint32_t timestamp_ms);