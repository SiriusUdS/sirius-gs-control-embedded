#pragma once

#include "TemperatureSensorData.h"
#include "TemperatureSensorStatus.h"
#include "TemperatureSensorErrorStatus.h"

#include "../Telecommunication/TelecommunicationHeader.h"

typedef struct {
  TemperatureSensorData data;

  TemperatureSensorErrorStatus errorStatus;
  TemperatureSensorStatus      status;

  uint32_t timeStamp_ms;
}
TemperatureSensorPacketDataMembers;

typedef union {
  TemperatureSensorPacketDataMembers members;
  
  uint8_t values[sizeof(TemperatureSensorPacketDataMembers)];
}
TemperatureSensorPacketData;

typedef struct {
  TelecommunicationHeader header;
  TemperatureSensorPacketData  rawData;
}
TemperatureSensorPacketFields;

typedef union {
  TemperatureSensorPacketFields fields;

  uint8_t data[sizeof(TemperatureSensorPacketFields)];
}
TemperatureSensorPacket;