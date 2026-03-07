#include <ion/timing.h>
//#include <esp_timer.h>
//#include <rom/ets_sys.h>
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "svcall.h"

namespace Ion {
namespace Timing {
    /*
void compensated_delay_us(uint32_t target_us) {
    uint64_t start = esp_timer_get_time();
    uint64_t target = start + target_us;
    // 计算补偿值（根据实际测量调整）
    const uint32_t overhead = 5; // 大约5us的函数调用开销
    while (true) {
        uint64_t now = esp_timer_get_time();
        if (now >= target) {
            break;
        }
        // 计算剩余时间
        uint64_t remaining = target - now;
        if (remaining > overhead * 2) {
            // 使用忙等待，但留出一点余量
            ets_delay_us(remaining - overhead);
        } else {
            // 最后几微秒，空循环等待
            // __asm__ volatile("nop");
        }
    }
}*/
void usleep(uint32_t us) {
  //SVC_RETURNING_VOID(SVC_TIMING_USLEEP)
  //  compensated_delay_us(us);
}

void msleep(uint32_t ms){
    //SVC_RETURNING_VOID(SVC_TIMING_MSLEEP)
   // vTaskDelay(pdMS_TO_TICKS(ms));  // 延迟10毫秒
}

uint64_t millis() {
  //SVC_RETURNING_R0R1(SVC_TIMING_MILLIS, uint64_t)
   // return esp_timer_get_time() / 1000;
    return 0;
}

}  // namespace Timing
}  // namespace Ion
