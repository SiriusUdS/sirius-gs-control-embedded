#include "../../../Inc/LowLevelDriver/ADC/ADC12.h"

void ADC12_initDefault(ADC12* instance, uint8_t activeChannelsAmt) {
  instance->tick = (ADC12_tick)ADC12_tickDefault;
  instance->status.value = 0;
  instance->errorStatus.value = 0;
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void ADC12_tickDefault(ADC12* instance) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}