#pragma once

#include "Storage.h"

extern void ExternalFlash_init(Storage* instance);

extern void ExternalFlash_store4kbData(Storage* instance, uint8_t* data);

extern void ExternalFlash_fetch4kbData(Storage* instance, uint8_t* data);