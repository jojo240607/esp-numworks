#include <escher/window.h>
#include <ion.h>
#include <esp_log.h>

extern "C" {
#include <assert.h>
}

namespace Escher {
    static const char *TAG = "Escher.Window";
void Window::redraw(bool force) {
//    ESP_LOGI(TAG, "redraw force %d", force);
  if (force) {
    markWholeFrameAsDirty();
//      ESP_LOGI(TAG, "markWholeFrameAsDirty ok");
  }
  Ion::Display::waitForVBlank();
//    ESP_LOGI(TAG, "waitForVBlank ok");
  View::redraw(bounds());
//    ESP_LOGI(TAG, "redraw ok");
}

void Window::setContentView(View* contentView) {
  m_contentView = contentView;
  markWholeFrameAsDirty();
  layoutSubviews();
}

int Window::numberOfSubviews() const {
  return (m_contentView == nullptr ? 0 : 1);
}

View* Window::subviewAtIndex(int index) {
  assert(m_contentView != nullptr && index == 0);
  return m_contentView;
}

void Window::layoutSubviews(bool force) {
  if (m_contentView != nullptr) {
    setChildFrame(m_contentView, bounds(), force);
  }
}

#if ESCHER_VIEW_LOGGING
const char* Window::className() const { return "Window"; }

#endif

}  // namespace Escher
