#include "../../../Inc/LowLevelDriver/UART/UARTHAL.h"

#include "stm32f4xx_hal.h"

void UARTHAL_init(UART* instance) {
  instance->transmit = (UART_transmit)UARTHAL_transmit;
  instance->receive = (UART_receive)UARTHAL_receive;
  instance->status.value = 0;
  instance->errorStatus.value = 0;
}

void UARTHAL_transmit(UART* instance, uint8_t* data, uint16_t size) {
  HAL_UART_Transmit((UART_HandleTypeDef*)instance->externalHandle, data, size, UART_COMMUNICATION_TIMEOUT_MS);
}

void UARTHAL_receive(UART* instance, uint8_t* data, uint16_t size) {
  HAL_UART_Receive((UART_HandleTypeDef*)instance->externalHandle, data, size, UART_COMMUNICATION_TIMEOUT_MS);
}