#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "memory_canvas.h"

#define SDL_MAIN_NEEDED
#include <SDL2/SDL.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SCREEN_WIDTH  270
#define SCREEN_HEIGHT 350

#define GET_DISPLAY(obj) ((Display *)obj)
// 类声明
typedef struct _Display Display;
typedef struct _DisplayFun DisplayFun;

// 类成员函数结构
struct _DisplayFun {
    void (*destroy)(Display *self);

    void (*render)(Display *self);

};

// 类结构
struct _Display {
    const DisplayFun *fun;
    // TODO: 添加数据成员
    uint16_t width;
    uint16_t height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    Memory_canvas *canvas;
};

// 构造函数声明
Display *display_create(uint16_t width, uint16_t height);

void display_init(Display *self, uint16_t width, uint16_t height);

// 析构函数声明
void display_deinit(Display *self);

#ifdef __cplusplus
}
#endif
#endif // DISPLAY_H