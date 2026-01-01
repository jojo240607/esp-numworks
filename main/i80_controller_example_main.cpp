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
void my_task(void *pvParameters) {
    while (1) {
        // 你的主循环逻辑
        printf("Task running...\n");
        vTaskDelay(pdMS_TO_TICKS(1000));  // 每秒执行一次
    }
}
extern "C" void app_main(void)
{
    // 立即记录状态
    ESP_LOGE("CRASH", "=== 崩溃调试信息 ===");
    ESP_LOGE("CRASH", "ESP-IDF版本: %s", esp_get_idf_version());
    ESP_LOGE("CRASH", "芯片型号: ESP32-S3");
    ESP_LOGE("CRASH", "Free Heap: %d bytes", esp_get_free_heap_size());
    ESP_LOGE("CRASH", "Min Free Heap: %d bytes", esp_get_minimum_free_heap_size());

    // 检查栈配置
    ESP_LOGE("CRASH", "Main task stack size: %d bytes",
             CONFIG_ESP_MAIN_TASK_STACK_SIZE);
    //Ion::Device::Init::configureRAM();
    Ion::Init();
    ESP_LOGI(TAG, "ion Init ok");
    // Initialize slotInfo to be accessible to Kernel
    Ion::Device::USB::slotInfo();
    ESP_LOGI(TAG, "slotInfo ok");
    Ion::ExternalApps::deleteApps(Ion::ExamMode::get().isActive());
    ESP_LOGI(TAG, "deleteApps ok");
    ion_main(0, nullptr);
    ESP_LOGI(TAG, "ion_main ok");
    abort();
    ESP_LOGI(TAG, "abort ok");


    //
    /*Exprition
     * Ion::Display::drawString
     * */
}
