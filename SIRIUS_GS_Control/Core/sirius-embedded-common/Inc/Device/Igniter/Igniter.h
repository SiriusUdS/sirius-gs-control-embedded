#pragma once

#include "../../../sirius-headers-common/Igniter/IgniterErrorStatus.h"
#include "../../../sirius-headers-common/Igniter/IgniterStatus.h"

#include "../../LowLevelDriver/GPIO/GPIO.h"

typedef void (*Igniter_init)(struct Igniter* instance);

typedef void (*Igniter_ignite)(struct Igniter* instance);

typedef struct {
  Igniter_init    init;
  Igniter_ignite  ignite;

  GPIO* gpio;

  IgniterErrorStatus errorStatus;
  IgniterStatus      status;
}
Igniter;

extern void Igniter_initDefault(Igniter* instance);

extern void Igniter_igniteDefault(Igniter* instance);