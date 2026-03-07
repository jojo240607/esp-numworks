#ifndef SYSTEMSTATEMETHINE_H
#define SYSTEMSTATEMETHINE_H
#include <stdint.h>
#include <stdbool.h>
#include "machine.h"
#include "serialconv.h"
#include "key_code_tab.h"

#define SCAN_TABLE (0x01)
#define KEY_SCAN_TABLE (8)
#define COL_BIT_NUM (6)
#define MAX_MULTI_KEYNUMBER (6)
#define UNDEFINED_KEY_CODE (0xFF)
#define GET_SYSTEMSTATEMETHINE_VTABLE(obj) GET_MACHINE_VTABLE(obj) //(*(SystemstatemethineVTable **)obj)
#define GET_SYSTEMSTATEMETHINE(obj) ((Systemstatemethine *)obj)

// 派生类声明
typedef struct _Keymethine Keymethine;
typedef struct _KeymethineFun KeymethineFun;
typedef enum _key_state Keystate;
typedef struct _key_matrix Keymatrix;
typedef enum _event Event;
typedef struct _key_event Keyevent;
typedef struct _raw_keys Rawkeys;

typedef void (*onKeyEventCallback)(Keyevent keyevent);

// 状态枚举
enum _key_state {
    STATE_IDLE = 0x10,
    STATE_RECHECK,
    STATE_EVENT_SYNC,
    STATE_MAX
};
enum _event {
    ON_KEY_NONE = 0x00, //未检测到
    ON_KEY_DOWN,        //按下
    ON_KEY_UP,          //抬起
    ON_KEY_KEEP_DOWN    //持续按下
};
struct _key_event {
    KeycodeTab key_code;
    Event event;
    bool used;
};

struct _raw_keys {
    uint8_t current_key_tab;//: 当前扫描的原始状态（未消抖）
    uint8_t prev_key_tab;//: 上一次扫描状态（用于边缘检测）
    uint8_t up_key_tab;//抬起状态 1 -> 0
    uint8_t down_key_tab;//按下状态 0 -> 1
};

struct _key_matrix {
    Rawkeys rawkeys[KEY_SCAN_TABLE];//按键行检测
    uint8_t pressed_num;//当前按键按下个数
    Keyevent key_events[MAX_MULTI_KEYNUMBER];//最多支持10个按键同时按下(十根手指)
};
// 类成员函数结构
struct _KeymethineFun {
    void (*destroy)(Keymethine* self);
	bool (*doEvent)(Keymethine* self);

	void (*initEvent)(Keymethine* self, Nextevent nextevent);
    void (*register_key_event)(Keymethine* self, onKeyEventCallback callback);
};
struct _Keymethine {
    Machine base;  // 基类作为第一个成员
    const KeymethineFun* fun;
    // TODO: 添加派生类特有的数据成员
    Nextevent nextevent;
    Serialconv *serialconv;
    Keymatrix keymatrix;
    onKeyEventCallback callback;
};

// 构造函数声明
Keymethine* keymethine_create();
void keymethine_init(Keymethine* self);

// 析构函数声明
void keymethine_deinit(Keymethine* self);

#endif // SYSTEMSTATEMETHINE_H