#pragma once

#include "AccelerometerData.h"
#include "AccelerometerStatus.h"
#include "AccelerometerErrorStatus.h"

#include "../Telecommunication/TelecommunicationHeader.h"

typedef struct {
  AccelerometerData         data;

  AccelerometerErrorStatus  errorStatus;
  AccelerometerStatus       status;
  uint32_t                  timeStamp_ms;
}
AccelerometerDataMembers;

typedef union {
  AccelerometerDataMembers members;
  
  uint8_t values[sizeof(AccelerometerDataMembers)];
}
AccelerometerPacketData;

typedef struct {
  TelecommunicationHeader header;
  AccelerometerPacketData rawData;
}
AccelerometerPacketFields;

typedef union {
  AccelerometerPacketFields fields;

  uint8_t data[sizeof(AccelerometerPacketFields)];
}
AccelerometerPacket;