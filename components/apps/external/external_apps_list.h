//
// Created by zhiwei.gong on 2026/1/15.
//

#ifndef I80_CONTROLLER_EXTERNAL_APPS_LIST_H
#define I80_CONTROLLER_EXTERNAL_APPS_LIST_H
#include "escher/image.h"
#include "sample_c/src/sample_c_main.h"
#include "sample_c/src/sample_c_icon.h"
#include "sample_cpp/src/sample_cpp_main.h"
#include "sample_cpp/src/sample_cpp_icon.h"
#include "rpn_cpp/src/rpn_main.h"
#include "rpn_cpp/src/rpn_icon.h"
#include "khicas/khicas_main.h"
#include "khicas/khicas_icon.h"

namespace Ion {
    namespace ExternalApps {
        // C++11风格的类型别名
        using FuncPtr = void (*)();
        using IconPtr = const Escher::Image* const;

        // 结构体定义
        struct ExternalAppInfo {
            const char* app_name;
            FuncPtr main_ptr;
            IconPtr icon_ptr;
            const uint32_t api_level;
        };


        class ExternalAppsList {
        public:
            static inline int externalAppNum = 1;
            static inline ExternalAppInfo app_list[1] = {
                    //{"sample c", sample_c_main, ImageStore::SampleCIcon, 1},
                    //{"sample cpp", sample_cpp_main, ImageStore::SampleCppIcon, 1},
                    //{"Rpn", rpn_main, ImageStore::RpnIcon, 1},
                    {"KhiCas", khicas_main, ImageStore::KhicasIcon, 1}
            };
        };
    }
}
#endif //I80_CONTROLLER_EXTERNAL_APPS_LIST_H
