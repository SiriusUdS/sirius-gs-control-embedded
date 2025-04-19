#include "../../../Inc/Device/Storage/SDCard.h"
#include "fatfs.h"
#include <string.h>
#include <stdint.h>

#define BYTE_SIZE 4096/8

static FRESULT SDCard_mount(Storage* instance,TCHAR *path) {
  FATFS* fatfs = (FATFS*)instance->externalInstance;
  FRESULT fr_status;
  fr_status = f_mount(fatfs, path, 1);
  if (fr_status != FR_OK) {
    instance->errorStatus.bits.notInitialized = 1;
  }

  return fr_status;
}

static FRESULT SDCard_unmount(Storage* instance) {
  FRESULT fr_status;
  fr_status = f_mount(NULL, "", 0);
  if (fr_status == FR_OK) {
    instance->errorStatus.bits.notInitialized = 1;
  }
  return fr_status;
}

static FRESULT SDCard_size_free_space(Storage * instance, uint16_t * total_sectors, uint16_t * free_sectors) {
  FATFS* fatfs = (FATFS*)instance->externalInstance;
  FATFS** pfatfs = &fatfs;
  FRESULT fr_status;
  DWORD free_clusters;
  fr_status = f_getfree("", &free_clusters, pfatfs);
  if (fr_status == FR_OK) {
    *total_sectors = (fatfs->n_fatent - 2) * fatfs->csize;
    *free_sectors = free_clusters * fatfs->csize;
  }
  return fr_status;
}

void SDCard_init(Storage* instance) {
  instance->status.value = 0;
  instance->errorStatus.value = 0;
}

void SDCard_store4kbData(Storage* instance, uint8_t* data) {
  FRESULT fr_status = SDCard_mount(instance, "");
  if (fr_status != FR_OK) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
  uint16_t total_sectors = 0;
  uint16_t free_sectors = 0;
  fr_status = SDCard_size_free_space(instance, &total_sectors, &free_sectors);
  if (fr_status != FR_OK) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
  if (free_sectors < 4) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
  FIL file;
  fr_status = f_open(&file, "data.txt", FA_OPEN_APPEND | FA_WRITE);
  if (fr_status != FR_OK) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
  UINT bytes_written;
  fr_status = f_write(&file, data, BYTE_SIZE, &bytes_written);
  if (fr_status != FR_OK) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
  if (bytes_written != BYTE_SIZE) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
  fr_status = f_close(&file);
  if (fr_status != FR_OK) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
  fr_status = SDCard_unmount(instance);
  if (fr_status != FR_OK) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
}

void SDCard_fetch4kbData(Storage* instance, uint8_t* data) {

  FRESULT fr_status = SDCard_mount(instance, "");
  if (fr_status != FR_OK) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
  FIL file;
  fr_status = f_open(&file, "data.txt", FA_READ);
  if (fr_status != FR_OK) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
  UINT bytes_read;
  fr_status = f_read(&file, data, BYTE_SIZE, &bytes_read);
  if (fr_status != FR_OK) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
  if (bytes_read != BYTE_SIZE) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
  fr_status = f_close(&file);
  if (fr_status != FR_OK) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
  fr_status = SDCard_unmount(instance);
  if (fr_status != FR_OK) {
    instance->errorStatus.bits.notInitialized = 1;
    return;
  }
}