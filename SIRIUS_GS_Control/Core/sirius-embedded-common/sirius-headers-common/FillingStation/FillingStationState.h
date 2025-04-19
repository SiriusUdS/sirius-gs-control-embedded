#pragma once

#include <stdint.h>

#define FILLING_STATION_STATE_INIT    (uint8_t)0x00
#define FILLING_STATION_STATE_IDLE    (uint8_t)0x01
#define FILLING_STATION_STATE_ABORT   (uint8_t)0x02
#define FILLING_STATION_STATE_ARMING  (uint8_t)0x03
#define FILLING_STATION_STATE_ARMED   (uint8_t)0x04
#define FILLING_STATION_STATE_FILLING (uint8_t)0x05
#define FILLING_STATION_STATE_FILLED  (uint8_t)0x06