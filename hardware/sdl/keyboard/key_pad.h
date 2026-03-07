#ifndef KEY_PAD_H
#define KEY_PAD_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../../keyinput/key_methine.h"
#include "../ui/button.h"

#define SDL_MAIN_NEEDED
#include <SDL2/SDL.h>

#define SCREEN_WIDTH  270
#define SCREEN_HEIGHT 350
#define GET_KEY_PAD(obj) ((Key_pad *)obj)
// 类声明
typedef struct _Key_pad Key_pad;
typedef struct _Key_padFun Key_padFun;
// 类成员函数结构
struct _Key_padFun {
    void (*destroy)(Key_pad* self);
	void (*show_key)(Key_pad* self, Keyevent event);
	uint32_t (*get_sdl_key)(Key_pad* self);
	uint8_t (*get_code)(Key_pad* self, Keyevent event);

};
// 类结构
struct _Key_pad {
    const Key_padFun* fun;
    // TODO: 添加数据成员
    SDL_Event event;
    uint8_t mouse_select;
    Button *buttons[48];
    uint8_t key_num;
    Point pad_point;
    int width;
    int height;
};

// 构造函数声明
Key_pad* key_pad_create(Memory_canvas *canvas, int x, int y, int width, int height);
void key_pad_init(Key_pad* self, Memory_canvas *canvas, int x, int y, int width, int height);

// 析构函数声明
void key_pad_deinit(Key_pad* self);

#endif // KEY_PAD_H