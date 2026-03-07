#include "gpio.h"
#include <stdio.h>

static void gpio_set(Gpio* self, bool bit);
static bool gpio_get(Gpio* self);
static void gpio_set_mode(Gpio* self, GpopMode mode);

// 析构函数声明
static void gpio_destroy(Gpio* self);

volatile uint8_t testkay_table_test[64] = {0};
static uint8_t num = 0;
// TODO: 初始化数据成员
static const GpioFun gpio_fun = {
    .destroy = gpio_destroy,
	.set = gpio_set,
	.get = gpio_get,
	.set_mode = gpio_set_mode,
};
// 构造函数实现
Gpio* gpio_create(uint8_t pin) {
    Gpio* obj = (Gpio*)malloc(sizeof(Gpio));
    if (obj) {
        memset(obj, 0, sizeof(Gpio));
        gpio_init(obj, pin);
    }
    return obj;
}

void gpio_init(Gpio* self, uint8_t pin) {
    self->fun = &(gpio_fun);
    // TODO: 初始化数据成员
    self->pin = pin;
}

void gpio_deinit(Gpio* self) {
    // TODO: 数据成员申请资源释放
}

// 析构函数实现
static void gpio_destroy(Gpio* self) {
    if (self != NULL) {
        gpio_deinit(self);
        free(self);
    }
}

// set method
static void gpio_set(Gpio* self, bool bit) {
    // TODO: add set method
    //printf("set pin %d to %d\n", self->pin, bit);
}
// get method
static bool gpio_get(Gpio* self) {
    // TODO: add get method
    num %= 64;
    return !testkay_table_test[num++];
}
// set_mode method
static void gpio_set_mode(Gpio* self, GpopMode mode) {
    // TODO: add set_mode method
    //printf("set pin %d mode to %d\n", self->pin, mode);
}

