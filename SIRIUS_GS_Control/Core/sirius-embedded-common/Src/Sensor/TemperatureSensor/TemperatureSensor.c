#include "../../../Inc/Sensor/TemperatureSensor/TemperatureSensor.h"

void TemperatureSensor_initDefault(TemperatureSensor* instance) {
  instance->errorStatus.value = 0;
  instance->status.value = 0;
  instance->errorStatus.bits.defaultFunctionCalled = 1;

  instance->tick = (TemperatureSensor_tick)TemperatureSensor_tickDefault;
}

void TemperatureSensor_tickDefault(TemperatureSensor* instance) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}