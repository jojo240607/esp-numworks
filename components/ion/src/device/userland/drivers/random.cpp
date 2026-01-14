#include <ion.h>
#include "esp_random.h"

//#include "svcall.h"

namespace Ion {

uint32_t random() {
    //SVC_RETURNING_R0(SVC_RANDOM, uint32_t)
    return esp_random();
}

}  // namespace Ion
