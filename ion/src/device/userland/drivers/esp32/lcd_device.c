#include "lcd_device.h"
#include <stdio.h>

// 析构函数声明
static void lcd_device_destroy(Lcd_device* self);

// TODO: 初始化数据成员
static const Lcd_deviceFun lcd_device_fun = {
    .destroy = lcd_device_destroy,
};
// 构造函数实现
Lcd_device* lcd_device_create() {
    Lcd_device* obj = (Lcd_device*)malloc(sizeof(Lcd_device));
    if (obj) {
        memset(obj, 0, sizeof(Lcd_device));
        lcd_device_init(obj);
    }
    return obj;
}

void lcd_device_init(Lcd_device* self) {
    if (self->vtable == NULL) {
        self->vtable = (Lcd_deviceVTable *) malloc(sizeof(Lcd_deviceVTable));
        memset(self->vtable , 0, sizeof(Lcd_deviceVTable));
    }
    self->fun = &(lcd_device_fun);
    // TODO: 初始化数据成员

}

void lcd_device_deinit(Lcd_device* self) {
    if (self->vtable != NULL) {
        free(self->vtable);
        self->vtable = NULL;
    }
    // TODO: 数据成员申请资源释放
}

// 析构函数实现
static void lcd_device_destroy(Lcd_device* self) {
    if (self != NULL) {
        lcd_device_deinit(self);
        free(self);
    }
}
