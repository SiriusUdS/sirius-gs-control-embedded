#pragma once

#include "../../LowLevelDriver/ADC/ADC12ChannelHAL.h"
#include "../../../sirius-headers-common/TemperatureSensor/TemperatureSensorData.h"
#include "../../../sirius-headers-common/TemperatureSensor/TemperatureSensorStatus.h"
#include "../../../sirius-headers-common/TemperatureSensor/TemperatureSensorErrorStatus.h"

struct TemperatureSensor;

typedef void (*TemperatureSensor_init)(struct TemperatureSensor* instance);

typedef void (*TemperatureSensor_tick)(struct TemperatureSensor* instance);

typedef struct {
  TemperatureSensor_init     init;
  TemperatureSensor_tick     tick;

  ADC12Channel* adcChannel;

  TemperatureSensorData currentTemperature_adc;

  TemperatureSensorErrorStatus errorStatus;
  TemperatureSensorStatus      status;
}
TemperatureSensor;

extern void TemperatureSensor_initDefault(TemperatureSensor* instance);

extern void TemperatureSensor_tickDefault(TemperatureSensor* instance);
