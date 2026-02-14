#ifndef SST7789_DEVICE_H
#define SST7789_DEVICE_H \
                         \
#include <stdint.h>
#include <stdbool.h>
//#include <freertos/semphr.h>
#include "lcd_device.h"
#include "driver/gpio.h"

#include "esp_timer.h"
#include "esp_err.h"
#include "esp_log.h"

#ifdef __cplusplus
extern "C" {
#endif



#define GET_SST7789_DEVICE_VTABLE(obj) (*(Sst7789_deviceVTable **)obj)
#define GET_SST7789_DEVICE(obj) ((Sst7789_device *)obj)

// 派生类声明
typedef struct _Sst7789_device Sst7789_device;
typedef struct _Sst7789_deviceFun Sst7789_deviceFun;
typedef struct _Sst7789_deviceVTable Sst7789_deviceVTable;
// 虚函数表结构
typedef struct _Sst7789_deviceVTable {
    Lcd_deviceVTable vtbase;
    // TODO: 添加其他虚函数

};
// 类成员函数结构
struct _Sst7789_deviceFun {
    void (*destroy)(Sst7789_device* self);
};
struct _Sst7789_device {
    Lcd_device base;  // 基类作为第一个成员
    const Sst7789_deviceFun* fun;
    // TODO: 添加派生类特有的数据成员
    uint64_t start_time, end_time;
    uint16_t colortest;
    uint16_t count;
    SemaphoreHandle_t vsync_semaphore;
    //volatile bool vsync_triggered;
};

// 构造函数声明
Sst7789_device* sst7789_device_create();
void sst7789_device_init(Sst7789_device* self);

// 析构函数声明
void sst7789_device_deinit(Sst7789_device* self);
#ifdef __cplusplus
}
#endif
#endif // SST7789_DEVICE_H