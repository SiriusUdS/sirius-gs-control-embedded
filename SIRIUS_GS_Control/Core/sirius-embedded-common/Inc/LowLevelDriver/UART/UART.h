#pragma once

#include "../../../sirius-headers-common/LowLevelDriver/UART/UARTErrorStatus.h"
#include "../../../sirius-headers-common/LowLevelDriver/UART/UARTStatus.h"

#define UART_COMMUNICATION_TIMEOUT_MS 50

struct UART;

typedef void (*UART_init)(struct UART* instance);

typedef void (*UART_transmit)(struct UART* instance, uint8_t* data, uint16_t size);

typedef void (*UART_receive)(struct UART* instance, uint8_t* data, uint16_t size);

typedef struct {
  UART_init     init;
  UART_transmit transmit;
  UART_receive  receive;

  void* externalHandle;
  
  UARTErrorStatus  errorStatus;
  UARTStatus       status;
}
UART;

extern void UART_initDefault(UART* instance);

extern void UART_transmitDefault(UART* instance, uint8_t* data, uint16_t size);

extern void UART_receiveDefault(UART* instance, uint8_t* data, uint16_t size);