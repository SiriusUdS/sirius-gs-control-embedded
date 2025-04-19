#include "../../../Inc/Device/Valve/HBL388.h"

static void incrementDutyCycle(Valve* instance);

static void decrementDutyCycle(Valve* instance);

static ValveMovementCompleted isMovementCompleted(Valve* instance);

void HBL388_init(Valve* instance) {
  instance->setIdle = (Valve_setIdle)HBL388_setIdle;
  instance->close = (Valve_close)HBL388_close;
  instance->open = (Valve_close)HBL388_open;
  instance->tick = (Valve_tick)HBL388_tick;

  instance->status.value = 0;
  instance->errorStatus.value = 0;

  instance->pwm->minDutyCycle_CCR = HBL388_PWM_DUTY_CYCLE_MIN_CCR;
  instance->pwm->maxDutyCycle_CCR = HBL388_PWM_DUTY_CYCLE_MAX_CCR;
  instance->pwm->prescaler = HBL388_PSC;
  instance->pwm->autoReload = HBL388_ARR;

  instance->pwm->currentDutyCycle_CCR = instance->pwm->minDutyCycle_CCR;
}

void HBL388_setIdle(Valve* instance) {
  instance->pwm->setDutyCycle(instance->pwm, 0);
  instance->status.bits.isIdle = 1;
}

void HBL388_close(Valve* instance, uint32_t timestamp_ms) {
  instance->pwm->setDutyCycle(instance->pwm, HBL388_CLOSED_DUTY_CYCLE_CCR);
  instance->lastDutyCycleChangeTimestamp_ms = timestamp_ms;
  instance->currentState = VALVE_STATE_CLOSING;
  instance->status.bits.isIdle = 0;
}

void HBL388_open(Valve* instance, uint32_t timestamp_ms) {
  instance->pwm->setDutyCycle(instance->pwm, HBL388_OPENED_DUTY_CYCLE_CCR);
  instance->lastDutyCycleChangeTimestamp_ms = timestamp_ms;
  instance->currentState = VALVE_STATE_OPENING;
  instance->status.bits.isIdle = 0;
}

void HBL388_tick(Valve* instance, uint32_t timestamp_ms) {
  if (instance->adjustmentsCount >= instance->maxAdjustmentsCount) {
    instance->errorStatus.bits.exceededMaxAdjustments = 1;
  }

  switch (instance->currentState) {
    case VALVE_STATE_CLOSED:
    case VALVE_STATE_OPENED:
      break;
    case VALVE_STATE_CLOSING:
      if (isMovementCompleted(instance) == VALVE_MOVEMENT_COMPLETED) {
        instance->currentState = VALVE_STATE_CLOSED;
        instance->setIdle(instance);
        instance->adjustmentsCount = 0;
      }
      /*else if (instance->lastDutyCycleChangeTimestamp_ms + instance->slowestExpectedMoveTime_ms < timestamp_ms) {
        decrementDutyCycle(instance);
        instance->adjustmentsCount++;
      }*/
      break;
    case VALVE_STATE_OPENING:
      if (isMovementCompleted(instance) == VALVE_MOVEMENT_COMPLETED) {
        instance->currentState = VALVE_STATE_OPENED;
        instance->setIdle(instance);
        instance->adjustmentsCount = 0;
      }
      /*else if (instance->lastDutyCycleChangeTimestamp_ms + instance->slowestExpectedMoveTime_ms < timestamp_ms) {
        incrementDutyCycle(instance);
        instance->adjustmentsCount++;
      }*/
      break;
    case VALVE_STATE_UNKNOWN:
    default:
      instance->currentState = VALVE_STATE_UNKNOWN;
      instance->errorStatus.bits.invalidState = 1;
      break;
  }
}

// Unused for now, but way be used with tests
void HBL388_setDutyCycle(Valve* instance, uint32_t dutyCycle_pct) {
  //instance->targetDutyCycle_CCR = (int16_t)((dutyCycle_pct * (uint32_t)HBL388_PWM_DUTY_CYCLE_MAX_CCR) / 100);
}

void incrementDutyCycle(Valve* instance) {
  if(HAL_GetTick() - instance->pwm->lastDutyCycleChangeTimestamp_ms >= HBL388_ELAPSED_DELAY_MS){
    instance->pwm->setDutyCycle((struct PWM*)instance->pwm, instance->pwm->currentDutyCycle_CCR + HBL388_STEP);
  }
}

void decrementDutyCycle(Valve* instance) {
  if(HAL_GetTick() - instance->pwm->lastDutyCycleChangeTimestamp_ms >= HBL388_ELAPSED_DELAY_MS){
    instance->pwm->setDutyCycle((struct PWM*)instance->pwm, instance->pwm->currentDutyCycle_CCR - HBL388_STEP);
  }
}

ValveMovementCompleted isMovementCompleted(Valve* instance) {
  switch (instance->currentState) {
    case VALVE_STATE_CLOSED:
    case VALVE_STATE_OPENED:
      return VALVE_MOVEMENT_COMPLETED;
    case VALVE_STATE_CLOSING:
      if (instance->gpio[VALVE_GPIO_CLOSED_INDEX]->read(instance->gpio[VALVE_GPIO_CLOSED_INDEX]) == GPIO_VALUE_HIGH) {
        return VALVE_MOVEMENT_COMPLETED;
      }
      return VALVE_MOVEMENT_NOT_COMPLETED;
    case VALVE_STATE_OPENING:
      if (instance->gpio[VALVE_GPIO_OPENED_INDEX]->read(instance->gpio[VALVE_GPIO_OPENED_INDEX]) == GPIO_VALUE_HIGH) {
        return VALVE_MOVEMENT_COMPLETED;
      }
      return VALVE_MOVEMENT_NOT_COMPLETED;
    case VALVE_STATE_UNKNOWN:
      return 0;
  }
}