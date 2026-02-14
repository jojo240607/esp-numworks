cmake_minimum_required(VERSION 3.10)
project(kandinsky CXX)  # 指定为 C 项目

set(COMPONENT_NAME kandinsky)

# 源文件列表（支持通配符或明确列表）
set(MODULE_SOURCES
        fonts/codepoint_to_glyph_index.cpp
        fonts/epsilon/LargeFont.cpp
        fonts/epsilon/SmallFont.cpp
        src/color.cpp
        src/context_circle.cpp
        src/context_line.cpp
        src/context_pixel.cpp
        src/context_rect.cpp
        src/context_text.cpp
        src/font.cpp
        src/framebuffer.cpp
        src/point.cpp
        src/rect.cpp
        )
# 创建静态库
add_library(${COMPONENT_NAME} STATIC
        ${MODULE_SOURCES}
)

# 设置包含目录
target_include_directories(${COMPONENT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/in