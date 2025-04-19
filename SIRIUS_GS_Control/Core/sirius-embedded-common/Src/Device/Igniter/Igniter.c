#include "../../../Inc/Device/Igniter/Igniter.h"

void Igniter_initDefault(Igniter* instance) {
  instance->ignite = (Igniter_ignite)Igniter_igniteDefault;
  instance->errorStatus.value = 0;
  instance->status.value = 0;

  instance->errorStatus.bits.defaultFunctionCalled = 1;
}

void Igniter_igniteDefault(Igniter* instance) {
  instance->errorStatus.bits.defaultFunctionCalled = 1;
}