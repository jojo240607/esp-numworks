#include "key_methine.h"
#include <stdio.h>
#include <stdlib.h>


static void keymethine_initEvent(Keymethine* self, Nextevent nextevent);

static bool keymethine_doEvent(Keymethine* self);

static void state_idle_handler(Machine *machine, Nextevent *nextstate);
static void state_recheck_handler(Machine *machine, Nextevent *nextstate);
static void state_event_sync_handler(Machine *machine, Nextevent *nextstate);
static void keymethine_register_key_event(Keymethine* self, onKeyEventCallback callback);
// 析构函数声明
static void keymethine_destroy(Keymethine* self);

// TODO: 初始化数据成员
static const KeymethineFun Keymethine_fun = {
    .destroy = keymethine_destroy,
	.doEvent = keymethine_doEvent,
	.initEvent = keymethine_initEvent,
    .register_key_event = keymethine_register_key_event
};
static const Statemachine statemachine_list[] = {
        {state_idle_handler, "STATE_IDLE"},//STATE_IDLE
        {state_recheck_handler, "STATE_RECHECK"},//STATE_READY
        {state_event_sync_handler, "STATE_EVENT_SYNC"},//STATE_WORKING
};

// 构造函数实现
Keymethine* keymethine_create() {
    Keymethine* obj = (Keymethine*)malloc(sizeof(Keymethine));
    if (obj) {
        memset(obj, 0, sizeof(Keymethine));
        keymethine_init(obj);
    }
    return obj;
}

void keymethine_init(Keymethine* self) {
    // 初始化基类部分
    machine_init(&self->base, statemachine_list, STATE_MAX - STATE_IDLE);
    self->fun = &(Keymethine_fun);
    // TODO: 初始化派生类特有成员
    self->nextevent.nextevent = STATE_IDLE;
    self->nextevent.timedelay = 0;
    memset(&self->keymatrix, 0, sizeof(Keymatrix));
    self->serialconv = serialconv_create();
}

void keymethine_deinit(Keymethine* self) {
    machine_deinit(GET_MACHINE(self));
    // TODO: 数据成员申请资源释放
    self->serialconv->fun->destroy(self->serialconv);
}
// 析构函数实现
static void keymethine_destroy(Keymethine* self) {
    if (self != NULL) {
        keymethine_deinit(self);
        free(self);
    }
}

static void clear_down_flag(Keymethine* self, uint8_t raw_bit) {
    //清除检测down标志
    (self->keymatrix.rawkeys + raw_bit)->down_key_tab = 0;
}
static void clear_up_flag(Keymethine* self, uint8_t raw_bit) {
    //清除检测up标志
    (self->keymatrix.rawkeys + raw_bit)->up_key_tab = 0;
}

static bool get_down_flag(Keymethine* self, uint8_t raw_bit, uint8_t change_bit) {
    //获取从0 -> 1变化的key
    (self->keymatrix.rawkeys + raw_bit)->down_key_tab |= change_bit & (self->keymatrix.rawkeys + raw_bit)->current_key_tab;
    if ((self->keymatrix.rawkeys + raw_bit)->down_key_tab) {
        return true;
    }
    return false;
}

static bool get_up_flag(Keymethine* self, uint8_t raw_bit, uint8_t change_bit) {
    //获取从1 -> 0变化的key
    (self->keymatrix.rawkeys + raw_bit)->up_key_tab |= change_bit & (self->keymatrix.rawkeys + raw_bit)->prev_key_tab;
    if ((self->keymatrix.rawkeys + raw_bit)->up_key_tab) {
        return true;
    }
    return false;
}

static uint8_t check_press_down(Keymethine* self, uint8_t raw_bit) {
    uint8_t press_down = (self->keymatrix.rawkeys + raw_bit)->current_key_tab & (self->keymatrix.rawkeys + raw_bit)->down_key_tab;
    if (!press_down) {
        //如果没有被按下，则需要更新prev_key_tab，避免prev_key_tab的值是按下的值,导致后续检测异常
        (self->keymatrix.rawkeys + raw_bit)->prev_key_tab &= ~(self->keymatrix.rawkeys + raw_bit)->down_key_tab;
    }
    clear_down_flag(self, raw_bit);
    return press_down;
}

static uint8_t check_press_up(Keymethine* self, uint8_t raw_bit) {
    uint8_t press_up = (~(self->keymatrix.rawkeys + raw_bit)->current_key_tab) & (self->keymatrix.rawkeys + raw_bit)->up_key_tab;
    if (!press_up) {
        //如果没有被抬起，则需要更新prev_key_tab，避免prev_key_tab的值是抬起的值,导致后续检测异常
        (self->keymatrix.rawkeys + raw_bit)->prev_key_tab |= (self->keymatrix.rawkeys + raw_bit)->up_key_tab;
    }
    clear_up_flag(self, raw_bit);
    return press_up;
}


/*
 *      检测按键相对于上次是否有变化，keytable按键按下对应bit变为1，未按下是0，
 *      将上次检测保存的keytable和这次检测值做个异或操作，非0则代表有变化
 *
 */
static bool check_key_change(Keymethine* self) {
    if (self == NULL) {
        return false;
    }
    bool ischange = false;
    for (uint8_t raw_bit = 0; raw_bit < KEY_SCAN_TABLE; raw_bit++) {
        self->serialconv->fun->write(self->serialconv, ~(SCAN_TABLE << raw_bit));//1111 1110
        self->keymatrix.rawkeys[raw_bit].current_key_tab = ~self->serialconv->fun->read(self->serialconv);

        //异或操作，两次不一样则代表有变化了,变化存到up_key_tab 和 down_key_tab中
        uint8_t changed = (self->keymatrix.rawkeys + raw_bit)->prev_key_tab ^ (self->keymatrix.rawkeys + raw_bit)->current_key_tab; //获取两次变化的key

        get_down_flag(self, raw_bit, changed);
        get_up_flag(self, raw_bit, changed);
        if (!ischange && changed) {
            //任意一行变化，都返回true
            ischange = true;
        }
        //检测完毕，prev_key_tab更新
        if ((self->keymatrix.rawkeys + raw_bit)->prev_key_tab != (self->keymatrix.rawkeys + raw_bit)->current_key_tab) {
            (self->keymatrix.rawkeys + raw_bit)->prev_key_tab = (self->keymatrix.rawkeys + raw_bit)->current_key_tab;
        }
    }
    return ischange;
}

static uint8_t find_key_code(Keymethine* self, uint8_t keycode) {
    for (uint8_t i = 0; i < self->keymatrix.pressed_num; i++) {
        if (self->keymatrix.key_events[i].key_code == keycode) {
            //printf("find key code %d\n", keycode);
            return i;
        }
    }
    return UNDEFINED_KEY_CODE;
}

static void add_key_code(Keymethine* self, uint8_t raw, uint8_t col, Event event) {
    for(uint8_t col_bit = 0; col_bit < COL_BIT_NUM; col_bit++) {
        if ((col >> col_bit) & 0x01) {
            uint8_t keycode = raw * COL_BIT_NUM + col_bit;
            //printf("self->keymatrix.pressed_num = %d\n", self->keymatrix.pressed_num);
            uint8_t findid = find_key_code(self, keycode);

            if (findid == UNDEFINED_KEY_CODE) {
                if (self->keymatrix.pressed_num < MAX_MULTI_KEYNUMBER) {
                    self->keymatrix.key_events[self->keymatrix.pressed_num].key_code = keycode;
                    self->keymatrix.key_events[self->keymatrix.pressed_num].event = event;
                    self->keymatrix.key_events[self->keymatrix.pressed_num].used = false;
                    self->keymatrix.pressed_num++;
                } else {
                    printf("%d pressed_num %d %d > 10 \n", col_bit, keycode, self->keymatrix.pressed_num);
                }
            } else {
                //printf("reset key event %d\n", keycode);
                self->keymatrix.key_events[findid].key_code = keycode;
                self->keymatrix.key_events[findid].event = event;
                self->keymatrix.key_events[findid].used = false;
            }
        }
    }
}

static void clear_key_events(Keymethine* self) {
    Keyevent temp_keyevents[self->keymatrix.pressed_num];
    uint8_t saveevents = 0;
    for (uint8_t key_num = 0; key_num < self->keymatrix.pressed_num; key_num++) {
        if (self->keymatrix.key_events[key_num].event == ON_KEY_DOWN) {
            temp_keyevents[saveevents++] = self->keymatrix.key_events[key_num];
        }
    }
    memset(self->keymatrix.key_events, 0, sizeof(self->keymatrix.key_events));
    self->keymatrix.pressed_num = 0;
    if (saveevents > 0) {
        for (int i = 0; i < saveevents; ++i) {
            self->keymatrix.key_events[i] = temp_keyevents[i];
        }
        self->keymatrix.pressed_num = saveevents;
    }
}


static bool get_key_event(Keymethine* self) {
    bool have_key = false;
    for (uint8_t raw_bit = 0; raw_bit < KEY_SCAN_TABLE; raw_bit++) {
        self->serialconv->fun->write(self->serialconv, ~(SCAN_TABLE << raw_bit));//1111 1110
        (self->keymatrix.rawkeys + raw_bit)->current_key_tab = ~self->serialconv->fun->read(self->serialconv);
        uint8_t down_check = check_press_down(self, raw_bit);
        //检测down按下持续
        if (down_check) {
            have_key = true;
            add_key_code(self, raw_bit, down_check, ON_KEY_DOWN);
        }

        uint8_t up_check = check_press_up(self, raw_bit);
        //检测up抬起持续
        if (up_check) {
            have_key = true;
            add_key_code(self, raw_bit, up_check, ON_KEY_UP);
        }
    }
    return have_key;
}

static void state_idle_handler(Machine *machine, Nextevent *nextstate) {
    if (machine == NULL || nextstate == NULL) {
        return;
    }
    Keymethine *keymethine = (Keymethine *)machine;
    //printf("event %s state_idle_handler\n", machine->fun->getCurrentmachine(machine)->eventname);
    if (check_key_change(keymethine)) {//按下，等待20ms后再检测
        nextstate->nextevent = STATE_RECHECK;
        nextstate->timedelay = 2;
    } else {
        //没有按下，依旧执行扫描
        nextstate->nextevent = STATE_IDLE;
        nextstate->timedelay = 0;
    }
}
static void state_recheck_handler(Machine *machine, Nextevent *nextstate) {
    if (machine == NULL || nextstate == NULL) {
        return;
    }
    Keymethine *keymethine = (Keymethine *)machine;
    //printf("event %s state_recheck_handler\n", machine->fun->getCurrentmachine(machine)->eventname);
    if (get_key_event(keymethine)) {
        //20ms后依然按下，代表真的按下
        nextstate->nextevent = STATE_EVENT_SYNC;
        nextstate->timedelay = 0;
    } else {
        //20ms后没有按下，消抖，过滤掉
        nextstate->nextevent = STATE_IDLE;
        nextstate->timedelay = 0;
    }
}
static void state_event_sync_handler(Machine *machine, Nextevent *nextstate) {
    if (machine == NULL || nextstate == NULL) {
        return;
    }
    Keymethine *keymethine = (Keymethine *)machine;
    //printf("event %s state_down_handler\n", machine->fun->getCurrentmachine(machine)->eventname);
    //当前是按键按下后，检测松开
    if (keymethine->keymatrix.pressed_num > 0) {
        //检测event,同步给上层
        for (uint8_t num = 0; num < keymethine->keymatrix.pressed_num; num++) {
            if (keymethine->callback != NULL && !keymethine->keymatrix.key_events[num].used) {
                keymethine->callback(keymethine->keymatrix.key_events[num]);
                keymethine->keymatrix.key_events[num].used = true;
            }
        }

        clear_key_events(keymethine);
        nextstate->nextevent = STATE_IDLE;
        nextstate->timedelay = 0;
    } else {
        //还是按下的状态
        nextstate->nextevent = STATE_IDLE;
        nextstate->timedelay = 0;

    }
}

// setEvent method
static bool keymethine_doEvent(Keymethine* self) {
    if (self == NULL) {
        return false;
    }
    const Statemachine *current_machine = GET_MACHINE(self)->fun->getCurrentmachine(GET_MACHINE(self));
    if (current_machine == NULL) {
        return false;
    }
    GET_MACHINE(self)->fun->doEvent(GET_MACHINE(self), &self->nextevent);
    self->fun->initEvent(self, self->nextevent);
    return true;
}


// initEvent method
static void keymethine_initEvent(Keymethine* self, Nextevent nextevent) {
    if (self == NULL) {
        return;
    }
    Nextevent new_nextevent = nextevent;
    new_nextevent.nextevent -= STATE_IDLE;
    GET_MACHINE(self)->fun->setEvent(GET_MACHINE(self), new_nextevent);
}

static void keymethine_register_key_event(Keymethine* self, onKeyEventCallback callback) {
    if (self == NULL) {
        return;
    }
    self->callback = callback;
}

