#include "external/eadk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


eadk_color_t random_color() { return (eadk_color_t)eadk_random(); }

eadk_rect_t random_screen_rect() {
  uint16_t x = eadk_random() % (EADK_SCREEN_WIDTH - 1);
  uint16_t y = eadk_random() % (EADK_SCREEN_HEIGHT - 1);
  uint16_t width = eadk_random() % (EADK_SCREEN_WIDTH - x);
  uint16_t height = eadk_random() % (EADK_SCREEN_HEIGHT - y);
  return (eadk_rect_t){x, y, width, height};
}

void draw_random_colorful_rectangles() {
  eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_black);
  for (int i = 0; i < 100; i++) {
    eadk_display_push_rect_uniform(random_screen_rect(), random_color());
  }
    eadk_display_sync();
}

void draw_random_buffer() {
  eadk_rect_t rect = {0, 0, 30, 30};
  size_t bufferSize = rect.width * rect.height * sizeof(eadk_color_t);
  eadk_color_t* pixels = (eadk_color_t*)malloc(bufferSize);
  if (pixels == NULL) {
    return;
  }
  memset(pixels, 0, bufferSize);
  for (int i = 0; i < rect.width * rect.height; i++) {
    pixels[i] = random_color();
  }
  eadk_display_push_rect(rect, pixels);
  eadk_display_sync();
  free(pixels);
}

void move_pointer() {
  uint16_t size = 10;
  eadk_rect_t cursor = {(EADK_SCREEN_WIDTH - size) / 2,
                        (EADK_SCREEN_HEIGHT - size) / 2, size, size};
  while (true) {
    eadk_keyboard_state_t keyboard = eadk_keyboard_scan();
    if (eadk_keyboard_key_down(keyboard, eadk_key_back)) {
      return;
    }
    if (eadk_keyboard_key_down(keyboard, eadk_key_left) && cursor.x > 0) {
      cursor.x -= 1;
    }
    if (eadk_keyboard_key_down(keyboard, eadk_key_up) && cursor.y > 0) {
      cursor.y -= 1;
    }
    if (eadk_keyboard_key_down(keyboard, eadk_key_right) &&
        cursor.x < EADK_SCREEN_WIDTH - size) {
      cursor.x += 1;
    }
    if (eadk_keyboard_key_down(keyboard, eadk_key_down) &&
        cursor.y < EADK_SCREEN_HEIGHT - size) {
      cursor.y += 1;
    }
    eadk_display_push_rect_uniform(cursor, random_color());
      eadk_display_sync();
    eadk_timing_msleep(20);
  }
}

void sample_c_main() {
  char *eadk_external_data1 = "eadk_external_data";
  printf("External data : '%s'\n", eadk_external_data1);
  eadk_timing_msleep(3000);
  draw_random_colorful_rectangles();
  draw_random_buffer();
  eadk_display_draw_string(eadk_external_data1, (eadk_point_t){0, 0}, true,
                           eadk_color_black, eadk_color_white);
    eadk_display_sync();
  move_pointer();
}
