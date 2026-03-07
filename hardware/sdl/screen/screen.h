#ifndef SCREEN_H
#define SCREEN_H
#include <stdint.h>
#include <stdbool.h>
#include "../ui/view.h"

#define GET_SCREEN_VTABLE(obj) GET_VIEW_VTABLE(obj) //(*(ScreenVTable **)obj)
#define GET_SCREEN(obj) ((Screen *)obj)

// 派生类声明
typedef struct _Screen Screen;
typedef struct _ScreenFun ScreenFun;
// 类成员函数结构
struct _ScreenFun {
    void (*destroy)(Screen* self);
	void (*set_pixel)(Screen* self, int x, int y, uint32_t color);

	void (*sync_screen)(Screen* self);

	uint32_t (*get_pixel)(Screen* self, int x, int y);

	void (*draw_rect)(Screen* self, int x, int y, int w, int h, uint32_t color);
    void (*draw_string)(Screen* self, int x, int y, const char* str, uint32_t fg_color, uint32_t bg_color);
};
struct _Screen {
    View base;  // 基类作为第一个成员
    const ScreenFun* fun;
    // TODO: 添加派生类特有的数据成员
};

// 构造函数声明
Screen* screen_create(Memory_canvas *canvas, int x, int y, int width, int height);
void screen_init(Screen* self, Memory_canvas *canvas, int x, int y, int width, int height);

// 析构函数声明
void screen_deinit(Screen* self);

#endif // SCREEN_H