#include "../../../Inc/LowLevelDriver/SPI/SPI.h"

void SPI_initDefault(SPI* instance) {
  instance->transmit = SPI_transmitDefault;
  instance->receive = SPI_receiveDefault;
  instance->status.value = 0;
  instance->errorStatus.value = 0;
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void SPI_transmitDefault(SPI* instance, uint8_t slaveId, uint8_t* data, uint16_t size) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void SPI_receiveDefault(SPI* instance, uint8_t slaveId, uint8_t* data, uint16_t size) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}