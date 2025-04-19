#pragma once

#include "USB.h"

extern void USBHAL_init(USB* instance);

extern void USBHAL_transmit(USB* instance, uint8_t* data, uint16_t size);