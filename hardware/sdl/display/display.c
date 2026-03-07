#include "display.h"
#include <stdio.h>

static void display_render(Display* self);

// 析构函数声明
static void display_destroy(Display* self);
static int8_t display_create_window(Display* self);

// TODO: 初始化数据成员
static const DisplayFun display_fun = {
    .destroy = display_destroy,
	.render = display_render,
};
// 构造函数实现
Display* display_create(uint16_t width, uint16_t height) {
    Display* obj = (Display*)malloc(sizeof(Display));
    if (obj) {
        memset(obj, 0, sizeof(Display));
        display_init(obj, width, height);
    }
    return obj;
}
// create_window method
static int8_t display_create_window(Display* self) {
    // TODO: add create_window method

    // SDL初始化
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL初始化失败: %s\n", SDL_GetError());
        return -1;
    }

    // 创建窗口
    self->window = SDL_CreateWindow(
            "SDL2",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            self->width,
            self->height,
            SDL_WINDOW_SHOWN
    );

    if (!self->window) {
        printf("create window failed: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    // 创建渲染器（用于绘制）
    self->renderer = SDL_CreateRenderer(self->window, -1,
                                        SDL_RENDERER_ACCELERATED |
                                        SDL_RENDERER_PRESENTVSYNC);
    if (!self->renderer) {
        printf("create renderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(self->window);
        SDL_Quit();
        return -1;
    }

    // 创建纹理
    self->texture = SDL_CreateTexture(self->renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             self->width, self->height);
    if (!self->texture) {
        printf("create texture failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(self->window);
        SDL_Quit();
        return -1;
    }
}
void display_init(Display* self, uint16_t width, uint16_t height) {
    self->fun = &(display_fun);
    // TODO: 初始化数据成员
    self->width = width;
    self->height = height;
    self->canvas = memory_canvas_create(self->width, self->height);
    if (!self->canvas) {
        printf("Failed to create canvas\n");
        return;
    }
    display_create_window(self);


    // 清除画布为黑色
    self->canvas->fun->clear_canvas(self->canvas, 0xFF000000);
    // 创建渐变背景
    self->canvas->fun->create_gradient_background(self->canvas,
                               0xFF1A1A2E,  // 深蓝色
                               0xFF16213E   // 蓝色
    );

}



void display_deinit(Display* self) {
    // TODO: 数据成员申请资源释放
    if (self->renderer != NULL) {
        SDL_DestroyRenderer(self->renderer);
    }
    if (self->window != NULL) {
        SDL_DestroyWindow(self->window);
    }
    if (self->texture != NULL) {
        SDL_DestroyTexture(self->texture);
    }
    if (self->canvas != NULL) {
        self->canvas->fun->destroy(self->canvas);
    }
    SDL_Quit();
}

// 析构函数实现
static void display_destroy(Display* self) {
    if (self != NULL) {
        display_deinit(self);
        free(self);
    }
}

// render method
static void display_render(Display* self) {
    // TODO: add render method

    // 更新纹理
    SDL_UpdateTexture(self->texture, NULL, self->canvas->pixels, self->canvas->pitch);
    // 渲染
    SDL_RenderClear(self->renderer);
    SDL_RenderCopy(self->renderer, self->texture, NULL, NULL);
    SDL_RenderPresent(self->renderer);
    //printf("render over\n");
    // 控制帧率
    //SDL_Delay(16);
}

