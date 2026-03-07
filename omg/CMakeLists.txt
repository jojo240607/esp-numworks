cmake_minimum_required(VERSION 3.10)
project(omg CXX)  # 指定为 C 项目

set(COMPONENT_NAME omg)

# 源文件列表（支持通配符或明确列表）
set(MODULE_SOURCES
        src/arithmetic.cpp
        src/code_point.cpp
        src/directions.cpp
        src/external/lz4/lz4.c
        src/float.cpp
        src/list.cpp
        src/memory.cpp
        src/memory_decompress.cpp
        src/print.cpp
        src/unicode_helper.cpp
        src/utf8_decoder.cpp
        src/utf8_helper.cpp
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
)

# 注册为IDF组件
#idf_component_register(
#        SRCS ${MODULE_SOURCES}
#        INCLUDE_DIRS "include" ${CMAKE_CURRENT_LIST_DIR}/include  # 绝对路径
#       # REQUIRES driver            # 依赖的组件
#)
