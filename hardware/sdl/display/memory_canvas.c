#include "memory_canvas.h"
#include <stdio.h>
static void memory_canvas_draw_text(Memory_canvas* self, int x, int y, int w, int h, const char *text);

static uint32_t memory_canvas_get_piexl(Memory_canvas* self, int x, int y);

static void memory_canvas_set_pixel(Memory_canvas* self, int x, int y, uint32_t color);
static void memory_canvas_draw_rect(Memory_canvas* self, int x, int y, int w, int h, uint32_t color);
static void memory_canvas_draw_circle(Memory_canvas* self, int cx, int cy, int radius, uint32_t color);
static void memory_canvas_draw_line(Memory_canvas* self, int x1, int y1, int x2, int y2, uint32_t color);
static void memory_canvas_clear_canvas(Memory_canvas* self, uint32_t color);
static void memory_canvas_create_gradient_background(Memory_canvas* self, uint32_t color1, uint32_t color2);

// 析构函数声明
static void memory_canvas_destroy(Memory_canvas* self);

// TODO: 初始化数据成员
static const Memory_canvasFun memory_canvas_fun = {
    .destroy = memory_canvas_destroy,
	.set_pixel = memory_canvas_set_pixel,
	.draw_rect = memory_canvas_draw_rect,
	.draw_circle = memory_canvas_draw_circle,
	.draw_line = memory_canvas_draw_line,
	.clear_canvas = memory_canvas_clear_canvas,
	.create_gradient_background = memory_canvas_create_gradient_background,
	.get_piexl = memory_canvas_get_piexl,
	.draw_text = memory_canvas_draw_text,
};
// 构造函数实现
Memory_canvas* memory_canvas_create(int width, int height) {
    Memory_canvas* obj = (Memory_canvas*)malloc(sizeof(Memory_canvas));
    if (obj) {
        memset(obj, 0, sizeof(Memory_canvas));
        memory_canvas_init(obj, width, height);
    }
    return obj;
}

void memory_canvas_init(Memory_canvas* self, int width, int height) {
    self->fun = &(memory_canvas_fun);
    // TODO: 初始化数据成员
    self->width = width;
    self->height = height;
    self->pitch = width * sizeof(uint32_t);
    self->pixels = (uint32_t *)malloc(width * height * sizeof(uint32_t));
    self->need_sync = false;
    if (!self->pixels) {
        printf("error: malloc pixels error\n");
    }
}

void memory_canvas_deinit(Memory_canvas* self) {
    // TODO: 数据成员申请资源释放
    if (self->pixels) {
        free(self->pixels);
    }
}

// 析构函数实现
static void memory_canvas_destroy(Memory_canvas* self) {
    if (self != NULL) {
        memory_canvas_deinit(self);
        free(self);
    }
}

// set_pixel method
static void memory_canvas_set_pixel(Memory_canvas* self, int x, int y, uint32_t color) {
    if (x >= 0 && x < self->width && y >= 0 && y < self->height) {
        self->pixels[y * self->width + x] = color;
    }
    self->need_sync = true;
}

// get_piexl method
static uint32_t memory_canvas_get_piexl(Memory_canvas* self, int x, int y) {
    uint32_t color = 0;
    if (x >= 0 && x < self->width && y >= 0 && y < self->height) {
        color = self->pixels[y * self->width + x];
    }
    return color;
}


// draw_rect method
static void memory_canvas_draw_rect(Memory_canvas* self, int x, int y, int w, int h, uint32_t color) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            self->fun->set_pixel(self, x + j, y + i, color);
        }
    }
}
// draw_circle method
static void memory_canvas_draw_circle(Memory_canvas* self, int cx, int cy, int radius, uint32_t color) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                self->fun->set_pixel(self, cx + x, cy + y, color);
            }
        }
    }
}
// draw_line method
static void memory_canvas_draw_line(Memory_canvas* self, int x1, int y1, int x2, int y2, uint32_t color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        self->fun->set_pixel(self, x1, y1, color);
        if (x1 == x2 && y1 == y2)
            break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}
// clear_canvas method
static void memory_canvas_clear_canvas(Memory_canvas* self, uint32_t color) {
    for (int i = 0; i < self->width * self->height; i++) {
        self->pixels[i] = color;
    }
}
// create_gradient_background method
static void memory_canvas_create_gradient_background(Memory_canvas* self, uint32_t color1, uint32_t color2) {
    // 提取颜色分量
    uint8_t r1 = (color1 >> 16) & 0xFF;
    uint8_t g1 = (color1 >> 8) & 0xFF;
    uint8_t b1 = color1 & 0xFF;

    uint8_t r2 = (color2 >> 16) & 0xFF;
    uint8_t g2 = (color2 >> 8) & 0xFF;
    uint8_t b2 = color2 & 0xFF;

    for (int y = 0; y < self->height; y++) {
        // 计算当前行的颜色
        float t = (float)y / self->height;
        uint8_t r = r1 + (uint8_t)((r2 - r1) * t);
        uint8_t g = g1 + (uint8_t)((g2 - g1) * t);
        uint8_t b = b1 + (uint8_t)((b2 - b1) * t);
        uint32_t color = (0xFF << 24) | (r << 16) | (g << 8) | b;
        for (int x = 0; x < self->width; x++) {
            self->fun->set_pixel(self, x, y, color);
        }
    }
}

// draw_text method
static void memory_canvas_draw_text(Memory_canvas* self, int x, int y, int w, int h, const char *text) {
    // TODO: add draw_text method
    
}

