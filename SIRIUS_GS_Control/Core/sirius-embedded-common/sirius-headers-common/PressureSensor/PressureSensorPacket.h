#pragma once

#include "PressureSensorData.h"
#include "PressureSensorStatus.h"
#include "PressureSensorErrorStatus.h"

#include "../Telecommunication/TelecommunicationHeader.h"

typedef struct {
  PressureSensorData data;

  PressureSensorStatus      status;
  PressureSensorErrorStatus errorStatus;

  uint32_t timeStamp_ms;
}
PressureSensorDataMembers;

typedef union {
  PressureSensorDataMembers members;
  
  uint8_t values[sizeof(PressureSensorDataMembers)];
}
PressureSensorPacketData;

typedef struct {
  TelecommunicationHeader   header;
  PressureSensorPacketData  rawData;
}
PressureSensorPacketFields;

typedef union {
  PressureSensorPacketFields fields;

  uint8_t data[sizeof(PressureSensorPacketFields)];
}
PressureSensorPacket;