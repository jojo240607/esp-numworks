#include <stdio.h>
#include "hardware/device_def.h"
#include "hardware/sdl/display/display.h"
//#include "hardware/sdl/keyboard/key_pad.h"
//#include "hardware/sdl/screen/screen.h"
#include <ion.h>
#include <ion/exam_mode.h>
#include <ion/src/shared/init.h>
//#include <shared/boot/rt0.h>
#include <shared/drivers/usb.h>
#include "hardware/self_strcpy.h"

int main (int argc, char **argv) {
    printf("main\n");
    init_device();
    printf("main\n");
    Ion::Init();
    //// Initialize slotInfo to be accessible to Kernel
    Ion::Device::USB::slotInfo();
    Ion::ExternalApps::deleteApps(Ion::ExamMode::get().isActive());
    ion_main(0, nullptr);
    abort();
    destory();
    return 0;
}
