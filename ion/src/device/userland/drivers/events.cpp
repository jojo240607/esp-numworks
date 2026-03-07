#include <ion/events.h>
#include <ion/keyboard/layout_events.h>

//#include "svcall.h"
#include "ion/src/shared/keyboard_queue.h"
#include <ion/src/shared/events.h>
namespace Ion {
namespace Events {
constexpr static size_t sharedExternalTextBufferSize = EventData::k_maxDataSize;
char* sharedExternalTextBuffer() {
    static char buffer[sharedExternalTextBufferSize];
    return buffer;
}
static size_t strlcpy(char * dst, const char * src, size_t dstSize) {
    const size_t srcLen = strlen(src);
    if (srcLen+1 < dstSize) {
        memcpy(dst, src, srcLen+1);
    } else if (dstSize != 0) {
        memcpy(dst, src, dstSize-1);
        dst[dstSize-1] = 0;
    }
    return srcLen;
}
bool waitForInterruptingEvent(int maximumDelay, int* timeout) {
    Keyboard::scan();
    /* As pressing keys on the simulator does not generate interruptions, we need
     * to poll the keyboard more regularly than on the device. */
    constexpr int simulatorDelay = 10;
    maximumDelay = std::min(simulatorDelay, maximumDelay);
    if (*timeout < maximumDelay) {
        Timing::msleep(*timeout);
        *timeout = 0;
    } else {
        Timing::msleep(maximumDelay);
        *timeout -= maximumDelay;
    }
    return !Keyboard::Queue::sharedQueue()->isEmpty();
}

const char* Event::text() const {
    if (*this == ExternalText) {
        return const_cast<const char*>(sharedExternalTextBuffer());
    }
    return defaultText();
}


size_t copyText(uint8_t eventId, char* buffer, size_t bufferSize) {
    return sharedCopyText(eventId, buffer, bufferSize);
}

bool isDefined(uint8_t eventId) { return sharedIsDefined(eventId); }

bool handlePreemption(bool) { return false; }

void didPressNewKey() {
    //Simulator::Haptics::rumble();
}

//void SVC_ATTRIBUTES setShiftAlphaStatus(ShiftAlphaStatus s){
//    //SVC_RETURNING_VOID(SVC_EVENTS_SET_SHIFT_ALPHA_STATUS)
//}
//
//ShiftAlphaStatus SVC_ATTRIBUTES shiftAlphaStatus() {
//    ShiftAlphaStatus shiftAlphaStatus1 = ShiftAlphaStatus();
//    return shiftAlphaStatus1;
//    //SVC_RETURNING_R0(SVC_EVENTS_SHIFT_ALPHA_STATUS, ShiftAlphaStatus)
//}

//int SVC_ATTRIBUTES longPressCounter(){
////    SVC_RETURNING_R0(SVC_EVENTS_LONG_PRESS_COUNTER, int)
//    return 0;
//}
/*
Event SVC_ATTRIBUTES getEvent(int* timeout) {
    //std::printf("getEvent timeout %d\n", *timeout);
    int key;
    numworks_getkey(&key);
    Event event = None;
    if (key != -1) {
        event = Event(key);
        std::printf("Event get %d\n", event.operator uint8_t());
    }
    if (event == None) {
        if (!m_idleWasSent) {
            m_idleWasSent = true;
            return Idle;
        }
    } else {
        m_idleWasSent = false;
    }
    return event;
    //SVC_RETURNING_R0(SVC_EVENTS_GET_EVENT, Event)
}*/
Event getEvent(int* timeout) {
    return sharedGetEvent(timeout);
}

void setSpinner(bool spiner) {
  //SVC_RETURNING_VOID(SVC_EVENTS_SET_SPINNER)
}

void openURL(const char* url) {}

}  // namespace Events
}  // namespace Ion
