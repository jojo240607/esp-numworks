#ifndef GPIO_H
#define GPIO_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define GET_GPIO(obj) ((Gpio *)obj)
// 类声明
typedef struct _Gpio Gpio;
typedef struct _GpioFun GpioFun;
typedef enum _GpopMode GpopMode;
// 类成员函数结构
struct _GpioFun {
    void (*destroy)(Gpio* self);
	void (*set)(Gpio* self, bool bit);
	bool (*get)(Gpio* self);
	void (*set_mode)(Gpio* self, GpopMode mode);

};

enum _GpopMode {
    highimpedance_input = 0,//高阻态输入
    pushpull_output,//推挽输出
    opendrain_output,//开漏输出
    mode3
};
// 类结构
struct _Gpio {
    const GpioFun* fun;
    // TODO: 添加数据成员
    uint8_t pin;
};
extern volatile uint8_t testkay_table_test[64];
// 构造函数声明
Gpio* gpio_create(uint8_t pin);
void gpio_init(Gpio* self, uint8_t pin);

// 析构函数声明
void gpio_deinit(Gpio* self);

#endif // GPIO_H