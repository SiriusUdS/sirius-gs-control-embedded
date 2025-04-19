#pragma once

#include "LoadCellData.h"
#include "LoadCellStatus.h"
#include "LoadCellErrorStatus.h"

#include "../Telecommunication/TelecommunicationHeader.h"

typedef struct {
  LoadCellData    data;

  LoadCellErrorStatus errorStatus;
  LoadCellStatus      status;

  uint32_t              timeStamp_ms;
}
LoadCellPacketDataMembers;

typedef union {
  LoadCellPacketDataMembers members;
  
  uint8_t values[sizeof(LoadCellPacketDataMembers)];
}
LoadCellPacketData;

typedef struct {
  TelecommunicationHeader header;
  LoadCellPacketData      rawData;
}
LoadCellPacketFields;

typedef union {
  LoadCellPacketFields fields;

  uint8_t data[sizeof(LoadCellPacketFields)];
}
LoadCellPacket;