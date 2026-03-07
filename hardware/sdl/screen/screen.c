#include "screen.h"
#include "hardware/sdl/font/font_ascii.h"
#include <stdio.h>
#include <stdlib.h>

static void screen_draw_rect(Screen* self, int x, int y, int w, int h, uint32_t color);

static uint32_t screen_get_pixel(Screen* self, int x, int y);
static void screen_sync_screen(Screen* self);
static void screen_set_pixel(Screen* self, int x, int y, uint32_t color);
static void screen_draw_string(Screen* self, int x, int y, const char* str,
                               uint32_t fg_color, uint32_t bg_color);
draw_override(screen_draw_impl);
// 析构函数声明
static void screen_destroy(Screen* self);

// TODO: 初始化数据成员
static const ScreenFun screen_fun = {
    .destroy = screen_destroy,
	.set_pixel = screen_set_pixel,
	.sync_screen = screen_sync_screen,
	.get_pixel = screen_get_pixel,
	.draw_rect = screen_draw_rect,
    .draw_string = screen_draw_string
};
// 构造函数实现
Screen* screen_create(Memory_canvas *canvas, int x, int y, int width, int height) {
    Screen* obj = (Screen*)malloc(sizeof(Screen));
    if (obj) {
        memset(obj, 0, sizeof(Screen));
        screen_init(obj, canvas, x, y, width, height);
    }
    return obj;
}

void screen_init(Screen* self, Memory_canvas *canvas, int x, int y, int width, int height) {
    // 初始化基类部分
    view_init(&self->base, canvas, x, y, width, height);
    self->fun = &(screen_fun);
    // TODO: 初始化派生类特有成员

	def_draw(self) = screen_draw_impl;
    canvas->fun->draw_rect(canvas, x, y, width, height, 0xff555555);
}

void screen_deinit(Screen* self) {
    view_deinit(GET_VIEW(self));
    // TODO: 数据成员申请资源释放
}
// 析构函数实现
static void screen_destroy(Screen* self) {
    if (self != NULL) {
        screen_deinit(self);
        free(self);
    }
}

// draw method
draw_override(screen_draw_impl) {
    // TODO: add draw method
    Screen *screen = (Screen *)self;
    //params 
    
}


// set_pixel method
static void screen_set_pixel(Screen* self, int x, int y, uint32_t color) {
    // TODO: add set_pixel method
    Point point = {x, y};
    if (x < GET_VIEW(self)->view_area.width && y < GET_VIEW(self)->view_area.height) {
        GET_VIEW(self)->canvas->fun->set_pixel(GET_VIEW(self)->canvas,
                                               GET_VIEW(self)->view_area.point.x + x,
                                               GET_VIEW(self)->view_area.point.y + y,
                                               color);
    }
}

// get_pixel method
static uint32_t screen_get_pixel(Screen* self, int x, int y) {
    Point point = {x, y};
    if (x < GET_VIEW(self)->view_area.width && y < GET_VIEW(self)->view_area.height) {
        return GET_VIEW(self)->canvas->fun->get_piexl(GET_VIEW(self)->canvas,
                                                      GET_VIEW(self)->view_area.point.x + x,
                                                      GET_VIEW(self)->view_area.point.y + y);
    }
    return 0;
}

// sync_screen method
static void screen_sync_screen(Screen* self) {
    virtual_draw(GET_VIEW(self));
}


// draw_rect method
static void screen_draw_rect(Screen* self, int x, int y, int w, int h, uint32_t color) {
    // TODO: add draw_rect method
    if ((x  + w) <= GET_VIEW(self)->view_area.width && (y + h) <= GET_VIEW(self)->view_area.height) {
        //printf("draw_rect\n");
        GET_VIEW(self)->canvas->fun->draw_rect(GET_VIEW(self)->canvas,
                                               GET_VIEW(self)->view_area.point.x + x,
                                               GET_VIEW(self)->view_area.point.y + y,
                                               w,
                                               h,
                                               color);
    } else {
        //printf("%d %d out of screen(%d %d %d %d)\n", x  + w, y + h, w,
        //       h,
        //       GET_VIEW(self)->view_area.width,
        //       GET_VIEW(self)->view_area.height);
    }
}

// ==================== ASCII字符绘制 ====================

// 绘制一个ASCII字符（5x7点阵）
static uint8_t screen_draw_char(Screen* self, int x, int y, char c, uint8_t font_w, uint8_t font_h,  const uint8_t* font_data, uint32_t fg_color, uint32_t bg_color) {
    // 字符ASCII值
    int ascii = (int)c;

    // 只绘制可打印字符（32-126）
    if (ascii < 32 || ascii > 126) {
        ascii = 32;  // 空格
    }

    // 获取字体数据
    //font_data = font_5x7_simple[ascii];

    // 如果该字符没有字体数据，使用空格
    if (font_data[0] == 0 && font_data[1] == 0 && font_data[2] == 0) {
        // 绘制背景色方块
        for (int fy = 0; fy < font_h; fy++) {
            for (int fx = 0; fx < font_w; fx++) {
                self->fun->set_pixel(self, x + fx, y + fy, bg_color);
            }
        }
        return font_w;
    }

    // 绘制字符
    for (int fy = 0; fy < font_h; fy++) {      // 7行
        uint8_t row = font_data[fy];      // 每行的位图

        for (int fx = 0; fx < font_w; fx++) {  // 5列
            // 检查每一位是否为1
            if (row & (1 << (font_w - 1 - fx))) {  // 高位在前
                self->fun->set_pixel(self, x + fx, y + fy, fg_color);
            } else {
                self->fun->set_pixel(self, x + fx, y + fy, bg_color);
            }
        }
    }
    return font_w;
}

// 绘制字符串
static void screen_draw_string(Screen* self, int x, int y, const char* str,
                 uint32_t fg_color, uint32_t bg_color) {
    int cursor_x = x;
    int cursor_y = y;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            cursor_x = x;
            cursor_y += 8;  // 换行，增加行高
        } else {
            screen_draw_char(self, cursor_x, cursor_y, str[i], 5, 7, font_5x7_simple[str[i]] , fg_color, bg_color);
            cursor_x += 6;  // 字符宽度5 + 1像素间距
        }
    }
}


