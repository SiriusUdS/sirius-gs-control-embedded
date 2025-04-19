#pragma once

#include "../../../sirius-headers-common/Storage/StorageStatus.h"
#include "../../../sirius-headers-common/Storage/StorageErrorStatus.h"

#include "../../../sirius-headers-common/Accelerometer/AccelerometerPacket.h"
#include "../../../sirius-headers-common/PressureSensor/PressureSensorPacket.h"

struct Storage;

typedef void (*Storage_init)(struct Storage* instance);

typedef void (*Storage_store4kbData)(struct Storage* instance, uint8_t* data);

typedef void (*Storage_fetch4kbData)(struct Storage* instance, uint8_t* data);

typedef struct {
  Storage_init         init;
  Storage_store4kbData storePage;
  Storage_fetch4kbData fetchData;

  void* externalInstance;

  StorageErrorStatus errorStatus;
  StorageStatus      status;
}
Storage;

extern void Storage_initDefault(Storage* instance);

extern void Storage_store4kbDataDefault(Storage* instance, uint8_t* data);

extern void Storage_fetch4kbDataDefault(Storage* instance, uint8_t* data);