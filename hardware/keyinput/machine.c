#include "machine.h"
#include <stdio.h>

static const Statemachine * machine_getCurrentmachine(Machine* self);

static void machine_setEvent(Machine* self, Nextevent event);

static void machine_doEvent(Machine* self, Nextevent *nextevent);

// 析构函数声明
static void machine_destroy(Machine* self);

// TODO: 初始化数据成员
static const MachineFun machine_fun = {
    .destroy = machine_destroy,
	.doEvent = machine_doEvent,
	.setEvent = machine_setEvent,
	.getCurrentmachine = machine_getCurrentmachine,
};
// 构造函数实现
/*Machine* machine_create(const Statemachine *statemachine, size_t size) {
    Machine* obj = (Machine*)malloc(sizeof(Machine));
    if (obj) {
        memset(obj, 0, sizeof(Machine));
        machine_init(obj, statemachine, size);
    }
    return obj;
}
*/
void machine_init(Machine* self, const Statemachine *statemachine, size_t size) {
    self->fun = &(machine_fun);
    // TODO: 初始化数据成员
    self->size = size;
    self->statemachine = statemachine;
}

void machine_deinit(Machine* self) {
    // TODO: 数据成员申请资源释放
}

// 析构函数实现
static void machine_destroy(Machine* self) {
    if (self != NULL) {
        machine_deinit(self);
        free(self);
    }
}

// doEvent method
static void machine_doEvent(Machine* self, Nextevent *nextevent) {
    if (self != NULL) {
        if (nextevent->timedelay > 0) {
            nextevent->timedelay--;
            //printf("timedelay %d\n", nextevent->timedelay);
        } else {
            Statemachine *machine = machine_getCurrentmachine(self);
            if (machine == NULL || machine->state_handler == NULL) {
                printf("machine is null or machine->state_handler is null\n");
                return;
            }
            machine->state_handler(self, nextevent);

        }
    }
}


// setEvent method
static void machine_setEvent(Machine* self, Nextevent event) {
    if (self == NULL) {
        return;
    }
    if (event.nextevent >= self->size) {
        printf("event is over size %zu\n", self->size);
        self->current_event = event;
        return;
    }
    self->current_event = event;
}


// getMachine method
static const Statemachine * machine_getCurrentmachine(Machine* self) {
    if (self == NULL) {
        return NULL;
    }
    if (self->statemachine == NULL) {
        printf("statemachine is null\n");
        return NULL;
    }
    if (self->current_event.nextevent >= self->size) {
        printf("current_event is over size %zu\n", self->size);
        return NULL;
    }
    return self->statemachine + self->current_event.nextevent;
}

