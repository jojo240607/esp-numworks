#include "button.h"
#include <stdio.h>
#include <stdlib.h>

static void button_set_pressed(Button* self, bool pressed);

draw_override(button_draw_impl);

// 析构函数声明
static void button_destroy(Button* self);

// TODO: 初始化数据成员
static const ButtonFun button_fun = {
    .destroy = button_destroy,
	.set_pressed = button_set_pressed,
};
// 构造函数实现
Button* button_create(Memory_canvas *canvas, uint8_t code, int x, int y, int width, int height) {
    Button* obj = (Button*)malloc(sizeof(Button));
    if (obj) {
        memset(obj, 0, sizeof(Button));
        button_init(obj, canvas, code, x, y, width, height);
    }
    return obj;
}

void button_init(Button* self, Memory_canvas *canvas, uint8_t code, int x, int y, int width, int height) {
    // 初始化基类部分
    view_init(&self->base, canvas, x, y, width, height);
    self->fun = &(button_fun);
    // TODO: 初始化派生类特有成员
    self->button_code = code;
    self->pressed = false;
	def_draw(self) = button_draw_impl;
    virtual_draw(GET_VIEW(self));
}

void button_deinit(Button* self) {
    view_deinit(GET_VIEW(self));
    // TODO: 数据成员申请资源释放
}
// 析构函数实现
static void button_destroy(Button* self) {
    if (self != NULL) {
        button_deinit(self);
        free(self);
    }
}

// draw method
draw_override(button_draw_impl) {
    // TODO: add draw method
    //printf("button_draw_impl\n");
    Button *button = (Button *)self;
    //params
    if (!self->canvas) {
        printf(" self->canvas is null\n");
        return;
    }
    //printf("button->pressed = %d\n", button->pressed);
    self->canvas->fun->draw_rect(self->canvas, self->view_area.point.x, self->view_area.point.y,
                                 self->view_area.width, self->view_area.height, button->pressed ? 0xFFFFFF00 : 0xFFFF0000);
}


// set_pressed method
static void button_set_pressed(Button* self, bool pressed) {
    self->pressed = pressed;
    virtual_draw(GET_VIEW(self));
}

