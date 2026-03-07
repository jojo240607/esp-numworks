cmake_minimum_required(VERSION 3.10)
project(poincare CXX)  # 指定为 C 项目


set(COMPONENT_NAME poincare)

# ========== 文件分组 ==========
# 1. 最小核心文件（必须最先编译）
set(_sources_group1
        src/init.cpp
        src/trigonometry.cpp
        src/preferences.cpp
        src/print_float.cpp
        src/sign.cpp
        src/pool_handle.cpp
        src/pool_object.cpp
        src/pool.cpp
        #src/init_no_pool.cpp:+nopool
        )

# 2. 检查点相关文件
set(_sources_group2
        src/memory/tree_stack_checkpoint.cpp
        src/pool_checkpoint.cpp
        src/circuit_breaker_checkpoint.cpp
        src/exception_checkpoint.cpp
        #src/pool_checkpoint_dummy.cpp:+nocheckpoint
        )

# 3. 存储相关文件
set(_sources_group3
        src/preferences_in_storage.cpp
        #src/preferences_no_storage.cpp:+nostorage
        )

# 4. 扩展功能文件
set(_sources_group4
        # expression 目录
        src/expression/advanced_operation.cpp
        src/expression/advanced_reduction.cpp
        src/expression/algebraic.cpp
        src/expression/aliases.cpp
        src/expression/arithmetic.cpp
        src/expression/approximation.cpp
        src/expression/approximation_derivative.cpp
        src/expression/approximation_helpers.cpp
        src/expression/approximation_integral.cpp
        src/expression/approximation_matrix.cpp
        src/expression/approximation_prepare.cpp
        src/expression/approximation_power.cpp
        src/expression/approximation_logarithm.cpp
        src/expression/approximation_trigonometry.cpp
        src/expression/beautification.cpp
        src/expression/binary.cpp
        src/expression/bounds.cpp
        src/expression/builtin.cpp
        src/expression/cas_disabled.cpp
        #src/expression/cas_enabled.cpp:+cas
        src/expression/continuity.cpp
        src/expression/decimal.cpp
        src/expression/degree.cpp
        src/expression/dependency.cpp
        src/expression/derivation.cpp
        src/expression/dimension.cpp
        src/expression/division.cpp
        src/expression/equal_sign.cpp
        src/expression/equation_solver.cpp
        src/expression/float_helper.cpp
        src/expression/infinity.cpp
        src/expression/integer.cpp
        src/expression/integer_serialization.cpp
        src/expression/integration.cpp
        src/expression/list.cpp
        src/expression/logarithm.cpp
        src/expression/matrix.cpp
        src/expression/metric.cpp
        src/expression/number.cpp
        src/expression/order.cpp
        src/expression/parametric.cpp
        src/expression/physical_constant.cpp
        src/expression/polynomial.cpp
        src/expression/power_like.cpp
        src/expression/projection.cpp
        src/expression/random.cpp
        src/expression/rational.cpp
        src/expression/sequence.cpp
        src/expression/expression_or_float.cpp
        src/expression/set.cpp
        src/expression/simplification.cpp
        src/expression/symbol.cpp
        src/expression/systematic_addition.cpp
        src/expression/systematic_operation.cpp
        src/expression/systematic_multiplication.cpp
        src/expression/systematic_reduction.cpp
        src/expression/trigonometry.cpp
        src/expression/trigonometry_exact_formulas.cpp
        src/expression/undefined.cpp
        src/expression/units/unit.cpp
        src/expression/units/representatives.cpp
        src/expression/variables.cpp
        src/expression/vector.cpp

        # function_properties 目录
        src/function_properties/conic.cpp
        src/function_properties/function_type.cpp
        src/function_properties/helper.cpp
        src/function_properties/integral.cpp

        # helpers 目录
        src/helpers/cas.cpp
        src/helpers/expression_equal_sign.cpp
        src/helpers/layout.cpp
        src/helpers/scatter_plot_iterable.cpp
        src/helpers/sequence.cpp
        src/helpers/store.cpp
        src/helpers/symbol.cpp
        src/helpers/trigonometry.cpp

        # layout 目录
        src/layout/autocompleted_pair.cpp
        src/layout/code_point_layout.cpp
        src/layout/cursor_motion.cpp
        src/layout/empty_rectangle.cpp
        src/layout/grid.cpp
        src/layout/input_beautification.cpp
        src/layout/layout_cursor.cpp
        src/layout/layout_selection.cpp
        src/layout/layout_serializer.cpp
        src/layout/layout_span.cpp
        src/layout/layout_span_decoder.cpp
        src/layout/layout_memoization.cpp
        src/layout/layouter.cpp
        src/layout/multiplication_symbol.cpp
        src/layout/parser.cpp
        src/layout/parsing/helper.cpp
        src/layout/parsing/latex_parser.cpp
        src/layout/parsing/rack_parser.cpp
        src/layout/parsing/tokenizer.cpp
        src/layout/rack_from_text.cpp
        src/layout/rack_layout.cpp
        src/layout/rack_layout_decoder.cpp
        src/layout/render.cpp
        src/layout/xnt.cpp

        # memory 目录
        src/memory/block_stack.cpp
        src/memory/tree_stack.cpp
        src/memory/n_ary.cpp
        src/memory/pattern_matching.cpp
        src/memory/tree.cpp
        src/memory/tree_ref.cpp
        src/memory/value_block.cpp
        src/memory/visualization.cpp

        # pool 目录
        src/pool/expression.cpp
        src/pool/layout.cpp
        src/pool/pool_layout_cursor.cpp

        # solver 目录
        src/solver/beta_function.cpp
        src/solver/erf_inv.cpp
        src/solver/matrix_array.cpp
        src/solver/point_of_interest_list.cpp
        src/solver/regularized_gamma_function.cpp
        src/solver/regularized_incomplete_beta_function.cpp
        src/solver/roots.cpp
        src/solver/solver.cpp
        src/solver/solver_algorithms.cpp
        src/solver/zoom.cpp

        # statistics 目录
        src/statistics/distributions/distribution_cumulative_function.cpp
        src/statistics/distributions/distribution_cumulative_inverse.cpp
        src/statistics/distributions/distribution_evaluate.cpp
        src/statistics/distributions/distribution_method.cpp
        src/statistics/distributions/distribution_parameters.cpp
        src/statistics/distributions/distribution.cpp
        src/statistics/inference/chi2_test.cpp
        src/statistics/inference/confidence_interval.cpp
        src/statistics/inference/inference.cpp
        src/statistics/inference/significance_test.cpp
        src/statistics/regression/affine_regression.cpp
        src/statistics/regression/cubic_regression.cpp
        src/statistics/regression/exponential_regression.cpp
        src/statistics/regression/linear_regression.cpp
        src/statistics/regression/logarithmic_regression.cpp
        src/statistics/regression/logistic_regression.cpp
        src/statistics/regression/median_regression.cpp
        src/statistics/regression/power_regression.cpp
        src/statistics/regression/proportional_regression.cpp
        src/statistics/regression/quadratic_regression.cpp
        src/statistics/regression/quartic_regression.cpp
        src/statistics/regression/regression.cpp
        src/statistics/regression/regression_switch.cpp
        src/statistics/regression/transformed_regression.cpp
        src/statistics/regression/trigonometric_regression.cpp
        src/statistics/data_table.cpp
        src/statistics/domain.cpp
        src/statistics/statistics_dataset.cpp
        src/statistics/statistics_dataset_column.cpp

        # 其他文件
        src/additional_results_helper.cpp
        src/comparison_operator.cpp
        src/context.cpp
        src/empty_context.cpp
        src/float_list.cpp
        src/pool_variable_context.cpp
        src/print.cpp
        src/random.cpp
        src/range.cpp
        src/tree_variable_context.cpp
        )

# 5. 测试文件
set(_sources_group5 "")
if(POINCARE_TEST)
    set(_sources_group5
            test/old/additional_results_helper.cpp
            test/old/approximation.cpp
            # ... 添加所有测试文件
            )
endif()
set(ALL_SOURCES
        ${_sources_group1}
        ${_sources_group2}
        ${_sources_group3}
        ${_sources_group4}
        ${_sources_group5}
        )
# 创建静态库
add_library(${COMPONENT_NAME} STATIC
        ${ALL_SOURCES}
)

# 设置包含目录
target_include_directories(${COMPONENT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/..
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/../omg/include
        ${CMAKE_CURRENT_LIST_DIR}/../kandinsky/include
        ${CMAKE_CURRENT_LIST_DIR}/../ion/include
        ${CMAKE_CURRENT_LIST_DIR}/../ion/include/ion/keyboard/epsilon
)

# 直接链接B库（因为B库在同一项目中已定义）
target_link_libraries(${COMPONENT_NAME}
        PUBLIC  # 或 PRIVATE，根据是否要传递依赖
        omg
        kandinsky
        ion
)

target_compile_options(${COMPONENT_NAME} PRIVATE
        #"-include ${CMAKE_CURRENT_LIST_DIR}/config.epsilon.h"
        "-Wno-error"
        "-Wno-expansion-to-defined"
        "-Wno-error=unused-value"
        "-Wno-error=char-subscripts"
        "-Wno-error=array-parameter="
        "-Wno-switch-outside-range")


# 降低警告级别
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error")

# 或关闭特定警告
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-expansion-to-defined")
# 注册为IDF组件
#idf_component_register(
#        SRCS ${ALL_SOURCES}
#        INCLUDE_DIRS
#            "include" ${CMAKE_CURRENT_SOURCE_DIR}
#            ${CMAKE_CURRENT_SOURCE_DIR}/..
#            ${CMAKE_CURRENT_SOURCE_DIR}/include
#            ${CMAKE_CURRENT_SOURCE_DIR}/../omg/include
#            ${CMAKE_CURRENT_SOURCE_DIR}/../kandinsky/include
#            ${CMAKE_CURRENT_SOURCE_DIR}/../ion/include
#            ${CMAKE_CURRENT_SOURCE_DIR}/../ion/include/ion/keyboard/epsilon
#
#        # COMPILE_OPTIONS
#        #     "-include ${CMAKE_CURRENT_LIST_DIR}/config.epsilon.h"  # 注意：整个选项用引号包裹
#        #$<$<CXX_COMPILER_ID:GNU>:-Wno-switch-outside-range>
#        #$<$<CXX_COMPILER_ID:Clang>:-Wno-tautological-constant-out-of-range-compare>
#        #COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS}
#        REQUIRES
#            kandinsky  # 字体依赖
#            omg
#            ion
#)
#target_sources(${COMPONENT_TARGET} PRIVATE
#        "config.epsilon.h"
#        )
# 然后设置编译选项（如果 ${COMPONENT_NAME} 已定义）
#if(TARGET ${COMPONENT_NAME})
#    target_compile_options(${COMPONENT_NAME} PRIVATE
#            "-include ${CMAKE_CURRENT_LIST_DIR}/config.epsilon.h"
#            )
#
#    # 根据编译器设置警告选项
#    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#        target_compile_options(${COMPONENT_NAME} PRIVATE
#                -Wno-switch-outside-range
#                )
#    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#        target_compile_options(${COMPONENT_NAME} PRIVATE
#                -Wno-tautological-constant-out-of-range-compare
#                )
#    endif()
#endif()