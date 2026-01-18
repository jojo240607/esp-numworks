#include <ion/events.h>
#include <ion/keyboard/layout_events.h>

//#include "svcall.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
namespace Ion {
namespace Events {

size_t copyText(uint8_t eventId, char* buffer,
                               size_t bufferSize) {
  //SVC_RETURNING_R0(SVC_EVENTS_COPY_TEXT, size_t)
    return bufferSize;
}

bool isDefined(uint8_t eventId) {
    return true;
    //SVC_RETURNING_R0(SVC_EVENTS_IS_DEFINED, bool)
}

void setShiftAlphaStatus(ShiftAlphaStatus s){
    //SVC_RETURNING_VOID(SVC_EVENTS_SET_SHIFT_ALPHA_STATUS)
}

ShiftAlphaStatus shiftAlphaStatus() {
    ShiftAlphaStatus shiftAlphaStatus1 = ShiftAlphaStatus();
    return shiftAlphaStatus1;
    //SVC_RETURNING_R0(SVC_EVENTS_SHIFT_ALPHA_STATUS, ShiftAlphaStatus)
}

int longPressCounter(){
//    SVC_RETURNING_R0(SVC_EVENTS_LONG_PRESS_COUNTER, int)
    return 0;
}
/*
Event getEvent(int* timeout) {
    Event event = Ion::Events::None;
    return event;
    //SVC_RETURNING_R0(SVC_EVENTS_GET_EVENT, Event)
}*/
const int loopLength = 12;
const Event sequence[] = {Down, Down, Down, Down, OK, Back, OK, Home,
                          OK,  OK,  OK,    Up, Up,   Up,   Up,   Up};

Event getEvent(int* timeout) {
    static int i = 0;
    constexpr int sequenceLength = std::size(sequence);
    if (i == sequenceLength) {
        i = sequenceLength - loopLength;
    }
    if (i > sequenceLength) {
        while (1) {
        }
    }
    vTaskDelay(pdMS_TO_TICKS(1000));  // 延迟10毫秒
    return sequence[i++];
}

void setSpinner(bool spiner) {
  //SVC_RETURNING_VOID(SVC_EVENTS_SET_SPINNER)
}

void openURL(const char* url) {}

}  // namespace Events
}  // namespace Ion
