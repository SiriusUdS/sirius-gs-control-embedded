#include "../../../Inc/Sensor/PressureSensor/ETM375.h"

void ETM375_init(PressureSensor* instance) {
  instance->errorStatus.value = 0;
  instance->status.value = 0;

  instance->tick = (PressureSensor_tick)ETM375_tick;
}

void ETM375_tick(PressureSensor* instance) {
  instance->currentPressure_adc = *(PressureSensorData*)instance->adcChannel->currentValue;
}