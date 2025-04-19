#include "../../../Inc/Device/Storage/ExternalFlash.h"

void ExternalFlash_init(Storage* instance) {
  instance->status.value = 0;
  instance->errorStatus.value = 0;
}

void ExternalFlash_store4kbData(Storage* instance, uint8_t* data) {
  
}

void ExternalFlash_fetch4kbData(Storage* instance, uint8_t* data) {
  
}