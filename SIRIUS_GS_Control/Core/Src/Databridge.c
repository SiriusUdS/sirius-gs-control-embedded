#include "../Inc/DataBridge.h"

void DataBridge_initDefault(DataBridge* instance) {
  instance->transmitCommand = (DataBridge_transmitCommand)DataBridge_transmitCommandDefault;
  instance->transmitResponse = (DataBridge_transmitResponse)DataBridge_transmitResponseDefault;
  instance->tick = (DataBridge_tick)DataBridge_tickDefault;

  instance->status.value = 0;
  instance->errorStatus.value = 0;
}

void DataBridge_transmitCommandDefault(DataBridge* instance) {
  
}

void DataBridge_transmitResponseDefault(DataBridge* instance, uint32_t previous) {

}

void DataBridge_tickDefault(DataBridge* instance) {

}

void DATABRIDGE_sendUSBtoUART(DataBridge* instance, uint8_t* data, uint16_t size) {
    if (instance->usb->status.bits.rxDataReady == 1) {
        instance->usb->status.bits.rxDataReady = 0;

        //instance->uart->transmit((struct UART*)instance->uart, data, size);
    }
}

void DATABRIDGE_sendUARTtoUSB(DataBridge* instance, uint8_t* data, uint32_t previous) {
    if (HAL_GetTick() - previous >= 100) {
        previous = HAL_GetTick();

        instance->usb->transmit((struct USB*)instance->usb, data, sizeof(data));
    }
}



void DATABRIDGE_receiveUSB(DataBridge* instance) {
    if (instance->usb->status.bits.rxDataReady == 1) {
        instance->usb->status.bits.rxDataReady = 0;
        //memcpy(&instance->lastCommand, instance->usb->rxBuffer, sizeof(BoardCommand));
    }
}