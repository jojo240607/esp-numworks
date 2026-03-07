#ifndef BUTTON_H
#define BUTTON_H
#include <stdint.h>
#include <stdbool.h>
#include "view.h"

#define GET_BUTTON_VTABLE(obj) GET_VIEW_VTABLE(obj) //(*(ButtonVTable **)obj)
#define GET_BUTTON(obj) ((Button *)obj)

// 派生类声明
typedef struct _Button Button;
typedef struct _ButtonFun ButtonFun;
// 类成员函数结构
struct _ButtonFun {
    void (*destroy)(Button* self);
	void (*set_pressed)(Button* self, bool pressed);

};
struct _Button {
    View base;  // 基类作为第一个成员
    const ButtonFun* fun;
    // TODO: 添加派生类特有的数据成员
    uint8_t button_code;
    bool pressed;
};

// 构造函数声明
Button* button_create(Memory_canvas *canvas, uint8_t code, int x, int y, int width, int height);
void button_init(Button* self, Memory_canvas *canvas, uint8_t code, int x, int y, int width, int height);

// 析构函数声明
void button_deinit(Button* self);

#endif // BUTTON_H