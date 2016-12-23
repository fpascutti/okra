
# Copyright Franck Pascutti 2016.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(okra CXX)

option(OKRA_BUILD_TESTS "Build Okra's tests." OFF)
option(OKRA_BUILD_SAMPLES "Build Okra's samples." OFF)

option(OKRA_CONFIG_USEBOOSTREGEX "Use Boost for regular expressions." OFF)
option(OKRA_CONFIG_USEBOOSTWSTRINGCONVERT "Use Boost for wide to byte string conversions." OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

function(okra_library name)
    add_library(${name} INTERFACE)

    target_compile_features(${name} INTERFACE
        cxx_auto_type
        cxx_rvalue_references
        cxx_variadic_templates
    )
    target_include_directories(${name}
        INTERFACE ${CMAKE_SOURCE_DIR}/includes
    )

    if(OKRA_CONFIG_USEBOOSTREGEX)
        target_compile_definitions(${name}
            INTERFACE "OKRA_CONFIG_USEBOOSTREGEX=1"
        )
        target_link_libraries(${name}
            INTERFACE ${CONAN_LIBS_BOOST}
        )
    endif()

    if(OKRA_CONFIG_USEBOOSTWSTRINGCONVERT)
        target_compile_definitions(${name}
            INTERFACE "OKRA_CONFIG_USEBOOSTWSTRINGCONVERT=1"
        )
        target_link_libraries(${name}
            INTERFACE ${CONAN_LIBS_BOOST}
        )
    endif()

endfunction()

okra_library(okra
    includes/okra/detail/find_and_call.h
    includes/okra/detail/function_traits.h
    includes/okra/detail/implementation.h
    includes/okra/detail/invoke.h
    includes/okra/detail/typed_registry.h
    includes/okra/detail/wstring_convert.h
    includes/okra/googletest/framework.h
    includes/okra/compiler_detection.h
    includes/okra/config.h
    includes/okra/ignore.h
    includes/okra/impl.h
    includes/okra/re.h
    includes/okra/scenario.h
    includes/okra/step.h
)

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

if(OKRA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
endif()

if(OKRA_BUILD_SAMPLES)
    add_subdirectory(${CMAKE_SOURCE_DIR}/samples)
endif()
