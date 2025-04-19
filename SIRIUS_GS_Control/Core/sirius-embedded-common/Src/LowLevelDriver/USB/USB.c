#include "../../../Inc/LowLevelDriver/USB/USB.h"

void USB_initDefault(USB* instance) {
  instance->transmit = (USB_transmit)USB_transmitDefault;
  instance->status.value = 0;
  instance->errorStatus.value = 0;
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void USB_transmitDefault(USB* instance, uint8_t* data, uint16_t size) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}