#pragma once

#include "SPI.h"

extern void SPIHAL_init(SPI* instance);

extern void SPIHAL_transmit(SPI* instance, uint8_t slaveId, uint8_t* data, uint16_t size);

extern void SPIHAL_receive(SPI* instance, uint8_t slaveId, uint8_t* data, uint16_t size);