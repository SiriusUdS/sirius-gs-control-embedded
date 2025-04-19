#pragma once

#include "UART.h"

extern void UARTHAL_init(UART* instance);

extern void UARTHAL_transmit(UART* instance, uint8_t* data, uint16_t size);

extern void UARTHAL_receive(UART* instance, uint8_t* data, uint16_t size);