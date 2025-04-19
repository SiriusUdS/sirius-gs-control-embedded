#include "../../../Inc/Sensor/PressureSensor/PressureSensor.h"

void PressureSensor_initDefault(PressureSensor* instance) {
  instance->status.value = 0;
  instance->errorStatus.value = 0;
  instance->errorStatus.bits.defaultFunctionCalled = 1;

  instance->tick = (PressureSensor_tick)PressureSensor_tickDefault;
}

void PressureSensor_tickDefault(PressureSensor* instance) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}