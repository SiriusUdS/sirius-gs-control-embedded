#include "../../../Inc/LowLevelDriver/PWM/PWM.h"

void PWM_initDefault(PWM* instance) {
  instance->setDutyCycle = (PWM_setDutyCycle)PWM_setDutyCycleDefault;
  instance->status.value = 0;
  instance->errorStatus.value = 0;
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void PWM_setDutyCycleDefault(PWM* instance, uint16_t dutyCycle_CCR) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}