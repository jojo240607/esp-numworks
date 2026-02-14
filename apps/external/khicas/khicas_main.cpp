extern "C" {
#include "external/eadk.h"
#include "giac/src/k_defs.h"
}
#include <cstdint>
#include <cstring>
#include <esp_system.h>
#include <esp_heap_caps.h>

extern "C" {
unsigned _heap_size = 0;
void * _heap_ptr = nullptr;
void * _heap_base = nullptr;
bool inexammode() {
  return false;
}

void sync_screen() {
    eadk_display_sync();
}

int ext_main();
void khicas_main() {
    // 获取内部内存（DRAM）的空闲大小
    size_t free_internal = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
    printf("Free internal RAM: %u bytes\n", free_internal);
    // 获取SPIRAM（即PSRAM）的空闲大小
    size_t free_spiram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    printf("Free SPIRAM: %u bytes\n", free_spiram);
    // 获取总空闲内存（包括所有区域）
    size_t free_total = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    printf("Free total (DEFAULT): %u bytes\n", free_total);
// 强制从 PSRAM 分配 1MB 内存
    void *buffer = heap_caps_malloc(30 * 1024, MALLOC_CAP_SPIRAM);
    if (buffer == NULL) {
        // 分配失败处理
        printf("heap_caps_malloc error\n");
    } else {
        _heap_size = 30 * 1024;
        _heap_ptr = buffer;
        _heap_base = buffer;
    }
  ext_main();
}

uint64_t millis() {
  return eadk_timing_millis();
}

int os_file_browser(const char ** filenames, int maxrecords, const char * extension, int storage) {
  filenames[0] = 0;
  return 0;
}

bool file_exists(const char * filename) {
  return false;
}

bool erase_file(const char * filename){
  return false;
}

const char * read_file(const char * filename){
  return nullptr;
}

bool write_file(const char * filename, const char * content, size_t len) {
  return false;
}

int numworks_draw_string(int x, int y, int c, int bg, const char * text, bool fake) {
  if (!fake) {
    eadk_display_draw_string(text, (eadk_point_t){(uint16_t)x,(uint16_t)(y+18)}, true, (eadk_color_t)c, (eadk_color_t)bg);
  }
  return x+strlen(text)*10;
}

int numworks_draw_string_small(int x, int y, int c, int bg, const char * text, bool fake) {
  if (!fake) {
    eadk_display_draw_string(text, (eadk_point_t){(uint16_t)x,(uint16_t)(y+18)}, false, (eadk_color_t)c, (eadk_color_t)bg);
  }
  return x+strlen(text)*7;
}

bool waitforvblank() {
  return eadk_display_wait_for_vblank();
}

void numworks_set_pixel(int x, int y, int color) {
  if (x<0 || x>=EADK_SCREEN_WIDTH|| y<0 || y>=EADK_SCREEN_HEIGHT-18)
    return;
  eadk_display_push_rect_uniform((eadk_rect_t){(uint16_t)x, (uint16_t)(y+18),1,1},(eadk_color_t)color);
}

void numworks_fill_rect(int x, int y, int w, int h, int c) {
  if (x<0) {
    w += x;
    x=0;
  }
  if (y<0) {
    h += y;
    y=0;
  }
 if (h+y>=EADK_SCREEN_HEIGHT-18)
   h=EADK_SCREEN_HEIGHT-18-y;
 if (x+w>=EADK_SCREEN_WIDTH)
   w=EADK_SCREEN_WIDTH-x;
  if (h<=0 || w<=0)
    return;
  eadk_display_push_rect_uniform((eadk_rect_t){(uint16_t)x,(uint16_t)(y+18),(uint16_t)w,(uint16_t)h}, (eadk_color_t)c);
}

int numworks_get_pixel(int x, int y) {
  uint16_t color;
  eadk_display_pull_rect((eadk_rect_t){(uint16_t)x,(uint16_t)y,1,1}, &color);
  return color;
}

void numworks_wait_1ms(int ms) {
  for (int i=0; i<ms/128; ++i) {
    eadk_keyboard_state_t scan = eadk_keyboard_scan();
    if (eadk_keyboard_key_down(scan, eadk_key_back)) {
      return;
    }
    eadk_timing_msleep(128);
  }
  eadk_timing_msleep(ms % 128);
}

static bool interruptible = true;

bool back_key_pressed() {
  static int c = 0;

  ++c;
  printf("back_key_pressed c = %d interruptible = %d\n", c, interruptible);
  if (!interruptible || c%1024 != 0) {
    return false;
  } else {
    return false;//eadk_keyboard_key_down(eadk_keyboard_scan(), eadk_key_back);
  }
}

void enable_back_interrupt() {
  interruptible = true;
}

void disable_back_interrupt() {
  interruptible = false;
}

bool os_set_angle_unit(int mode) {
  return false;
}

int os_get_angle_unit() {
  return 0;
}

void numworks_hide_graph() {
}

void numworks_show_graph() {
}

void statuslinemsg(const char * msg) {
  uint16_t c;
  eadk_display_pull_rect((eadk_rect_t){0,0,1,1}, &c);
  eadk_display_push_rect_uniform((eadk_rect_t){0,0,280,18}, c);
  eadk_display_draw_string(msg, (eadk_point_t){0, 0}, (strlen(msg)<=25), eadk_color_white, c);
}

void statusline(int mode) {
}

void lock_alpha() {
}

void reset_kbd() {
}

bool alphawasactive(int * key) {
  return false;
}

static const int translated_keys[]=
  {
   // non shifted
   KEY_CTRL_LEFT,KEY_CTRL_UP,KEY_CTRL_DOWN,KEY_CTRL_RIGHT,KEY_CTRL_OK,KEY_CTRL_EXIT,
   KEY_CTRL_MENU,KEY_PRGM_ACON,KEY_PRGM_ACON,9,10,11,
   KEY_CTRL_SHIFT,KEY_CTRL_ALPHA,KEY_CTRL_XTT,KEY_CTRL_VARS,KEY_CTRL_CATALOG,KEY_CTRL_DEL,
   KEY_CHAR_EXPN,KEY_CHAR_LN,KEY_CHAR_LOG,KEY_CHAR_IMGNRY,',',KEY_CHAR_POW,
   KEY_CHAR_SIN,KEY_CHAR_COS,KEY_CHAR_TAN,KEY_CHAR_PI,KEY_CHAR_ROOT,KEY_CHAR_SQUARE,
   '7','8','9','(',')',-1,
   '4','5','6','*','/',-1,
   '1','2','3','+','-',-1,
   '0','.',KEY_CHAR_EXPN10,KEY_CHAR_ANS,KEY_CTRL_EXE,-1,
   // shifted
   KEY_SHIFT_LEFT,KEY_CTRL_PAGEUP,KEY_CTRL_PAGEDOWN,KEY_SHIFT_RIGHT,KEY_CTRL_OK,KEY_CTRL_EXIT,
   KEY_CTRL_MENU,KEY_PRGM_ACON,KEY_PRGM_ACON,9,10,11,
   KEY_CTRL_SHIFT,KEY_CTRL_ALPHA,KEY_CTRL_CUT,KEY_CTRL_CLIP,KEY_CTRL_PASTE,KEY_CTRL_AC,
   KEY_CHAR_LBRCKT,KEY_CHAR_RBRCKT,KEY_CHAR_LBRACE,KEY_CHAR_RBRACE,'_',KEY_CHAR_STORE,
   KEY_CHAR_ASIN,KEY_CHAR_ACOS,KEY_CHAR_ATAN,'=','<','>',
   KEY_CTRL_F7,KEY_CTRL_F8,KEY_CTRL_F9,KEY_CTRL_F13,KEY_CTRL_F14,-1,
   KEY_CTRL_F4,KEY_CTRL_F5,KEY_CTRL_F6,KEY_CHAR_FACTOR,'%',-1,
   KEY_CTRL_F1,KEY_CTRL_F2,KEY_CTRL_F3,KEY_CHAR_NORMAL,'\\',-1,
   KEY_CTRL_F10,KEY_CTRL_F11,KEY_CTRL_F12,KEY_SHIFT_ANS,KEY_CTRL_MENU,-1,
   // alpha
   KEY_CTRL_LEFT,KEY_CTRL_UP,KEY_CTRL_DOWN,KEY_CTRL_RIGHT,KEY_CTRL_OK,KEY_CTRL_EXIT,
   KEY_CTRL_MENU,KEY_PRGM_ACON,KEY_PRGM_ACON,9,10,11,
   KEY_CTRL_SHIFT,KEY_CTRL_ALPHA,':',';','"',KEY_CTRL_DEL,
   'a','b','c','d','e','f',
   'g','h','i','j','k','l',
   'm','n','o','p','q',-1,
   'r','s','t','u','v',-1,
   'w','x','y','z',' ',-1,
   '?','!',KEY_CHAR_EXPN10,KEY_CHAR_ANS,KEY_CTRL_EXE,-1,
   // alpha shifted
   KEY_SHIFT_LEFT,KEY_CTRL_PAGEUP,KEY_CTRL_PAGEDOWN,KEY_SHIFT_RIGHT,KEY_CTRL_OK,KEY_CTRL_EXIT,
   KEY_CTRL_MENU,KEY_PRGM_ACON,KEY_PRGM_ACON,9,10,11,
   KEY_CTRL_SHIFT,KEY_CTRL_ALPHA,':',';','\'','%',
   'A','B','C','D','E','F',
   'G','H','I','J','K','L',
   'M','N','O','P','Q',-1,
   'R','S','T','U','V',-1,
   'W','X','Y','Z',' ',-1,
   '?','!',KEY_CHAR_EXPN10,KEY_CHAR_ANS,KEY_CTRL_EXE,-1,
  };

int getkey(int allow_suspend) {
  int32_t timeout = 1000;
  eadk_event_t e = eadk_event_get(&timeout);
  // Workaround to make shift-number work
  if (e >= eadk_event_seven && e <= eadk_event_exe) {
    if (eadk_keyboard_key_down(eadk_keyboard_scan(), eadk_key_shift)) {
      e += 54;
    }
  }
  return translated_keys[e];
}

void GetKey(int * key) {
  *key = getkey(true);
}

}

