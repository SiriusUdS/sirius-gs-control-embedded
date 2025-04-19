#pragma once

#include "ADC12.h"

extern void ADC12HAL_init(ADC12* instance, uint8_t activeChannelsAmt);

extern void ADC12HAL_tick(ADC12* instance);