#include <ion/battery.h>

#include "svcall.h"

namespace Ion {
namespace Battery {

bool SVC_ATTRIBUTES isCharging(){
    //SVC_RETURNING_R0(SVC_BATTERY_IS_CHARGING, bool)
    return false;
}

Charge SVC_ATTRIBUTES level() {
    Charge charge = Charge();
    return charge;
  //SVC_RETURNING_R0(SVC_BATTERY_LEVEL, Charge)
}

float SVC_ATTRIBUTES voltage() {
    return 1.0;
    //SVC_RETURNING_S0(SVC_BATTERY_VOLTAGE, float)
}

}  // namespace Battery
}  // namespace Ion
