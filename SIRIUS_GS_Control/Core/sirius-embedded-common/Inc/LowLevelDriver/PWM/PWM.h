#pragma once

#include "../../../sirius-headers-common/LowLevelDriver/PWM/PWMErrorStatus.h"
#include "../../../sirius-headers-common/LowLevelDriver/PWM/PWMStatus.h"

struct PWM;

typedef void (*PWM_init)(struct PWM* instance);

typedef void (*PWM_setDutyCycle)(struct PWM* instance, uint16_t dutyCycle_CCR);

typedef struct {
  PWM_init         init;
  PWM_setDutyCycle setDutyCycle;

  uint16_t minDutyCycle_CCR;
  uint16_t maxDutyCycle_CCR;

  // for now, used for HAL comptatibility
  void* externalHandle;
  void* timer;

  // When used with HAL, both = 0
  uint8_t timerId;
  uint8_t channel;

  uint32_t clockFrequency_Hz;

  uint16_t prescaler;
  uint16_t autoReload;

  uint16_t       currentDutyCycle_CCR;
  uint16_t       maxCCRValue;
  unsigned long lastDutyCycleChangeTimestamp_ms;

  PWMErrorStatus errorStatus;
  PWMStatus      status;
}
PWM;

extern void PWM_initDefault(PWM* instance);

extern void PWM_setDutyCycleDefault(PWM* instance, uint16_t dutyCycle_CCR);