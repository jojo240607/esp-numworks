#include <assert.h>
#include <ion/external_apps.h>
#include "external/external_apps_list.h"

#define EXTERNAL_APPS_API_LEVEL 1

#ifndef EXTERNAL_APPS_API_LEVEL
#error This file expects EXTERNAL_APPS_API_LEVEL to be defined
#endif


namespace Ion {
namespace ExternalApps {



App::App(uint8_t* a) : m_startAddress(a) {
}


uint32_t App::appInfo(AppInfo info) const {
  return 0;
}

uint8_t* App::appInfoToAddress(AppInfo info) const {
  return nullptr;
}

const uint32_t App::APILevel() const {
    return ((ExternalAppInfo *)m_startAddress)->api_level;
}

const char* App::name() const {
    return ((ExternalAppInfo *)m_startAddress)->app_name;
}

uint32_t App::iconSize() const {
    return ((ExternalAppInfo *)m_startAddress)->icon_ptr->compressedPixelDataSize();
}

const uint8_t* App::iconData() const {
    return ((ExternalAppInfo *)m_startAddress)->icon_ptr->compressedPixelData();
}

void* App::entryPoint() const {
  if (APILevel() != EXTERNAL_APPS_API_LEVEL) {
    return nullptr;
  }
  /* As stated in ARM Cortex guide generic user guide: Bit[0] of any address
   * you write to the PC with a BX, BLX, LDM, LDR, or POP instruction must be
   * 1 for correct execution, because this bit indicates the required
   * instruction set, and the Cortex-M7 processor only supports Thumb
   * instructions.
   */
  return (void *)((ExternalAppInfo *)m_startAddress)->main_ptr;
}

void App::eraseMagicCode() {
}


AppIterator& AppIterator::operator++() {
  m_currentAddress = nullptr;
  return *this;
}

bool hideExternalApps(bool isExamModeActive) { return isExamModeActive; }

AppIterator Apps::begin() const {
  uint8_t* storageStart = (uint8_t *)ExternalAppsList::app_list;
  if (m_isExamModeActive ) {
    return end();
  }
  return AppIterator(storageStart);
}

int numberOfApps(bool isExamModeActive) {
  int counter = 0;
  for (App a : Apps(isExamModeActive)) {
    assert(!hideExternalApps(isExamModeActive));
    (void)a;
    counter++;
  }
  return counter;
}

void deleteApps(bool isExamModeActive) {

}

bool allowThirdParty() { return true; }

void updateClearanceLevel(bool isExamModeActive) {

}

}  // namespace ExternalApps
}  // namespace Ion
