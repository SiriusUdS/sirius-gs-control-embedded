#pragma once

#include "../../../sirius-headers-common/LowLevelDriver/USB/USBErrorStatus.h"
#include "../../../sirius-headers-common/LowLevelDriver/USB/USBStatus.h"

struct USB;

typedef void (*USB_init)(struct USB* instance);

typedef void (*USB_transmit)(struct USB* instance, uint8_t* data, uint16_t size);

typedef struct {
  USB_init     init;
  USB_transmit transmit;

  uint8_t* rxBuffer;
  uint16_t rxBufferSize_bytes;
  
  USBErrorStatus errorStatus;
  USBStatus      status;
}
USB;

extern void USB_initDefault(USB* instance);

extern void USB_transmitDefault(USB* instance, uint8_t* data, uint16_t size);