#ifndef MACHINE_H
#define MACHINE_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define GET_MACHINE(obj) ((Machine *)obj)
// 类声明
typedef struct _Machine Machine;
typedef struct _MachineFun MachineFun;
typedef struct _Statemachine Statemachine;
typedef struct _nextevent Nextevent;
typedef void (*Statehandler)(Machine *machine, Nextevent *nextstate);
// 状态机结构
struct _Statemachine {
    Statehandler state_handler;
    const char *eventname;
};
struct _nextevent {
    uint16_t nextevent;
    uint16_t timedelay;
};
// 类成员函数结构
struct _MachineFun {
    void (*destroy)(Machine* self);
	void (*doEvent)(Machine* self, Nextevent *nextevent);
	void (*setEvent)(Machine* self, Nextevent event);
    const Statemachine * (*getCurrentmachine)(Machine* self);
};
// 类结构
struct _Machine {
    const MachineFun* fun;
    // TODO: 添加数据成员
    Nextevent current_event;
    size_t size;
    const Statemachine *statemachine;
};

// 构造函数声明
//Machine* machine_create(const Statemachine *statemachine, size_t size);
void machine_init(Machine* self, const Statemachine *statemachine, size_t size);

// 析构函数声明
void machine_deinit(Machine* self);

#endif // MACHINE_H