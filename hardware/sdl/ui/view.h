#ifndef VIEW_H
#define VIEW_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../display/memory_canvas.h"

#define GET_VIEW_VTABLE(obj) (*(ViewVTable **)obj)

#define draw_override(func_name) static void func_name(View* self)
#define def_draw(obj) (GET_VIEW_VTABLE(obj)->draw)
#define virtual_draw(obj, ...) def_draw(obj)(obj, ##__VA_ARGS__)

#define GET_VIEW(obj) ((View *)obj)
// 类声明
typedef struct _View View;
typedef struct _ViewFun ViewFun;
typedef struct _ViewVTable ViewVTable;
typedef struct _point Point;
typedef struct _view_area ViewArea;
typedef struct _view_color ViewColor;

// 虚函数表结构
typedef struct _ViewVTable {
    // TODO : 添加其他虚函数

	void (*draw)(View* self);

};
// 类成员函数结构
struct _ViewFun {
    void (*destroy)(View* self);
	bool (*in_view)(View* self, Point point);

};

struct _point {
    int x;
    int y;
};
struct _view_area {
    Point point;
    int width;
    int height;
};
struct _view_color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// 类结构
struct _View {
    ViewVTable* vtable;
    const ViewFun* fun;
    // TODO: 添加数据成员
    ViewArea view_area;
    ViewColor color;
    Memory_canvas* canvas;
};

// 构造函数声明
View* view_create(Memory_canvas *canvas, int x, int y, int width, int height);
void view_init(View* self, Memory_canvas *canvas, int x, int y, int width, int height);

// 析构函数声明
void view_deinit(View* self);

#endif // VIEW_H