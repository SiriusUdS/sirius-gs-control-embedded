#pragma once

#include "Storage.h"

extern void SDCard_init(Storage* instance);

extern void SDCard_store4kbData(Storage* instance, uint8_t* data);

extern void SDCard_fetch4kbData(Storage* instance, uint8_t* data);