#include "../../../Inc/Sensor/Accelerometer/Accelerometer.h"

void Accelerometer_initDefault(Accelerometer* instance) {
  instance->status.value = 0;
  instance->status.bits.invalidFunctionPointer = 1;

  instance->gatherData = Accelerometer_gatherDataDefault;
}

void Accelerometer_gatherDataDefault(Accelerometer* instance, AccelerometerData* rawData) {
  instance->status.value = 0;
  instance->status.bits.defaultFunctionCalled = 1;

  rawData->rawX = 0;
  rawData->rawY = 0;
  rawData->rawZ = 0;
}