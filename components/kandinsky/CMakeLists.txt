set(COMPONENT_NAME kandinsky)

# 源文件列表（支持通配符或明确列表）
set(MODULE_SOURCES
        fonts/codepoint_to_glyph_index.cpp
        #fonts/epsilon/LargeFont.cpp
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

# 注册为IDF组件
idf_component_register(
        SRCS ${MODULE_SOURCES}
        INCLUDE_DIRS "include" ${CMAKE_CURRENT_LIST_DIR}/include  ${CMAKE_CURRENT_LIST_DIR}/../omg/include # 绝对路径
       # REQUIRES driver            # 依赖的组件
)
