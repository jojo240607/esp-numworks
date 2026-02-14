#ifndef LCD_DEVICE_H
#define LCD_DEVICE_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#ifdef __cplusplus
extern "C" {
#endif



#define GET_LCD_DEVICE_VTABLE(obj) (*(Lcd_deviceVTable **)obj)

#define init_override(func_name) static void func_name(Lcd_device* self)
#define def_init(obj) (GET_LCD_DEVICE_VTABLE(obj)->init)
#define virtual_init(obj, ...) def_init(obj)(obj, ##__VA_ARGS__)
#define ondraw_override(func_name) static void func_name(Lcd_device* self)
#define def_ondraw(obj) (GET_LCD_DEVICE_VTABLE(obj)->ondraw)
#define virtual_ondraw(obj, ...) def_ondraw(obj)(obj, ##__VA_ARGS__)

#define GET_LCD_DEVICE(obj) ((Lcd_device *)obj)
// 类声明
typedef struct _Lcd_device Lcd_device;
typedef struct _Lcd_deviceFun Lcd_deviceFun;
typedef struct _Lcd_deviceVTable Lcd_deviceVTable;
typedef struct _Framebuffer Framebuffer;
// 虚函数表结构
struct _Lcd_deviceVTable {
    // TODO : 添加其他虚函数

	void (*init)(Lcd_device* self);
	void (*ondraw)(Lcd_device* self);

};
// 类成员函数结构
struct _Lcd_deviceFun {
    void (*destroy)(Lcd_device* self);
};
struct _Framebuffer {
    void *frontbuf;
   // void *backbuf;
    void *currentbuf;
    bool isready;
    size_t draw_buffer_sz;
};
// 类结构
struct _Lcd_device {
    Lcd_deviceVTable* vtable;
    const Lcd_deviceFun* fun;
    // TODO: 添加数据成员
    esp_lcd_panel_io_handle_t io_handle;
    esp_lcd_panel_handle_t panel_handle;
    Framebuffer framebuf;
};

// 构造函数声明
Lcd_device* lcd_device_create();
void lcd_device_init(Lcd_device* self);

// 析构函数声明
void lcd_device_deinit(Lcd_device* self);
#ifdef __cplusplus
}
#endif
#endif // LCD_DEVICE_H