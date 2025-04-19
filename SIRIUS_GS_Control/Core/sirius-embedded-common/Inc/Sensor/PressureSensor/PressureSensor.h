#pragma once

#include "../../LowLevelDriver/ADC/ADC12ChannelHAL.h"

#include "../../../sirius-headers-common/PressureSensor/PressureSensorData.h"
#include "../../../sirius-headers-common/PressureSensor/PressureSensorStatus.h"
#include "../../../sirius-headers-common/PressureSensor/PressureSensorErrorStatus.h"

struct PressureSensor;

typedef void (*PressureSensor_init)(struct PressureSensor* instance);

typedef void (*PressureSensor_tick)(struct PressureSensor* instance);

typedef struct {
  PressureSensor_init init;
  PressureSensor_tick tick;

  ADC12Channel* adcChannel;

  PressureSensorData currentPressure_adc;

  PressureSensorErrorStatus errorStatus;
  PressureSensorStatus      status;
}
PressureSensor;

extern void PressureSensor_initDefault(PressureSensor* instance);

extern void PressureSensor_tickDefault(PressureSensor* instance);