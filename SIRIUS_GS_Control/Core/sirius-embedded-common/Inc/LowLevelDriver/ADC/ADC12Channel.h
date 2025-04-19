#pragma once

#include <stdint.h>

#include "../../../sirius-headers-common/LowLevelDriver/ADC/ADCChannelErrorStatus.h"
#include "../../../sirius-headers-common/LowLevelDriver/ADC/ADCChannelStatus.h"

struct ADC12Channel;

typedef void (*ADC12Channel_init)(struct ADC12Channel* instance);

typedef void (*ADC12Channel_tick)(struct ADC12Channel* instance);

typedef struct {
  ADC12Channel_init init;
  ADC12Channel_tick tick;

  void* externalHandle;
  
  uint32_t channelNumber;
  uint32_t rank;
  uint32_t sampleTime_adcClockCyles;

  int16_t* currentValue;
  int16_t  previousValue;

  int16_t  disconnectedValue;
  int16_t  shortCircuitValue;
  
  ADCChannelErrorStatus  errorStatus;
  ADCChannelStatus       status;
}
ADC12Channel;

extern void ADC12Channel_initDefault(ADC12Channel* instance);

extern void ADC12Channel_tickDefault(ADC12Channel* instance);