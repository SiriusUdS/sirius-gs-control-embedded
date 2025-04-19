#include "../../../Inc/LowLevelDriver/SPI/SPIHAL.h"

#include "stm32f4xx_hal.h"

static void raiseCS(SPI* instance, uint8_t slaveId);

static void lowerCS(SPI* instance, uint8_t slaveid);

void SPIHAL_init(SPI* instance) {
  instance->transmit = SPIHAL_transmit;
  instance->receive = SPIHAL_receive;
  instance->status.value = 0;
  instance->errorStatus.value = 0;
}

void SPIHAL_transmit(SPI* instance, uint8_t slaveId, uint8_t* data, uint16_t size) {
  lowerCS(instance, slaveId);
  HAL_SPI_Transmit(instance->externalHandle, data, size, SPI_COMMUNICATION_TIMEOUT_MS);
  raiseCS(instance, slaveId);
}

void SPIHAL_receive(SPI* instance, uint8_t slaveId, uint8_t* data, uint16_t size) {
  lowerCS(instance, slaveId);
  HAL_SPI_Receive(instance->externalHandle, data, size, SPI_COMMUNICATION_TIMEOUT_MS);
  raiseCS(instance, slaveId);
}

void raiseCS(SPI* instance, uint8_t slaveId) {
  instance->chipSelects[slaveId].write(instance->chipSelects[slaveId], GPIO_VALUE_HIGH);
}

void lowerCS(SPI* instance, uint8_t slaveId) {
  instance->chipSelects[slaveId].write(instance->chipSelects[slaveId], GPIO_VALUE_LOW);
}