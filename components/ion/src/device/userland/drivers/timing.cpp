#include <ion/timing.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "svcall.h"

namespace Ion {
namespace Timing {

void usleep(uint32_t us) {
  //SVC_RETURNING_VOID(SVC_TIMING_USLEEP)
}

void msleep(uint32_t ms){
    //SVC_RETURNING_VOID(SVC_TIMING_MSLEEP)
    vTaskDelay(pdMS_TO_TICKS(ms));  // 延迟10毫秒
}

uint64_t millis() {
  //SVC_RETURNING_R0R1(SVC_TIMING_MILLIS, uint64_t)
    return 0;
}

}  // namespace Timing
}  // namespace Ion
