#pragma once

#include "ValveStatus.h"
#include "ValveErrorStatus.h"

#include "../Telecommunication/TelecommunicationHeader.h"

typedef struct {
  ValveErrorStatus errorStatus;
  ValveStatus      status;

  uint32_t timeStamp_ms;
}
ValvePacketDataMembers;

typedef union {
  ValvePacketDataMembers members;
  
  uint8_t values[sizeof(ValvePacketDataMembers)];
}
ValvePacketData;

typedef struct {
  TelecommunicationHeader header;
  ValvePacketData         rawData;
}
ValvePacketFields;

typedef union {
  ValvePacketFields fields;

  uint8_t data[sizeof(ValvePacketFields)];
}
ValvePacket;