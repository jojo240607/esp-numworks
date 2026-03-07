#include "key_pad.h"
#include <stdio.h>

static uint8_t key_pad_getcode(Key_pad* self, Keyevent event);

static uint32_t key_pad_get_sdl_key(Key_pad* self);

static void key_pad_show_key(Key_pad* self, Keyevent event);

static void key_pad_show(Key_pad* self, Memory_canvas *canvas);
// 析构函数声明
static void key_pad_destroy(Key_pad* self);

// TODO: 初始化数据成员
static const Key_padFun key_pad_fun = {
    .destroy = key_pad_destroy,
	.show_key = key_pad_show_key,
	.get_sdl_key = key_pad_get_sdl_key,
	.get_code = key_pad_getcode,
};
// 构造函数实现
Key_pad* key_pad_create(Memory_canvas *canvas, int x, int y, int width, int height) {
    Key_pad* obj = (Key_pad*)malloc(sizeof(Key_pad));
    if (obj) {
        memset(obj, 0, sizeof(Key_pad));
        key_pad_init(obj, canvas, x, y, width, height);
    }
    return obj;
}

void key_pad_init(Key_pad* self, Memory_canvas *canvas, int x, int y, int width, int height) {
    self->fun = &(key_pad_fun);
    // TODO: 初始化数据成员
    self->key_num = 0;
    self->mouse_select = 0xff;
    self->pad_point.x = x;
    self->pad_point.y = y;
    self->width = width;
    self->height = height;
    printf("key_pad_init \n");
    key_pad_show(self, canvas);
    printf("key_pad_init over\n");
}

void key_pad_deinit(Key_pad* self) {
    // TODO: 数据成员申请资源释放
    // 清理资源
    for (uint8_t button_index = 0; button_index < self->key_num; button_index++) {
        //printf("button_index %d\n", button_index);
        if (*(self->buttons + button_index) != NULL) {
            (*(self->buttons + button_index))->fun->destroy(*(self->buttons + button_index));
        }
    }
}

// 析构函数实现
static void key_pad_destroy(Key_pad* self) {
    if (self != NULL) {
        key_pad_deinit(self);
        free(self);
    }
}

static void key_pad_show(Key_pad* self, Memory_canvas *canvas) {
    int index_x = self->pad_point.x, index_y = self->pad_point.y;
    //raw 1 // 6
    //up 1
    *(self->buttons + self->key_num) = button_create(canvas, 1, index_x + 40, index_y, 30, 30);
    self->key_num++;
    index_y += 35;
    //left 0
    *(self->buttons + self->key_num) = button_create(canvas, 0, index_x, index_y, 30, 30);
    self->key_num++;
    //right 3
    *(self->buttons + self->key_num) = button_create(canvas, 3, index_x + 80, index_y, 30, 30);
    self->key_num++;
    //down 2
    *(self->buttons + self->key_num) = button_create(canvas, 2, index_x + 40, index_y + 35, 30, 30);
    self->key_num++;
    //ok  4
    *(self->buttons + self->key_num) = button_create(canvas, 4, index_x + 160, index_y, 30, 30);
    self->key_num++;
    //back 5
    *(self->buttons + self->key_num) = button_create(canvas, 5, index_x + 220, index_y, 30, 30);
    self->key_num++;
    index_y += 35 + 5;
    //raw 3 ~ 5
    uint8_t interval = self->width / (6 + 2);
    for (uint8_t raw = 2; raw < 5; raw++) {//4 x 6 = 24
        index_y +=  interval;
        index_x = self->pad_point.x + interval;
        for (uint8_t col = 0; col < 6; col++) {
            *(self->buttons + self->key_num) = button_create(canvas, raw * 6 + col, index_x, index_y, 30, 30);
            self->key_num++;
            index_x += interval;
        }
    }
    //raw 6 ~ 8
    interval = self->width / (5 + 2);
    for (uint8_t raw = 5; raw < 9; raw++) { // 3 x 5 = 15
        index_y +=  interval;
        index_x = self->pad_point.x + interval;
        for (uint8_t col = 0; col < 5; col++) {
            *(self->buttons + self->key_num) = button_create(canvas, raw * 6 + col, index_x, index_y, 35, 30);
            self->key_num++;
            index_x +=  interval;
        }
    }
}
// show_key method
static void key_pad_show_key(Key_pad* self, Keyevent event) {
    if (event.key_code < self->key_num) {
        if (*(self->buttons + event.key_code) != NULL) {
            (*(self->buttons + event.key_code))->fun->set_pressed((*(self->buttons + event.key_code)),
                                                                  (event.event == ON_KEY_DOWN) ? true : false);
        }
    }
}

static void sync_keyboard(Key_pad* self) {
    const Uint8* keyboardState = NULL;
    // 获取当前所有按键状态
    keyboardState = SDL_GetKeyboardState(NULL);
    // 检查特定按键
    uint8_t keyboard_index = SDL_SCANCODE_A;
    for(uint8_t keypad_raw = 0; keypad_raw < 8; keypad_raw++) {
        for(uint8_t keypad_col = 0; keypad_col < 6; keypad_col++) {
            *(testkay_table_test + keypad_raw * 8 + keypad_col) = *(keyboardState + keyboard_index);
            keyboard_index++;
        }
    }
    if (self->mouse_select != 0xff) {
        testkay_table_test[self->mouse_select] = 1;
    }
}
// get_sdl_key method
static uint32_t key_pad_get_sdl_key(Key_pad* self) {
    // TODO: add get_sdl_key method
    SDL_PollEvent(&self->event);
    switch (self->event.type) {
        case SDL_QUIT:  // 点击窗口关闭按钮
            break;

        case SDL_WINDOWEVENT:
            if (self->event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                // 窗口获得焦点时，强制英文输入法
                printf("windows gain focus, try english input\n");
                //SDL_StopTextInput();
            }
            break;
        case SDL_MOUSEBUTTONDOWN:  // 鼠标点击事件
            if (self->event.button.button == SDL_BUTTON_LEFT) {

                Point point = {self->event.button.x, self->event.button.y};
                for (uint8_t button_index = 0; button_index < self->key_num; button_index++) {
                    if (*(self->buttons + button_index) != NULL) {
                        if (GET_VIEW(*(self->buttons + button_index))->fun->in_view(GET_VIEW(*(self->buttons + button_index)), point)) {
                            //printf("button_index = %d\n", button_index);
                            self->mouse_select = button_index % 6 + (button_index / 6 * 8);
                            testkay_table_test[self->mouse_select] = 1;
                        }
                    }
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:  // 鼠标点击事件
            if (self->event.button.button == SDL_BUTTON_LEFT) {
                // printf("click raw_bit %d col_bit %d\n", raw_bit, col_bit);
                if (self->mouse_select >= 0 && self->mouse_select < 64) {
                    testkay_table_test[self->mouse_select] = 0;

                }
                self->mouse_select = 0xff;
            }
            break;
        default:
            sync_keyboard(self);
            break;
    }


    return self->event.type;
}


// getcode method
static uint8_t key_pad_getcode(Key_pad* self, Keyevent event) {
    // TODO: add getcode method
    return (*(self->buttons + event.key_code))->button_code;
}

