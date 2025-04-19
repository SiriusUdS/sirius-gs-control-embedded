#pragma once

#include "RocketStatus.h"
#include "RocketErrorStatus.h"

#include "../Telecommunication/TelecommunicationHeader.h"

#define STORAGE_DEVICE_AMOUNT 2

typedef union {
  struct {
    RocketErrorStatus  errorStatus;
    RocketStatus       status;

    uint32_t            timeStamp_ms;
  }
  members;
  
  uint8_t values[sizeof(members)];
}
RocketPacketData;

typedef union {
  struct {
    TelecommunicationHeader   header;
    RocketPacketData         rawData[STORAGE_DEVICE_AMOUNT];
  }
  packet;

  uint8_t data[sizeof(packet)];
}
RocketPacket;