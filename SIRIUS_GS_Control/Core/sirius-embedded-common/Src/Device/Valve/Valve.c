#include "../../../Inc/Device/Valve/Valve.h"

void Valve_initDefault(Valve* instance) {
  instance->setIdle = (Valve_setIdle)Valve_setIdleDefault;
  instance->close = (Valve_close)Valve_closeDefault;
  instance->open = (Valve_close)Valve_openDefault;
  instance->tick = (Valve_tick)Valve_tickDefault;
  
  instance->status.value = 0;
  instance->errorStatus.value = 0;
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void Valve_setIdleDefault(Valve* instance) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void Valve_closeDefault(Valve* instance, uint32_t timestamp_ms) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void Valve_openDefault(Valve* instance, uint32_t timestamp_ms) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void Valve_tickDefault(Valve* instance, uint32_t timestamp_ms) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}