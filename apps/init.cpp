#include "init.h"

#include "apps_container_storage.h"
#include "global_preferences.h"
#include "shared/global_context.h"

namespace Apps {

void Init() {
  Ion::Storage::FileSystem::sharedFileSystem
      ->initSystemRecord<GlobalPreferences>();

  ::Shared::GlobalContext::s_sequenceStore.init();
  std::printf("s_sequenceStore.init()\n");
  ::Shared::GlobalContext::s_sequenceCache.init(Shared::GlobalContext::s_sequenceStore.get());
    std::printf("s_sequenceCache.init()\n");
  ::Shared::GlobalContext::s_continuousFunctionStore.init();
    std::printf("s_continuousFunctionStore.init()\n");
  ::AppsContainerStorage::sharedAppsContainerStorage.init();
    std::printf("sharedAppsContainerStorage.init()\n");
}

}  // namespace Apps
