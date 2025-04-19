#pragma once

#include "../../LowLevelDriver/ADC/ADC12ChannelHAL.h"

#include "../../../sirius-headers-common/LoadCell/LoadCellData.h"
#include "../../../sirius-headers-common/LoadCell/LoadCellStatus.h"
#include "../../../sirius-headers-common/LoadCell/LoadCellErrorStatus.h"

struct LoadCell;

typedef void (*LoadCell_init)(struct LoadCell* instance);

typedef void (*LoadCell_tick)(struct LoadCell* instance);

typedef struct {
  LoadCell_init init;
  LoadCell_tick readData;

  ADC12Channel* adcChannel;

  LoadCellData currentPressure_adc;

  LoadCellErrorStatus errorStatus;
  LoadCellStatus      status;
}
LoadCell;

extern void LoadCell_initDefault(LoadCell* instance);

extern void LoadCell_tickDefault(LoadCell* instance);