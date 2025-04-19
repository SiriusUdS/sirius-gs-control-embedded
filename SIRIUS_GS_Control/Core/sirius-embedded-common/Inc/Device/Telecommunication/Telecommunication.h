#pragma once

#include <stdint.h>

#include "../../../sirius-headers-common/Telecommunication/TelecommunicationStatus.h"
#include "../../../sirius-headers-common/Telecommunication/TelecommunicationErrorStatus.h"

#include "../../LowLevelDriver/UART/UART.h"
#include "../../LowLevelDriver/UART/UARTHAL.h"
#include "stm32f4xx_hal.h"

struct Telecommunication;
/*typedef union {

  struct{
    uint8_t DH[2];
    uint8_t DL[2];
    uint8_t networkID[2];
  } frame;

  uint8_t data[6];
}TelecommunicationSetup;*/


typedef void (*Telecommunication_init)(struct Telecommunication* instance);

typedef void (*Telecommunication_sendData)(struct Telecommunication* instance, uint8_t* data, uint16_t size);

typedef void (*Telecommunication_receiveData)(struct Telecommunication* instance, uint8_t* data, uint16_t size);

typedef void (*Telecommunication_setup)(struct Telecommunication* instance);

typedef struct {
  Telecommunication_init        init;
  Telecommunication_sendData    sendData;
  Telecommunication_receiveData receiveData;
  Telecommunication_setup       setupTelecom;
  UART* uart;
  TelecommunicationErrorStatus  errorStatus;
  TelecommunicationStatus       status;
}
Telecommunication;


extern void TELECOM_init(Telecommunication* instance);

extern void TELECOM_sendData(Telecommunication* instance, uint8_t* data, uint16_t size);

extern void TELECOM_receiveData(Telecommunication* instance, uint8_t* data, uint16_t size);

extern void TELECOM_setup(Telecommunication* instance);