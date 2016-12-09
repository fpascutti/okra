
# Copyright Franck Pascutti 2016.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(okra CXX)

option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)
option(OKRA_BUILD_TESTS "Build Okra's tests." OFF)
option(OKRA_BUILD_SAMPLES "Build Okra's samples." OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# library
function(okra_library name)
    add_library(${name} INTERFACE)
    target_compile_features(okra INTERFACE
        cxx_auto_type
        cxx_rvalue_references
        cxx_variadic_templates
    )
endfunction()

okra_library(okra
    includes/okra/detail/function_traits.h
    includes/okra/compiler_detection.h
    includes/okra/config.h
    includes/okra/export.h
    includes/okra/scenario.h
    includes/okra/step.h
)

# tests
function(okra_test name)
    add_executable(${name} ${ARGN})
    target_include_directories(${name}
        PRIVATE ${CMAKE_SOURCE_DIR}/includes
    )
    target_sources(${name}
        PRIVATE tests/gtest-main.cpp
    )
    target_link_libraries(${name}
        PRIVATE okra
        PRIVATE ${CONAN_LIBS_GTEST} gmock gmock_main
    )
    add_test(NAME ${name} COMMAND ${name} --gtest_shuffle)
endfunction()

if(OKRA_BUILD_TESTS)
    enable_testing()
    okra_test(unit-tests
        tests/unit-tests/detail/function_traits.cpp
        tests/unit-tests/scenario.cpp
        tests/unit-tests/step.cpp
    )
endif()

# samples
function(okra_sample name)
    add_executable(${name} ${ARGN})
    target_include_directories(${name}
        PRIVATE ${CMAKE_SOURCE_DIR}/includes
    )
    target_sources(${name}
        PRIVATE samples/gtest-main.cpp
    )
    target_link_libraries(${name}
        PRIVATE okra
        PRIVATE ${CONAN_LIBS_GTEST} gmock gmock_main
    )
endfunction()
if(OKRA_BUILD_SAMPLES)
    okra_sample(inline-scenario
        samples/inline-scenario/inline-scenario.cpp
    )
endif()

# generated files
include(WriteCompilerDetectionHeader)
write_compiler_detection_header(
    FILE "${CMAKE_SOURCE_DIR}/includes/okra/compiler_detection.h"
    PREFIX "OKRA"
    COMPILERS AppleClang Clang GNU MSVC
    FEATURES
        cxx_constexpr
        cxx_noexcept
        cxx_override
)
