#pragma once

#include "GyroscopeData.h"
#include "GyroscopeStatus.h"
#include "GyroscopeErrorStatus.h"

#include "../Telecommunication/TelecommunicationHeader.h"

typedef struct {
  GyroscopeAxis         data;

  GyroscopeErrorStatus  errorStatus;
  GyroscopeStatus       status;

  uint32_t              timeStamp_ms;
}
GyroscopeDataMembers;

typedef union {
  GyroscopeDataMembers members;
  
  uint8_t values[sizeof(GyroscopeDataMembers)];
}
GyroscopeData;

typedef struct {
  TelecommunicationHeader header;
  GyroscopeData           rawData;
}
GyroscopePacketFields;

typedef union {
  GyroscopePacketFields fields;

  uint8_t data[sizeof(GyroscopePacketFields)];
}
GyroscopePacket;