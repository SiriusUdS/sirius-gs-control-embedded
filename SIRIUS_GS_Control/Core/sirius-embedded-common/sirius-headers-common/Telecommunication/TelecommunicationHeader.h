#pragma once

#include <stdint.h>

#define DATA_HEADER_CODE 0xB256BE00 // If performance permits (rnd)

#define ACCELEROMETER_DATA_HEADER_CODE      0x41434300 // "ACC"
#define GYROSCOPE_DATA_HEADER_CODE          0x47595200 // "GYR"
#define ALTIMETER_DATA_HEADER_CODE          0x414C5400 // "ALT"
#define GPS_DATA_HEADER_CODE                0x47505300 // "GPS"
#define MAGNETOMETER_DATA_HEADER_CODE       0x4D414700 // "MAG"
#define PRESSURE_SENSOR_DATA_HEADER_CODE    0x50525300 // "PRS"
#define ROCKET_DATA_HEADER_CODE             0x524B5400 // "RKT"
#define TEMPERATURE_SENSOR_DATA_HEADER_CODE 0x54484D00 // "THM"
#define VALVE_DATA_HEADER_CODE              0x564C5600 // "VLV"

#define HEADER_SIZE_4BYTE 1
#define HEADER_SIZE_BYTE 4 * HEADER_SIZE_4BYTE

typedef struct {
  uint32_t values[HEADER_SIZE_4BYTE];
}
TelecommunicationHeader;