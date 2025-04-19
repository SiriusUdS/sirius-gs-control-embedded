#pragma once

#include "../../../sirius-headers-common/LowLevelDriver/ADC/ADCErrorStatus.h"
#include "../../../sirius-headers-common/LowLevelDriver/ADC/ADCStatus.h"

#include "ADC12Channel.h"

#define ADC_CHANNEL_NUMBER 16

struct ADC12;

typedef void (*ADC12_init)(struct ADC12* instance, uint8_t activeChannelsAmt);

typedef void (*ADC12_tick)(struct ADC12* instance);

typedef struct {
  ADC12_init init;
  ADC12_tick tick;

  ADC12Channel channels[ADC_CHANNEL_NUMBER];
  uint8_t activeChannelsAmt;

  int16_t values[ADC_CHANNEL_NUMBER];

  void* externalHandle;
  
  ADCErrorStatus  errorStatus;
  ADCStatus       status;
}
ADC12;

extern void ADC12_initDefault(ADC12* instance, uint8_t activeChannelsAmt);

extern void ADC12_tickDefault(ADC12* instance);