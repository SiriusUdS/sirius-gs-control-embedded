#include "../../../Inc/LowLevelDriver/UART/UART.h"

void UART_initDefault(UART* instance) {
  instance->transmit = (UART_transmit)UART_transmitDefault;
  instance->receive = (UART_receive)UART_receiveDefault;
  instance->status.value = 0;
  instance->errorStatus.value = 0;
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void UART_transmitDefault(UART* instance, uint8_t* data, uint16_t size) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void UART_receiveDefault(UART* instance, uint8_t* data, uint16_t size) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}