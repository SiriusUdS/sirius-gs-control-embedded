#pragma once

#include "../sirius-embedded-common/Inc/Device/Storage/SDCard.h"

#include "../sirius-embedded-common/Inc/LowLevelDriver/GPIO/GPIOHAL.h"
#include "../sirius-embedded-common/Inc/LowLevelDriver/UART/UARTHAL.h"
#include "../sirius-embedded-common/Inc/LowLevelDriver/USB/USBHAL.h"

#include "../sirius-embedded-common/sirius-headers-common/GSControl/GSControlStatus.h"
#include "../sirius-embedded-common/sirius-headers-common/GSControl/GSControlErrorStatus.h"
#include "../sirius-embedded-common/sirius-headers-common/GSControl/GSControlSensors.h"
#include "../sirius-embedded-common/sirius-headers-common/GSControl/GSControlState.h"

#include "../sirius-embedded-common/Inc/Device/Telecommunication/Telecommunication.h"

#include "../sirius-embedded-common/Inc/Device/Telecommunication/xbee.h"

#include "Databridge.h"

#include "stm32f4xx_hal.h"

#include "../sirius-embedded-common/Inc/Device/Button/ButtonActiveLow.h"
#include "../sirius-embedded-common/Inc/Device/Button/ButtonActiveHigh.h"

#define FUNCTION_NULL_POINTER 0

typedef struct {
  GSControlErrorStatus errorStatus;
  GSControlStatus      status;

  uint8_t currentState;

  GPIO*  gpios;
  UART*  uart;
  USB*   usb;
  DataBridge* DataBridge;
  Telecommunication* telecom;
  Button* buttons;
}
GSControl;

extern void GSControl_init(GPIO* gpios, UART* uart, USB* usb, Telecommunication* telecom, Button* buttons, DataBridge* databridge);

extern void GSControl_tick(uint32_t timestamp_ms);

extern void GSControl_execute(uint32_t timestamp_ms);