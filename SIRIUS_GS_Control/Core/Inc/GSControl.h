#pragma once

#include "../sirius-embedded-common/Inc/Sensor/PressureSensor/ETM375.h"
#include "../sirius-embedded-common/sirius-headers-common/PressureSensor/PressureSensorData.h"

#include "../sirius-embedded-common/Inc/Sensor/TemperatureSensor/NTC3950.h"
#include "../sirius-embedded-common/sirius-headers-common/TemperatureSensor/TemperatureSensorPacket.h"

#include "../sirius-embedded-common/Inc/Device/Valve/HBL388.h"

#include "../sirius-embedded-common/Inc/Device/Storage/SDCard.h"

#include "../sirius-embedded-common/Inc/LowLevelDriver/GPIO/GPIOHAL.h"
#include "../sirius-embedded-common/Inc/LowLevelDriver/PWM/PWMHAL.h"
#include "../sirius-embedded-common/Inc/LowLevelDriver/ADC/ADC12HAL.h"
#include "../sirius-embedded-common/Inc/LowLevelDriver/UART/UARTHAL.h"
#include "../sirius-embedded-common/Inc/LowLevelDriver/USB/USBHAL.h"

#include "../sirius-embedded-common/sirius-headers-common/GSControl/GSControlStatus.h"
#include "../sirius-embedded-common/sirius-headers-common/GSControl/GSControlErrorStatus.h"
#include "../sirius-embedded-common/sirius-headers-common/GSControl/GSControlSensors.h"
#include "../sirius-embedded-common/sirius-headers-common/GSControl/GSControlState.h"

#include "../sirius-embedded-common/Inc/Device/Telecommunication/Telecommunication.h"

#include "stm32f4xx_hal.h"

#define FUNCTION_NULL_POINTER 0

typedef struct {
  GSControlErrorStatus errorStatus;
  GSControlStatus      status;

  uint8_t currentState;

  ADC12* adc;
  PWM*   pwms;
  GPIO*  gpios;
  UART*  uart;
  USB*   usb;

  Valve*             valves;
  TemperatureSensor* temperatureSensors;
  PressureSensor*    pressureSensors;
  Telecommunication* telecom;
}
GSControl;

extern void GSControl_init(PWM* pwms, ADC12* adc, GPIO* gpios, UART* uart, USB* usb, Valve* valves, TemperatureSensor* temperatureSensors, Telecommunication* telecom);

extern void GSControl_tick(uint32_t timestamp_ms);

extern void GSControl_execute(uint32_t timestamp_ms);