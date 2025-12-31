#include <ion/events.h>
#include <ion/keyboard/layout_events.h>

#include "svcall.h"

namespace Ion {
namespace Events {

size_t SVC_ATTRIBUTES copyText(uint8_t eventId, char* buffer,
                               size_t bufferSize) {
  //SVC_RETURNING_R0(SVC_EVENTS_COPY_TEXT, size_t)
    return bufferSize;
}

bool SVC_ATTRIBUTES isDefined(uint8_t eventId) {
    return true;
    //SVC_RETURNING_R0(SVC_EVENTS_IS_DEFINED, bool)
}

void SVC_ATTRIBUTES setShiftAlphaStatus(ShiftAlphaStatus s){
    //SVC_RETURNING_VOID(SVC_EVENTS_SET_SHIFT_ALPHA_STATUS)
}

ShiftAlphaStatus SVC_ATTRIBUTES shiftAlphaStatus() {
    ShiftAlphaStatus shiftAlphaStatus1 = ShiftAlphaStatus();
    return shiftAlphaStatus1;
    //SVC_RETURNING_R0(SVC_EVENTS_SHIFT_ALPHA_STATUS, ShiftAlphaStatus)
}

int SVC_ATTRIBUTES longPressCounter(){
//    SVC_RETURNING_R0(SVC_EVENTS_LONG_PRESS_COUNTER, int)
    return 0;
}

Event SVC_ATTRIBUTES getEvent(int* timeout) {
    Event event = Event();
    return event;
    //SVC_RETURNING_R0(SVC_EVENTS_GET_EVENT, Event)
}

void SVC_ATTRIBUTES setSpinner(bool spiner) {
  //SVC_RETURNING_VOID(SVC_EVENTS_SET_SPINNER)
}

void openURL(const char* url) {}

}  // namespace Events
}  // namespace Ion
