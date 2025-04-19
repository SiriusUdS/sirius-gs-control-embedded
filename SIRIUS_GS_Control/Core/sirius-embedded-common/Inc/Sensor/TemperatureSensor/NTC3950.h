#pragma once

#include "TemperatureSensor.h"

extern void NTC3950_init(TemperatureSensor* instance);

extern void NTC3950_tick(TemperatureSensor* instance);