#include "../../../Inc/LowLevelDriver/USB/USBHAL.h"

#include "usbd_cdc_if.h"

void USBHAL_init(USB* instance) {
  instance->transmit = (USB_transmit)USBHAL_transmit;
  instance->status.value = 0;
  instance->errorStatus.value = 0;
}

void USBHAL_transmit(USB* instance, uint8_t* data, uint16_t size) {
  CDC_Transmit_FS(data, size);
}