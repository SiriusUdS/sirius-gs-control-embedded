#include "../../../Inc/Device/Valve/SG90.h"

static void incrementDutyCycle(Valve* instance);

static void decrementDutyCycle(Valve* instance);

void SG90_init(Valve* instance) {
  instance->status.value = 0;
  instance->errorStatus.value = 0;

  instance->pwm->minDutyCycle_CCR = SG90_PWM_DUTY_CYCLE_MIN_CCR;
  instance->pwm->maxDutyCycle_CCR = SG90_PWM_DUTY_CYCLE_MAX_CCR;
  instance->pwm->prescaler = SG90_PSC;
  instance->pwm->autoReload = SG90_ARR;

  instance->pwm->currentDutyCycle_CCR = instance->pwm->minDutyCycle_CCR;
}

void SG90_tick(Valve* instance) {
  /*if(instance->pwm->currentDutyCycle_CCR == instance->targetDutyCycle_CCR) {
    instance->status.bits.isClosing = 0;
    instance->status.bits.isOpening = 0;
    return;
  }

  if (instance->pwm->currentDutyCycle_CCR - instance->targetDutyCycle_CCR < 0) {
    incrementDutyCycle(instance);
    instance->status.bits.isClosing = 0;
    instance->status.bits.isOpening = 1;
  }
  else {
    decrementDutyCycle(instance);
    instance->status.bits.isClosing = 1;
    instance->status.bits.isOpening = 0;
  }*/
}

void incrementDutyCycle(Valve* instance) {
  if (instance->pwm->currentDutyCycle_CCR + SG90_ELAPSED_STEP >= instance->targetDutyCycle_CCR) {
    instance->pwm->setDutyCycle((struct PWM*)instance->pwm, instance->targetDutyCycle_CCR);
    instance->pwm->currentDutyCycle_CCR = instance->targetDutyCycle_CCR;
    return;
  }

  if(HAL_GetTick() - instance->pwm->lastDutyCycleChangeTimestamp_ms >= SG90_ELAPSED_DELAY_MS){
    instance->pwm->setDutyCycle((struct PWM*)instance->pwm, instance->pwm->currentDutyCycle_CCR + SG90_ELAPSED_STEP);
  }
}

void decrementDutyCycle(Valve* instance) {
  if (instance->pwm->currentDutyCycle_CCR - SG90_ELAPSED_STEP <= instance->targetDutyCycle_CCR) {
    instance->pwm->setDutyCycle((struct PWM*)instance->pwm, instance->targetDutyCycle_CCR);
    instance->pwm->currentDutyCycle_CCR = instance->targetDutyCycle_CCR;
    return;
  }

  if(HAL_GetTick() - instance->pwm->lastDutyCycleChangeTimestamp_ms >= SG90_ELAPSED_DELAY_MS){
    instance->pwm->setDutyCycle((struct PWM*)instance->pwm, instance->pwm->currentDutyCycle_CCR - SG90_ELAPSED_STEP);
  }
}
