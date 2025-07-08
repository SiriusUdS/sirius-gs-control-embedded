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
#include "../sirius-embedded-common/sirius-headers-common/Telecommunication/BoardCommand.h"
#include "../sirius-embedded-common/sirius-headers-common/Telecommunication/CommandResponse.h"
#include "../sirius-embedded-common/sirius-headers-common/Telecommunication/GSCommand.h"
#include "../sirius-embedded-common/sirius-headers-common/Telecommunication/TelemetryPacket.h"
#include "../sirius-embedded-common/sirius-headers-common/Telecommunication/PacketHeaderVariable.h"

#include "../sirius-embedded-common/Inc/Device/Telecommunication/xbee.h"

#include "stm32f4xx_hal.h"

#include "../sirius-embedded-common/Inc/Device/Button/ButtonActiveLow.h"
#include "../sirius-embedded-common/Inc/Device/Button/ButtonActiveHigh.h"

#define FUNCTION_NULL_POINTER 0

#define UART_BUFFER_SIZE (uint16_t)0x40

#define GS_CONTROL_DELAY_BETWEEN_STATUS_PACKETS_MS 50

typedef struct {
  uint8_t currentState;

  GPIO*         gpios;
  UART*         uart;
  volatile USB* usb;

  Telecommunication* telecommunication;
  Button* buttons;

  uint32_t communicationTimestampTarget_ms;
  uint8_t packetsReadyToSend;

  GSControlErrorStatus errorStatus;
  GSControlStatus      status;
}
GSControl;

void GSControl_init(GPIO* gpios, UART* uart, volatile USB* usb, Telecommunication* telecom, Button* buttons);

void GSControl_tick(uint32_t timestamp_ms);

void GSControl_execute(uint32_t timestamp_ms);