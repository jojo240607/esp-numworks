#include <ion/authentication.h>

#include "svcall.h"

namespace Ion {
namespace Authentication {

ClearanceLevel SVC_ATTRIBUTES clearanceLevel() {
    ClearanceLevel clearanceLevel1 = ClearanceLevel();
    return clearanceLevel1;
  //SVC_RETURNING_R0(SVC_AUTHENTICATION_CLEARANCE_LEVEL, ClearanceLevel)
}

}  // namespace Authentication
}  // namespace Ion
