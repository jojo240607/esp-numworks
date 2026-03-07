#include "view.h"
#include <stdio.h>

static bool view_in_view(View* self, Point point);

// 析构函数声明
static void view_destroy(View* self);

// TODO: 初始化数据成员
static const ViewFun view_fun = {
    .destroy = view_destroy,
	.in_view = view_in_view,
};
// 构造函数实现
View* view_create(Memory_canvas *canvas, int x, int y, int width, int height) {
    View* obj = (View*)malloc(sizeof(View));
    if (obj) {
        memset(obj, 0, sizeof(View));
        view_init(obj, canvas, x, y, width, height);
    }
    return obj;
}

void view_init(View* self, Memory_canvas *canvas, int x, int y, int width, int height) {
    if (self->vtable == NULL) {
        self->vtable = (ViewVTable *) malloc(sizeof(ViewVTable));
        memset(self->vtable , 0, sizeof(ViewVTable));
    }
    self->fun = &(view_fun);
    // TODO: 初始化数据成员
    self->canvas = canvas;
    self->view_area.point.x = x;
    self->view_area.point.y = y;
    self->view_area.width = width;
    self->view_area.height = height;
}

void view_deinit(View* self) {
    if (self->vtable != NULL) {
        free(self->vtable);
        self->vtable = NULL;
    }
    // TODO: 数据成员申请资源释放
}

// 析构函数实现
static void view_destroy(View* self) {
    if (self != NULL) {
        view_deinit(self);
        free(self);
    }
}

// in_view method
static bool view_in_view(View* self, Point point) {
    if (self == NULL) {
        return false;
    }

    if (point.x >= self->view_area.point.x && (point.x < self->view_area.point.x + self->view_area.width) &&
        point.y >= self->view_area.point.y && (point.y < self->view_area.point.y + self->view_area.height)) {
        //printf("(%d, %d) (%d, %d) (%d, %d)\n", point.x, point.y,
        //       self->view_area.point.x,  self->view_area.point.y,
        //       self->view_area.width, self->view_area.height);
        return true;
    } else {

    }
    return false;
}

