#ifndef SERIALCONV_H
#define SERIALCONV_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "gpio.h"
#define SERIAL_DATA     (0)
#define SERIAL_CLK      (1)
#define SERIAL_READ     (2)
#define SERIAL_WRITE    (3)


#define GET_SERIALCONV(obj) ((Serialconv *)obj)
// 类声明
typedef struct _Serialconv Serialconv;
typedef struct _SerialconvFun SerialconvFun;
// 类成员函数结构
struct _SerialconvFun {
    void (*destroy)(Serialconv* self);
	uint8_t (*read)(Serialconv* self);
	void (*write)(Serialconv* self, uint8_t data);

};
// 类结构
struct _Serialconv {
    const SerialconvFun* fun;
    // TODO: 添加数据成员
    Gpio *clk;        //时钟线
    Gpio *data;       //数据线
    Gpio *read_ctrl;  //控制线
    Gpio *write_ctrl; //写控制线
};

// 构造函数声明
Serialconv* serialconv_create();
void serialconv_init(Serialconv* self);

// 析构函数声明
void serialconv_deinit(Serialconv* self);

#endif // SERIALCONV_H