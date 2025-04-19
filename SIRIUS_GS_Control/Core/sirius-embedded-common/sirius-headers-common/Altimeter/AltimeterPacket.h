#pragma once

#include "AltimeterData.h"
#include "AltimeterStatus.h"
#include "AltimeterErrorStatus.h"

#include "../Telecommunication/TelecommunicationHeader.h"

typedef struct {
  AltimeterData         data;

  AltimeterErrorStatus  errorStatus;
  AltimeterStatus       status;
  uint32_t              timeStamp_ms;
}
AltimeterPacketDataMembers;


typedef union {
  AltimeterPacketDataMembers members;
  
  uint8_t values[sizeof(AltimeterPacketDataMembers)];
}
AltimeterPacketData;

typedef struct {
  TelecommunicationHeader header;
  AltimeterPacketData     rawData;
}
AltimeterPacketFields;

typedef union {
  AltimeterPacketFields fields;

  uint8_t data[sizeof(AltimeterPacketFields)];
}
AltimeterPacket;