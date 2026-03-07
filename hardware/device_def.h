//
// Created by zhiwei.gong on 2026/2/2.
//

#ifndef CAS_DEVICE_DEF_H
#define CAS_DEVICE_DEF_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef union {
    uint16_t value;
    struct {
        uint16_t b: 5;  // 蓝色分量
        uint16_t g: 6;  // 绿色分量
        uint16_t r: 5;  // 红色分量
    } bits;
} Color16;

typedef union {
    uint32_t value;
    struct {
        uint8_t b;  // 蓝色分量
        uint8_t g;  // 绿色分量
        uint8_t r;  // 红色分量
        uint8_t a;  // Alpha通道（可选）
    } channels;
} Color32;


void init_device(void);
void device_set_pixel(int x, int y, int color);
void device_fill_rect(int x, int y, int w, int h, int c);
int device_get_pixel(int x, int y);
int device_waitforvblank();
//int getkey(int allow_suspend);
void device_getkey(int *key);

//void khicas_main();

void destory();

void run_loop();

#ifdef __cplusplus
}
#endif
#endif //CAS_DEVICE_DEF_H
