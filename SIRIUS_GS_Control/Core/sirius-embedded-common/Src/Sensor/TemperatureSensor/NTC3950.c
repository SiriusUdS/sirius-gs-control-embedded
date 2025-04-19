#include "../../../Inc/Sensor/TemperatureSensor/NTC3950.h"

void NTC3950_init(TemperatureSensor* instance) {
  instance->status.value = 0;
  instance->errorStatus.value = 0;

  instance->tick = (TemperatureSensor_tick)NTC3950_tick;
}

void NTC3950_tick(TemperatureSensor* instance) {
  instance->currentTemperature_adc = *(TemperatureSensorData*)instance->adcChannel->currentValue;
}