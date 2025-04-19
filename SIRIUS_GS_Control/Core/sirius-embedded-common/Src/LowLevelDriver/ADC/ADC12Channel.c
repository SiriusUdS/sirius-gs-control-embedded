#include "../../../Inc/LowLevelDriver/ADC/ADC12Channel.h"

void ADC12Channel_initDefault(ADC12Channel* instance) {
  instance->tick = (ADC12Channel_tick)ADC12Channel_tickDefault;
  instance->status.value = 0;
  instance->errorStatus.value = 0;
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void ADC12Channel_tickDefault(ADC12Channel* instance) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}