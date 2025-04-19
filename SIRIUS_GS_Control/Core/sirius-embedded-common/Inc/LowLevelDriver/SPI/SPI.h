#pragma once

#include "../../../sirius-headers-common/LowLevelDriver/SPI/SPIErrorStatus.h"
#include "../../../sirius-headers-common/LowLevelDriver/SPI/SPIStatus.h"

#include "../GPIO/GPIO.h"

#define SPI_COMMUNICATION_TIMEOUT_MS 50

struct SPI;

typedef void (*SPI_init)(struct SPI* instance);

typedef void (*SPI_transmit)(struct SPI* instance, uint8_t slaveId,uint8_t* data, uint16_t size);

typedef void (*SPI_receive)(struct SPI* instance, uint8_t slaveId, uint8_t* data, uint16_t size);

typedef struct {
  SPI_init     init;
  SPI_transmit transmit;
  SPI_receive  receive;

  uint8_t slaveAmt;
  GPIO** chipSelects;

  void* externalHandle;
  
  SPIErrorStatus  errorStatus;
  SPIStatus       status;
}
SPI;

extern void SPI_initDefault(SPI* instance);

extern void SPI_transmitDefault(SPI* instance, uint8_t slaveId, uint8_t* data, uint16_t size);

extern void SPI_receiveDefault(SPI* instance, uint8_t slaveId, uint8_t* data, uint16_t size);