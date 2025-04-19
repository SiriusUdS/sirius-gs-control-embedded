#pragma once

#include <stdint.h>

#include "../../../sirius-headers-common/Valve/ValveErrorStatus.h"
#include "../../../sirius-headers-common/Valve/ValveStatus.h"
#include "../../../sirius-headers-common/Valve/ValveState.h"

#include "../../LowLevelDriver/PWM/PWM.h"
#include "../../LowLevelDriver/GPIO/GPIO.h"

struct Valve;

#define VALVE_GPIO_AMT 2

#define VALVE_GPIO_OPENED_INDEX 0
#define VALVE_GPIO_CLOSED_INDEX 1

typedef enum {
  VALVE_MOVEMENT_NOT_COMPLETED,
  VALVE_MOVEMENT_COMPLETED
}
ValveMovementCompleted;

typedef void (*Valve_init)(struct Valve* instance);

typedef void (*Valve_setIdle)(struct Valve* instance);

typedef void (*Valve_close)(struct Valve* instance, uint32_t timestamp_ms);

typedef void (*Valve_open)(struct Valve* instance, uint32_t timestamp_ms);

typedef void (*Valve_tick)(struct Valve* instance, uint32_t timestamp_ms);

typedef struct {
  Valve_init    init;
  Valve_setIdle setIdle;
  Valve_close   close;
  Valve_open    open;
  Valve_tick    tick;

  PWM*  pwm;
  GPIO* gpio[VALVE_GPIO_AMT];

  uint8_t openDutyCycle_pct;
  uint8_t closeDutyCycle_pct;

  uint8_t currentState;

  uint32_t slowestExpectedMoveTime_ms;
  uint32_t lastDutyCycleChangeTimestamp_ms;

  uint8_t adjustmentsCount;
  uint8_t maxAdjustmentsCount;

  ValveErrorStatus errorStatus;
  ValveStatus      status;
}
Valve;

extern void Valve_initDefault(Valve* instance);

extern void Valve_setIdleDefault(Valve* instance);

extern void Valve_closeDefault(Valve* instance, uint32_t timestamp_ms);

extern void Valve_openDefault(Valve* instance, uint32_t timestamp_ms);

extern void Valve_tickDefault(Valve* instance, uint32_t timestamp_ms);