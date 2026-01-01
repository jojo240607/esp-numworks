#include "display.h"

#include <ion/display.h>

#include "svcall.h"
#include <esp_log.h>
#include "esp32/sst7789_device.h"

namespace Ion {
namespace Display {
    static const char *TAG = "Ion.Display";
    static Lcd_device *lcd_device = (Lcd_device *)sst7789_device_create();
void SVC_ATTRIBUTES pushRect(KDRect r, const KDColor* pixels) {
    //Todo push Rect to frame
  //SVC_RETURNING_VOID(SVC_DISPLAY_PUSH_RECT)
    if (lcd_device->framebuf.currentbuf == NULL) {
        ESP_LOGI(TAG, "lcd_device virtual_init");
        virtual_init(lcd_device);
    }

    for (uint16_t pushh = 0; pushh < r.size().height(); pushh++) {
        for (uint16_t pushw = 0;pushw < r.size().width(); pushw++) {
            *(((uint16_t *)lcd_device->framebuf.currentbuf) + (320 - r.origin().x() - pushw) * 240 + (r.origin().y() + pushh)) = *(pixels++);
        }
    }
    //ESP_LOGI(TAG, "pushRect(%d, %d, %d,%d)", r.size().width(), r.size().height(), r.origin().x(), r.origin().y());
}

void SVC_ATTRIBUTES pushRectUniform(KDRect r, KDColor c) {
    //Todo push Rect color
  //SVC_RETURNING_VOID(SVC_DISPLAY_PUSH_RECT_UNIFORM)
  if (lcd_device->framebuf.currentbuf == NULL) {
      ESP_LOGI(TAG, "lcd_device virtual_init");
      virtual_init(lcd_device);
  }
    //ESP_LOGI(TAG, "pushRectUniform(%d, %d, %d,%d) color %d %d %d", r.size().width(), r.size().height(), r.origin().x(), r.origin().y(), c.red(), c.green(), c.blue());
    for (uint16_t pushh = 0; pushh < r.size().height(); pushh++) {
        for (uint16_t pushw = 0;pushw < r.size().width(); pushw++) {
            *(((uint16_t *)lcd_device->framebuf.currentbuf) + (320 - r.origin().x() - pushw) * 240 + (r.origin().y() + pushh)) = c;
        }
    }
}

void SVC_ATTRIBUTES pullRect(KDRect r, KDColor* pixels) {
    //Todo pull Rect to frame
  //SVC_RETURNING_VOID(SVC_DISPLAY_PULL_RECT)
    for (uint16_t pushh = 0; pushh < r.size().height(); pushh++) {
        for (uint16_t pushw = 0;pushw < r.size().width(); pushw++) {
            *(pixels++) = *(((uint16_t *)lcd_device->framebuf.currentbuf) + (320 - r.origin().x() - pushw) * 240 + (r.origin().y() + pushh));
        }
    }
    ESP_LOGI(TAG, "pullRect");
}

bool SVC_ATTRIBUTES waitForVBlank() {
    //Todo wait TE Sync
  //SVC_RETURNING_R0(SVC_DISPLAY_WAIT_FOR_V_BLANK, bool)
    ESP_LOGI(TAG, "waitForVBlank");
    return false;
}

void SVC_ATTRIBUTES POSTPushMulticolor(int rootNumberTiles, int tileSize) {
  //SVC_RETURNING_VOID(SVC_DISPLAY_POST_PUSH_MULTICOLOR)
}

void SVC_ATTRIBUTES syncDisplay() {
    virtual_ondraw(lcd_device);
}
// This assert ensures that the signature of drawString stays the same
template <class T1, class T2>
struct SameType {
  enum { value = false };
};
template <class T>
struct SameType<T, T> {
  enum { value = true };
};
static_assert(
    SameType<decltype(&drawString),
             void (*)(const char* text, KDPoint point, bool largeFont,
                      KDColor textColor, KDColor backgroundColor)>::value,
    "Signature of drawString changed");

void drawString(const char* text, KDPoint point, bool largeFont,
                KDColor textColor, KDColor backgroundColor) {
  KDContext* ctx = Ion::Display::Context::SharedContext;
  ctx->setOrigin(KDPointZero);
  ctx->setClippingRect(Ion::Display::Rect);
  ctx->drawString(text, point,
                  KDGlyph::Style{.glyphColor = textColor,
                                 .backgroundColor = backgroundColor,
                                 .font = largeFont ? KDFont::Size::Large
                                                   : KDFont::Size::Small},
                  255);
}

void setScreenshotCallback(void (*)(void)) {}

}  // namespace Display
}  // namespace Ion
