#ifndef MEMORY_CANVAS_H
#define MEMORY_CANVAS_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define GET_MEMORY_CANVAS(obj) ((Memory_canvas *)obj)
// 类声明
typedef struct _Memory_canvas Memory_canvas;
typedef struct _Memory_canvasFun Memory_canvasFun;
// 类成员函数结构
struct _Memory_canvasFun {
    void (*destroy)(Memory_canvas* self);
	void (*set_pixel)(Memory_canvas* self, int x, int y, uint32_t color);
	void (*draw_rect)(Memory_canvas* self, int x, int y, int w, int h, uint32_t color);
	void (*draw_circle)(Memory_canvas* self, int cx, int cy, int radius, uint32_t color);
	void (*draw_line)(Memory_canvas* self, int x1, int y1, int x2, int y2, uint32_t color);
	void (*clear_canvas)(Memory_canvas* self, uint32_t color);
	void (*create_gradient_background)(Memory_canvas* self, uint32_t color1, uint32_t color2);

	uint32_t (*get_piexl)(Memory_canvas* self, int x, int y);

	void (*draw_text)(Memory_canvas* self, int x, int y, int w, int h, const char *text);

};
// 类结构
struct _Memory_canvas {
    const Memory_canvasFun* fun;
    // TODO: 添加数据成员
    uint32_t *pixels;
    int width;
    int height;
    int pitch;  // 每行字节数
    bool need_sync;
};

// 构造函数声明
Memory_canvas* memory_canvas_create(int width, int height);
void memory_canvas_init(Memory_canvas* self, int width, int height);

// 析构函数声明
void memory_canvas_deinit(Memory_canvas* self);

#endif // MEMORY_CANVAS_H