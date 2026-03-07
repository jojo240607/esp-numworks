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
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/../omg/include
)

# 直接链接B库（因为B库在同一项目中已定义）
target_link_libraries(${COMPONENT_NAME}
        PUBLIC  # 或 PRIVATE，根据是否要传递依赖
        omg
)
# 注册为IDF组件
#idf_component_register(
#        SRCS ${MODULE_SOURCES}
#        INCLUDE_DIRS "include"
#            ${CMAKE_CURRENT_LIST_DIR}/include
#            ${CMAKE_CURRENT_LIST_DIR}/../omg/include # 绝对路径
#       REQUIRES
#            omg            # 依赖的组件
#)
