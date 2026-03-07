#include <ion/display.h>
#include <ion/storage/file_system.h>
#include "ion/src/shared/events_modifier.h"
#include "ion/src/shared/events.h"

namespace Ion {

void Init() {
    Display::Context::SharedContext.init();
    Events::SharedModifierState.init();
    Events::SharedState.init();
    Storage::FileSystem::sharedFileSystem.init();
}

}  // namespace Ion
