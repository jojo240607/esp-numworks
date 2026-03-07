#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "hardware/sdl/display/display.h"
#include "hardware/sdl/keyboard/key_pad.h"
#include "hardware/sdl/screen/screen.h"
#include "device_def.h"

char _external_apps_flash_start = 0;
char _external_apps_flash_end = 0;
char _external_apps_RAM_start = 0;
char _external_apps_RAM_end = 0;
char _device_name_sector_start = 0;
char _device_name_sector_end = 0;

//unsigned _heap_size = 30 *1024;
static bool interruptible = true;

int cache_key = -1;
bool key_shift = false;
bool key_alpha = false;
static const uint8_t status_line_height = 0;//giac = 18;
static Display *display = NULL;
static Key_pad *keyPad = NULL;
static Screen *screen = NULL;
static Keymethine *keymethine = NULL;

void onKeyEvent(Keyevent keyevent) {
    printf("onKeyEvent code %d event %s\n", keyevent.key_code, !(keyevent.event - ON_KEY_DOWN) ? "down" : "up");
    keyPad->fun->show_key(keyPad, keyevent);
    if (keyevent.event == ON_KEY_DOWN) {
        uint8_t button_code = keyPad->fun->get_code(keyPad, keyevent);

        cache_key = button_code;
    }
}

void init_device(void) {
    printf("init_device\n");
    display = display_create(350, 700);
    keyPad = key_pad_create(display->canvas, 15, 250, 320, 240);
    screen = screen_create(display->canvas, 15, 0, 320, 240);

    keymethine = keymethine_create();
    Nextevent nextevent = {STATE_IDLE, 0};
    keymethine->fun->initEvent(keymethine, nextevent);
    keymethine->fun->register_key_event(keymethine, onKeyEvent);
    display->fun->render(display);
}
void run_loop() {
    bool running = true;
    while (running) {
        if (keyPad->fun->get_sdl_key(keyPad) == SDL_QUIT) {
            printf("SDL_QUIT\n");
            running = false;
        }
        display->fun->render(display);
        if (keymethine->fun->doEvent(keymethine)) {
            usleep(10 * 1000);//10ms扫描任务执行周期
        } else {
            printf("error sync key pad error!\n");
            usleep(1000 * 1000);
        }
    }
}
void destory() {
    printf("destory\n");
    //keymethine->fun->destroy(keymethine);
    printf("destory1\n");
    keyPad->fun->destroy(keyPad);
    printf("destory2\n");
    screen->fun->destroy(screen);
    display->fun->destroy(display);
}


// 转换函数
void rgb565_to_rgb888(Color16 *src, Color32 *dst) {
    if (!src || !dst) return;

    // 16位转24位（保留精度）
    dst->channels.r = (src->bits.r << 3) | (src->bits.r >> 2);
    dst->channels.g = (src->bits.g << 2) | (src->bits.g >> 4);
    dst->channels.b = (src->bits.b << 3) | (src->bits.b >> 2);
    dst->channels.a = 0xFF;  // 完全不透明
}

void rgb888_to_rgb565(Color32 *src, Color16 *dst) {
    if (!src || !dst) return;

    // 24位转16位（有精度损失）
    dst->bits.r = src->channels.r >> 3;  // 取高5位
    dst->bits.g = src->channels.g >> 2;  // 取高6位
    dst->bits.b = src->channels.b >> 3;  // 取高5位
}

int device_waitforvblank() {
    display->fun->render(display);
    return true;//eadk_display_wait_for_vblank();
}

//int back_key_pressed() {
//    return false;
//}
//int ext_main();
//void khicas_main() {
//    printf("ext_main\n");
//    ext_main();
//}
/*
bool inexammode() {
    return false;
}

double millis() {
    return 0.0f;
}

int write_file(const char * filename, const char * content, int len) {
    return false;
}
int os_file_browser(const char ** filenames, int maxrecords, const char * extension, int storage) {
    filenames[0] = 0;
    return 0;
}

int file_exists(const char * filename) {
    return false;
}
int erase_file(const char * filename){
    return false;
}

const char * read_file(const char * filename){
    return NULL;
}

int device_draw_string(int x, int y, int c, int bg, const char * text, bool fake) {
    if (!fake) {
        y += status_line_height;
        //eadk_display_draw_string(text, (eadk_point_t){(uint16_t)x,(uint16_t)(y+18)}, true, (eadk_color_t)c, (eadk_color_t)bg);
        //printf("device_draw_string %s\n", text);
        Color32 dst_color;
        rgb565_to_rgb888((Color16 *)&bg, &dst_color);
        screen->fun->draw_string(screen, x, y, text, c, dst_color.value);
    }
    return x+strlen(text)*10;
}

int device_draw_string_small(int x, int y, int c, int bg, const char * text, bool fake) {
    if (!fake) {
        y += status_line_height;
        //printf("device_draw_string_small %s\n", text);
       // eadk_display_draw_string(text, (eadk_point_t){(uint16_t)x,(uint16_t)(y+18)}, false, (eadk_color_t)c, (eadk_color_t)bg);
        Color32 dst_color;
        rgb565_to_rgb888((Color16 *)&bg, &dst_color);
        screen->fun->draw_string(screen, x, y, text, c, dst_color.value);
    }
    return x+strlen(text)*7;
}



void device_wait_1ms(int ms) {
    printf("device_wait_1ms %d\n", ms);
}
void enable_back_interrupt() {
    printf("enable_back_interrupt\n");
    interruptible = true;
}

void disable_back_interrupt() {
    printf("disable_back_interrupt\n");
    interruptible = false;
}

int os_set_angle_unit(int mode) {
    return false;
}

int os_get_angle_unit() {
    return 0;
}

void device_hide_graph() {
}

void device_show_graph() {
}
void statuslinemsg(const char * msg) {
   // printf("statuslinemsg %s\n", msg);
    screen->fun->draw_rect(screen, 0, 0, 320, 18, 0x555555);
    Color32 dst_color = {.value = 0x555555};
    screen->fun->draw_string(screen, 30, 5, msg, 0xFFFF00, dst_color.value);
}

void extapp_clipboardStore(const char *text) {

}

const char * extapp_clipboardText() {

}

void statusline(int mode) {
}

void lock_alpha() {
}

void reset_kbd() {
}

int alphawasactive(int * key) {
    return false;
}



*/

void device_set_pixel(int x, int y, int color) {
    //printf("device_set_pixel\n");
    y += status_line_height;
    Color32 dst_color;
    rgb565_to_rgb888((Color16 *)&color, &dst_color);
    screen->fun->set_pixel(screen, x, y, dst_color.value);
}
void device_fill_rect(int x, int y, int w, int h, int c) {
    //printf("device_fill_rect(%d, %d %d %d %x)\n", x, y, w, h, c);
    y += status_line_height;
    Color32 dst_color;
    rgb565_to_rgb888((Color16 *)&c, &dst_color);
    screen->fun->draw_rect(screen, x, y, w, h, dst_color.value);
}

int device_get_pixel(int x, int y) {
    y += status_line_height;
    uint16_t color;
    //eadk_display_pull_rect((eadk_rect_t){(uint16_t)x,(uint16_t)y,1,1}, &color);
    uint32_t scolor = screen->fun->get_pixel(screen, x, y);
    rgb888_to_rgb565((Color32 *)&scolor, (Color16 *)&color);
    return color;
}
static int self_getkey(int allow_suspend) {
    if (keyPad->fun->get_sdl_key(keyPad) == SDL_QUIT) {
        printf("SDL_QUIT\n");
        // running = false;
    }
    display->fun->render(display);
    keymethine->fun->doEvent(keymethine);
    if (keymethine->fun->doEvent(keymethine)) {
        usleep(10 * 1000);//10ms扫描任务执行周期
    }
    return 0;
}

void device_getkey(int *key) {
    //printf("GetKey\n");
    self_getkey(true);
    *key = cache_key;
    cache_key = -1;
}

