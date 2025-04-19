#pragma once

#include "IgniterStatus.h"
#include "IgniterErrorStatus.h"

#include "../Telecommunication/TelecommunicationHeader.h"

typedef struct {
  IgniterErrorStatus errorStatus;
  IgniterStatus      status;

  uint32_t              timeStamp_ms;
}
IgniterPacketDataMembers;

typedef union {
  IgniterPacketDataMembers members;
  
  uint8_t values[sizeof(IgniterPacketDataMembers)];
}
IgniterPacketData;

typedef struct {
  TelecommunicationHeader header;
  IgniterPacketData      rawData;
}
IgniterPacketFields;

typedef union {
  IgniterPacketFields fields;

  uint8_t data[sizeof(IgniterPacketFields)];
}
IgniterPacket;