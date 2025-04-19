#pragma once

#include "../../../sirius-headers-common/Accelerometer/AccelerometerData.h"

struct Accelerometer;

typedef void (*Accelerometer_init)(struct Accelerometer* instance);

typedef void (*Accelerometer_gatherData)(struct Accelerometer* instance);

typedef struct {
  Accelerometer_init        init;
  Accelerometer_gatherData  gatherData;

  AccelerometerAxis         data;
  AccelerometerStatus       status;
}
Accelerometer;

extern void Accelerometer_initDefault(Accelerometer* instance);

extern void Accelerometer_gatherDataDefault(Accelerometer* instance);