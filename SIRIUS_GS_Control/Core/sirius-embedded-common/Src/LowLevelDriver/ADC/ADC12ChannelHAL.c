#include "../../../Inc/LowLevelDriver/ADC/ADC12ChannelHAL.h"

#include "stm32f4xx_hal.h"

void ADC12ChannelHAL_init(ADC12Channel* instance) {
  instance->tick = (ADC12Channel_tick)ADC12ChannelHAL_tick;
  instance->errorStatus.value = 0;
  instance->status.value = 0;

  *instance->currentValue = 0;
}

void ADC12ChannelHAL_tick(ADC12Channel* instance) {
  if (*instance->currentValue != instance->previousValue) {
    if (*instance->currentValue <= instance->disconnectedValue) {
      instance->errorStatus.bits.notConnected = 1;
    }
    else if (*instance->currentValue >= instance->shortCircuitValue) {
      instance->errorStatus.bits.isShortCircuited = 1;
    }
  }
}