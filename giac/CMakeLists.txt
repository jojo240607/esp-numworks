set(COMPONENT_NAME giac)

# 源文件列表（支持通配符或明确列表）
set(MODULE_SOURCES
        src/alg_ext.cc
        src/cocoa.cc
        src/csturm.cc
        src/derive.cc
        src/desolve.cc
        src/ezgcd.cc
        src/first.cc
        src/gauss.cc
        src/gausspol.cc
        src/gen.cc
        src/global.cc
        src/help.cc
        src/identificateur.cc
        src/ifactor.cc
        src/index.cc
        src/input_lexer.cc
        src/input_parser.cc
        src/intg.cc
        src/intgab.cc
        src/isom.cc
        src/k_csdk.c
        src/kadd.cc
        src/kdisplay.cc
        src/lin.cc
        src/maple.cc
        src/mathml.cc
        src/misc.cc
        src/modfactor.cc
        src/modpoly.cc
        src/moyal.cc
        src/pari.cc
        src/permu.cc
        src/plot.cc
        src/plot3d.cc
        src/prog.cc
        src/quater.cc
        src/risch.cc
        src/rpn.cc
        src/series.cc
        src/solve.cc
        src/sparse.cc
        src/subst.cc
        src/sym2poly.cc
        src/symbolic.cc
        src/tex.cc
        src/threaded.cc
        src/ti89.cc
        src/tinymt32.cc
        src/unary.cc
        src/usual.cc
        src/vecteur.cc
        )

# 注册为IDF组件
idf_component_register(
        SRCS ${MODULE_SOURCES}
        INCLUDE_DIRS
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}
        # ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/../gmp/include
        ${CMAKE_CURRENT_LIST_DIR}/../mpfr/include
        ${CMAKE_CURRENT_LIST_DIR}/../mpfi/include
        REQUIRES mpfi          # 依赖的组件gmp mpfr
)
target_compile_options(${COMPONENT_LIB} PRIVATE
        -Os
        --specs=nosys.specs
        -frtti
        -DHAVE_CONFIG_H
        -DDEVICE
        -DNUMWORKS
        -DHAVE_CONFIG_H
        #-DNO_RTTI
        -DIN_GIAC
        -DTIMEOUT
        -DNO_STDEXCEPT
        -DGIAC_BINARY_ARCHIVE
        -DNO_UNARY_FUNCTION_COMPOSE
        -DNO_TEMPLATE_MULTGCD
        -DGIAC_NO_OPTIMIZATIONS
        -DSTATIC_BUILTIN_LEXER_FUNCTIONS
        "-w"
        "-Wno-psabi"
        "-Wno-deprecated-declarations"
        "-Wno-c++20-extensions]"
        "-Wno-error=unused-value" "-Wno-error=char-subscripts" "-Wno-error=array-parameter="
        "-Wno-error=reorder" "-Wno-unused-label" "-Wno-psabi" "-Wno-deprecated-declarations"
        "-Wno-c++20-extensions" "-Wno-address" "-Wno-error=maybe-uninitialized")
