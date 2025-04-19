#include "../../../Inc/Device/Storage/Storage.h"

void Storage_initDefault(Storage* instance) {
  instance->status.value = 0;
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void Storage_store4kbDataDefault(Storage* instance, uint8_t* data) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void Storage_fetch4kbDataDefault(Storage* instance, uint8_t* data) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}