//
// Created by Administrator on 2026/1/14/014.
//

#ifndef I80_CONTROLLER_EXTERNAL_APPS_LIST_H
#define I80_CONTROLLER_EXTERNAL_APPS_LIST_H

#include <escher/image.h>
#include <array>
#include "sample_c/src/sample_c_icon.h"
#include "sample_c/src/sample_c_main.h"
#include "sample_cpp/src/sample_cpp_main.h"

namespace Ion {
    namespace ExternalApps {
        typedef void (*FuncPtr)();
        typedef const Escher::Image *const IconPtr;

        struct ExternalAppInfo {
            const char* app_name;
            FuncPtr main_ptr;
            IconPtr icon_ptr;
            uint32_t api_level;
        };
        class ExternalAppsList {
        public:
            static inline int externalAppNum = 2;
            // 使用已定义的类型
            static inline ExternalAppInfo app_list[2] = {
                    {"sample c", sample_c_main, ImageStore::SampleCIcon, 1},
                    {"sample cpp", sample_cpp_main, ImageStore::SampleCIcon, 1}
            };

        };

    }
}
#endif //I80_CONTROLLER_EXTERNAL_APPS_LIST_H
