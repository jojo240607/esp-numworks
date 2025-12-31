/*
 * SPDX-FileCopyrightText: 2021-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <sys/lock.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_spiffs.h"
#include "driver/gpio.h"

#include <ion.h>
#include <ion/exam_mode.h>
#include <ion/src/shared/init.h>
#include <shared/boot/rt0.h>
#include <shared/drivers/usb.h>

static const char *TAG = "example";

extern "C" void app_main(void)
{
    //Ion::Device::Init::configureRAM();
    Ion::Init();
    // Initialize slotInfo to be accessible to Kernel
    Ion::Device::USB::slotInfo();
    Ion::ExternalApps::deleteApps(Ion::ExamMode::get().isActive());
    ion_main(0, nullptr);
    abort();


    //
    /*Exprition
     * Ion::Display::drawString
     * */
}
