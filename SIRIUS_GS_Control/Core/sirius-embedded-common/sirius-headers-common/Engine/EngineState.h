#pragma once

#include <stdint.h>

#define ENGINE_STATE_INIT             (uint8_t)0x00
#define ENGINE_STATE_IDLE             (uint8_t)0x01
#define ENGINE_STATE_ABORT            (uint8_t)0x02
#define ENGINE_STATE_ARMING           (uint8_t)0x03
#define ENGINE_STATE_ARMED            (uint8_t)0x04
#define ENGINE_STATE_IGNITION         (uint8_t)0x05
#define ENGINE_STATE_POWERED_FLIGHT   (uint8_t)0x06
#define ENGINE_STATE_UNPOWERED_FLIGHT (uint8_t)0x07