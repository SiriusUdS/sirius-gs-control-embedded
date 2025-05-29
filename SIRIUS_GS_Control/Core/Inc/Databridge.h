#pragma once

#include <stdint.h>
#include "../sirius-embedded-common/Inc/LowLevelDriver/UART/UART.h"
#include "../sirius-embedded-common/Inc/LowLevelDriver/USB/USB.h"
#include "../sirius-embedded-common/sirius-headers-common/Telecommunication/BoardCommand.h"
#include "../sirius-embedded-common/sirius-headers-common/DataBridge/DataBridgeStatus.h"
#include "../sirius-embedded-common/sirius-headers-common/DataBridge/DataBridgeErrorStatus.h"
#include "../sirius-embedded-common/Inc/Device/Telecommunication/Telecommunication.h"


struct DataBridge;

typedef void (*DataBridge_init)(struct DataBridge* instance, volatile USB* usb,Telecommunication* telecommunication);
typedef void (*DataBridge_transmitCommand)(struct DataBridge* instance);
typedef void (*DataBridge_transmitResponse)(struct DataBridge* instance);
typedef void (*DataBridge_tick)(struct DataBridge* instance, uint32_t timestamp_ms);

typedef struct DataBridge {
  DataBridge_init             init;
  DataBridge_transmitCommand  transmitCommand;
  DataBridge_transmitResponse transmitResponse;
  DataBridge_tick             tick;

  volatile USB* usb;
  Telecommunication* telecommunication;

  BoardCommand currentCommand;
  uint8_t canSendCommand;
  uint8_t isCommandExecuted;

  DataBridgeStatus status;
  DataBridgeErrorStatus errorStatus;
} DataBridge;

void DataBridge_initDefault(DataBridge* instance);

void DataBridge_transmitCommandDefault(DataBridge* instance);

void DataBridge_transmitResponseDefault(DataBridge* instance, uint32_t previous);

void DataBridge_tickDefault(DataBridge* instance);
