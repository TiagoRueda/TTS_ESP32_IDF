#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <ctype.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include <sys/param.h>
#include "esp_system.h"
#include <esp_log.h>
#include "spi_flash_mmap.h"
#include <nvs_flash.h>
#include "driver/i2s.h"
#include "flite.h"

static const char *TAG = "TTS";

cst_voice *register_cmu_us_kal(const char *voxdir);

static QueueHandle_t text_queue;

#endif
